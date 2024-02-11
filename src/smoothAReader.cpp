#include <Arduino.h>
#include "smoothAReader.hpp"

SmoothAReader::SmoothAReader(uint8_t pin) : pin(pin) {}

int SmoothAReader::getReading()
{
    return total / NUM_READINGS;
}

void SmoothAReader::read()
{
    total -= readings[readIndex];
    readings[readIndex] = analogRead(pin);
    total += readings[readIndex];
    readIndex = (readIndex + 1) % NUM_READINGS;
}
