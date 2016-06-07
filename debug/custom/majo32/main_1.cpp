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
using namespace std;

SunStorm::Logger & logger = SunStorm::LoggerManager::RegisterLogger("main");

class fn : public SunStorm::Function<Messages::StateMachineService::CHANGE_TO_STATE_START_TRANSACTION> {
public:

    fn() :
    SunStorm::Function<Messages::StateMachineService::CHANGE_TO_STATE_START_TRANSACTION>() {
    }

    void Run(Messages::StateMachineService::CHANGE_TO_STATE_START_TRANSACTION transaction) {
        logger.debug(std::string("Change to state -> "));
    }
};

class TestS : public ECU::Service{
public:

    class Context : public ECU::GenericContext {
        
    };

    TestS(std::string name) :
    ECU::Service(name,ct), ct() {
    }

    void RegisterListeners() {
        this->GetGenericListener().AddOnStateChangeFunction(fx);
    }

     Context ct;
     fn fx;
   
};

int main(int argc, char** argv) {

    SunStorm::System::Initialize();
    SunStorm::System::RegisterService<Services::StateMachineService>("STATE_MACHINE");
    SunStorm::System::RegisterService<Services::TestService>("TEST_SERVICE");
    SunStorm::System::RegisterService<TestS>("TESTS");
    SunStorm::System::RegisterService<TestS>("TESTS2");
    
    SunStorm::Message msg("__INIT__");
    SunStorm::System::GetMessenger().SendMessage(msg);

    string s;
    while (1) {
        cin >> s;
        
        SunStorm::Message msg2("StateMachineService::CHANGE_TO_STATE_REQUEST");
        msg2.SetContent(Messages::StateMachineService::CHANGE_TO_STATE_REQUEST({
            Types::StateMachine::STATES::BOOT
        }));
        SunStorm::System::GetMessenger().SendMessage(msg2);
        
        logger.debug("Changing to state");
    }

    SunStorm::System::Join();

    return 0;
}

