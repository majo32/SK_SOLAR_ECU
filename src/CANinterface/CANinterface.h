/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CANinterface.h
 * Author: Lenovo
 *
 * Created on Streda, 2016, apríla 27, 22:58
 */

#ifndef CANINTERFACE_H
#define CANINTERFACE_H

class CANinterface {
public:

    struct CANmessage {
        int ID;
        int lenght;
        int data [8];
    };

    CANinterface();

    virtual ~CANinterface() {
    };
    CANmessage receiveCANMsg(CANmessage canMessage);
    int sendCANMsg(CANmessage canMessage);


};

#endif /* CANINTERFACE_H */

