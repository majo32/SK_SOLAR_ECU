/* 
 * File:   BlinkerService.h
 * Author: Ĺ imon
 *
 * Created on Sobota, 2016, februĂˇr 6, 14:42
 */

#ifndef BLINKERSERVICE_H
#define	BLINKERSERVICE_H




#include <SunStorm/Framework.h>
#include "Services/Blinker/MSG_STRUCTURES.h"
namespace Services {

    class BlinkerService : public SunStorm::Service {
    public:

        BlinkerService(std::string name) :
        SunStorm::Service(name), ctx(*this), listener1(*this, ctx), listenerSettingPeriod(*this, ctx), listenerPressedButtomLongShort(*this, ctx) {
        }

        virtual ~BlinkerService() {
        }
        void Initialize();
    public:

        class Context {
        public:

            Context(BlinkerService& blinkerService) :
            blinkerService(blinkerService) {
            }

            BlinkerService & blinkerService;
            int counterLeftBlinker; //counter na ukladanie dlzky stlacenia tlacidla LS
            int counterRightBlinker;

            bool longShortPressed;
            bool controlL;
            bool controlR;
            bool controlW;
            bool helpValue;
            long intervalIdLeft;
            long intervalIdRight;
            long intervalIdWarningLights;
            long intervalIdLongOrShortLEFT;
            long intervalIdLongOrShortRight;
            long counterInterval;
            int counterPeriod1; //counter na striedanie 1 a 0
            int counterPeriod2; //counter na poccitanie cyklu bliknuti
            bool localStatusBtnLeft;
            bool localStatusBtnRight;
 Messages::BlinkerServiceS::CAN_SC messagesSC;

        };

        class ListenerBase : public SunStorm::ServiceListener<BlinkerService, Context> {
        public:

            ListenerBase(BlinkerService & t, Context & ctx) :
            ServiceListener(t, ctx), ctx(ctx) {
            }
        protected:
            Context & ctx;

        };

        class Listener_1Can_CU_STATE_Blinkers : public ListenerBase {
        public:

            Listener_1Can_CU_STATE_Blinkers(BlinkerService & t, Context& ctx) :
            ListenerBase(t, ctx) {
            }

            void handle(const SunStorm::Message& message);

        };

        class ListenerSetPeriod : public ListenerBase {
        public:

            ListenerSetPeriod(BlinkerService & t, Context & ctx) :
            ListenerBase(t, ctx) {
            }

            void handle(const SunStorm::Message& message);

        };

        class ListenerButtonPressed : public ListenerBase {
        public:

            ListenerButtonPressed(BlinkerService& t, Context& ctx) :
            ListenerBase(t, ctx) {
            }
            void handle(const SunStorm::Message& message);
        };
    public:
        void setSendContent(SunStorm::Message & message, Messages::BlinkerServiceS::CAN_SC stukturka);
    private:
        Context ctx;
        Listener_1Can_CU_STATE_Blinkers listener1;
        ListenerSetPeriod listenerSettingPeriod;
        ListenerButtonPressed listenerPressedButtomLongShort;

    };


};

#endif	/* BLINKERSERVICE_H */

