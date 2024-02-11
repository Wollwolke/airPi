#include <Arduino.h>

#include "tempProbe.hpp"
#include "configuration.hpp"
#include "debugUtils.hpp"

namespace airPi
{
    TempProbe::TempProbe(uint8_t input_pin, uint8_t adc_resolution) : a(TEMP_A), b(TEMP_B), c(TEMP_C), ref_resistance(TEMP_R), adc_resolution(adc_resolution), pin(input_pin){
        analog = ResponsiveAnalogRead(pin, false);
    }

    float TempProbe::getTemp()
    {
        float average_reading = analog.getValue();
        float resistance = ref_resistance * average_reading / (pow(2, adc_resolution) - average_reading);

        return calcTemp(resistance);
    }

    void TempProbe::update()
    {
        analog.update();
    }

    float TempProbe::calcTemp(float resistance)
    {
        float steinhart = a + (b * log(resistance) + c * pow(log(resistance), 3));
        return (1.0 / steinhart) + K_TO_C;
    }
}
