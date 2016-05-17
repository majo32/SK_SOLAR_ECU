/* 
 * File:   Task.h
 * Author: majo32
 *
 * Created on Utorok, 2016, marca 1, 23:14
 */
#pragma once
#ifndef TASK_H
#define	TASK_H



#include "../Util/Runnable.h"

namespace SunStorm {

    class Task : public Runnable {
    public:

        Task() {

        }

        Task(const Task& orig) {

        }

        virtual ~Task() {

        }
        virtual void Run() = 0;
    private:

    };
};
#endif	/* TASK_H */

