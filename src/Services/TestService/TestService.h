/* 
 * File:   TestService.h 4445
 * Author: majo32
 *
 * Created on Utorok, 2016, februára 16, 15:58
 */

#ifndef TESTSERVICE_H
#define	TESTSERVICE_H

#include <SunStorm/Framework.h>


namespace Services {

    /**
     * Testovací service
     * robí .....
     */
    class TestService : public SunStorm::Service {
    public:
        
        
        
        /**
         * zavola sa konstruktor parenta (SunStorm::Service(ex, messenger)) - parametre dostane TestService,
         * inicializuju sa jednotlive parametre classy ( ctx({0,0}), l1(*this,ctx), l2(*this,ctx) )
         * @param ex
         * @param messenger
         */
        TestService(std::string name) :
        SunStorm::Service(name), ctx({0,0}), l1(*this,ctx), l2(*this,ctx), l3(*this,ctx) {
           
        }

        virtual ~TestService() {
        }
        void Initialize(); // pozri cpp


        //////////////////// Definícia vnorených tried
    public:

        /**
         * Kontext pomocou ktorého si listenry predávajú data vnútri servicu ( zdielaju ho medzi sebou )       
         */
        class Context {
        public:
            int a;
            int b;
            int c;
        };

        /**
         * Generická trieda od ktorej dedia všetky listenery pre tento service
         * parentovy predám konkrétny service
         * context predávam ako referenciu
         */
        class ListenerBase : public SunStorm::ServiceListener<TestService,Context> {
        public:

            ListenerBase(TestService & t,Context & ctx) :
            ServiceListener(t,ctx), ctx(ctx) {
            }
        protected:
            Context & ctx;
            
        };

        /**
         * Konkrétne instancie listenerov dedia od ListenerBase
         * Počuva na správu ...
         * robí...
         */
        class Listener_1 : public ListenerBase {
        public:

            Listener_1(TestService & t,Context& ctx) :
            ListenerBase(t,ctx) {
            }
            
            void handle(const SunStorm::Message & message) ; // pozri cpp
        };
        
        
        class Listener_3 : public ListenerBase {
        public:

            Listener_3(TestService & t,Context& ctx) :
            ListenerBase(t,ctx) {
            }
            
            void handle(const SunStorm::Message & message) ; // pozri cpp
        };
        
        /**
         * Počuva na správu ...
         * robí...
         */
        class Listener_2 : public ListenerBase {
        public:

            Listener_2(TestService & t,Context& ctx) :
            ListenerBase(t,ctx) {
            }
            void handle(const SunStorm::Message & message) ;
        };

        /**
         * struktúra správy
         */
        struct MSG_A {
            int k;
            int l;
        };
    ///// paramtre classy, konkrétne instancie , co service bude obsahovat
    private:
        Context ctx;
        Listener_1 l1; // vytvorenie objektu vnorenej triedy
        Listener_2 l2;
        Listener_3 l3;
    };

}
#endif	/* TESTSERVICE_H */

