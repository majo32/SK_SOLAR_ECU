/* 
 * File:   StateMachineService_messages.h
 * Author: majo32
 *
 * Created on Streda, 2016, marca 9, 13:30
 */

#ifndef STATEMACHINESERVICE_MESSAGES_H
#define	STATEMACHINESERVICE_MESSAGES_H

#include "StateMachineService_types.h"

namespace Messages {
    namespace StateMachineService {
        struct REGISTER_SERVICE_RESPONSE{
            long id_service;
        };
        struct REGISTER_SERVICE_REQUEST{
            long a;
        };
        /**
         * 
         */
        struct CHANGE_TO_STATE_REQUEST {
            Types::StateMachine::STATES state;
        };

        struct CHANGE_TO_STATE_START_TRANSACTION {
            Types::StateMachine::STATES prev_state;
            Types::StateMachine::STATES state;
            long id_transaction;
        };

        struct CHANGE_TO_STATE_CONFIRM {
            Types::StateMachine::STATES prev_state;
            Types::StateMachine::STATES state;
            long id_transaction;
            long id_service;

        };
        
        struct CHANGE_TO_STATE_REJECT {
            Types::StateMachine::STATES prev_state;
            Types::StateMachine::STATES state;
            long id_transaction;
            long id_service;
        };

        struct CHANGE_TO_STATE_COMMIT_TRANSACTION {
            Types::StateMachine::STATES prev_state;
            Types::StateMachine::STATES state;
            long id_transaction;
        };
        
        struct CHANGE_TO_STATE_ROLLBACK_TRANSACTION {
            Types::StateMachine::STATES prev_state;
            Types::StateMachine::STATES state;
            long id_transaction;
        };


    };
};

#endif	/* STATEMACHINESERVICE_MESSAGES_H */

