/* 
 * File:   Service.h
 * Author: majo32
 *
 * Created on Utorok, 2016, februára 16, 13:12
 */
#pragma once
#ifndef SUNSTORM_SERVICE_H
#define	SUNSTORM_SERVICE_H


#include <string>

#include "../Messenger/MessengerExecutor.h"
#include "../Messenger/Message.h"
#include "../Messenger/MessengerListener.h"
#include "../Util/Id.h"
#include "../Messenger/MailBox.h"
#include "../System.h"
#include "../Logging/Logger.h"


namespace SunStorm {

    class Service : public IDClass {
    public:

        Service(std::string name) : IDClass(), name(name), executor(RegisterExecutor<MessengerExecutor>()), logger(LoggerManager::RegisterLogger(name)) {

        }
        Service(const Service& orig) = delete;

        virtual ~Service() {
        }
        virtual void Initialize() = 0;

        MessengerExecutor & GetExecutor() {
            return executor;
        }

        std::string GetName() const {
            return name;
        }


        long RegisterListener(std::string name, Listener & listener);
        void RemoveListener(long id);
        void SendMessage(const Message & m);

        long RegisterTimeout(long time, Listener & listener);
        long RegisterInterval(long time, Listener & listener);
        void RemoveInterval(long id);

        template <class T, typename... _Args>
        T& RegisterThread(_Args&&... __args) {
            return (System::RegisterThread<T>(std::forward<_Args>(__args)...));
        }

        template <class T, typename... _Args>
        T& RegisterExecutor(_Args&&... __args) {
            return (System::RegisterThread<T>(std::forward<_Args>(__args)...));
        }

        Logger & Log() {
            return logger;
        }
    private:
        std::string name;
        MessengerExecutor &executor;
        Logger & logger;
        void Run();
        friend System;

    };
}
#endif	/* SERVICE_H */

