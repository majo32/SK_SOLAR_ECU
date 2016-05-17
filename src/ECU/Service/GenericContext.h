/* 
 * File:   ECUGenericContext.h
 * Author: majo32
 *
 * Created on Štvrtok, 2016, apríla 28, 14:12
 */

#pragma once
#ifndef ECU_GENERICCONTEXT_H
#define	ECU_GENERICCONTEXT_H

#include <SunStorm/Framework.h>

#include <Services/StateMachineService/StateMachineService_types.h>

namespace ECU {
    class GenericListener;

    class GenericContext {
    public:
        
        GenericContext() : state(Types::StateMachine::STATES::DEFAULT),prevState(Types::StateMachine::STATES::DEFAULT){
        }

        
        GenericContext(const GenericContext& other) = delete;

        Types::StateMachine::STATES GetPrevState() const {
            return prevState;
        }

        Types::StateMachine::STATES GetState() const {
            return state;
        }


    protected:
        friend class GenericListener;

        void SetStates(Types::StateMachine::STATES state, Types::StateMachine::STATES prev_state) {
            this->state = state;
            this->prevState = prev_state;
        }

        Types::StateMachine::STATES state;
        Types::StateMachine::STATES prevState;
    };
}


#endif	/* ECUGENERICCONTEXT_H */

