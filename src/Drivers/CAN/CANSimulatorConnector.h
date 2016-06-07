/* 
 * File:   CANConnector.h
 * Author: majo32
 *
 * Created on Utorok, 2016, mája 24, 20:40
 */

#ifndef CANSIMULATORCONNECTOR_H
#define	CANSIMULATORCONNECTOR_H

#include "CANConnectorInterface.h"
#include <Socket/inetclientstream.hpp>
#include <Socket/exception.hpp>
#include <memory>
#include <string>
#include <chrono>
#include <sstream>

namespace Drivers {
    namespace CAN {

        class CANSimulatorConnector : public CANConnectorInterface {
        public:

            CANSimulatorConnector(SunStorm::Logger & logger);
            virtual bool Connect();
            virtual bool WaitForMessage(CANMessage& message);
            virtual bool SendMessage(const CANMessage& message);
            virtual bool Disconnect();

        private:
            bool NextConnectAttempt();
            void AssemblyMessageObject(std::string s, CANMessage& message);
            void AssemblyMessageString(std::string & s, const CANMessage& message);
            unsigned int ParseLittleEndian(const std::string & s, std::uint8_t * data);
            unsigned int ParseBigEndian(const std::string & s, std::uint8_t * data);
            
            void AppendLittleEndian(std::string& s, const CANMessage& message);
            void AppendBigEndian(std::string& s, const CANMessage& message);
        private:
            libsocket::inet_stream sock;
        };
    }
};
#endif	/* CANCONNECTOR_H */

