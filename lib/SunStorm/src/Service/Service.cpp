/* 
 * File:   Service.cpp
 * Author: majo32
 *
 * Created on Utorok, 2016, februára 16, 13:12
 */
#include "Service.h"

long SunStorm::Service::RegisterListener(std::string name, Listener& listener) {
    SunStorm::MailBox mailBox(executor, listener, name);
    return System::GetMessenger().RegisterMailBox(mailBox);
}

void SunStorm::Service::RemoveListener(long id) {
    System::GetMessenger().RemoveMailBox(id);
}

void SunStorm::Service::SendMessage(const Message& m) {
    System::GetMessenger().SendMessage(m);
}

long SunStorm::Service::RegisterTimeout(long time, Listener& listener) {
    return System::GetClock().RegisterTimeout(time, executor, listener);
}

long SunStorm::Service::RegisterInterval(long time, Listener& listener) {
    return System::GetClock().RegisterInterval(time, executor, listener);
}

void SunStorm::Service::RemoveInterval(long id) {
    System::GetClock().StopInterval(id);
}


void SunStorm::Service::Run(){
    Initialize();
}

