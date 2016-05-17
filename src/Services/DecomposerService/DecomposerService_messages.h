/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DecomposerService_messages.h
 * Author: Lenovo
 *
 * Created on Pondelok, 2016, marca 14, 12:34
 */

#ifndef DECOMPOSERSERVICE_MESSAGES_H
#define DECOMPOSERSERVICE_MESSAGES_H
namespace Messages{
    namespace DecomposerService{
        
        struct ReCan0x10Cu{
            bool btnSpeedUp;
            bool btnSpeedDown;
            bool btnBreak;
            bool btnAutopilot;
            bool btnHorn;
            bool btnFrontLights;
            bool btnBlinkerL;
            bool btnBlinkerR;
            bool btnEmergencyFlasher;
            bool btnSwitchDisplay;
        };
        
        struct ReCan0x12ScState{
           bool breakLights;
           bool horn;
           bool frontLights;
           bool blinkerL;
           bool blinkerR;
        };
        
        struct ReCan0x20TmuMeasData{
            int data[8];
        };
       struct ReLan{
           int speedActual;
           int speedRqst;
           int speedOptimal;
       };
    };
};


#endif /* DECOMPOSERSERVICE_MESSAGES_H */

