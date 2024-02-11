#pragma once
#include <ResponsiveAnalogRead.h>
#include <stdint.h>

namespace airPi
{

    class TempProbe
    {
    private:
        const uint8_t NUM_READINGS = 20;
        const float K_TO_C = -273.15;

        const float a;
        const float b;
        const float c;
        const int ref_resistance;
        const uint8_t adc_resolution;

        const uint8_t pin;

        ResponsiveAnalogRead analog;

        float calcTemp(float resistance);

    public:
        TempProbe(uint8_t input_pin, uint8_t adc_resolution);
        float getTemp();
        void update();
    };
}
