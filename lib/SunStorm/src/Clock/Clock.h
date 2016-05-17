/* 
 * File:   Clock.h
 * Author: majo32
 *
 * Created on Utorok, 2016, februára 16, 20:34
 */
#pragma once
#ifndef SUNSTORM_CLOCK_H
#define	SUNSTORM_CLOCK_H


#include <queue>          
#include <vector>         
#include <functional> 
#include <mutex>
#include <chrono>
#include <memory>

#include "../Messenger/Messenger.h"


namespace SunStorm {
    class System;

    class ClockIdentifier {
    public:

        ClockIdentifier(long id) :
        id(id) {
        }

        ClockIdentifier() :
        id(0) {
        }

        long GetId() const {
            return id;
        }

    private:
        long id;
    };

    class ClockMailBox {
    public:

        ClockMailBox(MessengerExecutor* executor, Listener* listener) :
        executor(executor), listener(listener), identifier() {
        }

        ClockMailBox(MessengerExecutor* executor, Listener* listener, ClockIdentifier identifier) :
        executor(executor), listener(listener), identifier(identifier) {
        }

        ClockMailBox(const ClockMailBox& other) :
        executor(other.executor), listener(other.listener), identifier(other.identifier) {
        }

        MessengerExecutor* GetExecutor() const {
            return executor;
        }

        Listener* GetListener() const {
            return listener;
        }

        ClockIdentifier GetIdentifier() const {
            return identifier;
        }

        void AcceptMessage(const Message & m);

    private:
        MessengerExecutor * executor;
        Listener * listener;
        ClockIdentifier identifier;
    };

    class Interval {
    public:

        Interval(long duration) :
        duration(duration), active(true), id(-1) {
        }

        Interval(const Interval& other) :
        duration(other.duration), active(other.active), id(other.id) {
        }

        bool IsActive() const {
            return active;
        }

        const long GetDuration() const {
            return duration;
        }

        long GetId() const {
            return id;
        }

        void SetId(long id) {
            this->id = id;
        }

        void Disable() {
            active = false;
        }

    private:
        const long duration;
        bool active;
        long id;
    };

    class Timeout {
    public:

        Timeout(long time, std::shared_ptr<Interval>interval, ClockMailBox & mailbox) :
        exp(std::chrono::system_clock::now() + std::chrono::milliseconds(time)), interval(interval), _isInterval(true), mailbox(mailbox), id(-1) {
        }

        Timeout(long time, ClockMailBox & mailbox) :
        exp(std::chrono::system_clock::now() + std::chrono::milliseconds(time)), interval(), _isInterval(false), mailbox(mailbox), id(-1) {
        }

        Timeout(const Timeout& other) :
        exp(other.exp), interval(other.interval), _isInterval(other._isInterval), mailbox(other.mailbox), id(other.id) {
        }

        std::chrono::system_clock::time_point GetExp() const {
            return exp;
        }

        void ReuseInFuture() {
            if (IsInterval()) {
                exp = std::chrono::system_clock::now() + std::chrono::milliseconds(this->interval.get()->GetDuration());
            }
        }

        std::shared_ptr<Interval> GetInterval() const {
            return interval;
        }

        ClockMailBox & GetMailbox() {
            return mailbox;
        }

        long GetId() const {
            return id;
        }

        void SetId(long id) {
            this->id = id;
        }

        bool IsInterval() const {
            return _isInterval;
        }


    private:
        std::chrono::system_clock::time_point exp;
        std::shared_ptr<Interval> interval;
        bool _isInterval;
        ClockMailBox mailbox;
        long id;
    };

    class Clock {
    public:

        Clock(long heart_beat) : scheduler(), mutex(), counter(0), heart_beat(heart_beat) {
        }

        Clock(const Clock& orig) = delete;

        virtual ~Clock() {
        }

        long RegisterTimeout(long time, MessengerExecutor & executor, Listener & listener);
        long RegisterInterval(long time, MessengerExecutor & executor, Listener & listener);
        long RegisterTimeout(long time, MessengerExecutor & executor, Listener & listener, long id);
        long RegisterInterval(long time, MessengerExecutor & executor, Listener & listener, long id);
        void StopInterval(long id);

        class TimeoutComparison {
        public:

            bool operator()(const Timeout& lhs, const Timeout&rhs) const {
                return (lhs.GetExp() > rhs.GetExp());
            }
        };
        typedef std::priority_queue<Timeout, std::vector<Timeout>, TimeoutComparison> clock_heap;

        class ClockThread : public Thread {
        public:

            ClockThread(Clock& clock) :
            Thread(), clock(clock) {
            }
        private:
            void Run();
            Clock & clock;
        };

        class ClockResponse {
        public:

            ClockResponse(ClockIdentifier identifier, std::chrono::system_clock::time_point time) :
            identifier(identifier), time(time) {
            }

            ClockIdentifier GetIdentifier() const {
                return identifier;
            }

            std::chrono::system_clock::time_point GetTime() const {
                return time;
            }

        private:
            ClockIdentifier identifier;
            std::chrono::system_clock::time_point time;
        };
    private:
        clock_heap scheduler;
        std::mutex mutex;
        long counter;
        long heart_beat;
        std::list<std::shared_ptr<Interval>> intervals;

        void Tick();
        const Message CreateMessage(Timeout & tout);


    };
};
#endif	/* CLOCK_H */

