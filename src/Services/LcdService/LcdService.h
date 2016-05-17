/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LcdService.h
 * Author: jozef-E550
 *
 * Created on Utorok, 2016, marca 15, 11:56
 */

#ifndef LCDSERVICE_H
#define LCDSERVICE_H

#include <SunStorm/Framework.h>
#include <iostream>
#include "Services/LcdService/LcdService_Message.h"
#include "IPC.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sched.h>
#include <limits.h>

namespace Services {

    class LcdService : public SunStorm::Service {
    public:

        ///konstruktor
        LcdService(std::string name) :
        SunStorm::Service(name),
        ctx(*this),
        l_can_sc_state(*this, ctx),
        l_can_temp_cv(*this, ctx),
        l_lan_switch(*this, ctx),
        l_lan_speed_rqst(*this, ctx),
        l_lan_speed_actual(*this, ctx),
        l_lan_speed_opt(*this, ctx) {
        }

        ///destruktor
        virtual ~LcdService() {

        }

        ///inicializacia
        void Initialize();

    public:

        /**
         * Kontext pomocou ktorého si listenry predávajú data vnútri servicu ( zdielaju ho medzi sebou )       
         */
        class Context {
        public:

            Context(LcdService& lcdService) :
            lcdService(lcdService) {

            }
            LcdService & lcdService;

        };

        class ListenerBase : public SunStorm::ServiceListener<LcdService, Context> {
        public:

            ListenerBase(LcdService & t, Context & ctx) :
            ServiceListener(t, ctx), ctx(ctx) {
            }
        protected:
            Context & ctx;

        };

        class Listener_CAN_SC_STATE : public ListenerBase {
        public:

            Listener_CAN_SC_STATE(LcdService & t, Context& ctx) :
            ListenerBase(t, ctx) {
            }

            void handle(const SunStorm::Message & message); // pozri cpp
        };

        class Listener_CAN_TEMP_CV : public ListenerBase {
        public:

            Listener_CAN_TEMP_CV(LcdService & t, Context& ctx) :
            ListenerBase(t, ctx) {
            }

            void handle(const SunStorm::Message & message); // pozri cpp
        };

        class Listener_LAN_SWITCH : public ListenerBase {
        public:

            Listener_LAN_SWITCH(LcdService & t, Context& ctx) :
            ListenerBase(t, ctx) {
            }

            void handle(const SunStorm::Message & message); // pozri cpp
        };

        class Listener_LAN_SPEED_RQST : public ListenerBase {
        public:

            Listener_LAN_SPEED_RQST(LcdService & t, Context& ctx) :
            ListenerBase(t, ctx) {
            }

            void handle(const SunStorm::Message & message); // pozri cpp
        };

        class Listener_LAN_SPEED_ACTUAL : public ListenerBase {
        public:

            Listener_LAN_SPEED_ACTUAL(LcdService & t, Context& ctx) :
            ListenerBase(t, ctx) {
            }

            void handle(const SunStorm::Message & message); // pozri cpp
        };

        class Listener_LAN_SPEED_OPT : public ListenerBase {
        public:

            Listener_LAN_SPEED_OPT(LcdService & t, Context& ctx) :
            ListenerBase(t, ctx) {
            }

            void handle(const SunStorm::Message & message); // pozri cpp
        };



    public:
        void send_msg_IPC();
        
    protected:
        

        ///// paramtre classy, konkrétne instancie , co service bude obsahovat
    private:
        Context ctx;

        // vytvorenie objektu vnorenej triedy
        Listener_CAN_SC_STATE l_can_sc_state;
        Listener_CAN_TEMP_CV l_can_temp_cv;
        Listener_LAN_SWITCH l_lan_switch;
        Listener_LAN_SPEED_RQST l_lan_speed_rqst;
        Listener_LAN_SPEED_ACTUAL l_lan_speed_actual;
        Listener_LAN_SPEED_OPT l_lan_speed_opt;

        CAN_SC_STATE_IPC msg_IPC;
        
    };

}

#endif /* LCDSERVICE_H */
