/* 
 * File:   Logger.h
 * Author: majo32
 *
 * Created on Sobota, 2016, marca 5, 14:48
 */
#pragma once
#ifndef SUNSTORM_LOGGER_H
#define	SUNSTORM_LOGGER_H
#include <string>
#include <chrono>
#include <iostream>
#include <ctime>
#include <list>
#include <stdexcept>

namespace SunStorm {
    class System;
    class LoggerManager;

    class Logger {
    private:

        Logger(std::string name) :
        name(name) {
        }

        Logger(const Logger& orig) = delete;

        virtual ~Logger() {

        }
    public:

        void info(const std::string txt) {
            std::cout << nowLongVal() << " " << name << " INFO " << txt << std::endl;
        }

        void warn(const std::string txt) {
            std::cout << nowLongVal() << " " << name << " WARN " << txt << std::endl;
        }

        void error(const std::string txt) {
            std::cout << nowLongVal() << " " << name << " ERROR " << txt << std::endl;
        }

        void debug(const std::string txt) {
            std::cout << nowLongVal() << " " << name << " DEBUG " << txt << std::endl;
        }

    private:
        std::string name;

        long nowLongVal() {
            auto now = std::chrono::system_clock::now();
            auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
            auto epoch = now_ms.time_since_epoch();
            auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
            long duration = value.count();
            return duration;
        }
        friend System;
        friend LoggerManager;
    };

    class LoggerManager {
    public:

        static Logger & RegisterLogger(std::string name) {
            Logger * logger = new Logger(name);
            LoggerManager::GetLoggerManagerContext().loggers.push_back(LoggerItem(logger, name));
            return *logger;
        }

        static Logger & GetLogger(std::string name) {
            auto it = LoggerManager::GetLoggerManagerContext().loggers.begin();
            while (it != LoggerManager::GetLoggerManagerContext().loggers.end()) {
                if (it->GetName() == name) {
                    return *it->GetLogger();
                }
                it++;
            }
            throw std::invalid_argument("Logger does not exist" + name);
        }

        static Logger & GetMainLogger() {
            return LoggerManager::GetLoggerManagerContext().mainLogger;
        }
    private:

        class LoggerItem {
        public:

            LoggerItem(Logger* logger, std::string name) :
            logger(logger), name(name) {
            }

            Logger* GetLogger() const {
                return logger;
            }

            std::string GetName() const {
                return name;
            }

        private:
            Logger * logger;
            std::string name;
        };

        class LoggerManagerContext {
        public:

            LoggerManagerContext(std::string loggerName) :
            loggers(), mainLogger(loggerName) {
            }

            std::list<LoggerItem> loggers;
            Logger mainLogger;
        };

        static LoggerManagerContext & GetLoggerManagerContext() {
            static LoggerManagerContext ctx("__MAIN__");
            return ctx;
        }
    };
};
#endif	/* LOGGER_H */

