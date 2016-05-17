/* 
 * File:   BlinkerService_message.h
 * Author: Simon
 *
 * Created on Streda, 2016, marec 9, 18:49
 */

#ifndef BLINKERSERVICE_MESSAGE_H
#define	BLINKERSERVICE_MESSAGE_H

namespace Messages {
    namespace BlinkerServiceS {

        struct CAN_CU {
            bool BrakePedal;
            bool btnHorn;
            bool btnFrontLight;
            bool btnLeftBlinker;
            bool btnRightBlinker;
            bool btnWarningLights;
            
        };

        struct CAN_SC {
            bool cmdBrakeLight;
            bool cmdHorn;
            bool cmdFrontLight;
            bool cmdLeftBlinker;
            bool cmdRightBlinker;
            bool cmdWarningLights;
        };
    };
};

#endif	/* BLINKERSERVICE_MESSAGE_H */

