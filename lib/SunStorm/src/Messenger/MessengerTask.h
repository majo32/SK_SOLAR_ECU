/* 
 * File:   Task.h
 * Author: majo32
 *
 * Created on January 26, 2016, 9:55 PM
 */
#pragma once
#ifndef SUNSTORM_MESSENGERTASK_H
#define	SUNSTORM_MESSENGERTASK_H


#include "MessengerListener.h"
#include "../Concurrency/Task.h"
#include "Message.h"


namespace SunStorm {

    class MessengerTask : public Task {
    public:

        MessengerTask() :
        Task(), listener(NULL), message() {
        }

        MessengerTask(Listener * listener, const Message & message) :
        Task(), listener(listener), message(message) {
        }

        MessengerTask(const MessengerTask& orig) :
        Task(orig), listener(orig.listener), message(orig.message) {
        }

        virtual ~MessengerTask() {
        }

        void Run(void);
    private:
        Listener * listener;
        Message message;
    };
}

#endif	/* TASK_H */

