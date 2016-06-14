/* 
 * File:   FrontBreakLightService.h
 * Author: Simon
 *
 * Created on Pondelok, 2016, marec 21, 13:44
 */

#ifndef FRONT_BREAKLIGHTSERVICE_H
#define	FRONT_BREAKLIGHTSERVICE_H

#include <SunStorm/Framework.h>
#include "Services/Blinker/MSG_STRUCTURES.h"
#include "../../Services/CAN/CAN_0Service/CAN_0Service_messages.h"

namespace Services {

    class FrontBreakLightService : public SunStorm::Service {
    public:
        Messages::BlinkerServiceS::CAN_SC commands;
        
        FrontBreakLightService(std::string name) :
        SunStorm::Service(name),
        ctx(*this), listenerCan_CU_STATE(*this, ctx) {
            commands = {1, 1, 1, 0, 1, 0};
        }

        virtual ~FrontBreakLightService() {
        }
        void Initialize();

        class Context {
        public:

            Context(FrontBreakLightService& frontBreakLightService) :
            frontBreakLightService(frontBreakLightService) {
            }


            FrontBreakLightService & frontBreakLightService;
            Messages::CAN::CAN_0Service::BreakLightResponse cmdBrake;
            Messages::CAN::CAN_0Service::FrontLightResponse cmdFrontLight;
             SunStorm::Message mB{"CAN::CAN_0Service::BreakLightResponse"};
             SunStorm::Message mF{"CAN::CAN_0Service::FrontLightResponse"}  ;

        };

        class ListenerBase : public SunStorm::ServiceListener<FrontBreakLightService, Context> {
        public:

            ListenerBase(FrontBreakLightService & service, Context & context) :
            ServiceListener(service, context), ctx(context) {
            }
        protected:
            Context & ctx;
        };

        class Listener_CAN_CU_STATE : public ListenerBase {
        public:

            Listener_CAN_CU_STATE(FrontBreakLightService& service, Context& context) :
            ListenerBase(service, context) {
            }
            void handle(const SunStorm::Message& message);

        };
    private:
        Context ctx;
        Listener_CAN_CU_STATE listenerCan_CU_STATE;
        
    public:
        void setSendContent(SunStorm::Message & message, Messages::CAN::CAN_0Service::FrontLightResponse content);
        void setSendContent(SunStorm::Message & message,  Messages::CAN::CAN_0Service::BreakLightResponse content);
    };

};


#endif	/* FRONT_BREAKLIGHTSERVICE_H */

