#pragma once
#include <stdint.h>

#define DEBUG 1

constexpr uint8_t SCREEN_TIMEOUT_S = 120;
constexpr uint8_t STARTUP_TIME_S = 3;
constexpr uint8_t PROFILE_COUNT = 3;

constexpr uint16_t SERIAL_TIMEOUT_MS = 2000;
constexpr uint16_t WATCHDOG_TIMEOUT = 500;

// Antifreeze
constexpr int8_t TEMP_MIN = -40;
constexpr uint8_t TEMP_MAX = 50;
constexpr float TEMP_ANTIFREEZE_MIN = 1.5;
constexpr float TEMP_ANTIFREEZE_MAX = 3;

// Temperature sensor calibration
// Messgeräte
// constexpr float TEMP_A = 1.756973732e-3;
// constexpr float TEMP_B = 1.243720261e-4;
// constexpr float TEMP_C = 5.242421439e-7;

// Pico
constexpr float TEMP_A = 2.044911948e-3;
constexpr float TEMP_B = 0.7596940045e-4;
constexpr float TEMP_C = 7.252226919e-7;
constexpr int TEMP_R = 9860;
// 46 4,82 4,858
// 23.8 12.34 12,370
// -12  61.1   57000

// Pins
namespace Pins
{
    constexpr uint8_t MODE_1 = 12;
    constexpr uint8_t MODE_2 = 11;
    constexpr uint8_t MODE_3 = 10;
    constexpr uint8_t BUTTON = 15;
    constexpr uint8_t TACHO_1 = 16;
    constexpr uint8_t TACHO_2 = 17;
    constexpr uint8_t FAN_1 = 19;
    constexpr uint8_t FAN_2 = 21;
    constexpr uint8_t TRIMMER = 26;
    constexpr uint8_t TEMP = 27;
}

// Errors
namespace Errors
{
    constexpr char TEMP_SHORTED[] = "Temperature sensor shorted";
    constexpr char TEMP_OPEN[] = "Temperature sensor not connected";
    constexpr char FAN_NO_TACHO_SIGNAL[] = "No Tacho Signal for fan ";
}

// Commands
namespace Commands
{
    constexpr char SWITCH_FAN[] = "switch";
    constexpr char STORE[] = "store";
}
