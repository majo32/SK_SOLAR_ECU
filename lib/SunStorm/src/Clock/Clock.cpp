/* 
 * File:   Clock.cpp
 * Author: majo32
 * 
 * Created on Utorok, 2016, februára 16, 20:34
 */

#include "Clock.h"


void SunStorm::ClockMailBox::AcceptMessage(const SunStorm::Message & m) {
    MessengerTask t(listener, m);
    executor->EnqueueTask(t);
}

const SunStorm::Message SunStorm::Clock::CreateMessage(Timeout & tout){
    SunStorm::Message m("__TIMER__");
    m.SetContent<ClockResponse>(ClockResponse(tout.GetMailbox().GetIdentifier(), tout.GetExp()));
    return m;
}

long SunStorm::Clock::RegisterTimeout(long time, MessengerExecutor & executor, Listener & listener, long id) {
    std::lock_guard<std::mutex> lock(mutex);
    counter++;
    long l = counter;
    
    SunStorm::ClockMailBox cmb(&executor, &listener, ClockIdentifier(l));
    SunStorm::Timeout tout(time, cmb);
    
    tout.SetId(l);
    this->scheduler.push(tout);
    return l;

}

long SunStorm::Clock::RegisterInterval(long time, MessengerExecutor & executor, Listener & listener, long id) {
    std::lock_guard<std::mutex> lock(mutex);
    counter++;
    long l = counter;
    
    SunStorm::ClockMailBox cmb(&executor, &listener, ClockIdentifier(l));
    SunStorm::Timeout tout(time, std::make_shared<Interval>(time), cmb);
    
    tout.SetId(l);
    tout.GetInterval().get()->SetId(l);
    intervals.push_back(tout.GetInterval());
    this->scheduler.push(tout);
    return l;
}

long SunStorm::Clock::RegisterTimeout(long time, MessengerExecutor & executor, Listener & listener) {
    return RegisterTimeout(time, executor, listener, 0);

}

long SunStorm::Clock::RegisterInterval(long time, MessengerExecutor & executor, Listener & listener) {
    return RegisterInterval(time, executor, listener, 0);
}

void SunStorm::Clock::StopInterval(long id) {
    std::lock_guard<std::mutex> lock(mutex);
    std::list<std::shared_ptr < Interval>>::iterator it = intervals.begin();

    while (it != intervals.end()) {
        if (it->get()->GetId() == id) {
            it->get()->Disable();
            intervals.erase(it++);
        } else {
            ++it;
        }
    }
}

void SunStorm::Clock::Tick() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!this->scheduler.empty()) {
        SunStorm::Timeout tout = this->scheduler.top();
        auto now = std::chrono::system_clock::now();
        while (tout.GetExp() <= now) {
            if (tout.IsInterval() && !tout.GetInterval().get()->IsActive()) {

            } else {
                SunStorm::Message m = CreateMessage(tout);
                tout.GetMailbox().AcceptMessage(m);
            }
            this->scheduler.pop();

            if (tout.IsInterval() && tout.GetInterval().get()->IsActive()) {
                tout.ReuseInFuture();
                this->scheduler.push(tout);
            }

            if (this->scheduler.empty()) {
                break;
            }
            tout = this->scheduler.top();
        }
    }
}

void SunStorm::Clock::ClockThread::Run() {
    while (1) {
        clock.Tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(clock.heart_beat));
    }
}
