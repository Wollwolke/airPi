#include "fan.hpp"
#include <Arduino.h>

namespace airPi
{
    static FanControl *instance = nullptr;

    FanControl::FanControl(uint8_t fan_1_pwm_pin, uint8_t fan_1_tacho_pin, uint8_t fan_2_pwm_pin, uint8_t fan_2_tacho_pin) : fan_1_pwm_pin(fan_1_pwm_pin),
                                                                                                                             fan_1_tacho_pin(fan_1_tacho_pin),
                                                                                                                             fan_2_pwm_pin(fan_2_pwm_pin),
                                                                                                                             fan_2_tacho_pin(fan_2_tacho_pin)
    {
    }

    void FanControl::begin()
    {
        // Tacho
        instance = this;
        pinMode(fan_1_tacho_pin, INPUT_PULLUP);
        pinMode(fan_2_tacho_pin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(fan_1_tacho_pin), isrFan1, FALLING);
        attachInterrupt(digitalPinToInterrupt(fan_2_tacho_pin), isrFan2, FALLING);

        // PWM
        pinMode(fan_1_pwm_pin, OUTPUT_8MA);
        pinMode(fan_2_pwm_pin, OUTPUT_8MA);
    }

    uint16_t FanControl::getSpeed(uint8_t fan)
    {
        switch (fan)
        {
        case 0:
            return fan_1_rpm.getSpeed();
        case 1:
            return fan_2_rpm.getSpeed();
        default:
            return 0;
        }
    }

    void FanControl::setPwm(uint8_t fan, uint16_t value)
    {
        switch (fan)
        {
        case 0:
            analogWrite(fan_1_pwm_pin, value);
            break;
        case 1:
            analogWrite(fan_2_pwm_pin, value);
            break;
        default:
            break;
        }
    }

    void FanControl::isrFan1()
    {
        instance->fan_1_rpm.interrupt();
    }

    void FanControl::isrFan2()
    {
        instance->fan_2_rpm.interrupt();
    }

}
