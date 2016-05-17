/* 
 * File:   Messenger.h
 * Author: majo32
 *
 * Created on January 26, 2016, 9:54 PM
 */
#pragma once
#ifndef SUNSTORM_MESSENGER_H
#define	SUNSTORM_MESSENGER_H



#include <list>
#include <mutex>
#include "MailBox.h"
#include "Message.h"
namespace SunStorm {

    class Messenger {
    public:

        Messenger() : list(), mutex(),counter(0) {
        }

        Messenger(const Messenger& orig) = delete;

        virtual ~Messenger() {
        }

        long RegisterMailBox(MailBox & mb);
        
        long RegisterMailBox(MessengerExecutor & executor, Listener & listener, std::string name_pattern);
        
        void RemoveMailBox(long id);

        void SendMessage(const Message & m);
    private:
        std::list<MailBox> list;
        std::mutex mutex;
        long counter;
    };
}
#endif	/* MESSENGER_H */

