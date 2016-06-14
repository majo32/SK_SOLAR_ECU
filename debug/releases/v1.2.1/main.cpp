#include <env.h>
#include <SunStorm/Framework.h>
#include <Services/CAN/CAN_0Service/CAN_0Service.h>
#include <Drivers/CAN/CANSimulatorConnector.h>
#include <Drivers/CAN/CANConnector.h>

#include <Services/SpeedService/SpeedService.h>
#include <Services/Blinker/BlinkerService.h>
#include <Services/FrontBreakLightService/FrontBreakLightService.h>
#include <Services/HornService/HornService.h>
#include <Services/LcdService/LcdService.h>

int main(int argc, char** argv) {
    SunStorm::Logger & logger = SunStorm::LoggerManager::RegisterLogger("MAIN");
    SunStorm::System::Initialize();
#if SOLAR_SIMULATION == 1
    Drivers::CAN::CANSimulatorConnector can_connector(logger);
#else
    Drivers::CAN::CANConnector can_connector(logger,"can0");
#endif
    can_connector.Connect();

    SunStorm::System::RegisterService<Services::CAN::CAN_0Service>("CAN::CAN_0Service", can_connector);
    SunStorm::System::RegisterService<Services::SpeedService>("SpeedService");
    SunStorm::System::RegisterService<Services::BlinkerService>("BlinkerService");
    SunStorm::System::RegisterService<Services::FrontBreakLightService>("FrontBreakLightService");
    SunStorm::System::RegisterService<Services::HornService>("HornService");
    SunStorm::System::RegisterService<Services::LcdService>("LcdService");
    SunStorm::Message msg("__INIT__");
    SunStorm::System::GetMessenger().SendMessage(msg);

    SunStorm::System::Join();

    can_connector.Disconnect();
    return 0;
}

