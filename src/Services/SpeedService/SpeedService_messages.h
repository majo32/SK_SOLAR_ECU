/* 
 * File:   SpeedService_messages.h
 * Author: Michal Moravcik
 *
 * Created on Utorok, 2016, marca 8, 22:50
 */

#ifndef SPEEDSERVICE_MESSAGES_H
#define	SPEEDSERVICE_MESSAGES_H

namespace Messages {
    namespace SpeedService {
        //sprava "SpeedService::AutopilotStatus" pre PCU a display - nastavit pozadovanu rychlost        

        struct SpeedRqstSet {
            int value;
        };
        //sprava "SpeedService::SpeedRqstSet" pre PCU a display - zapnutie/vypnutie autopilota        

        struct AutopilotStatus {
            bool status;
        };
        //prijimane spravy
        //sprava od CU

        struct CanCuStatus {
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

        struct PCU_SpeedAct {
            int value;
        };
        struct PCU_SpeedOpt {
            int value;
        };
        struct PCU_SpeedRqst {
            int value;
        };
        

    }
}

#endif	/* SPEEDSERVICE_MESSAGES_H */

