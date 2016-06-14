#include "LcdService.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sched.h>
#include <limits.h>
#include <string.h>
#include "IPC.h"
#include "Services/CAN/CAN_0Service/CAN_0Service_messages.h"
#include <iostream>

//#include "Services/LcdService/LcdService_Message.h"
namespace Services {

    void Services::LcdService::Initialize() {

        this ->RegisterListener("CAN::CAN_0Service::HornResponse", this->l_can_sc_state);
        this ->RegisterListener("CAN::CAN_0Service::BlinkerResponse", this->l_can_sc_state);
        this ->RegisterListener("CAN::CAN_0Service::FrontLightResponse", this->l_can_sc_state);
        this ->RegisterListener("CAN::CAN_0Service::BreakLightResponse", this->l_can_sc_state);
        this ->RegisterListener("CAN_TEMP_CV", this->l_can_temp_cv);
        this ->RegisterListener("CAN::CAN_0Service::ControlUnitRequest", this->l_lan_switch);
        this ->RegisterListener("CAN::CAN_0Service::SpeedRqstRequest", this->l_lan_speed_rqst);
        this ->RegisterListener("CAN::CAN_0Service::SpeedActRequest", this->l_lan_speed_actual);
        this ->RegisterListener("CAN::CAN_0Service::SpeedOptRequest", this->l_lan_speed_opt);

    }

    void Services::LcdService::Listener_CAN_SC_STATE::handle(const SunStorm::Message& message) {


        if (message.GetName() == "CAN::CAN_0Service::HornResponse") {

            this->ctx.lcdService.msg_IPC.horn = message.GetContent<Messages::CAN::CAN_0Service::HornResponse>().state;
        }
        if (message.GetName() == "CAN::CAN_0Service::BlinkerResponse") {

            this->ctx.lcdService.msg_IPC.blinker_left = message.GetContent<Messages::CAN::CAN_0Service::BlinkerResponse>().left;
            this->ctx.lcdService.msg_IPC.blinker_right = message.GetContent<Messages::CAN::CAN_0Service::BlinkerResponse>().right;
        }
        if (message.GetName() == "CAN::CAN_0Service::FrontLightResponse") {

            this->ctx.lcdService.msg_IPC.front_light = message.GetContent<Messages::CAN::CAN_0Service::FrontLightResponse>().state;
        }
        if (message.GetName() == "CAN::CAN_0Service::BreakLightResponse") {

            this->ctx.lcdService.msg_IPC.break_light = message.GetContent<Messages::CAN::CAN_0Service::BreakLightResponse>().state;
        }

        //std::cout << "CAN_SC_STATE: " << std::endl;

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



        if (message.GetName() == "CAN::CAN_0Service::ControlUnitRequest") {

            this->ctx.lcdService.msg_IPC.autopilot = message.GetContent<Messages::CAN::CAN_0Service::ControlUnitRequest>().btnAutopilot;
        }



        ctx.lcdService.send_msg_IPC();
    }

    void Services::LcdService::Listener_LAN_SPEED_RQST::handle(const SunStorm::Message& message) {



        if (message.GetName() == "CAN::CAN_0Service::SpeedRqstRequest") {

            this->ctx.lcdService.msg_IPC.speed_rqst = message.GetContent<Messages::CAN::CAN_0Service::SpeedRqstRequest>().value;
        }



        ctx.lcdService.send_msg_IPC();

    }

    void Services::LcdService::Listener_LAN_SPEED_ACTUAL::handle(const SunStorm::Message& message) {



        if (message.GetName() == "CAN::CAN_0Service::SpeedActRequest") {

            this->ctx.lcdService.msg_IPC.speed_act = message.GetContent<Messages::CAN::CAN_0Service::SpeedActRequest>().value;
        }



        ctx.lcdService.send_msg_IPC();

    }

    void Services::LcdService::Listener_LAN_SPEED_OPT::handle(const SunStorm::Message& message) {



        if (message.GetName() == "CAN::CAN_0Service::SpeedOptRequest") {

            this->ctx.lcdService.msg_IPC.speed_opt = message.GetContent<Messages::CAN::CAN_0Service::SpeedOptRequest>().value;
        }



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

        static const key_t msg_IPC_key = (key_t) 0x123123351;
        int msg_IPC_id = msgget(msg_IPC_key, IPC_CREAT | 0660);

        if (msgsnd(msg_IPC_id, &msg_IPCl, sizeof (msg_IPCl), 0) < 0) {
            Log().error("Could not send join message");
        }

    };
};