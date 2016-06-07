/* 
 * File:   main.cpp
 * Author: majo32
 *
 * Created on January 25, 2016, 1:10 PM
 * 
 */

#include <cstdlib>
#include <iostream>
#include <SunStorm/Framework.h>
#include <ECU/Common.h>
#include <Services/CAN/CAN_0Service/CAN_0Service.h>
#include <Drivers/CAN/CANSimulatorConnector.h>

int main(int argc, char** argv) {
    try {
        SunStorm::Logger & logger = SunStorm::LoggerManager::RegisterLogger("xxx");
        SunStorm::System::Initialize();

        Drivers::CAN::CANSimulatorConnector can_connector(logger);
        can_connector.Connect();
        
        SunStorm::System::RegisterService<Services::CAN::CAN_0Service>("CAN::CAN_0Service", can_connector);

        SunStorm::Message msg("__INIT__");
        SunStorm::System::GetMessenger().SendMessage(msg);
        SunStorm::System::Join();
        
        can_connector.Disconnect();
    }    catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Log failed: " << ex.what() << std::endl;
    }

    return 0;
}

