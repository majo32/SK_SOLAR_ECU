/* 
 * File:   Thread.h
 * Author: majo32
 *
 * Created on Utorok, 2016, februára 16, 13:57
 */
#pragma once
#ifndef SUNSTORM_THREAD_H
#define	SUNSTORM_THREAD_H



#include <thread>


namespace SunStorm {

    class Thread {
    public:

        Thread() : started(false), thread() {

        }

        Thread(const Thread& orig) = delete;

        virtual ~Thread() {
        }
        void Start(void);
        void Join(void);
    protected:
        virtual void Run() = 0;
    private:
        bool started;
        std::thread thread;
        
        void ThreadFn();
    };
}
#endif	/* THREAD_H */

