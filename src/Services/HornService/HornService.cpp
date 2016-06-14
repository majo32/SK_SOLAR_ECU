#include <assert.h>

#include "HornService.h"
//#include "Services/Blinker/MSG_STRUCTURES.h"
namespace Services {

    /*Registracia Listenera na spravu na ktoru pocuva*/
    void Services::HornService::Initialize() {
        this->RegisterListener("CAN::CAN_0Service::ControlUnitRequest", listener1);
       SunStorm::Message msg("CAN::CAN_0Service::HornResponse");
       ctx.hornService.setSendContent(msg,ctx.comannd);

    };

    /*Reakcia na stlacenie tlacidla klaksonu*/
    void Services::HornService::Listener_1Can_CU_STATE::handle(const SunStorm::Message& message) {
//        Messages::BlinkerServiceS::CAN_CU messages = message.GetContent<Messages::BlinkerServiceS::CAN_CU>();
         Messages::CAN::CAN_0Service::ControlUnitRequest messages =message.GetContent<Messages::CAN::CAN_0Service::ControlUnitRequest>();
//        Messages::BlinkerServiceS::CAN_SC commands;
        SunStorm::Message msg("CAN::CAN_0Service::HornResponse");
        if (messages.btnHorn == 0) {
            ctx.comannd.state=0;
//            std::cout << "TRUBENIE -TESTOVANIE " << std::endl;
            ctx.hornService.setSendContent(msg,ctx.comannd);

        }
        if (messages.btnHorn == 1) {
           
           ctx.comannd.state=1;
//            commands.cmdHorn = 1;
//            setSendContent(msg,ctx.messagesSC);
             ctx.hornService.setSendContent(msg,ctx.comannd);
             std::cout << "Horning testing command" << messages.btnHorn << std::endl;
             
//            ctx.hornService.setSendContent(msg, commands);
        }
    };

        /*Funkcia pre nastavenie obsahu spravy a jej odoslanie*/
    void HornService::setSendContent(SunStorm::Message& message, Messages::CAN::CAN_0Service::HornResponse  content) {

        message.SetContent<Messages::CAN::CAN_0Service::HornResponse>(content);
        this->SendMessage(message);

    };



};