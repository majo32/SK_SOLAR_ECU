/* 
 * File:   CANConnector.h
 * Author: majo32
 *
 * Created on Utorok, 2016, mája 24, 20:40
 */

#ifndef CANCONNECTOR_H
#define	CANCONNECTOR_H

#include "CANConnectorInterface.h"
#include <Connectors/CanSocket/Socket.h>
namespace Drivers {
    namespace CAN {
        class CANConnector : public CANConnectorInterface{
        public:
            CANConnector(SunStorm::Logger & logger, const std::string name);
            virtual bool WaitForMessage(CANMessage& message);
            virtual bool SendMessage(const CANMessage& message);
            virtual bool Connect();
            virtual bool Disconnect();
            virtual ~CANConnector();

        private:
            Connectors::CanSocket::Socket socket;
        };
    }
};
#endif	/* CANCONNECTOR_H */

