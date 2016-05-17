#include <assert.h>

#include "HornService.h"
#include "Services/Blinker/MSG_STRUCTURES.h"
namespace Services {

    /*Registracia Listenera na spravu na ktoru pocuva*/
    void Services::HornService::Initialize() {
        this->RegisterListener("CAN_CU_STATE", listener1);

    };

    /*Reakcia na stlacenie tlacidla klaksonu*/
    void Services::HornService::Listener_1Can_CU_STATE::handle(const SunStorm::Message& message) {
        Messages::BlinkerServiceS::CAN_CU messages = message.GetContent<Messages::BlinkerServiceS::CAN_CU>();
        Messages::BlinkerServiceS::CAN_SC commands;
        SunStorm::Message msg("CAN_SC_COMMAND");
        if (messages.btnHorn == 0) {
            commands.cmdHorn = 0;
            std::cout << "TRUBENIE -TESTOVANIE " << std::endl;
            ctx.hornService.setSendContent(msg, commands);

        }
        if (messages.btnHorn == 1) {
            std::cout << "NETRUBENIE -TESTOVANIE" << messages.btnHorn << std::endl;
            commands.cmdHorn = 1;
            ctx.hornService.setSendContent(msg, commands);
        }
    };

        /*Funkcia pre nastavenie obsahu spravy a jej odoslanie*/
    void HornService::setSendContent(SunStorm::Message& message, Messages::BlinkerServiceS::CAN_SC structure) {

        message.SetContent<Messages::BlinkerServiceS::CAN_SC>(structure);
        this->SendMessage(message);

    };



};