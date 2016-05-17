/* 
 * File:   ECUGenericListener.h
 * Author: majo32
 *
 * Created on Štvrtok, 2016, apríla 28, 14:10
 */
#pragma once
#ifndef ECU_GENERICLISTENER_H
#define	ECU_GENERICLISTENER_H

#include <SunStorm/Framework.h>

#include <Services/StateMachineService/StateMachineService_messages.h>
#include <Services/StateMachineService/StateMachineService_types.h>

#include "GenericContext.h"

namespace ECU {
    class Service;
    class GenericListener : public SunStorm::ServiceListener<ECU::Service, ECU::GenericContext> {
    public:
        GenericListener(ECU::Service& service, ECU::GenericContext & context) :
        ServiceListener(service, context) {
        }

        void handle(const SunStorm::Message & message) {

            if (message.GetName() == "StateMachineService::REGISTER_SERVICE_REQUEST") {
                Log().debug("Registering to state machine");
                this->handleRegisterServiceRequest(message.GetContent<Messages::StateMachineService::REGISTER_SERVICE_REQUEST>());

            }

            if (message.GetName() == "StateMachineService::CHANGE_TO_STATE_START_TRANSACTION") {
                this->handleChangeStateStartTransaction(message.GetContent<Messages::StateMachineService::CHANGE_TO_STATE_START_TRANSACTION>());

            }
        }

        long AddOnStateChangeFunction(SunStorm::Function<Messages::StateMachineService::CHANGE_TO_STATE_START_TRANSACTION> & fn) {
            return functionComposer.AddFunction(fn);
        }
    protected:

        void handleRegisterServiceRequest(const Messages::StateMachineService::REGISTER_SERVICE_REQUEST & rqst) {
            SunStorm::Message msg("StateMachineService::REGISTER_SERVICE_RESPONSE");
            msg.SetContent(Messages::StateMachineService::REGISTER_SERVICE_RESPONSE({this->GetServiceBase().GetID()}));
            this->SendMessage(msg);
        }

        void handleChangeStateStartTransaction(const Messages::StateMachineService::CHANGE_TO_STATE_START_TRANSACTION & transaction) {
            SunStorm::Message msg("StateMachineService::CHANGE_TO_STATE_CONFIRM");
            msg.SetContent(Messages::StateMachineService::CHANGE_TO_STATE_CONFIRM({
                transaction.prev_state,
                transaction.state,
                transaction.id_transaction,
                this->GetServiceBase().GetID()
            }));
            this->GetContext().SetStates(transaction.state, transaction.prev_state);
            this->functionComposer.Run(transaction);
            this->SendMessage(msg);
        }

    private:
        SunStorm::FunctionComposer<Messages::StateMachineService::CHANGE_TO_STATE_START_TRANSACTION> functionComposer;

    };
}

#endif	/* ECUGENERICLISTENER_H */

