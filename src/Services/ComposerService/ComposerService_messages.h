/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ComposerService_messages.h
 * Author: Lenovo
 *
 * Created on Pondelok, 2016, mája 2, 18:38
 */

#ifndef COMPOSERSERVICE_MESSAGES_H
#define COMPOSERSERVICE_MESSAGES_H
namespace Messages{
    namespace ComposerService{
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
       struct ReLan{
           int speedActual;
           int speedRqst;
           int speedOptimal;
       };
       struct CanScCommand0x11{
           bool breakLights;
           bool horn;
           bool frontLights;
           bool blinkerL;
           bool blinkerR;
           
       };
       
       struct LanSpeedRqstSet0x20{
           int speedSet;
       };
       /*
        Odpovede pre skladac zo servisov svetiel
        */
       struct FrontLightResponse{
           bool state;
       };
       
       struct BlinkerResponse{
           bool right;
           bool left;
       };
       
       struct HornResponse{
           bool state;
       };
       
       struct BreakLightResponse{
           bool state;
       };
       
    };
};


#endif /* COMPOSERSERVICE_MESSAGES_H */

