/* 
 * File:   Test2Service.h
 * Author: majo32
 *
 * Created on Utorok, 2016, marca 8, 17:58
 */

#ifndef TEST2SERVICE_H
#define	TEST2SERVICE_H
#include <SunStorm/Framework.h>
#include "Test2Service_messages.h"


namespace Services {
    // toto je servis

    class Test2Service : public SunStorm::Service {
    public:

        Test2Service(std::string name) :
        Service(name), context(), l1(*this, context), t1(RegisterThread<Thread1>(*this)) {
        }

        void Initialize();

        class Context {
        };

        class Listener1 : public SunStorm::ServiceListener<Test2Service, Context> {
        public:

            Listener1(Test2Service& service, Context& context) :
            ServiceListener(service, context) {
            }
            void handle(const SunStorm::Message & message);
        };

        class Thread1 : public SunStorm::ServiceThread {
        public:

            Thread1(Service& service) :
            ServiceThread(service) {
            }
        protected:
            void Run();

        };
        
    private:
        Context context;
        Listener1 l1;
        Thread1 & t1;
    };
};
#endif	/* TEST2SERVICE_H */

