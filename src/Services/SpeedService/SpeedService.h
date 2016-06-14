/* 
 * File:   SpeedService.h
 * Author: Michal Moravcik
 *
 * Created on Pondelok, 2016, februára 8, 19:56
 */

#ifndef SPEEDSERVICE_H
#define	SPEEDSERVICE_H

#include <SunStorm/Framework.h>
#include "SpeedService_messages.h"
#include "../CAN/CAN_0Service/CAN_0Service_messages.h"

#include <string>

using namespace std;

namespace Services {

    class SpeedService : public SunStorm::Service {
    public:

        SpeedService(std::string name) :
        SunStorm::Service(name),
        ctx(*this),
        lCanCuStatus(*this, ctx),
        lSpeedRqst(*this, ctx),
        lSpeedActual(*this, ctx),
        lSpeedOpt(*this, ctx),
        lTimeout(*this, ctx),
        lLongPressSender(*this, ctx) {
        }
        virtual ~SpeedService();
        void Initialize();


    public:

        class Context {
        public:

            Context(SpeedService& service) :
            service(service) {
            }

            SpeedService & service; //referencia na servis
            string stateMachine; //stav systemu
            bool localAutopilotStatus; //stav autopilota

            bool localBtnSpeedUpStatus; // stav tlacidla zvysenia rychlosti z poslednej spravy
            bool localBtnSpeedDownStatus; // stav tlacidla znizenia rychlosti z poslednej spravy
            bool localBtnAutopilotStatus; // stav tlacidla autopilot z poslednej spravy
            bool localPedalStatus; // stav brzdoveho pedalu z poslednej spravy

            uint8_t shortPressChangeValue = 1; // hodnota zmeny rychlosti pri kratkom stlaceni tlacidla
            uint8_t longPressChangeValue = 5; // hodnota zmeny rychlosti pri dlhom stlaceni tlacidla
            long btnHoldTime = 1000; // cas v ms po ktorom sa berie stlacenie tlacidla za dlhe stlacenie
            long longPressSendingIntervalTime = 400; // interval v ms v ktorom sa bude zvysovat rychlost pri dlhom stlaceni tlacidla

            uint8_t speedUpChangeValue; // hodnota o ktoru sa ma zvysit rychlost
            uint8_t speedDownChangeValue; // hodnota o ktoru sa ma znizit rychlost

            long btnUpTimeoutID; // ID casovaca timeout pre tlacidlo zvysenia rychlosti (casovac nespusteny -> ID=0)
            long btnDownTimeoutID; // ID casovaca timeout pre tlacidlo znizenia rychlosti (casovac nespusteny -> ID=0)

            long longPressUpSenderID; // ID casovaca interval pre tlacidlo zvysenia rychlosti (casovac nespusteny -> ID=0)
            long longPressDownSenderID; // ID casovaca interval pre tlacidlo zvysenia rychlosti (casovac nespusteny -> ID=0)


            uint8_t localRequiredSpeed; // pozadovana rychlost
            uint8_t localActualSpeed; // aktualna rychlost z poslednej spravy
            uint8_t localOptimalSpeed; // optimalna rychlost z poslednej spravy
        };

        class ListenerBase : public SunStorm::ServiceListener<SpeedService, Context> {
        public:

            ListenerBase(SpeedService & s, Context & ctx) :
            ServiceListener(s, ctx), ctx(ctx) {
            }
        protected:
            Context & ctx;
        };

        class Listener_CAN_CU_STATUS : public ListenerBase {
        public:

            Listener_CAN_CU_STATUS(SpeedService & s, Context& ctx) :
            ListenerBase(s, ctx) {
            }

            void handle(const SunStorm::Message & message); // pozri cpp
        };

        class Listener_PCU_SPEED_RQST : public ListenerBase {
        public:

            Listener_PCU_SPEED_RQST(SpeedService & s, Context& ctx) :
            ListenerBase(s, ctx) {
            }
            void handle(const SunStorm::Message & message);
        };

        class Listener_PCU_SPEED_ACTUAL : public ListenerBase {
        public:

            Listener_PCU_SPEED_ACTUAL(SpeedService & s, Context& ctx) :
            ListenerBase(s, ctx) {
            }
            void handle(const SunStorm::Message & message);
        };

        class Listener_PCU_SPEED_OPT : public ListenerBase {
        public:

            Listener_PCU_SPEED_OPT(SpeedService & s, Context& ctx) :
            ListenerBase(s, ctx) {
            }
            void handle(const SunStorm::Message & message);
        };

        class Listener_TIMEOUT : public ListenerBase {
        public:

            Listener_TIMEOUT(SpeedService & s, Context& ctx) :
            ListenerBase(s, ctx) {
            }
            void handle(const SunStorm::Message & message);
        };

        class Listener_LONG_PRESS_SENDER : public ListenerBase {
        public:

            Listener_LONG_PRESS_SENDER(SpeedService & s, Context& ctx) :
            ListenerBase(s, ctx) {
            }
            void handle(const SunStorm::Message & message);
        };

    public:

        void brake_pedal_handle(bool state);
        void btn_up_handle(bool state, SpeedService &s);
        void btn_down_handle(bool state, SpeedService &s);
        void btn_autopilot_handle(bool state);
        void speed_autopilot(bool status);
        void speed_rqst_down(uint8_t bValue);
        void speed_rqst_up(uint8_t bValue);
        void speed_rqst_set(uint8_t value);
        void btn_start_timeout(int btn, SpeedService &s);

    private:
        Context ctx;

        Listener_CAN_CU_STATUS lCanCuStatus;
        Listener_PCU_SPEED_RQST lSpeedRqst;
        Listener_PCU_SPEED_ACTUAL lSpeedActual;
        Listener_PCU_SPEED_OPT lSpeedOpt;
        Listener_TIMEOUT lTimeout;
        Listener_LONG_PRESS_SENDER lLongPressSender;
    };

}

#endif	/* SPEEDSERVICE_H */

