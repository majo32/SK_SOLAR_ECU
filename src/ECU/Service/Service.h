/* 
 * File:   ECUCommon.h
 * Author: majo32
 *
 * Created on Streda, 2016, marca 9, 13:41
 */
#pragma once
#ifndef ECU_SERVICE_H
#define	ECU_SERVICE_H

#include <SunStorm/Framework.h>

#include "GenericListener.h"
#include <Services/StateMachineService/StateMachineService_messages.h>
#include <Services/StateMachineService/StateMachineService_types.h>

namespace ECU {

    class Service : public SunStorm::Service {
    public:

        Service(std::string name) :
        SunStorm::Service(name), genericContext(), genericListener(*this, genericContext) {
        }

        void Initialize() {
            this->RegisterListener("StateMachineService::REGISTER_SERVICE_REQUEST", genericListener);
            this->RegisterListener("StateMachineService::CHANGE_TO_STATE_START_TRANSACTION", genericListener);
            this->RegisterListeners();
        }

        virtual void RegisterListeners() = 0;
    
    protected:

        GenericListener & GetGenericListener() {
            return genericListener;
        }


    private:
        GenericContext genericContext;
        GenericListener genericListener;

    };
}


#endif	/* ECUCOMMON_H */

