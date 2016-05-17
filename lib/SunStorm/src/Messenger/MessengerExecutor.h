/* 
 * File:   Executor.h
 * Author: majo32
 * 
 * Created on January 26, 2016, 9:55 PM
 */
#pragma once
#ifndef SUNSTORM_MESSENGEREXECUTOR_H
#define	SUNSTORM_MESSENGEREXECUTOR_H


#include "MessengerTask.h"
#include "../Concurrency/Executor.h"
#include <string>

namespace SunStorm {

    class MessengerExecutor : public Executor<MessengerTask> {
   
    };
}
#endif	/* EXECUTOR_H */

