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

#include <spdlog/spdlog.h>



namespace SunStorm {
    class System;
    class LoggerManager;

    class Logger {
    private:

        Logger(const std::string name) :
        name(name), logger(spdlog::stdout_logger_mt(name, true)) {
        }

        Logger(const Logger& orig) = delete;

        virtual ~Logger() {

        }
    public:

        /*template<typename... Args>
        spdlog::details::line_logger warn(const char* msg, const Args&... args) {
            return logger->warn(msg, args...);
        }

        template<typename... Args>
        spdlog::details::line_logger error(const char* msg, const Args&... args) {
            return logger->error(msg, args...);
        }

        template<typename... Args>
        spdlog::details::line_logger info(const char* msg, const Args&... args) {
            return logger->info(msg, args...);
        }

        template<typename... Args>
        spdlog::details::line_logger debug(const char* msg, const Args&... args) {
            return logger->debug(msg, args...);
        }
*/
        template<typename T>
        spdlog::details::line_logger warn(const T& msg) {
            return logger->warn(msg);
        }

        template<typename T>
        spdlog::details::line_logger error(const T& msg) {
            return logger->error(msg);
        }

        template<typename T>
        spdlog::details::line_logger info(const T& msg) {
            return logger->info(msg);
        }

        template<typename T>
        spdlog::details::line_logger debug(const T& msg) {
            return logger->debug(msg);
        }

        spdlog::details::line_logger warn() {
            return logger->warn("sds");

        }

        spdlog::details::line_logger error() {
            return logger->error();

        }

        spdlog::details::line_logger info() {
            return logger->info();

        }

        spdlog::details::line_logger debug() {
            return logger->debug();

        }
       

        /* void info(const std::string txt) {
             logger->debug();
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
         }*/

    private:
        const std::string name;
        std::shared_ptr<spdlog::logger> logger;

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
                spdlog::set_level(spdlog::level::debug);
            }

            std::list<LoggerItem> loggers;
            Logger mainLogger;
        };

        static LoggerManagerContext & GetLoggerManagerContext() {
            static LoggerManagerContext ctx("main");
            return ctx;
        }
    };
};
#endif	/* LOGGER_H */

