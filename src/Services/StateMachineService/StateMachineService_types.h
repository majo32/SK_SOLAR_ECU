/* 
 * File:   StateMachineService_types.h
 * Author: majo32
 *
 * Created on Streda, 2016, marca 9, 13:35
 */

#ifndef STATEMACHINESERVICE_TYPES_H
#define	STATEMACHINESERVICE_TYPES_H

namespace Types {
    namespace StateMachine {

        enum STATES {
            DEFAULT, BOOT, INIT, NORMAL, STAND_BY, ERROR
        };
    };
};


#endif	/* STATEMACHINESERVICE_TYPES_H */

