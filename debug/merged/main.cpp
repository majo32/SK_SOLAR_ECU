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

#include <Services/TestService/TestService.h>
#include <Services/Test2Service/Test2Service.h>
#include <Services/StateMachineService/StateMachineService.h>

#include <SunStorm/src/FunctionComposer/FunctionComposer.h>

#include "Services/StateMachineService/StateMachineService.h"

int main(int argc, char** argv) {

    SunStorm::System::Initialize();
    SunStorm::System::RegisterService<Services::StateMachineService>("STATE_MACHINE");
   

    SunStorm::Message msg("__INIT__");
    SunStorm::System::GetMessenger().SendMessage(msg);


    SunStorm::System::Join();

    return 0;
}

