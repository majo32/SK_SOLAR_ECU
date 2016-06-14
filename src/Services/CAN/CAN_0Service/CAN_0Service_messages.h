/* 
 * File:   CAN_0DecomposerService_messages.h
 * Author: majo32
 *
 * Created on Utorok, 2016, mája 24, 18:51
 */
#pragma once
#ifndef CAN_0DECOMPOSERSERVICE_MESSAGES_H
#define	CAN_0DECOMPOSERSERVICE_MESSAGES_H

namespace Messages {
    namespace CAN {
        namespace CAN_0Service {

            struct ControlUnitRequest {
                bool btnSpeedUp;
                bool btnSpeedDown;
                bool brakePedal;
                bool btnAutopilot;
                bool btnHorn;
                bool btnFrontLights;
                bool btnLeftBlinker;
                bool btnRightBlinker;
                bool btnWarningLights;
                bool btnSwitchDisplay;
            };

            struct SpeedActRequest {
                std::uint8_t value;
            };

            struct SpeedOptRequest {
                std::uint8_t value;
            };

            struct SpeedRqstRequest {
                std::uint8_t value;
            };
            
            
            struct AutopilotResponse {
                bool status;
            };
            
            struct SpeedRqstResponse {
                std::uint8_t value;
            };

            struct FrontLightResponse {
                bool state;
            };

            struct BlinkerResponse {
                bool right;
                bool left;
            };

            struct HornResponse {
                bool state;
            };

            struct BreakLightResponse {
                bool state;
            };
        }
    }
}


#endif	/* CAN_0DECOMPOSERSERVICE_MESSAGES_H */

