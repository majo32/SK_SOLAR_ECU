/* 
 * File:   MailBox.h
 * Author: majo32
 *
 * Created on Streda, 2016, februára 3, 19:06
 */
#pragma once
#ifndef SUNSTORM_MAILBOX_H
#define	SUNSTORM_MAILBOX_H


#include "MessengerExecutor.h"
#include "MessengerListener.h"
#include "Message.h"
#include "MessengerTask.h"
#include <string>

namespace SunStorm {

    class MailBoxIndentifier {
    public:

        MailBoxIndentifier() :
        group(-1), reciever(-1), name_pattern(), all_names(true) {
        }

        MailBoxIndentifier(std::string name_pattern, long group, long reciever) :
        group(group), reciever(reciever), name_pattern(name_pattern), all_names(false) {
        }

        MailBoxIndentifier(long group, long reciever) :
        group(group), reciever(reciever), name_pattern(), all_names(true) {
        }

        MailBoxIndentifier(std::string name_pattern) :
        group(-1), reciever(-1), name_pattern(name_pattern), all_names(false) {
        }

        bool compare(const MessageIndentifier & other) const{
            if (name_pattern == other.GetName()) {
                return true;
            }
            return false;
        };
    private:
        long group;
        long reciever;
        std::string name_pattern;
        bool all_names;
    };

    class MailBox {
    public:

        MailBox(MessengerExecutor& executor, Listener& listener, const std::string message_pattern) :
        executor(executor), listener(listener), identifier(message_pattern), id(-1) {
        }

        MailBox(MessengerExecutor& executor, Listener& listener, const MailBoxIndentifier identifier) :
        executor(executor), listener(listener), identifier(identifier), id(-1) {
        }

        MailBox(const MailBox& orig) :
        executor(orig.executor), listener(orig.listener), identifier(orig.identifier), id(orig.id) {

        }

        virtual ~MailBox() {

        }

        bool AproveMessage(const Message & m);
        void AcceptMessage(const Message & m);
        
        long GetId() const {
            return id;
        }

        void SetId(long id) {
            this->id = id;
        }



    private:
        MessengerExecutor & executor;
        Listener & listener;
        const MailBoxIndentifier identifier;
        long id;
       
    };
}
#endif	/* MAILBOX_H */

