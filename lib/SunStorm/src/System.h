/* 
 * File:   System.h
 * Author: majo32
 *
 * Created on Utorok, 2016, februára 16, 20:09
 */
#pragma once
#ifndef SUNSTORM_SYSTEM_H
#define	SUNSTORM_SYSTEM_H


#include <memory>
#include <list>

#include "Messenger/Messenger.h"
#include "Concurrency/Thread.h"
#include "Clock/Clock.h"
#include "Logging/Logger.h"

namespace SunStorm {
    class Service;

    class System {
    public:

        System() = delete;
        System(const System& orig) = delete;

        template <class T, typename... _Args>
        static T& RegisterThread(_Args&&... __args) {
            T* ptr = new T(std::forward<_Args>(__args)...);
            SunStorm::System::GetDefaultSystemContext().threads.push_back(ptr);
            ptr->Start();
            return *ptr;
        }

        template <class T, typename... _Args>
        static T& RegisterService(_Args&&... __args) {
            T* ptr = new T(std::forward<_Args>(__args)...);
            SunStorm::System::GetDefaultSystemContext().services.push_back(ptr);
            ptr->Run();
            return *ptr;
        }
        static Messenger& GetMessenger();
        static Clock& GetClock();
        static void Initialize();
        static void Join();
    private:

        class SystemContext {
        public:

            SystemContext() : messenger(), clock(2), threads(), services(), logger("__MAIN__"), initialized(false) {
            }
        public:
            Messenger messenger;
            Clock clock;
            std::list<Thread*> threads;
            std::list<Service*> services;
            Logger logger;
            bool initialized;
        };

        static SystemContext& GetDefaultSystemContext() {
            static SystemContext sysCtx;
            return sysCtx;
        }

    };
};
#endif	/* SYSTEM_H */

