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



SunStorm::Logger & logger = SunStorm::LoggerManager::RegisterLogger("__MAIN__");

int main(int argc, char** argv) {

    SunStorm::System::Initialize();
    
    //SunStorm::System::RegisterService<Services::CAN::Decomposer::CAN_0DecomposerService>("CAN::DECOMPOSER::CAN_0DecomposerService");
    
    SunStorm::Message msg("__INIT__");
    SunStorm::System::GetMessenger().SendMessage(msg);

    std::string s;
    while (1) {
        std::cin >> s;
        
       
        
        logger.debug("Changing to state");
    }

    SunStorm::System::Join();

    return 0;
}

