/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IPC.h
 * Author: jozef
 *
 * Created on Pondelok, 2016, apríl 4, 10:57
 */

#ifndef IPC_H
#define IPC_H

#include <sys/types.h>
#include <cstdlib>
#include <iostream>

///sturktura, ktora sa bude posielat cez IPC
struct CAN_SC_STATE_IPC{
    
    ///velkost spravy
    long msg_type;

       ///hodnoty na stalacanie tlacidiel
    bool autopilot;
    bool blinker_left;
    bool blinker_right;
    bool front_light;
    bool break_light;
    bool horn;
    
        ///stav
    int state;

        ///hodnota baterie
    int batery_st_crnt;
    int batery_st_chrg;
    int batery_st_volt;

        ///vydialenost
    int dist_cntrl;
    int dist_finish;

        ///rychlost
    int speed_rqst;
    int speed_act;
    int speed_opt;

        ///teplota
    int tmp_SP_avrg;
    int tmp_SP_max;
    int tmp_BAT_avrg;
    int tmp_BAT_max;
    int tmp_ENG_avrg;
    int tmp_ENG_max;
    int tmp_CONV;

    ///GPS suradnice
        /// GPS_coor;
};
//char text[4]; ked sa bude nahodou posielat stav ako retazec
#endif /* IPC_H */

