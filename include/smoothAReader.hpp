#pragma once
#include <stdint.h>

class SmoothAReader
{
private:
    static constexpr auto NUM_READINGS = 20;

    uint8_t pin = 0;
    int readings[NUM_READINGS] = {0};
    int total = 0;
    int readIndex = 0;
    int average = 0;

public:
    SmoothAReader(uint8_t pin);
    int getReading();
    void read();
};
