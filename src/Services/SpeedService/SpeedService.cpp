/* 
 * File:   SpeedService.cpp
 * Author: Michal Moravcik
 * 
 * Created on Pondelok, 2016, februára 8, 19:56
 */

#include "SpeedService.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace Messages::SpeedService;
namespace Services {

    //nastavenie pociatocnych hodnot, registracia listenerov    

    void Services::SpeedService::Initialize() {
        this->RegisterListener("ControlUnit::CuStatus", this->lCanCuStatus);
        this->RegisterListener("PCU::SpeedRqst", this->lSpeedRqst);
        this->RegisterListener("PCU::SpeedActual", this->lSpeedActual);
        this->RegisterListener("PCU_SpeedOptimal", this->lSpeedOpt);
        this->RegisterListener("__TIMER__", this->lTimeout);
        this->RegisterListener("__TIMER__", this->lLongPressSender);

        //nastavenie pociatocnych hodnot
        ctx.localAutopilotStatus = 0;
        ctx.localBtnSpeedUpStatus = 0;
        ctx.localBtnSpeedDownStatus = 0;
        ctx.localBtnAutopilotStatus = 0;
        ctx.localPedalStatus = 0;
        ctx.localRequiredSpeed = 10;
        ctx.localActualSpeed = 15;
        ctx.localOptimalSpeed = 20;
        ctx.speedUpChangeValue = ctx.shortPressChangeValue;
        ctx.speedDownChangeValue = ctx.shortPressChangeValue;
        ctx.btnUpTimeoutID = 0;
        ctx.btnDownTimeoutID = 0;
        ctx.longPressUpSenderID = 0;
        ctx.longPressDownSenderID = 0;
    }

    /**
     * obsluha spravy CAN_CU_STATUS od CU
     * @param message   sprava od CU obsahujuca stavy tlacidiel
     */
    void Services::SpeedService::Listener_CAN_CU_STATUS::handle(const SunStorm::Message& message) {
        CanCuStatus canMessage = message.GetContent<CanCuStatus>();

        //stlacenie brzdoveho pedalu
        ////////////////////////////////////////////////////////////////////////
        if (canMessage.brakePedal == 1) {
            ctx.service.brake_pedal_handle(true);
        } else if (canMessage.brakePedal == 0) {
            ctx.service.brake_pedal_handle(false);
        }
        //uluzenie aktualnej hodnoty pedalu
        ctx.localPedalStatus = canMessage.brakePedal;

        //stlacenie tlacidla zvysenia rychlosti
        ////////////////////////////////////////////////////////////////////////            
        if (canMessage.btnSpeedUp == 1 && canMessage.btnSpeedDown == 0 && canMessage.brakePedal == 0) {
            ctx.service.btn_up_handle(true, this->GetService());
        } else if (canMessage.btnSpeedUp == 0) {
            ctx.service.btn_up_handle(false, this->GetService());
        }
        ctx.localBtnSpeedUpStatus = canMessage.btnSpeedUp;
        //stlacenie tlacidla znizenia
        ////////////////////////////////////////////////////////////////////////
        if (canMessage.btnSpeedDown == 1 && canMessage.btnSpeedUp == 0 && canMessage.brakePedal == 0) {
            ctx.service.btn_down_handle(true, this->GetService());
        } else if (canMessage.btnSpeedDown == 0) {
            ctx.service.btn_down_handle(false, this->GetService());
        }
        ctx.localBtnSpeedDownStatus = canMessage.btnSpeedDown;

        //stlacenie tlacidla autopilota
        ////////////////////////////////////////////////////////////////////////
        if (canMessage.btnAutopilot == 1 && canMessage.btnSpeedDown == 0 && canMessage.btnSpeedUp == 0 && canMessage.brakePedal == 0) {
            ctx.service.btn_autopilot_handle(true);
        }
        ctx.localBtnAutopilotStatus = canMessage.btnAutopilot;
    }

    /**
     * obsluha spravy PCU::SpeedActual z PCU
     * @param message   sprava od PCU obsahujuca aktualnu rychlost
     */
    void Services::SpeedService::Listener_PCU_SPEED_ACTUAL::handle(const SunStorm::Message& message) {
        PCU_SpeedAct actSpeed = message.GetContent<PCU_SpeedAct>();
        ctx.localActualSpeed = actSpeed.value;

        //test vystup
        std::cout << "localActualSpeed=" << ctx.localActualSpeed << std::endl;
    }

    /**
     * obsluha spravy PCU_SpeedOptimal z PCU
     * @param message   sprava od PCU obsahujuca optimalnu rychlost
     */
    void Services::SpeedService::Listener_PCU_SPEED_OPT::handle(const SunStorm::Message& message) {
        PCU_SpeedOpt optSpeed = message.GetContent<PCU_SpeedOpt>();
        ctx.localOptimalSpeed = optSpeed.value;

        if (ctx.localAutopilotStatus == 1) {//nastavovanie optimalnej rychlosti pri zapnutom autopilotovi
            ctx.localRequiredSpeed = ctx.localOptimalSpeed;
            ctx.service.speed_rqst_set(ctx.localRequiredSpeed);
        }
        //test vystup
        std::cout << "localOptimalSpeed=" << ctx.localOptimalSpeed << std::endl;
    }

    /**
     * obsluha spravy PCU::SpeedRqst z PCU
     * @param message   sprava od PCU obsahujuca pozadovanu rychlost
     */
    void Services::SpeedService::Listener_PCU_SPEED_RQST::handle(const SunStorm::Message& message) {
        PCU_SpeedRqst rqstSpeed = message.GetContent<PCU_SpeedRqst>();

        if (rqstSpeed.value != ctx.localRequiredSpeed) {
            std::cout << "required speeds match error!! " << std::endl;

        }
        //test vystup
        std::cout << "prijem spravy  PCU::SpeedRqst" << std::endl
                << "localRequiredSpeed=" << ctx.localRequiredSpeed << std::endl;
    }

    /**
     * obsluha listenera ktory je budeny timerom
     * @param message   sprava od timera obsahujuca jeho ID
     */
    void Services::SpeedService::Listener_TIMEOUT::handle(const SunStorm::Message& message) {
        SunStorm::ClockIdentifier msg = message.GetContent<SunStorm::ClockIdentifier>();
        long timID = msg.GetId();

        if (timID == ctx.btnUpTimeoutID) {
            ctx.speedUpChangeValue = ctx.longPressChangeValue;
        }
        if (timID == ctx.btnDownTimeoutID) {
            ctx.speedDownChangeValue = ctx.longPressChangeValue;
        }
    }

    /**
     * obsluha listenera ktory je budeny intervalom
     * @param message   sprava od timera obsahujuca jeho ID
     */
    void Services::SpeedService::Listener_LONG_PRESS_SENDER::handle(const SunStorm::Message& message) {
        SunStorm::ClockIdentifier msg = message.GetContent<SunStorm::ClockIdentifier>();
        long timID = msg.GetId();

        if (timID == ctx.longPressUpSenderID && ctx.speedUpChangeValue == ctx.longPressChangeValue) {
            ctx.service.speed_rqst_up(ctx.longPressChangeValue); //zvysenie pozadovanej rychlosti
        }
        if (timID == ctx.longPressDownSenderID && ctx.speedDownChangeValue == ctx.longPressChangeValue) {
            ctx.service.speed_rqst_down(ctx.longPressChangeValue); //znizenie pozadovanej rychlosti
        }
    }
    /////////////////////////////////////////////////////////////////////////////////

    Services::SpeedService::~SpeedService() {
    }

    /**
     * obsluha stavu brzdoveho pedalu
     * @param status    stav pedalu
     */
    void Services::SpeedService::brake_pedal_handle(bool status) {
        if (status == true) {
            if (ctx.localPedalStatus == 0) {
                ctx.service.speed_autopilot(false); //vypnutie autopilota
                ctx.localRequiredSpeed = -1;
                ctx.service.speed_rqst_set(ctx.localRequiredSpeed); //nastavenie pozadovanej rychlosti
            }
        } else if (status == false) {
            if (ctx.localPedalStatus == 1) {
                ctx.longPressUpSenderID = 0;
                ctx.longPressDownSenderID = 0;
                ctx.localRequiredSpeed = ctx.localActualSpeed;
                ctx.service.speed_rqst_set(ctx.localRequiredSpeed);
            }
        }
    };

    /**
     * obsluha stavu tlacidla zvysenia rychlosti
     * @param status    stav tlacidla
     */
    void Services::SpeedService::btn_up_handle(bool status, SpeedService &s) {
        if (status == true) {
            if (ctx.localBtnSpeedUpStatus == 0) {
                ctx.service.speed_autopilot(false); //vypnutie autopilota            
                ctx.service.btn_start_timeout(1, s); //identifikovanie dlheho stlacenia
                ctx.service.speed_rqst_up(ctx.speedUpChangeValue); //zvysenie pozadovanej rychlosti


            } else if (ctx.speedUpChangeValue == ctx.longPressChangeValue && ctx.longPressUpSenderID == 0) {

                ctx.service.speed_rqst_up(ctx.speedUpChangeValue); //zvysenie pozadovanej rychlosti
                ctx.longPressUpSenderID = this->RegisterInterval(ctx.longPressSendingIntervalTime, s.lLongPressSender); //zaregistrovanie intervalu na cyklicke odosielanie zvysenia rychlosti pri dlhom stlaceni

            }
        } else if (status == false) {
            if (ctx.localBtnSpeedUpStatus == 1) {
                if (ctx.longPressUpSenderID != 0) {
                    this->RemoveInterval(ctx.longPressUpSenderID);
                    ctx.longPressUpSenderID = 0;
                }
                ctx.speedUpChangeValue = ctx.shortPressChangeValue;
                ctx.btnUpTimeoutID = 0;
            }
        }
    };

    /**
     * obsluha stavu tlacidla znizenia rychlosti
     * @param status    stav tlacidla
     */
    void Services::SpeedService::btn_down_handle(bool status, SpeedService &s) {
        if (status == true) {
            if (ctx.localBtnSpeedDownStatus == 0) {
                ctx.service.speed_autopilot(false); //vypnutie autopilota            
                ctx.service.btn_start_timeout(2, s); //identifikovanie dlheho stlacenia
                ctx.service.speed_rqst_down(ctx.speedDownChangeValue); //znizenie pozadovanej rychlosti


            } else if (ctx.speedDownChangeValue == ctx.longPressChangeValue && ctx.longPressDownSenderID == 0) {
                ctx.service.speed_rqst_down(ctx.speedDownChangeValue); //znizenie pozadovanej rychlosti
                ctx.longPressDownSenderID = this->RegisterInterval(ctx.longPressSendingIntervalTime, s.lLongPressSender); //zaregistrovanie intervalu na cyklicke odosielanie zvysenia rychlosti pri dlhom stlaceni

            }
        } else if (status == false) {
            if (ctx.localBtnSpeedDownStatus == 1) {
                if (ctx.longPressDownSenderID != 0) {
                    this->RemoveInterval(ctx.longPressDownSenderID);
                    ctx.longPressDownSenderID = 0;
                }
                ctx.speedDownChangeValue = ctx.shortPressChangeValue;
                ctx.btnDownTimeoutID = 0;
            }
        }
    };

    /**
     * obsluha stavu tlacidla autopilota
     * @param status    stav tlacidla
     */
    void Services::SpeedService::btn_autopilot_handle(bool status) {
        if (status == true) {
            if (ctx.localBtnAutopilotStatus == 0) {
                if (ctx.localAutopilotStatus == 0) {
                    ctx.service.speed_autopilot(true); //zapnutie autopilota 
                } else {
                    ctx.service.speed_autopilot(false); //vypnutie autopilota 
                    ctx.localRequiredSpeed = ctx.localActualSpeed;
                    ctx.service.speed_rqst_set(ctx.localRequiredSpeed); //nastavenie pozadovanej rychlosti
                }
            }
        } else if (status == false) {
        }
    };

    /**
     * funkcia na zapnutie/vyypnutie autopilota
     * @param status    stav ktory ma nastat
     */
    void Services::SpeedService::speed_autopilot(bool status) {
        if (ctx.localAutopilotStatus != status) {

            SunStorm::Message message("SpeedService::AutopilotStatus"); //vytvorenie spravy pre PCU o zapnuti/vypnuti autopilota
            AutopilotStatus content({status}); //vytvorenie obsahu sprvay
            message.SetContent<AutopilotStatus>(content); //pridanie obsahu sprave
            this->SendMessage(message); //odoslanie spravy

            ctx.localAutopilotStatus = status; //ulozenie stavu autopilota
            //test vystup
            std::cout << "autopilotStatus= " << ctx.localAutopilotStatus << endl;
        }

    };

    /**
     * znizenie a nastavenie hodnoty pozadovanej rychlosti
     * @param bValue    hodnota o ktoru sa ma znizit rychlost
     */
    void Services::SpeedService::speed_rqst_down(int bValue) {
        if ((ctx.localRequiredSpeed - bValue) >= 0) {
            ctx.localRequiredSpeed = ctx.localRequiredSpeed - bValue;
        } else {
            ctx.localRequiredSpeed = 0;
        }
        ctx.service.speed_rqst_set(ctx.localRequiredSpeed); //nastavenie pozadovanej rychlosti
    };

    /**
     * zvysenie a nastavenie hodnoty pozadovanej rychlosti
     * @param bValue    hodnota o ktoru sa ma zvysit rychlost
     */
    void Services::SpeedService::speed_rqst_up(int bValue) {
        if ((ctx.localRequiredSpeed + bValue) < 256) {
            ctx.localRequiredSpeed = ctx.localRequiredSpeed + bValue;
        } else {
            ctx.localRequiredSpeed = 255;
        }
        ctx.service.speed_rqst_set(ctx.localRequiredSpeed); //nastavenie pozadovanej rychlosti
    };

    /**
     * nastavenie pozadovanej rychlosti
     * @param value hodnota rychlosti ktora sa ma nastavit
     */
    void Services::SpeedService::speed_rqst_set(int value) {
        SunStorm::Message message("SpeedService::SpeedRqstSet"); //vytvorenie spravy pre PCU - nastavenie pozadovanej rychlosti rychlosti
        SpeedRqstSet content({value}); //vytvorenie obsahu sprvay
        message.SetContent<SpeedRqstSet>(content); //pridanie obsahu sprave
        this->SendMessage(message); //odoslanie spravy
        //test vystup
        std::cout << "speedRqstSet= " << ctx.localRequiredSpeed << endl;
    };

    /**
     * testovanie dlheho stlacenia tlacidla
     * @param btn   tlacidlo ktore bolo stlacene
     * @param s     referencia na servis kvoli predaniu listenera timeoutu
     */
    void Services::SpeedService::btn_start_timeout(int btn, SpeedService &s) {
        if (btn == 1) {
            if (ctx.btnUpTimeoutID == 0) {
                ctx.btnUpTimeoutID = s.RegisterTimeout(ctx.btnHoldTime, s.lTimeout); //spustenie casovaca pre btn1
            }
        } else if (btn == 2) {
            if (ctx.btnDownTimeoutID == 0) {
                ctx.btnDownTimeoutID = s.RegisterTimeout(ctx.btnHoldTime, s.lTimeout); //spustenie casovaca pre btn2
            }
        }
    };
}
