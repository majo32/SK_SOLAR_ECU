/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComposerService.h
 * Author: Lenovo
 *
 * Created on Pondelok, 2016, marca 14, 12:35
 */

#ifndef COMPOSERSERVICE_H
#define COMPOSERSERVICE_H

#include <SunStorm/Framework.h>
#include "ComposerService_messages.h"
#include <string>
#include "../../CANinterface/CANinterface.h"


using namespace std;

namespace Services {

    class ComposerService : public SunStorm::Service {
    public:

        ComposerService(std::string name) :
        SunStorm::Service(name),
        context(*this),
        l1(*this, context),
        l2 (*this, context){
        }

        virtual ~ComposerService() {
        }
        void Initialize();

    public:

        class Context {
        public:

            Context(ComposerService & composerService) :
            composerService(composerService) {
            }

            ComposerService & composerService;

            CANinterface::CANmessage cMsg0x11;

        };

        class ListenerBase : public SunStorm::ServiceListener<ComposerService, Context> {
        public:

            ListenerBase(ComposerService& service, Context& context) :
            ServiceListener<ComposerService, Context>(service, context), context(context) {
            }

        protected:
            Context & context;
        };

        class Listener_ReCan0x10Cu : public ListenerBase {
        public:

            Listener_ReCan0x10Cu(ComposerService& composerService, Context& context) :
            ListenerBase(composerService, context) {
            }


            void handle(const SunStorm::Message& message);
        };

        class Listener_SendCanMsg : public ListenerBase {
        public:

            Listener_SendCanMsg(ComposerService& composerService, Context& context) :
            ListenerBase(composerService, context) {
            }


            void handle(const SunStorm::Message& message);
        };

    private:
        Context context;
        Listener_ReCan0x10Cu l1;
        Listener_SendCanMsg l2;
    };
};
#endif /* COMPOSERSERVICE_H */

