/* 
 * File:   System.cpp
 * Author: majo32
 * 
 * Created on Utorok, 2016, februára 16, 20:09
 */

#include "System.h"

SunStorm::Messenger& SunStorm::System::GetMessenger() {
    return SunStorm::System::GetDefaultSystemContext().messenger;
}

SunStorm::Clock& SunStorm::System::GetClock() {
    return SunStorm::System::GetDefaultSystemContext().clock;
}

void SunStorm::System::Initialize(){
    SunStorm::System::RegisterThread<SunStorm::Clock::ClockThread>(SunStorm::System::GetClock());
}

void SunStorm::System::Join(){
    std::list<Thread*>::iterator i = SunStorm::System::GetDefaultSystemContext().threads.begin();
    while (i != SunStorm::System::GetDefaultSystemContext().threads.end()) {
        (*i)->Join();
        ++i;
    }
}