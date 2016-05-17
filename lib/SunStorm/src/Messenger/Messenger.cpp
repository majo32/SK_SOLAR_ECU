/* 
 * File:   Messenger.cpp
 * Author: majo32
 * 
 * Created on January 26, 2016, 9:54 PM
 */

#include "Messenger.h"


long SunStorm::Messenger::RegisterMailBox(MessengerExecutor & executor, Listener & listener, std::string name_pattern) {
    MailBox mb(executor, listener, name_pattern);
    return RegisterMailBox(mb);
}

long SunStorm::Messenger::RegisterMailBox(MailBox & mb) {
    std::lock_guard<std::mutex> lock(mutex);
    counter++;
    long l = counter;
    mb.SetId(l);
    list.push_back(mb);
    return l;
}

void SunStorm::Messenger::RemoveMailBox(long id) {
    std::lock_guard<std::mutex> lock(mutex);
    std::list<MailBox>::iterator i = list.begin();
    while (i != list.end()) {
        if (i->GetId() == id) {
            list.erase(i++);
        } else {
            ++i;
        }
    }
}

void SunStorm::Messenger::SendMessage(const SunStorm::Message & m) {
    std::lock_guard<std::mutex> lock(mutex);
    for (std::list<MailBox>::iterator it = list.begin(); it != list.end(); ++it) {
        if (it->AproveMessage(m)) {
            it->AcceptMessage(m);
        }
    }
}
