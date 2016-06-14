/* 
 * File:   BlinkerService.cpp
 * Author: Simon
 *
 * Created on Nedeľa, 2016, február 28, 13:28
 */
#include <assert.h>
#include <iostream>
#include "BlinkerService.h"
#include "Services/Blinker/MSG_STRUCTURES.h"


namespace Services {

    /*Registrovanie listenerov na spravy, na ktore pocuvaju +
 nastavenie hodnot
     */
    void BlinkerService::Initialize() {
        this->RegisterListener("CAN::CAN_0Service::ControlUnitRequest", this->listener1);
        ctx.counterPeriod1 = 0;
        ctx.counterPeriod2 = 0;
        ctx.counterLeftBlinker = 0;
        ctx.counterRightBlinker = 0;
        ctx.controlR = false;
        ctx.controlL = false;
        ctx.controlW = false;
        ctx.longShortPressed = false;
        ctx.helpValue = false;
    };

    /*funkcia na zistenie dlzky stlacenie tlacidla*/
    void BlinkerService::ListenerButtonPressed::handle(const SunStorm::Message& message) {
        ctx.counterInterval++;
         // std::cout << "kratke stlacenie timer "<< std::endl;
        if (ctx.counterInterval < 3) {
            ctx.longShortPressed = false;
              
        } else {
            ctx.longShortPressed = true;
            //                        std::cout << "dlhe stlacenie timer " << std::endl;
        }
    };

    /*Listener na ovladanie smeroviek  obsah smeroviek sa ulozi do struktury a  na zaklade stavu  tlacidla zisti o ktoru smerovku sa jedna a spusti resp. vymaze interval blikania*/
    void BlinkerService::Listener_1Can_CU_STATE_Blinkers::handle(const SunStorm::Message& message) {
        Messages::CAN::CAN_0Service::ControlUnitRequest messagesCU =message.GetContent<Messages::CAN::CAN_0Service::ControlUnitRequest>();
//        Messages::BlinkerServiceS::CAN_CU messagesCU = message.GetContent<Messages::BlinkerServiceS::CAN_CU>();
        //        Messages::BlinkerServiceS::CAN_SC messagesSC;
            ctx.localStatusBtnLeft = messagesCU.btnLeftBlinker;
            ctx.localStatusBtnRight = messagesCU.btnRightBlinker;
       // SunStorm::Message m("BlinkerResponse");
        /*reakcia na stlacene tlacidlo LAVA smerovka - ignoruje vykonavanie ked su zapnute vystrazne svetla*/
        if (messagesCU.btnLeftBlinker == 1 && messagesCU.btnWarningLights == 0) {
            ctx.counterLeftBlinker++;
            ctx.controlL = true;
            ctx.controlR = false;
            ctx.counterRightBlinker = 0;
            //odstranenie intervalu pravej smerovky
            this->RemoveInterval(ctx.intervalIdRight);
        //    this->RemoveInterval(ctx.intervalIdLongOrShortRight);
            //podmienka aby sa interval spustil pri stlaceni tlacidla iba raz , counter sa resetuje pri uvoleni tlacidla
            if (ctx.counterLeftBlinker == 1) {
                ctx.counterPeriod1 = 0;
                ctx.longShortPressed = 0;
                //1060     ctx.intervalIdLongOrShortLEFT = this->RegisterInterval(100, this->GetService().listenerPressedButtomLongShort);
                ctx.intervalIdLeft = this->RegisterInterval(100, this->GetService().listenerSettingPeriod);
            }
        }
        if (messagesCU.btnLeftBlinker == 0 && messagesCU.btnWarningLights == 0) {
           // std::cout << "Tlacidlo btn leftBbutton  1 " << std::endl;
            //1060     this->RemoveInterval(ctx.intervalIdLongOrShortLEFT); //odstrani interval kt hovori ako dlho je stlacene tlacidlo
            ctx.counterInterval = 0;
           // ctx.counterLeftBlinker = 0;
        }
        /*reakcia na stlacene tlacidlo PRAVA SMEROVKA- ignoruje ked su zapnute vystrazne svetla */
        if (messagesCU.btnRightBlinker == 1 && messagesCU.btnWarningLights == 0) {
            ctx.counterRightBlinker++;
            ctx.controlR = true;
            ctx.controlL = false;
            ctx.counterLeftBlinker = 0;
            //odstranenie intervalu lavej smerovky
            this->RemoveInterval(ctx.intervalIdLeft);
              //1060   this->RemoveInterval(ctx.intervalIdLongOrShortLEFT);
            //podmienka aby sa interval spustil pri stlaceni tlacidla iba raz , counter sa resetuje pri uvoleni tlacidla
            if (ctx.counterRightBlinker == 1) {
                ctx.counterPeriod1 = 0;
                ctx.longShortPressed = 0;
               // std::cout << "counter right blinker : " << ctx.counterRightBlinker << std::endl;
            //1060    ctx.intervalIdLongOrShortRight = this->RegisterInterval(100, this->GetService().listenerPressedButtomLongShort);
                ctx.intervalIdRight = this->RegisterInterval(100, this->GetService().listenerSettingPeriod);
            }
        }
        if (messagesCU.btnRightBlinker == 0 && messagesCU.btnWarningLights == 0) {
         //   std::cout << "Tlacidlo btn rightButton 1 " << std::endl;
            this->RemoveInterval(ctx.intervalIdLongOrShortRight);
          //  ctx.counterRightBlinker = 0;
            ctx.counterInterval = 0;
        }
        /*reakcia na stlacene tlacidlo vystraznych svetiel*/
        if (messagesCU.btnWarningLights == 1) {
            ctx.setValues=false;
            //odstranenie intervalov lavej a pravej smerovky
            this->RemoveInterval(ctx.intervalIdRight);
            this->RemoveInterval(ctx.intervalIdLeft);
            ctx.controlW = true;
            ctx.controlL = false;
            ctx.controlR = false;
            ctx.messagesSC.left=0;
            ctx.messagesSC.right=0;
//            ctx.messagesSC.cmdLeftBlinker = 0;  right
//            ctx.messagesSC.cmdRightBlinker = 0; left
//            ctx.messagesSC.cmdWarningLights = 1; 
           // std::cout << "Tlacidlo btn WARNING button 1 " << std::endl;
            if (ctx.helpValue == false) {
                ctx.intervalIdWarningLights = this->RegisterInterval(100, this->GetService().listenerSettingPeriod);
                ctx.helpValue = true;
            }

        }
        if (messagesCU.btnWarningLights == 0) {
            ctx.controlW = false;
            ctx.helpValue = false;
            if(ctx.setValues==false){
            ctx.messagesSC.left=false;
            ctx.messagesSC.right=false;
             SunStorm::Message m("CAN::CAN_0Service::BlinkerResponse");
             ctx.blinkerService.setSendContent(m, ctx.messagesSC);
             ctx.setValues=true;
            }
      
       
               this->RemoveInterval(ctx.intervalIdWarningLights);

        }
    };

    void BlinkerService::ListenerSetPeriod::handle(const SunStorm::Message& message) {
        ctx.counterPeriod2++; //counter urcujuci kedy sa posiela jednotka alebo nula 
        SunStorm::Message m("CAN::CAN_0Service::BlinkerResponse");
//        Messages::BlinkerServiceS::CAN_SC messagesSC;
        /*Podmienky na rozlisenie toho co sa ma odosielat , rozlisovanie na zaklade stlaceneho tlacidla 
         controlL , controlR, controlW sa nastavia na true pri stlaceni tlacidla jednotlivych smeroviek , pri uvolneni tlacidla sa nastavia na false
         kazdy 100 ms je odoslana sprava  bud 1 alebo 0 na zaklade hodnoty counterPeriod*/
        if (ctx.controlL == 1) {
            ctx.messagesSC.right=0;
//            ctx.messagesSC.cmdRightBlinker = 0;
            if (ctx.counterPeriod2 <= 5) {
                ctx.messagesSC.left=1;
                ctx.blinkerService.setSendContent(m, ctx.messagesSC);
                 std::cout << "no.1 Left Testing Command! " << ctx.messagesSC.left << std::endl;
            }
            if (ctx.counterPeriod2 > 5) {
                ctx.messagesSC.left=0;
//                ctx.messagesSC.cmdLeftBlinker = 0;
                ctx.blinkerService.setSendContent(m, ctx.messagesSC);
                std::cout << ctx.counterPeriod2 << "no.2 Left Testing Command " << ctx.messagesSC.left << std::endl;
            }
        }
        if (ctx.controlR == 1) {
            if (ctx.counterPeriod2 <= 5) {
//                ctx.messagesSC.cmdRightBlinker = 1;
                 ctx.messagesSC.right=1;
                ctx.blinkerService.setSendContent(m, ctx.messagesSC);
                std::cout << "no.1 Right Testing Command " << ctx.messagesSC.right << std::endl;
            }
            if (ctx.counterPeriod2 > 5) {
                ctx.messagesSC.right = 0;
                ctx.blinkerService.setSendContent(m, ctx.messagesSC);
                std::cout << ctx.counterPeriod2 << "no.2 Right Testing Command" << ctx.messagesSC.right << std::endl;
            }
        }
        if (ctx.controlW == 1) {
            if (ctx.counterPeriod2 <= 5) {
                ctx.messagesSC.left=1;
                 ctx.messagesSC.right=1;
                ctx.blinkerService.setSendContent(m, ctx.messagesSC);
                std::cout << " no.1 Emergency Testing Command " << ctx.messagesSC.left << std::endl;
            }
            if (ctx.counterPeriod2 > 5) {
//                ctx.messagesSC.cmdWarningLights = 0;
                 ctx.messagesSC.right=0;
                 ctx.messagesSC.right=0;
                ctx.blinkerService.setSendContent(m, ctx.messagesSC);
                std::cout << " no.2 Emergency Testing Command " << ctx.messagesSC.right << std::endl;
            }
        }
        /*rusenie intervalov na zaklade dlheho a kratkeho stalcenia*/
        if (ctx.counterPeriod2 == 10) {
            ctx.counterPeriod1++;
            ctx.counterPeriod2 = 0;
            /*vypisy na testovanie */
            //            std::cout << "perioda blikanie " << ctx.counterPeriod1 << " krat " << std::endl;
            //            std::cout << "LONGSHORT  LAVA " << ctx.longShortPressed << " pressed left   " << std::endl;
            //            std::cout << "LONG SHORT PRAVA " << ctx.longShortPressedRIGHT << " pressed right " << std::endl;
            //PODMIENKA ABY blikla smerovka pri kratkom stlaceni alebo dlhom vzdy aspon tri krat
            //10.6.2016 && ctx.longShortPressed == false 
            if ((ctx.counterPeriod1 >= 3&& ctx.longShortPressed == false  && ctx.localStatusBtnLeft==0&&ctx.localStatusBtnRight==0)) {
                
                 ctx.messagesSC.right=0;
                 ctx.messagesSC.right=0;//10.06.2016
                ctx.blinkerService.setSendContent(m, ctx.messagesSC);
                ctx.counterPeriod1 = 0;
               // ctx.longShortPressed = false;
                this->RemoveInterval(ctx.intervalIdLeft);
                this->RemoveInterval(ctx.intervalIdRight);
                //this->RemoveInterval(ctx.intervalIdLongOrShortLEFT);
                //this->RemoveInterval(ctx.intervalIdLongOrShortRight);
               
                ctx.counterLeftBlinker = 0;
                ctx.counterRightBlinker =0;
                
                ctx.messagesSC.left = 0;
                ctx.messagesSC.right = 0;
                std::cout << "End of sending commands Reset All values and deleting intervals" << std::endl;
            }
//         
        };
    };

    /*funkcia pre nastavenie obsahu spravy a jej odoslanie */

    void BlinkerService::setSendContent(SunStorm::Message & message,  Messages::CAN::CAN_0Service::BlinkerResponse structure) {
        message.SetContent< Messages::CAN::CAN_0Service::BlinkerResponse>(structure);
        this->SendMessage(message);
    };
};