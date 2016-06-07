/* 
 * File:   CANConnector.cpp
 * Author: majo32
 * 
 * Created on Utorok, 2016, mája 24, 20:40
 */

#include "CANConnector.h"

namespace Drivers {
    namespace CAN {

        CANConnector::CANConnector(SunStorm::Logger& logger, const std::string name) :
        CANConnectorInterface(logger),
        socket(name.c_str()) {

        }

        bool CANConnector::Connect() {
            socket.Connect();
            return true;
        }

        bool CANConnector::SendMessage(const CANMessage& message) {
            socket.SendMessage(message);
            return true;
        }

        bool CANConnector::WaitForMessage(CANMessage& message) {
            socket.WaitForMessage(message);
            return true;
        }

        bool CANConnector::Disconnect() {
            socket.Disconnect();
            return true;
        }

        CANConnector::~CANConnector() {

        }

    }
};
