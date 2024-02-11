#pragma once
#include <stdint.h>
#include "FanRPM.h"

namespace airPi
{

    class FanControl
    {
    private:
        const uint8_t fan_1_pwm_pin;
        const uint8_t fan_2_pwm_pin;
        const uint8_t fan_1_tacho_pin;
        const uint8_t fan_2_tacho_pin;

        FanRPM fan_1_rpm;
        FanRPM fan_2_rpm;

        static void isrFan1();
        static void isrFan2();

    public:
        FanControl(uint8_t fan_1_pwm_pin, uint8_t fan_1_tacho_pin, uint8_t fan_2_pwm_pin, uint8_t fan_2_tacho_pin);
        void begin();
        uint16_t getSpeed(uint8_t fan);
        void setPwm(uint8_t fan, uint16_t value);
    };
}
