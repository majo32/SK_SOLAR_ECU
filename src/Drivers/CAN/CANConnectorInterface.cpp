/* 
 * File:   CANConnector.cpp
 * Author: majo32
 * 
 * Created on Utorok, 2016, mája 24, 20:40
 */

#include "CANConnectorInterface.h"

namespace Drivers {
    namespace CAN {
        //CANMessage

        CANMessage::CANMessage() {
            SetData(0, 0, NULL);
        }

        CANMessage::CANMessage(std::uint16_t id, std::uint8_t length, const std::uint8_t* data) {
            SetData(id, length, data);
        }

        const std::uint8_t* CANMessage::GetData() const {
            return data;
        }

        std::uint8_t CANMessage::GetDataAt(std::uint8_t i) const {
            if (i >= CAN_MSG_LEN) {
                return 0;
            }
            return data[i];
        }

        std::uint16_t CANMessage::GetId() const {
            return id;
        }

        std::uint8_t CANMessage::GetLength() const {
            return length;
        }

        void CANMessage::SetData(std::uint16_t id, std::uint8_t length, const std::uint8_t* data) {
            int i;
            if (data != NULL) {
                for (i = 0; i < length; i++) {
                    this->data[i] = data[i];
                }

                for (i = length; i < CAN_MSG_LEN; i++) {
                    this->data[i] = 0;
                }
            } else {
                for (i = 0; i < CAN_MSG_LEN; i++) {
                    this->data[i] = 0;
                }
            }
            this->length = length;
            this->id = id;

        }

        bool CANMessage::IsBitSet(std::uint8_t pos) const {
            std::uint8_t bit = pos / 8;
            std::uint8_t mask = 1 << (pos % 8);
            return !((this->data[bit] & mask) == 0);
        }

        void CANMessage::SetBit(std::uint8_t pos, bool state) {
            std::uint8_t bit = pos / 8;
            std::uint8_t mask = 1 << (pos % 8);
            if (state) {
                this->data[bit] |= mask;
            } else {
                this->data[bit] &= ~mask;
            }
        }

        void CANMessage::SetByte(std::uint8_t pos, std::uint8_t data) {
            this->data[pos] = data;
        }

        //LogMessage

        void LogMessage(SunStorm::Logger & logger, const CANMessage & msg) {
            logger.debug("Can Message : ")
                    << " L [" 
                    << msg.GetLength()
                    << "] ID ["
                    << msg.GetId()
                    << "] "
                    << msg.GetDataAt(0)
                    << ":"
                    << msg.GetDataAt(1)
                    << ":"
                    << msg.GetDataAt(2)
                    << ":"
                    << msg.GetDataAt(3)
                    << ":"
                    << msg.GetDataAt(4)
                    << ":"
                    << msg.GetDataAt(5)
                    << ":"
                    << msg.GetDataAt(6)
                    << ":"
                    << msg.GetDataAt(7);
        }
        //CanConnectorInterface

        CANConnectorInterface::CANConnectorInterface(SunStorm::Logger & logger) : logger(logger) {
        }

        SunStorm::Logger& CANConnectorInterface::Log() {
            return logger;
        }

        CANConnectorInterface::~CANConnectorInterface() {
        }



    }
};
