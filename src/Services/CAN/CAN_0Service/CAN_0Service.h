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
#include <Drivers/CAN/CANConnectorInterface.h>
#include "CAN_0Service_messages.h"
#include <iostream>
#include <chrono>

namespace Services {
    namespace CAN {

        class CAN_0Service : public ECU::Service {
        public:

            class Context : public ECU::GenericContext {
            public:

                class DecomposerContext {
                };

                class ComposerContext {
                public:

                    template<typename T>
                    class ResponseSegment {
                    public:

                        ResponseSegment() :
                        changed(), value(false) {
                        }

                        void Change(T value) {
                            this->value = value;
                            this->changed = std::chrono::system_clock::now();
                        }

                        std::chrono::system_clock::time_point GetChanged() const {
                            return changed;
                        }

                        T Value() const {
                            return value;
                        }

                    private:
                        std::chrono::system_clock::time_point changed;
                        T value;
                    };

#define CU_RESPONSE_SEGMENTS_NAMES_COUNT 6

                    enum CUResponseSegmentsNames {
                        Breake,
                        Horn,
                        FrontLights,
                        LeftBlinker,
                        RightBlinker,
                        WarningLights
                    };
                    ResponseSegment<bool> CUResponseSegments[CU_RESPONSE_SEGMENTS_NAMES_COUNT];
                    ResponseSegment<std::uint8_t> speedRqst;
                    ResponseSegment<bool> autopilot;
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

            CAN_0Service(std::string name, Drivers::CAN::CANConnectorInterface & connector) :
            Service(name, context),
            context(),
            composer(*this),
            decomposer(*this),
            decomposerThread(
            SunStorm::System::RegisterThread<CAN_0Service::DecomposerThread>(
            *this,
            decomposer,
            connector
            )),
            connector(connector) {
            }

            Context & GetContext() {
                return context;
            }

            Drivers::CAN::CANConnectorInterface& GetConnector() {
                return connector;
            }


        protected:

            void RegisterListeners() {
                composer.RegisterListeners();
            }


        public:

            class Component {
            public:

                Component(CAN_0Service& service) : service(service), context(service.GetContext()) {
                }


                Component(const Component& orig) = delete;

                Context& GetContext() {
                    return context;
                }

                CAN_0Service& GetService() {
                    return service;
                }


            private:
                CAN_0Service & service;
            protected:
                Context & context;
            };

            class Composer : public Component {
            public:

                class ComposerListener : public ECU::Listener<CAN_0Service, Context> {
                public:

                    ComposerListener(CAN_0Service & service, Context & context) : ECU::Listener<CAN_0Service, Context>(service, context) {
                    }

                    void handle(const SunStorm::Message& message) {
                        if (message.GetName() == "CAN::CAN_0Service::FrontLightResponse") {
                            handleFrontLight(message.GetContent<Messages::CAN::CAN_0Service::FrontLightResponse>());
                        }

                        if (message.GetName() == "CAN::CAN_0Service::BlinkerResponse") {
                            handleBlinker(message.GetContent<Messages::CAN::CAN_0Service::BlinkerResponse>());
                        }

                        if (message.GetName() == "CAN::CAN_0Service::HornResponse") {
                            handleHorn(message.GetContent<Messages::CAN::CAN_0Service::HornResponse>());
                        }

                        if (message.GetName() == "CAN::CAN_0Service::BreakLightResponse") {
                            handleBreakLight(message.GetContent<Messages::CAN::CAN_0Service::BreakLightResponse>());
                        }

                        if (message.GetName() == "CAN::CAN_0Service::SpeedRqstResponse") {
                            handleSpeedRqst(message.GetContent<Messages::CAN::CAN_0Service::SpeedRqstResponse>());
                        }

                        if (message.GetName() == "CAN::CAN_0Service::AutopilotResponse") {
                            handleAutopilotStatus(message.GetContent<Messages::CAN::CAN_0Service::AutopilotResponse>());
                        }
                    }

                    void handleSpeedRqst(Messages::CAN::CAN_0Service::SpeedRqstResponse response) {
                        GetContext().GetComposerContext().speedRqst.Change(response.value);
                    }

                    void handleAutopilotStatus(Messages::CAN::CAN_0Service::AutopilotResponse response) {
                        GetContext().GetComposerContext().autopilot.Change(response.status);
                    }

                    void handleFrontLight(Messages::CAN::CAN_0Service::FrontLightResponse response) {
                        GetContext().GetComposerContext().CUResponseSegments[
                                CAN_0Service::Context::ComposerContext::CUResponseSegmentsNames::FrontLights
                                ].Change(response.state);

                    }

                    void handleBlinker(Messages::CAN::CAN_0Service::BlinkerResponse response) {
                        GetContext().GetComposerContext().CUResponseSegments[
                                CAN_0Service::Context::ComposerContext::CUResponseSegmentsNames::LeftBlinker
                                ].Change(response.left);
                        GetContext().GetComposerContext().CUResponseSegments[
                                CAN_0Service::Context::ComposerContext::CUResponseSegmentsNames::RightBlinker
                                ].Change(response.right);
                    }

                    void handleHorn(Messages::CAN::CAN_0Service::HornResponse response) {
                        GetContext().GetComposerContext().CUResponseSegments[
                                CAN_0Service::Context::ComposerContext::CUResponseSegmentsNames::Horn
                                ].Change(response.state);
                    }

                    void handleBreakLight(Messages::CAN::CAN_0Service::BreakLightResponse response) {
                        GetContext().GetComposerContext().CUResponseSegments[
                                CAN_0Service::Context::ComposerContext::CUResponseSegmentsNames::Breake
                                ].Change(response.state);
                    }

                };

                class ComposerCUTimerListener : public ECU::Listener<CAN_0Service, Context> {
                public:

                    ComposerCUTimerListener(CAN_0Service & service, Context & context) : ECU::Listener<CAN_0Service, Context>(service, context) {
                    }

                    void handle(const SunStorm::Message& message) {
                        GetService().composer.Send0x11ToCan();
                    }

                };

                class ComposerSpeedRqstTimerListener : public ECU::Listener<CAN_0Service, Context> {
                public:

                    ComposerSpeedRqstTimerListener(CAN_0Service & service, Context & context) : ECU::Listener<CAN_0Service, Context>(service, context) {
                    }

                    void handle(const SunStorm::Message& message) {
                        GetService().composer.Send0x30ToCan();
                        GetService().composer.Send0x31ToCan();
                    }

                };

                void RegisterListeners() {
                    this->GetService().RegisterInterval(1000, CUtimerListener);
                    this->GetService().RegisterInterval(1000, speedRqstTimer);
                    this->GetService().RegisterListener("CAN::CAN_0Service::FrontLightResponse", listener);
                    this->GetService().RegisterListener("CAN::CAN_0Service::BlinkerResponse", listener);
                    this->GetService().RegisterListener("CAN::CAN_0Service::HornResponse", listener);
                    this->GetService().RegisterListener("CAN::CAN_0Service::BreakLightResponse", listener);
                    this->GetService().RegisterListener("CAN::CAN_0Service::SpeedRqstResponse", listener);
                }
            public:

                Composer(CAN_0Service& service) :
                Component(service),
                CUtimerListener(service, service.GetContext()),
                speedRqstTimer(service, service.GetContext()),
                listener(service, service.GetContext()) {
                }

                void Send0x11ToCan() {
                    Drivers::CAN::CANMessage msg(0x11, 8, NULL);

                    
                    msg.SetBit(1, GetContext().GetComposerContext().CUResponseSegments[Context::ComposerContext::CUResponseSegmentsNames::Breake].Value());
                    msg.SetBit(2, GetContext().GetComposerContext().CUResponseSegments[Context::ComposerContext::CUResponseSegmentsNames::Horn].Value());
                    msg.SetBit(3, GetContext().GetComposerContext().CUResponseSegments[Context::ComposerContext::CUResponseSegmentsNames::FrontLights].Value());
                    msg.SetBit(4, GetContext().GetComposerContext().CUResponseSegments[Context::ComposerContext::CUResponseSegmentsNames::LeftBlinker].Value());
                    msg.SetBit(5, GetContext().GetComposerContext().CUResponseSegments[Context::ComposerContext::CUResponseSegmentsNames::RightBlinker].Value());
                    
                    msg.SetBit(7, true);
                    
                    
                    GetService().connector.SendMessage(msg);
                }

                void Send0x30ToCan() {
                    Drivers::CAN::CANMessage msg(0x30, 1, NULL);
                    
                    msg.SetByte(0, GetContext().GetComposerContext().speedRqst.Value());
                    GetService().connector.SendMessage(msg);
                }

                void Send0x31ToCan() {
                    Drivers::CAN::CANMessage msg(0x31, 1, NULL);
                    
                    msg.SetBit(0, GetContext().GetComposerContext().autopilot.Value());
                    msg.SetBit(1, true);
                    GetService().connector.SendMessage(msg);
                }

            private:

                ComposerCUTimerListener CUtimerListener;
                ComposerSpeedRqstTimerListener speedRqstTimer;
                ComposerListener listener;

            };

            class Decomposer : public Component {
            public:

                class DecomposerListener : public ECU::Listener<CAN_0Service, Context> {
                public:

                    DecomposerListener(CAN_0Service & service, Context & context) : ECU::Listener<CAN_0Service, Context>(service, context) {
                    }

                };
            public:

                Decomposer(CAN_0Service& service) :
                Component(service) {
                }

                void ParseMessage(Drivers::CAN::CANMessage & message) {
                    switch (message.GetId()) {
                        case 0x10:
                            ParseMessage0x10(message);
                            break;
                        case 0x21:
                            ParseMessage0x21(message);
                            break;
                        case 0x22:
                            ParseMessage0x22(message);
                            break;
                        case 0x23:
                            ParseMessage0x23(message);
                            break;
                    }
                };
            private:

                void ParseMessage0x10(Drivers::CAN::CANMessage & message) {
                    //GetService().Log().debug("Parsing 10 : ") << message.GetDataAt(0);
                    Messages::CAN::CAN_0Service::ControlUnitRequest cuRequest({
                        message.IsBitSet(1),
                        message.IsBitSet(2),
                        message.IsBitSet(3),
                        message.IsBitSet(4),
                        message.IsBitSet(9),
                        message.IsBitSet(10),
                        message.IsBitSet(11),
                        message.IsBitSet(12),
                        message.IsBitSet(13),
                        message.IsBitSet(17)
                    });
                    SunStorm::Message msg("CAN::CAN_0Service::ControlUnitRequest");
                    msg.SetContent<Messages::CAN::CAN_0Service::ControlUnitRequest>(cuRequest);
                    this->GetService().SendMessage(msg);
                }

                void ParseMessage0x21(Drivers::CAN::CANMessage & message) {
                    //GetService().Log().debug("Parsing 21 : ") << message.GetDataAt(0);
                    Messages::CAN::CAN_0Service::SpeedRqstRequest speedRequest({
                        message.GetDataAt(0)
                    });
                    SunStorm::Message msg("CAN::CAN_0Service::SpeedRqstRequest");
                    msg.SetContent<Messages::CAN::CAN_0Service::SpeedRqstRequest>(speedRequest);
                    this->GetService().SendMessage(msg);
                }

                void ParseMessage0x22(Drivers::CAN::CANMessage & message) {
                    //GetService().Log().debug("Parsing 22 : ") << message.GetDataAt(0);
                    Messages::CAN::CAN_0Service::SpeedActRequest speedRequest({
                        message.GetDataAt(0)
                    });
                    SunStorm::Message msg("CAN::CAN_0Service::SpeedActRequest");
                    msg.SetContent<Messages::CAN::CAN_0Service::SpeedActRequest>(speedRequest);
                    this->GetService().SendMessage(msg);
                }

                void ParseMessage0x23(Drivers::CAN::CANMessage & message) {
                    //GetService().Log().debug("Parsing 23 : ") << message.GetDataAt(0);
                    Messages::CAN::CAN_0Service::SpeedOptRequest speedRequest({
                        message.GetDataAt(0)
                    });
                    SunStorm::Message msg("CAN::CAN_0Service::SpeedOptRequest");
                    msg.SetContent<Messages::CAN::CAN_0Service::SpeedOptRequest>(speedRequest);
                    this->GetService().SendMessage(msg);
                }

            };

            class DecomposerThread : public SunStorm::ServiceThread {
            public:

                DecomposerThread(Service& service, Services::CAN::CAN_0Service::Decomposer & decomposer, Drivers::CAN::CANConnectorInterface & connector) :
                ServiceThread(service), decomposer(decomposer), connector(connector) {
                }

                void Run() {
                    Drivers::CAN::CANMessage msg;
                    while (connector.WaitForMessage(msg)) {
                        decomposer.ParseMessage(msg);
                    }

                }
            private:
                Services::CAN::CAN_0Service::Decomposer & decomposer;
                Drivers::CAN::CANConnectorInterface & connector;

            };

        private:
            Context context;
            Composer composer;
            Decomposer decomposer;
            DecomposerThread & decomposerThread;
            Drivers::CAN::CANConnectorInterface & connector;
        };
    }
}

#endif	/* CAN_0SERVICE_H */

