#include "FrontBreakLightService.h"
#include "Services/Blinker/MSG_STRUCTURES.h"
#include "Services/HornService/HornService.h"
namespace Services {

    /*REGISTRACIA listenera na spravu s nazvom  ....*/
    void FrontBreakLightService::Initialize() {
        this->RegisterListener("CAN_CU_STATE", listenerCan_CU_STATE);


    };

    /*REakcia na stlacenie tlacidla prednaych stretavacich svetiel*/
    void FrontBreakLightService::Listener_CAN_CU_STATE::handle(const SunStorm::Message& message) {
        Messages::BlinkerServiceS::CAN_CU messagesCU = message.GetContent<Messages::BlinkerServiceS::CAN_CU> ();
        Messages::BlinkerServiceS::CAN_SC commands;
        SunStorm::Message m("CAN_SC_COMMAND");
        if (messagesCU.btnFrontLight == 1) {
            commands.cmdFrontLight = 1;
            ctx.frontBreakLightService.setSendContent(m, commands);
        }

        if (messagesCU.btnFrontLight == 0) {
            commands.cmdFrontLight = 0;
            ctx.frontBreakLightService.setSendContent(m, commands);
        }
        if (messagesCU.BrakePedal == 0) {
            commands.cmdBrakeLight = 0;
            ctx.frontBreakLightService.setSendContent(m, commands);
        }
        if (messagesCU.BrakePedal == 1) {
            commands.cmdBrakeLight = 1;
            ctx.frontBreakLightService.setSendContent(m, commands);
        }
    };

        /*Funkcia pre nastavenie obsahu spravy a jej odoslanie*/
    void FrontBreakLightService::setSendContent(SunStorm::Message& message, Messages::BlinkerServiceS::CAN_SC content) {
        message.SetContent<Messages::BlinkerServiceS::CAN_SC>(content);
        this->SendMessage(message);
    };


};