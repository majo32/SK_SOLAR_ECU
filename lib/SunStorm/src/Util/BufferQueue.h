/* 
 * File:   BufferQueue.h
 * Author: majo32
 *
 * Created on January 26, 2016, 9:59 PM
 */
#pragma once
#ifndef SUNSTORM_BUFFERQUEUE_H
#define	SUNSTORM_BUFFERQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

namespace SunStorm {

    template<class T>
    class BufferQueue {
    public:

        BufferQueue() : queue(), mutex(), cond(), status(1)  {

        }

        virtual ~BufferQueue() {

        }

        int waitAndPop(T& out) {
            std::unique_lock<std::mutex> lock(mutex);
            while (queue.empty()) {
                cond.wait(lock);
            }
            out = queue.front();
            queue.pop();
            return status;
        }

        void push(const T& in) {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(in);
            cond.notify_one();
        }

        long size() {
            std::lock_guard<std::mutex> lock(mutex);
            long l = queue.size();
            return l;
        }

    private:
        std::queue <T> queue;
        std::mutex mutex;
        std::condition_variable cond;
        int status;
    };
}
#endif	/* BUFFERQUEUE_H */

