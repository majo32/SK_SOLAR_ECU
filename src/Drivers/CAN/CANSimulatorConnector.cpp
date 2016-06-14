/* 
 * File:   CANConnector.cpp
 * Author: majo32
 * 
 * Created on Utorok, 2016, mája 24, 20:40
 */
#include "CANSimulatorConnector.h"

#define CAN_SIMULATOR_SERVER "localhost"
#define CAN_SIMULATOR_SERVER_PORT "12222"
namespace Drivers {
    namespace CAN {

        CANSimulatorConnector::CANSimulatorConnector(SunStorm::Logger& logger) : CANConnectorInterface(logger), sock() {

        }

        bool CANSimulatorConnector::Connect() {
            try {
                sock.connect(CAN_SIMULATOR_SERVER, CAN_SIMULATOR_SERVER_PORT, LIBSOCKET_IPv4);
                Log().info("Connected");
            } catch (const libsocket::socket_exception& exc) {
                Log().error("Error while connecting") << exc.mesg;
                return false;
            }
            return true;
        }

        bool CANSimulatorConnector::SendMessage(const CANMessage& message) {
            std::string s;
            s.resize(67);
            try {
                AssemblyMessageString(s, message);
                Log().debug("Sending : ") << s;
                sock << s;
            } catch (const libsocket::socket_exception& exc) {
                Log().error(exc.mesg);
                return false;
            }
            return true;
        }

        bool ParseNext(char c, char * s, int & i) {
            if (c == '\n' || c == '\r') {
                s[i] = '\0';
                i = 0;
                return true;
            } else {
                s[i] = c;
                i++;
                return false;
            }
        }

        bool CANSimulatorConnector::WaitForMessage(CANMessage& message) {
            std::string s,s1;
            s.resize(66);
            s1.resize(1);
            char str [100];
            bool r = false;
            int i=0;
            //char c;
            while (!r) {
                try {
                    sock >> s1;
                    while(ParseNext(s1.at(0),str,i) == false){
                        sock >> s1;
                    }
                    s = str;
                    this->AssemblyMessageObject(s, message);
                    r = true;
                } catch (const libsocket::socket_exception& exc) {
                    Log().error(exc.mesg);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    while (!this->NextConnectAttempt()) {
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                }
            }
            return true;
        }

        bool CANSimulatorConnector::Disconnect() {
            try {
                sock.destroy();
            } catch (const libsocket::socket_exception& exc) {
                Log().error(exc.mesg);
                return false;
            }
            return true;
        }

        void CANSimulatorConnector::AssemblyMessageObject(std::string s, CANMessage& message) {
            if (s.length() < 10) {
                return;
            }
            std::uint8_t data [CAN_MSG_LEN];
            unsigned int q = 0;
            Log().debug("Got message : ") << s;
            std::string id;
            std::stringstream str;
            id = s.substr(0, 2);
            str << id;
            int value;
            str >> std::hex >> value;
            Log().debug("ID message : ") << value;

            if (value == 0x10) {
                q = ParseLittleEndian(s, data);
            } else {
                q = ParseBigEndian(s, data);
            }
            message.SetData(value, q, data);
        }

        unsigned int CANSimulatorConnector::ParseLittleEndian(const std::string& s, std::uint8_t * data) {
            unsigned int i, j, q = 0;
            for (i = 2; i < s.length(); i++) {
                j = i - 2;
                if ((j % 8) == 0) {
                    data[(j / 8)] = 0;
                    q++;
                }
                if (s[i] == '1') {
                    data[(j / 8)] |= 1 << (j % 8);
                }
            }
            return q;
        }

        unsigned int CANSimulatorConnector::ParseBigEndian(const std::string& s, std::uint8_t * data) {
            unsigned int i, j, q = 0;
            for (i = 2; i < s.length(); i++) {
                j = i - 2;
                if ((j % 8) == 0) {
                    data[(j / 8)] = 0;
                    q++;
                }
                if (s[i] == '1') {
                    data[(j / 8)] |= 1 << (7 - (j % 8));
                }
            }
            return q;
        }

        void CANSimulatorConnector::AssemblyMessageString(std::string& s, const CANMessage& message) {

            std::stringstream str;
            str << std::hex << message.GetId();
            str >> s;
            if (message.GetId() == 0x11) {
                AppendLittleEndian(s, message);
            } else {
                AppendBigEndian(s, message);
            }
            s.append("\n");
        }

        void CANSimulatorConnector::AppendLittleEndian(std::string& s, const CANMessage& message) {
            int i;
            for (i = 0; i < (8 * message.GetLength()); i++) {
                s.append(message.IsBitSet(i) ? "1" : "0");
            }
        }

        void CANSimulatorConnector::AppendBigEndian(std::string& s, const CANMessage& message) {
            int i, byte, bit;
            for (i = 0; i < (8 * message.GetLength()); i++) {
                byte = i / 8;
                bit = (7 - (i % 8));
                s.append(message.IsBitSet((byte * 8) + bit) ? "1" : "0");
            }
        }

        bool CANSimulatorConnector::NextConnectAttempt() {
            Log().info("Connect Attempt");
            this->Disconnect();
            if (!this->Connect()) return false;
            return true;
        }

    }
};
