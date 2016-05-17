/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CANinterface.cpp
 * Author: Lenovo
 * 
 * Created on Streda, 2016, apríla 27, 22:58
 */

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <cstring>
#include <stdio.h>
#include <SunStorm/Framework.h>

#include "CANinterface.h"
using namespace std;

CANinterface::CANinterface() {
}
/**
 * 
 * @param canMessage -vstup spáva na naplnenie
 * @return vráti naplnenú správu 
 */
CANinterface::CANmessage CANinterface::receiveCANMsg(CANinterface::CANmessage canMessage) {
           cin >> canMessage.ID >> canMessage.lenght >> canMessage.data[0] >> canMessage.data[1]
                >> canMessage.data[2] >> canMessage.data[3] >> canMessage.data[4]
                >> canMessage.data[5] >> canMessage.data[6] >> canMessage.data[7];
           
           
           return canMessage;
           }
/**
 * 
 * @param canMessage - správa na odoslanie na CAN
 * @return 
 */
int CANinterface::sendCANMsg(CANmessage canMessage){
    cout << canMessage.ID << canMessage.lenght << canMessage.data[0] << canMessage.data[1]
                << canMessage.data[2] << canMessage.data[3] << canMessage.data[4]
                << canMessage.data[5] << canMessage.data[6] << canMessage.data[7]<<endl;
    
    return 0;
}






