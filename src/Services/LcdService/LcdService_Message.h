/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LcdService_Message.h
 * Author: jozef
 *
 * Created on Piatok, 2016, marec 18, 11:50
 */

#ifndef LCDSERVICE_MESSAGE_H
#define LCDSERVICE_MESSAGE_H
namespace Messages {
    namespace LcdService {
        ///struktury pre jednotlive typy sprav

        struct CAN_SC_STATE {
            bool break_light;
            bool front_light;
            bool blinker_left;
            bool blinker_right;
            bool horn;
        };

        struct CAN_TEMP_CV {
            int tmp_SP_avrg;
            int tmp_SP_max;
            int tmp_BAT_avrg;
            int tmp_BAT_max;
            int tmp_ENG_avrg;
            int tmp_ENG_max;
            int tmp_CONV;
        };

        struct LAN_SWITCH {
            bool autopilot;
        };

        struct LAN_SPEED_RQST {
            int speed_rqst;
        };

        struct LAN_SPEED_ACTUAL {
            int speed_act;
        };

        struct LAN_SPEED_OPT {
            int speed_opt;
        };


    };
};


#endif /* LCDSERVICE_MESSAGE_H */

