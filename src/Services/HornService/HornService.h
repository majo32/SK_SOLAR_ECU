/* 
 * File:   HornService.h
 * Author: Simon
 *
 * Created on Štvrtok, 2016, marec 10, 14:08
 */

#ifndef HORNSERVICE_H
#define	HORNSERVICE_H

#include <SunStorm/Framework.h>
#include "Services/Blinker/MSG_STRUCTURES.h"
namespace Services {

    class HornService : public SunStorm::Service {
    public:

        HornService(std::string name) :
        SunStorm::Service(name),
        ctx(*this), listener1(*this, ctx) {
        }

        virtual ~HornService() {
        }
        void Initialize();
        //    public:

        class Context {
        public:

            Context(HornService& hornService) :
            hornService(hornService) {
            }

            HornService & hornService;
            int counter;
        };

        class ListenerBase : public SunStorm::ServiceListener<HornService, Context> {
        public:

            ListenerBase(HornService & t, Context & ctx) :
            ServiceListener(t, ctx), ctx(ctx) {
            }

        protected:
            Context & ctx;

        };

        class Listener_1Can_CU_STATE : public ListenerBase {
        public:

            Listener_1Can_CU_STATE(HornService & service, Context& context) :
            ListenerBase(service, context) {
            }

            void handle(const SunStorm::Message& message);

        };



    private:
        Context ctx;
        Listener_1Can_CU_STATE listener1;

    public:
        void setSendContent(SunStorm::Message & message, Messages::BlinkerServiceS::CAN_SC structure);
    };
};

#endif	/* HORNSERVICE_H */

