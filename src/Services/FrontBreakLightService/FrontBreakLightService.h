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
        void setSendContent(SunStorm::Message & message, Messages::BlinkerServiceS::CAN_SC content);
    };

};


#endif	/* FRONT_BREAKLIGHTSERVICE_H */

