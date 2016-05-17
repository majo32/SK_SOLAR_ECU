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


#include <Services/TestService/TestService.h>
#include <Services/Test2Service/Test2Service.h>
#include <Services/StateMachineService/StateMachineService.h>

#include <SunStorm/src/FunctionComposer/FunctionComposer.h>

#include "Services/StateMachineService/StateMachineService.h"
using namespace std;

class A{
public:
    int i;
    virtual void print(){
        cout << "A " << i << endl;
    }
};
class B : public A{
public:
    int i;
    virtual void print(){
        cout << "B " << i << endl;
    }
};


template <class Clz>
void callMemberClass (void(Clz::*ptr)(void), Clz* self){
    (self->*ptr)();
}


typedef void(A::*PTR)(void);
SunStorm::Logger logger("main");
class L2 : public SunStorm::Listener{
    void handle(const SunStorm::Message & message){
        logger.debug("dsdd");;
    }
};

class fn : public SunStorm::Function<std::string &>{
public:
    void Run(std::string & a){
        logger.debug("A is ");
        logger.debug("--" + a);
        a = "44445";
    }
};

int main(int argc, char** argv) {

   
    
   SunStorm::FunctionComposer<std::string&> fc;
   fc.AddFunction(*(new fn()));
   fc.AddFunction(*(new fn()));
   std::string r = "4444";
   fc.Run(r);
    
    logger.warn("Asdf");
    

    SunStorm::System::Initialize();
    SunStorm::System::RegisterService<Services::StateMachineService>("STATE_MACHINE");
    SunStorm::System::RegisterService<Services::TestService>("TEST_SERVICE");
    SunStorm::Service  & sss = SunStorm::System::RegisterService<Services::Test2Service>("TEST2_SERVICE");

    string s;
    while (1) {
        cin >> s;
        SunStorm::Message m("L1_MSG");
        
        m.SetContent<std::string>(s);
        //cout << "Sending msg:" << s << endl;
        SunStorm::System::GetMessenger().SendMessage(m);
        SunStorm::System::GetClock().RegisterTimeout(500,sss.GetExecutor(),*(new L2()));
    }

    SunStorm::System::Join();

    return 0;
}

