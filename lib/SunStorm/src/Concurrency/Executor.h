/* 
 * File:   Executor.h
 * Author: majo32
 * 
 * Created on January 26, 2016, 9:55 PM
 */
#pragma once
#ifndef SUNSTORM_EXECUTOR_H
#define	SUNSTORM_EXECUTOR_H



#include "../Util/BufferQueue.h"
#include "../Util/Runnable.h"
#include "Thread.h"
#include <string>
#include <iostream>

namespace SunStorm {

    template<class T>
    class Executor : public Thread {
    public:

        Executor() : Thread(), queue() {

        }

        virtual ~Executor() {

        }

        std::string GetName() const {
            return name;
        }

        void SetName(std::string name) {
            this->name = name;
        }

        void EnqueueTask(const T & task) {
            queue.push(task);
        }

    private:
        std::string name;
        BufferQueue<T> queue;

        void Run() {
            T t;
            while (queue.waitAndPop(t)) {
                try {
                    t.Run();
                } catch (...) {
                    std::cout << "Fatality in executor !!!";
                }
            }
        }
    };
}
#endif	/* EXECUTOR_H */

