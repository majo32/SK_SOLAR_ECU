#include "FrontBreakLightService.h"
#include "Services/Blinker/MSG_STRUCTURES.h"
#include "Services/HornService/HornService.h"
namespace Services {

    /*REGISTRACIA listenera na spravu s nazvom  ....*/
    void FrontBreakLightService::Initialize() {
        this->RegisterListener("CAN::CAN_0Service::ControlUnitRequest", listenerCan_CU_STATE);
        ctx.cmdBrake.state=0;
        ctx.cmdFrontLight.state=0;
     SunStorm::Message mB("CAN::CAN_0Service::BreakLightResponse");
     SunStorm::Message mF("CAN::CAN_0Service::FrontLightResponse");
     setSendContent(mB,ctx.cmdBrake);
     setSendContent(mF,ctx.cmdBrake);

    };

    /*REakcia na stlacenie tlacidla prednaych stretavacich svetiel*/
    void FrontBreakLightService::Listener_CAN_CU_STATE::handle(const SunStorm::Message& message) {
//        Messages::BlinkerServiceS::CAN_CU messagesCU = message.GetContent<Messages::BlinkerServiceS::CAN_CU> ();
         Messages::CAN::CAN_0Service::ControlUnitRequest messagesCU =message.GetContent<Messages::CAN::CAN_0Service::ControlUnitRequest>();
//        Messages::BlinkerServiceS::CAN_SC commands;
       
        if (messagesCU.btnFrontLights == 1) {
             ctx.cmdFrontLight.state=1;
           
//            commands.cmdFrontLight = 1;
            ctx.frontBreakLightService.setSendContent(ctx.mF, ctx.cmdFrontLight);
              std::cout << "HeadLights testing command Lighting !!! " << std::endl;
        }

        if (messagesCU.btnFrontLights == 0) {
              ctx.cmdFrontLight.state=0;
//            commands.cmdFrontLight = 0;
            ctx.frontBreakLightService.setSendContent(ctx.mF, ctx.cmdFrontLight);
        }
        if (messagesCU.brakePedal == 0) {
              ctx.cmdBrake.state=0;
           // commands.cmdBrakeLight = 0;
            ctx.frontBreakLightService.setSendContent(ctx.mB,  ctx.cmdBrake);
        }
        if (messagesCU.brakePedal == 1) {
            ctx.cmdBrake.state=1;
           
           // commands.cmdBrakeLight = 1;
            ctx.frontBreakLightService.setSendContent(ctx.mB,  ctx.cmdBrake);
             std::cout << "Brake tessting Command Lighting!!! " << std::endl;
        }
    };

        /*Funkcia pre nastavenie obsahu spravy a jej odoslanie*/
    void FrontBreakLightService::setSendContent(SunStorm::Message& message,Messages::CAN::CAN_0Service::FrontLightResponse content) {
        message.SetContent<Messages::CAN::CAN_0Service::FrontLightResponse>(content);
        this->SendMessage(message);
    };

      void FrontBreakLightService::setSendContent(SunStorm::Message& message,Messages::CAN::CAN_0Service::BreakLightResponse content) {
        message.SetContent<Messages::CAN::CAN_0Service::BreakLightResponse>(content);
        this->SendMessage(message);
    };

};