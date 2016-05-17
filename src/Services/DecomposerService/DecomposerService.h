/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DecomposerService.h
 * Author: Lenovo
 *
 * Created on Pondelok, 2016, marca 14, 12:34
 */

#ifndef DECOMPOSERSERVICE_H
#define DECOMPOSERSERVICE_H

#define SIZE_OF_CAN_0X10_MESSAGE  8
#define SIZE_OF_CAN_0X11_MESSAGE  8
#define SIZE_OF_CAN_0X20_MESSAGE  8

#include <SunStorm/Framework.h>
#include "DecomposerService_messages.h"
#include <string>
#include "../../CANinterface/CANinterface.h"

using namespace std;

namespace Services {

    class DecomposerService : public SunStorm::Service {
    public:

        DecomposerService(std::string name) :
        SunStorm::Service(name),
        context(*this),
        // l1(*this, context),
        t1(RegisterThread<ThreadCan>(*this, context)) {
        }

        virtual ~DecomposerService() {
        }
        void Initialize();


    public:

        class Context {
        public:

            Context(DecomposerService & decomposerService) :
            decomposerService(decomposerService) {
            }

            DecomposerService & decomposerService;

            CANinterface::CANmessage cMsg0x10;
            CANinterface::CANmessage cMsg0x12;
            CANinterface::CANmessage cMsg0x20;

            bool ReCan0x10Cu[SIZE_OF_CAN_0X10_MESSAGE];
            bool ReCan0x12Cu[SIZE_OF_CAN_0X10_MESSAGE];
            bool ReCan0x20[SIZE_OF_CAN_0X20_MESSAGE];

        };

        /* class ListenerBase : public SunStorm::ServiceListener<DecomposerService, Context> {
         public:

             ListenerBase(DecomposerService & decomposerService, Context & context) :
             ServiceListener(decomposerService, context), context(context) {
             }
         protected:
             Context & context;
         };

         class Listener_ReCan0x10Cu : public ListenerBase {
         public:

             Listener_ReCan0x10Cu(Service & service, Context & context) :
             ListenerBase(service, context) 
             {
             }


             void handle(const SunStorm::Message& message);
         };
         */
        class ThreadCan : public SunStorm::ServiceThread {
        public:

            ThreadCan(Service & service, Context & context) :
            ServiceThread(service), context(context) {
            }
        protected:
            void Run();
        private:
            Context & context;
        };

    private:
        Context context;
        //Listener_ReCan0x10Cu l1;
        ThreadCan & t1;


    };
};

#endif /* DECOMPOSERSERVICE_H */

