/* 
 * File:   CAN_0Service.h
 * Author: majo32
 *
 * Created on Utorok, 2016, mája 24, 18:59
 */
#pragma once
#ifndef CAN_0SERVICE_H
#define	CAN_0SERVICE_H


#include <SunStorm/Framework.h>
#include <ECU/Common.h>
#include "CAN_0Service_messages.h"


namespace Services {
    namespace CAN {

        class CAN_0Service : public ECU::Service {
        public:

            class Context : public ECU::GenericContext {
            public:

                class DecomposerContext {
                };

                class ComposerContext {
                };

                Context() :
                GenericContext(), decomposerContext(), composerContext() {
                };

                ComposerContext & GetComposerContext() {
                    return composerContext;
                }

                DecomposerContext & GetDecomposerContext() {
                    return decomposerContext;
                }

            private:
                DecomposerContext decomposerContext;
                ComposerContext composerContext;


            };

        public:

            CAN_0Service(std::string name) :
            ECU::Service(name, context), context(), composer(*this), decomposer(*this) {
            }

            Context & GetContext() {
                return context;
            }

        protected:

            void RegisterListeners() {
            }


        public:

            class Component {
            public:

                Component(CAN_0Service& service);


                Component(const Component& orig) = delete;

                Context& GetContext() {
                    return context;
                }

                CAN_0Service& GetService() {
                    return service;
                }


            private:
                CAN_0Service & service;
                Context & context;
            };

            class Composer : public Component {
            public:

                class ComposerListener : public ECU::Listener<CAN_0Service, Context> {
                public:


                };
            public:

                Composer(CAN_0Service& service) :
                Component(service) {
                }

            };

            class Decomposer : public Component {
            public:

                class DecomposerListener : public ECU::Listener<CAN_0Service, Context> {
                public:


                };
            public:

                Decomposer(CAN_0Service& service) :
                Component(service) {
                }

            };
            
            class DecomposerThread : public SunStorm::ServiceThread{
                DecomposerThread(Service& service) :
                ServiceThread(service) {
                }

            };

        private:
            Context context;
            Composer composer;
            Decomposer decomposer;
        };
    }
}

#endif	/* CAN_0SERVICE_H */

