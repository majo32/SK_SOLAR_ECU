/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LcdService.cpp
 * Author: jozef
 * 
 * Created on Piatok, 2016, marec 18, 11:10
 */

#include "LcdService.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sched.h>
#include <limits.h>
#include <string.h>
#include "IPC.h"
#include <iostream>

//#include "Services/LcdService/LcdService_Message.h"

void Services::LcdService::Initialize() {

    this ->RegisterListener("CAN_SC_STATE", this->l_can_sc_state);
    this ->RegisterListener("CAN_TEMP_CV", this->l_can_temp_cv);
    this ->RegisterListener("LAN_SWITCH", this->l_lan_switch);
    this ->RegisterListener("LAN_SPEED_RQST", this->l_lan_speed_rqst);
    this ->RegisterListener("LAN_SPEED_ACTUAL", this->l_lan_speed_actual);
    this ->RegisterListener("LAN_SPEED_OPT", this->l_lan_speed_opt);

}

void Services::LcdService::Listener_CAN_SC_STATE::handle(const SunStorm::Message& message) {
    Messages::LcdService::CAN_SC_STATE my_message = message.GetContent<Messages::LcdService::CAN_SC_STATE>();

    this->ctx.lcdService.msg_IPC.horn = my_message.horn;
    this->ctx.lcdService.msg_IPC.break_light = my_message.break_light;
    this->ctx.lcdService.msg_IPC.front_light = my_message.front_light;
    this->ctx.lcdService.msg_IPC.blinker_left = my_message.blinker_left;
    this->ctx.lcdService.msg_IPC.blinker_right = my_message.blinker_right;

    std::cout << "CAN_SC_STATE: " << std::endl;

    ctx.lcdService.send_msg_IPC();
}

void Services::LcdService::Listener_CAN_TEMP_CV::handle(const SunStorm::Message& message) {

    Messages::LcdService::CAN_TEMP_CV my_message = message.GetContent < Messages::LcdService::CAN_TEMP_CV>();

    this->ctx.lcdService.msg_IPC.tmp_BAT_avrg = my_message.tmp_BAT_avrg;
    this->ctx.lcdService.msg_IPC.tmp_BAT_max = my_message.tmp_BAT_max;
    this->ctx.lcdService.msg_IPC.tmp_ENG_avrg = my_message.tmp_ENG_avrg;
    this->ctx.lcdService.msg_IPC.tmp_ENG_max = my_message.tmp_ENG_max;
    this->ctx.lcdService.msg_IPC.tmp_SP_avrg = my_message.tmp_SP_avrg;
    this->ctx.lcdService.msg_IPC.tmp_SP_max = my_message.tmp_SP_max;
    this->ctx.lcdService.msg_IPC.tmp_CONV = my_message.tmp_CONV;

    ctx.lcdService.send_msg_IPC();

}

void Services::LcdService::Listener_LAN_SWITCH::handle(const SunStorm::Message& message) {

    Messages::LcdService::LAN_SWITCH my_message = message.GetContent < Messages::LcdService::LAN_SWITCH>();

    this->ctx.lcdService.msg_IPC.autopilot = my_message.autopilot;

    ctx.lcdService.send_msg_IPC();
}

void Services::LcdService::Listener_LAN_SPEED_RQST::handle(const SunStorm::Message& message) {

    Messages::LcdService::LAN_SPEED_RQST my_message = message.GetContent < Messages::LcdService::LAN_SPEED_RQST>();

    this->ctx.lcdService.msg_IPC.speed_rqst = my_message.speed_rqst;

    ctx.lcdService.send_msg_IPC();

}

void Services::LcdService::Listener_LAN_SPEED_ACTUAL::handle(const SunStorm::Message& message) {

    Messages::LcdService::LAN_SPEED_ACTUAL my_message = message.GetContent < Messages::LcdService::LAN_SPEED_ACTUAL>();

    this->ctx.lcdService.msg_IPC.speed_act = my_message.speed_act;

    ctx.lcdService.send_msg_IPC();
    
}

void Services::LcdService::Listener_LAN_SPEED_OPT::handle(const SunStorm::Message& message) {

    Messages::LcdService::LAN_SPEED_OPT my_message = message.GetContent < Messages::LcdService::LAN_SPEED_OPT>();

    this->ctx.lcdService.msg_IPC.speed_opt = my_message.speed_opt;
    
    ctx.lcdService.send_msg_IPC();
    
}
///poslanie struktury cez IPC do GUI aplikacie
void Services::LcdService::send_msg_IPC() {

    CAN_SC_STATE_IPC msg_IPCl;
    msg_IPCl.speed_opt = this->ctx.lcdService.msg_IPC.speed_opt;

    msg_IPCl.speed_act = this->ctx.lcdService.msg_IPC.speed_act;

    msg_IPCl.speed_rqst = this->ctx.lcdService.msg_IPC.speed_rqst;

    msg_IPCl.autopilot = this->ctx.lcdService.msg_IPC.autopilot;

    msg_IPCl.tmp_BAT_avrg = this->ctx.lcdService.msg_IPC.tmp_BAT_avrg;
    msg_IPCl.tmp_BAT_max = this->ctx.lcdService.msg_IPC.tmp_BAT_max;
    msg_IPCl.tmp_ENG_avrg = this->ctx.lcdService.msg_IPC.tmp_ENG_avrg;
    msg_IPCl.tmp_ENG_max = this->ctx.lcdService.msg_IPC.tmp_ENG_max;
    msg_IPCl.tmp_SP_avrg = this->ctx.lcdService.msg_IPC.tmp_SP_avrg;
    msg_IPCl.tmp_SP_max = this->ctx.lcdService.msg_IPC.tmp_SP_max;
    msg_IPCl.tmp_CONV = this->ctx.lcdService.msg_IPC.tmp_CONV;

    msg_IPCl.horn = this->ctx.lcdService.msg_IPC.horn;
    msg_IPCl.front_light = this->ctx.lcdService.msg_IPC.front_light;
    msg_IPCl.break_light = this->ctx.lcdService.msg_IPC.break_light;
    msg_IPCl.blinker_left = this->ctx.lcdService.msg_IPC.blinker_left;
    msg_IPCl.blinker_right = this->ctx.lcdService.msg_IPC.blinker_right;

    msg_IPCl.dist_cntrl = this->ctx.lcdService.msg_IPC.dist_cntrl;

    static const key_t msg_IPC_key = (key_t) 0x123123356;
    int msg_IPC_id = msgget(msg_IPC_key, IPC_CREAT | 0660);

    if (msgsnd(msg_IPC_id, &msg_IPCl, sizeof (msg_IPCl), 0) < 0) {
        Log().error("Could not send join message");
        exit(1);
    }

}
