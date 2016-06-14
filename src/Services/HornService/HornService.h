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
#include "../../Services/CAN/CAN_0Service/CAN_0Service_messages.h"
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
        public:
            HornService & hornService;
            Messages::CAN::CAN_0Service::HornResponse comannd;
         
            
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


    public:
        void setSendContent(SunStorm::Message & message, Messages::CAN::CAN_0Service::HornResponse  content);
    private:
        Context ctx;
        Listener_1Can_CU_STATE listener1;

    
    };
};

#endif	/* HORNSERVICE_H */

