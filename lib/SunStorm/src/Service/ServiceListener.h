/* 
 * File:   ServiceListener.h
 * Author: majo32
 *
 * Created on Utorok, 2016, februára 16, 17:53
 */
#pragma once
#ifndef SUNSTORM_SERVICELISTENER_H
#define	SUNSTORM_SERVICELISTENER_H


#include "../Messenger/MessengerListener.h"
#include "../Service/Service.h"
#include "../Logging/Logger.h"

namespace SunStorm {

    template<class S, class C>
    class ServiceListener : public Listener {
    public:

        ServiceListener(S & service, C & context) : service(service), context(context) {
        }

        virtual ~ServiceListener() {
        }

        void SendMessage(const Message & message) const {
            this->service.SendMessage(message);
        }

        C & GetContext() {
            return context;
        }

        S & GetService() {
            return service;
        }

        SunStorm::Service & GetServiceBase() {
            return (SunStorm::Service &) service;
        }

        long RegisterTimeout(long time, Listener & listener) {
            return GetService().RegisterTimeout(time, listener);
        }

        long RegisterInterval(long time, Listener & listener) {
            return GetService().RegisterInterval(time, listener);
        }

        void RemoveInterval(long id) {
            GetService().RemoveInterval(id);
        }

        Logger & Log() {
            return GetServiceBase().Log();
        }

    private:
        S & service;
        C & context;

    };
}
#endif	/* SERVICELISTENER_H */

