/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComposerService.cpp
 * Author: Lenovo
 * 
 * Created on Pondelok, 2016, marca 14, 12:35
 */

#include "ComposerService.h"
#include "Services/DecomposerService/DecomposerService.h"
#include "math.h"

CANinterface * canInt = new CANinterface();
CANinterface::CANmessage CANmsg;

namespace Services {
    /**
     * Inicializácia - zaregisrtovanie všetkých listenerov
     */
    void Services::ComposerService::Initialize() {
        this->RegisterListener("ReCanCu", this->l1); //zaregistrovanie listenera na správy, neskôr pre každý servis
        this->RegisterInterval(5000, this->l2); // listener odosielajúci správy na can v intervale
    };
    /**
     * handl funkcia listenera počúvajúceho na správu od decomposera - vzor
     * @param message - prijatá správa predávaná ako parameter
     */
    void Services::ComposerService::Listener_ReCan0x10Cu::handle(const SunStorm::Message& message) {
        Messages::DecomposerService::ReCan0x10Cu reCanCu = message.GetContent<Messages::DecomposerService::ReCan0x10Cu>();
        cout << "handle" << endl;



        context.cMsg0x11.data[0] = ((reCanCu.btnSpeedUp * pow(2, 1)) + (reCanCu.btnSpeedDown * pow(2, 2))+ (reCanCu.btnBreak * pow(2, 3))+ (reCanCu.btnAutopilot * pow(2, 4)));




    };
    /**
     * funkcia listenera periodicky posielajúca správy na CAN
     * @param message  - odosielaná správa predávaná ako parameter
     */
    void Services::ComposerService::Listener_SendCanMsg::handle(const SunStorm::Message& message) {
        context.cMsg0x11.ID = 11;
        context.cMsg0x11.lenght = 8;
        canInt->sendCANMsg(context.cMsg0x11);
    };
}

