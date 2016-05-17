/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DecomposerService.cpp
 * Author: Lenovo
 * 
 * Created on Pondelok, 2016, marca 14, 12:34
 */

#include "DecomposerService.h"
#include <iostream>

using namespace std;
namespace Services {

    CANinterface * canInt = new CANinterface();
    CANinterface::CANmessage CANmsg;
    //Context* context = new Context();

    void Services::DecomposerService::Initialize() {
        // this->RegisterListener("start", this->l1);


    };

    /**
     * funkcia s prideleným vláknom počúvajúca na CAN pomocou interfejsu 
     */
    void DecomposerService::ThreadCan::Run() {
        int pom = 0;
        while (1) {
            CANmsg = canInt->receiveCANMsg(CANmsg);
            cout << CANmsg.ID;
            switch (CANmsg.ID) {
                case 10:
                {
                    /* cout << "ID 0x10" << endl << CANmsg.ID << " " << CANmsg.lenght << " " << CANmsg.data [0] << " " << CANmsg.data [1] << " "
                             << CANmsg.data [2] << " " << CANmsg.data [3] << " " << CANmsg.data [4] << " "
                             << CANmsg.data [5] << " " << CANmsg.data [6] << " " << CANmsg.data [7] << " " << endl;
                     */
                    //pre nultý bajt 
                    pom = CANmsg.data[0];
                    pom = pom / 2;
                    cout << pom << endl;
                    context.ReCan0x10Cu[1] = pom % 2;
                    pom = pom / 2;
                    cout << pom << endl;
                    context.ReCan0x10Cu[2] = pom % 2;
                    pom = pom / 2;
                    cout << pom << endl;
                    context.ReCan0x10Cu[3] = pom % 2;
                    pom = pom / 2;
                    cout << pom << endl;
                    context.ReCan0x10Cu[4] = pom % 2;

                    //pre prvý bajt
                    pom = CANmsg.data[1];
                    pom = pom / 2;
                    context.ReCan0x10Cu[9] = pom % 2;
                    pom = pom / 2;
                    context.ReCan0x10Cu[10] = pom % 2;
                    pom = pom / 2;
                    context.ReCan0x10Cu[11] = pom % 2;
                    pom = pom / 2;
                    context.ReCan0x10Cu[12] = pom % 2;
                    pom = pom / 2;
                    context.ReCan0x10Cu[13] = pom % 2;

                    //pre druhý bajt
                    pom = CANmsg.data[2];
                    pom = pom / 2;
                    context.ReCan0x10Cu[16] = pom % 2;

                    //cout << "tu som" << endl;
                    Messages::DecomposerService::ReCan0x10Cu reCanCu;

                    reCanCu.btnSpeedUp = context.ReCan0x10Cu[1];
                    reCanCu.btnSpeedDown = context.ReCan0x10Cu[2];
                    reCanCu.btnBreak = context.ReCan0x10Cu[3];
                    reCanCu.btnAutopilot = context.ReCan0x10Cu[4];
                    reCanCu.btnHorn = context.ReCan0x10Cu[9];
                    reCanCu.btnFrontLights = context.ReCan0x10Cu[10];
                    reCanCu.btnBlinkerL = context.ReCan0x10Cu[11];
                    reCanCu.btnBlinkerR = context.ReCan0x10Cu[12];
                    reCanCu.btnEmergencyFlasher = context.ReCan0x10Cu[13];
                    reCanCu.btnSwitchDisplay = context.ReCan0x10Cu[16];
                    //cout << "naplnene" << endl;
                    SunStorm::Message mReCanCu("ReCanCu");
                    mReCanCu.SetContent<Messages::DecomposerService::ReCan0x10Cu>(reCanCu);
                    //cout << "odosielam" << endl;
                    
                    GetService().SendMessage(mReCanCu);
                    //cout << "odoslane" << endl;
                }
                    break;


                case 12:
                {
                    cout << "ID 0x12 " << CANmsg.ID << " " << CANmsg.lenght << " " << CANmsg.data [0] << " " << CANmsg.data [1] << " "
                            << CANmsg.data [2] << " " << CANmsg.data [3] << " " << CANmsg.data [4] << " "
                            << CANmsg.data [5] << " " << CANmsg.data [6] << " " << CANmsg.data [7] << endl;


                    //rozloženie správy na bity
                    pom = CANmsg.data[0];
                    pom = pom / 2;
                    cout << pom << endl;
                    context.ReCan0x12Cu[1] = pom % 2;
                    pom = pom / 2;
                    cout << pom << endl;
                    context.ReCan0x12Cu[2] = pom % 2;
                    pom = pom / 2;
                    cout << pom << endl;
                    context.ReCan0x12Cu[3] = pom % 2;
                    pom = pom / 2;
                    cout << pom << endl;
                    context.ReCan0x12Cu[4] = pom % 2;
                    pom = pom / 2;
                    cout << pom << endl;
                    context.ReCan0x12Cu[5] = pom % 2;

                    Messages::DecomposerService::ReCan0x12ScState reCanScState; //vytvorenie správy

                    //naplnenie internej správy
                    reCanScState.breakLights = context.ReCan0x12Cu[1];
                    reCanScState.horn = context.ReCan0x12Cu[2];
                    reCanScState.frontLights = context.ReCan0x12Cu[3];
                    reCanScState.blinkerL = context.ReCan0x12Cu[4];
                    reCanScState.blinkerR = context.ReCan0x12Cu[5];

                    SunStorm::Message mReCanScState("ReCanScState");
                    mReCanScState.SetContent<Messages::DecomposerService::ReCan0x12ScState>(reCanScState);
                    GetService().SendMessage(mReCanScState); //odoslanie správy
                }
                    break;

                case 20:
                {

                    cout << "ID 0x20" << CANmsg.ID << CANmsg.lenght << CANmsg.data [0] << CANmsg.data [1]
                            << CANmsg.data [2] << CANmsg.data [3] << CANmsg.data [4]
                            << CANmsg.data [5] << CANmsg.data [6] << CANmsg.data [7];

                    Messages::DecomposerService::ReCan0x20TmuMeasData reCanTmuMeasData;
                    reCanTmuMeasData.data[0] = CANmsg.data[0];
                    reCanTmuMeasData.data[1] = CANmsg.data[1];
                    reCanTmuMeasData.data[2] = CANmsg.data[2];
                    reCanTmuMeasData.data[3] = CANmsg.data[3];
                    reCanTmuMeasData.data[4] = CANmsg.data[4];
                    reCanTmuMeasData.data[5] = CANmsg.data[5];
                    reCanTmuMeasData.data[6] = CANmsg.data[6];
                    reCanTmuMeasData.data[7] = CANmsg.data[7];

                    SunStorm::Message mReCanTmuMeasData("ReCanTmuMeasData");
                    mReCanTmuMeasData.SetContent<Messages::DecomposerService::ReCan0x20TmuMeasData>(reCanTmuMeasData);
                    GetService().SendMessage(mReCanTmuMeasData);
                }
                    break;
            }
        }
    }

    // void Services::DecomposerService::Listener_ReCan0x10Cu::handle(const SunStorm::Message& message) {



    //};


};








