/* 
 * File:   CANConnectorInterface.h
 * Author: majo32
 *
 * Created on June 2, 2016, 11:34 AM
 */
#pragma once
#ifndef CANCONNECTORINTERFACE_H
#define	CANCONNECTORINTERFACE_H

#include <cstdint>
#include <chrono>
#include <SunStorm/Framework.h>


#define CAN_MSG_LEN 8

namespace Drivers {
    namespace CAN {

        class CANMessage {
        public:
            CANMessage();
            CANMessage(std::uint16_t id, std::uint8_t length, const std::uint8_t * data);
            void SetData(std::uint16_t id, std::uint8_t length, const std::uint8_t * data);
            bool IsBitSet(std::uint8_t pos)const;
            void SetBit(std::uint8_t pos, bool state);
            void SetByte(std::uint8_t pos, std::uint8_t data);
            std::uint16_t GetId() const;
            std::uint8_t GetLength() const;
            std::uint8_t GetDataAt(std::uint8_t i) const;
            const std::uint8_t * GetData() const;
        private:
            std::uint16_t id;
            std::uint8_t length;
            std::uint8_t data[CAN_MSG_LEN];
        };

        class CANConnectorInterface {
        public:
            CANConnectorInterface(SunStorm::Logger & logger);
            CANConnectorInterface(const CANConnectorInterface & orig) = delete;
            virtual bool Connect() = 0;
            virtual bool WaitForMessage(CANMessage & message) = 0;
            virtual bool SendMessage(const CANMessage & message) = 0;
            virtual bool Disconnect() = 0;
            SunStorm::Logger & Log();
            virtual ~CANConnectorInterface();
        private:
            SunStorm::Logger & logger;
        };
    };
};
#endif	/* CANCONNECTORINTERFACE_H */

