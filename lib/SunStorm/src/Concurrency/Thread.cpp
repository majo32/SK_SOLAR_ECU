/* 
 * File:   Thread.cpp
 * Author: majo32
 * 
 * Created on Utorok, 2016, februára 16, 13:57
 */

#include "Thread.h"

void SunStorm::Thread::Join() {
    if (started) {
        thread.join();
    }
}

void SunStorm::Thread::Start() {
    if (!started) {
        thread = std::thread(&SunStorm::Thread::ThreadFn, this);
        started = true;
    }
}

void SunStorm::Thread::ThreadFn() {
    this->Run();
}
