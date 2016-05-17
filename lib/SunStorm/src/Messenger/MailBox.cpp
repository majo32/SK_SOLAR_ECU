/* 
 * File:   MailBox.cpp
 * Author: majo32
 * 
 * Created on Streda, 2016, februára 3, 19:06
 */

#include "MailBox.h"

bool SunStorm::MailBox::AproveMessage(const Message & m) {
    return identifier.compare(m.GetIdentifier());
}

void SunStorm::MailBox::AcceptMessage(const Message & m) {
    MessengerTask t(&listener, m);
    executor.EnqueueTask(t);
}