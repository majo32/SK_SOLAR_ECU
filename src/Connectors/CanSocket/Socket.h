/* 
 * File:   Socket.h
 * Author: majo32
 *
 * Created on June 7, 2016, 5:53 PM
 */

#ifndef SOCKET_H
#define	SOCKET_H

#include <string>
#include <Drivers/CAN/CANConnectorInterface.h>
#include "can_socket.h"
namespace Connectors {
    namespace CanSocket {

        struct Exception {
            std::string reason;

            Exception(const std::string & r) {
                reason = r;
            };
        };

        class Socket {
        public:

            Socket(const char * name) : name(name) {
            }
            Socket(const Socket& orig) = delete;

            virtual ~Socket() {
            }

            void Connect() {
                int error;
                socket = can_socket_start_socket(name, &error);
            }

            void WaitForMessage(Drivers::CAN::CANMessage & msg) {
                int id, len, error;
                std::uint8_t data [CAN_MSG_LEN];
                can_socket_wait_for_msg(socket, &id, &len, data, &error);
                msg.SetData(id, len, data);
            }

            void SendMessage(const Drivers::CAN::CANMessage & msg) {
                int error;
                //std::uint8_t data [CAN_MSG_LEN];
                can_socket_send_msg(socket, msg.GetId(), msg.GetLength(), msg.GetData(), &error);
            }

            void Disconnect() {
                int error;
                can_socket_destroy_socket(socket, &error);
            }
        private:
            const char * name;
            int socket;
        };
    };
};
#endif	/* SOCKET_H */

