//
///* 
// * File:   BlinkerService.cpp
// * Author: Simon
// *
// * Created on Nedeľa, 2016, február 28, 13:28
// */
//#include <assert.h>
//
//#include <iostream>
//#include <unicode/umachine.h>
//#include "BlinkerService.h"
//#include "Services/Blinker/MSG_STRUCTURES.h"
//#include "Services/HornService/HornService.h"
//namespace Services {
//
//    void BlinkerService::Initialize() {
//        //zaciatocnu inicializaciu dorobit ....
//        this->RegisterListener("CAN_CU_STATE", this->listener1);
//        ctx.counterPeriod1 = 0;
//        ctx.counterPeriod2 = 0;
//        ctx.counterLeftBlinker = 0;
//        ctx.counterRightBlinker = 0;
//        ctx.pressedLeft = false;
//        ctx.pressedRight = false;
//        ctx.controlR = false;
//        ctx.controlL = false;
//    };
//    //funkcia pre ovladanie lavej, pravej smerovky a vystraznych svetiel 
//    //ctx.counterPeriod1 nastavit na 0 ked prepnem na druhu smerovku
//
//    void BlinkerService::Listener_1Can_CU_STATE_Smerovky::handle(const SunStorm::Message& message) {
//        Messages::BlinkerServiceS::CAN_CU messagesCU = message.GetContent<Messages::BlinkerServiceS::CAN_CU>();
//        Messages::BlinkerServiceS::CAN_SC messagesSC;
//        ctx.localStatusBtnLeft = messagesCU.btnLeftBlinker;
//        ctx.localStatusBtnRight = messagesCU.btnRightBlinker;
//        SunStorm::Message m("CAN_SC_COMMAND");
//        //reakcia na stlacene tlacidlo alva smerovka - ignoruje vykonavanie ked su zapnute vystrazne svetla
//        if (messagesCU.btnLeftBlinker == 0 && messagesCU.btnWarningLights == 1) {
//            ctx.counterL++;
//            ctx.controlL = true;
//            ctx.controlR = false;
//            if (ctx.counterL == 1) {
//                ctx.intervalIdLongShortPressedLEFT = this->RegisterInterval(1000, this->GetService().dlheKratkeStlacenie);
//            }
//            if (ctx.longShortPressedLEFT == false) {
//                if (ctx.counterL == 1) {
//
//                }
//            }
//
//
//        }
//        if (messagesCU.btnLeftBlinker == 1 && messagesCU.btnWarningLights == 1) {
//            this->RemoveInterval(ctx.intervalIdLongShortPressedLEFT);
//            ctx.counterL = 0;
//            std::cout << "Tlacidlo btn left button  1 " << std::endl;
//
//            //else ctx.pressedLeft = false;
//        }
//        //reakcia na stlacene tlacidlo PRAVA SMEROVKA
//        if (messagesCU.btnRightBlinker == 0 && messagesCU.btnWarningLights == 1) {
//            
//        }
//        if (messagesCU.btnRightBlinker == 1 && messagesCU.btnWarningLights == 1) {
//         
//            std::cout << "Tlacidlo btn right button 1 " << std::endl;
//        }//reakcia na stlacene tlacidlo vystraznych svetiel
//
//        if (messagesCU.btnWarningLights == 0) {
//            messagesSC.cmdWarningLights = 0;
//            ctx.blinkerService.zapnutieVypnutie(m, messagesSC);
//            messagesSC = m.GetContent< Messages::BlinkerServiceS::CAN_SC>();
//
//        }
//        if (messagesCU.btnWarningLights == 1 && messagesCU.btnRightBlinker == 1 && messagesCU.btnLeftBlinker == 1) {
//            messagesSC.cmdWarningLights = 1;
//            ctx.blinkerService.zapnutieVypnutie(m, messagesSC);
//            messagesSC = m.GetContent< Messages::BlinkerServiceS::CAN_SC>();
//        }
//    };
//
//    void BlinkerService::Listener_long_shortPress::handle(const SunStorm::Message& message) {
//        ctx.counterInterval++;
//        if (ctx.counterInterval < 3) {
//            ctx.longShortPressedLEFT = false;
//            ctx.longShortPressedRIGHT = false;
//                        std::cout << "kratke stlacenie timer " << std::endl;
//        } else {
//            ctx.longShortPressedLEFT = true;
//            ctx.longShortPressedRIGHT = true;
//                        std::cout << "dlhe stlacenie timer " << std::endl;
//        }
//    };
//
//    void BlinkerService::ListenerSetPeriod::handle(const SunStorm::Message& message) {
//        ctx.counterPeriod2++;
//        //nula alebo jedna striedanie
//        SunStorm::Message m("CAN_SC_COMMAND");
//        Messages::BlinkerServiceS::CAN_SC messagesSC;
//        ////////////////////////////////////////////////////////////////////////////////
//        /*osetrit len jeden pre kazdu smerovku potrebne dorobit aby ked zasvieti opacna aby zahslo opacnu 
//         podmienka na striedanie jednotiek a nul pre pravu alebo lavu smerovku */
//        /*setting value of sending command 1 or 0 */
//        if ((ctx.longShortPressedLEFT == 0 || ctx.longShortPressedLEFT == 1) && ctx.controlL == true) {
//            if (ctx.counterPeriod2 <= 5) {
//                messagesSC.cmdLeftBlinker = 1;
//                std::cout << "prva podmienka " << messagesSC.cmdLeftBlinker << std::endl;
//                ctx.blinkerService.zapnutieVypnutie(m, messagesSC);
//            }
//            if (ctx.counterPeriod2 > 5) {
//                messagesSC.cmdLeftBlinker = 0;
//                ctx.blinkerService.zapnutieVypnutie(m, messagesSC);
//                std::cout << ctx.counterPeriod2 << "druha podmienka" << messagesSC.cmdLeftBlinker << std::endl;
//            }
//
//        }
//        if ((ctx.longShortPressedRIGHT == 0 || ctx.longShortPressedRIGHT == 1) && ctx.controlR == true) {
//            if (ctx.counterPeriod2 <= 5) {
//                messagesSC.cmdRightBlinker = 1;
//                ctx.blinkerService.zapnutieVypnutie(m, messagesSC);
//                std::cout << "prva podmienka prava" << messagesSC.cmdRightBlinker << std::endl;
//            }
//            if (ctx.counterPeriod2 > 5) {
//                messagesSC.cmdRightBlinker = 0;
//                ctx.blinkerService.zapnutieVypnutie(m, messagesSC);
//                std::cout << ctx.counterPeriod2 << "druha podmienka prava" << messagesSC.cmdRightBlinker << std::endl;
//            }
//        }
//
//        ////////////////rusenie intervalov na zaklade dlheho a kratkeho stalcenia
//        if (ctx.counterPeriod2 == 10) {
//            ctx.counterPeriod1++;
//            ctx.counterPeriod2 = 0;
//            std::cout << "perioda blikanie " << ctx.counterPeriod1 << " krat " << std::endl;
//            std::cout << "LONGSHORT  LAVA " << ctx.longShortPressedLEFT << " pressed left   " << ctx.pressedLeft << std::endl;
//            std::cout << "LONG SHORT PRAVA " << ctx.longShortPressedRIGHT << " pressed right " << ctx.pressedRight << std::endl;
//            //kratke stlacenie zrusenie
//            if (ctx.counterPeriod1 == 3 && ctx.longShortPressedLEFT == 0) {
//                this->RemoveInterval(ctx.intervalID);
//                ctx.controlL = false;
//                ctx.longShortPressedLEFT = 0;
//                ctx.counterPeriod1 = 0;//toto kedy nulovat s tymto je problem neviem kedy
//            }//dlhe stlacenie zrusenie
//            if (ctx.counterPeriod1 >= 3 && ctx.longShortPressedLEFT == 1 && ctx.localStatusBtnLeft == 1) {
//                ctx.controlL = false;
//                this->RemoveInterval(ctx.intervalID);
//                ctx.longShortPressedLEFT = 0;
//                ctx.counterPeriod1 = 0;
//
//            }
//            if (ctx.counterPeriod1 == 3 && ctx.longShortPressedRIGHT == 0) {
//                ctx.controlR = false;
//                ctx.longShortPressedRIGHT = 0;
//                ctx.counterPeriod1 = 0;
//                this->RemoveInterval(ctx.intervalIdRight);
//            }
//            if (ctx.counterPeriod1 >= 3 && ctx.longShortPressedRIGHT == 1 && ctx.localStatusBtnRight == 1) {
//                ctx.controlR = false;
//                this->RemoveInterval(ctx.intervalIdRight);
//                ctx.longShortPressedRIGHT = 0;
//                ctx.counterPeriod1 = 0;
//            }
//
//
//        };
//
//    };
//    //funkcia pre odoslanie prikazu 
//
//    void BlinkerService::zapnutieVypnutie(SunStorm::Message & message, Messages::BlinkerServiceS::CAN_SC structure) {
//        message.SetContent<Messages::BlinkerServiceS::CAN_SC>(structure);
//        this->SendMessage(message);
//    };
//
//    /*Funkcia longShortPressedButoon nastavi premennu na 0 alebo 1 podla toho ci je tlacidlo stlacene dlho alebo kratko 
//     * vrati false ked je tlacidlo stlacene kratko , true pri dlhom stlaceni
//     */
//    void BlinkerService::longShortPressedButton(int valueCounter, bool *shortLongPressed) {
//        if (valueCounter < 3) {
//            *shortLongPressed = false;
//        } else
//            *shortLongPressed = true;
//    };
//
//};