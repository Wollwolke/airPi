#include <Arduino.h>
#include "configuration.hpp"
#include "configStore.hpp"
#include "tempProbe.hpp"
#include "screen.hpp"
#include "fan.hpp"
#include "debugUtils.hpp"
#include <EasyButton.h>
#include <ResponsiveAnalogRead.h>

using namespace airPi;

void onPressed();
void onLongPress();
void onSequenceMatchedCallback();
void updateScreen();
void runTask();
void readProfile();
void serialSetup();
void checkForErrors();
uint16_t reverseAnalogValue(int16_t analogValue);
uint8_t analogToPercentage(int analogValue);

enum class State : uint8_t
{
    RUNNING,
    SETUP_FAN1,
    SETUP_FAN2,
    SETUP_SERIAL,
    FREEZE,
    ERROR,
};

EasyButton button(Pins::BUTTON, 35, true, true);
ResponsiveAnalogRead analog(Pins::TRIMMER, true);
FanControl fanControl(Pins::FAN_1, Pins::TACHO_1, Pins::FAN_2, Pins::TACHO_2);
TempProbe tempProbe(Pins::TEMP, 10);
Screen screen;
ConfigStore conf;

State state = State::RUNNING;
unsigned long timer = millis();
uint8_t profile_number = 1;
ConfigStore::Profile *profile = conf.getProfile(profile_number);

void setup()
{
    // PSU mode for potentially less noise on ADC
    pinMode(23, OUTPUT);
    digitalWrite(23, HIGH);

    // PWM Output
    analogWriteFreq(490);
    analogWriteResolution(10);

    // 3-way switch
    pinMode(Pins::MODE_1, INPUT_PULLUP);
    pinMode(Pins::MODE_2, INPUT_PULLUP);
    pinMode(Pins::MODE_3, INPUT_PULLUP);
    readProfile();

    // fans
    fanControl.begin();

    screen.begin();
    conf.begin();

    // Button
    button.begin();
    button.onPressed(onPressed);
    button.onPressedFor(2000, onLongPress);
    button.onSequence(3, 500, onSequenceMatchedCallback);

    Serial.setTimeout(SERIAL_TIMEOUT_MS);

    rp2040.wdt_begin(WATCHDOG_TIMEOUT);
    rp2040.wdt_reset();
}

void onPressed()
{
    switch (state)
    {
    case State::SETUP_FAN1:
        state = State::SETUP_FAN2;
        break;
    case State::SETUP_FAN2:
        state = State::SETUP_FAN1;
        break;
    default:
        screen.on();
        timer = millis();
        break;
    }
    D_print("Pressed\n");
}

void onLongPress()
{
    switch (state)
    {
    case State::SETUP_FAN1:
    case State::SETUP_FAN2:
        conf.store();
        timer = millis();
        state = State::RUNNING;
        break;
    case State::RUNNING:
        state = State::SETUP_FAN1;
        screen.on();
        break;
    default:
        screen.on();
        timer = millis();
        break;
    }
    D_print("Long press\n");
}

void onSequenceMatchedCallback()
{
    if (state == State::RUNNING)
    {
        state = State::SETUP_SERIAL;
    }
    D_println("Sequence");
}

void updateScreen()
{
    screen.setTemperature(tempProbe.getTemp());
    screen.setProfile(profile_number);
    screen.setRpm(0, fanControl.getSpeed(0));
    screen.setRpm(1, fanControl.getSpeed(1));
    screen.setSpeed(0, analogToPercentage(profile->fan_1));
    screen.setSpeed(1, analogToPercentage(profile->fan_2));
    switch (state)
    {
    case State::SETUP_FAN1:
        screen.setFanSetup(1);
        break;
    case State::SETUP_FAN2:
        screen.setFanSetup(2);
        break;
    case State::SETUP_SERIAL:
        screen.setFanSetup(3);
        break;
    case State::ERROR:
        break;
    default:
        screen.setFanSetup(0);
        break;
    }
}

void runTask()
{
    switch (state)
    {
    case State::RUNNING:
        // screen timeout
        if (timer + SCREEN_TIMEOUT_S * 1000 < millis())
        {
            screen.off();
        }

        // Control Fans
        fanControl.setPwm(0, profile->fan_1);
        fanControl.setPwm(1, profile->fan_2);
        break;
    case State::SETUP_FAN1:
    {
        auto analogValue = reverseAnalogValue(analog.getValue());
        profile->fan_1 = analogValue;
        fanControl.setPwm(0, analogValue);
    }
    break;
    case State::SETUP_FAN2:
    {
        auto analogValue = reverseAnalogValue(analog.getValue());
        profile->fan_2 = analogValue;
        fanControl.setPwm(1, analogValue);
    }
    break;
    case State::SETUP_SERIAL:
        serialSetup();
        break;
    case State::FREEZE:
        fanControl.setPwm(1, 0);
        if (tempProbe.getTemp() > TEMP_ANTIFREEZE_MAX)
        {
            state = State::RUNNING;
        }
        break;
    case State::ERROR:
        fanControl.setPwm(0, 0);
        fanControl.setPwm(1, 0);
        break;
    default:
        break;
    }
}

void readProfile()
{
    uint8_t old_profile = profile_number;

    if (digitalRead(Pins::MODE_1) == LOW)
    {
        profile_number = 1;
    }
    else if (digitalRead(Pins::MODE_2) == LOW)
    {
        profile_number = 2;
    }
    else if (digitalRead(Pins::MODE_3) == LOW)
    {
        profile_number = 3;
    }
    else
    {
        // missing connection --> profile 1
        profile_number = 1;
    }
    if (profile_number != old_profile)
    {
        profile = conf.getProfile(profile_number);
    }
}

void serialSetup()
{
    static uint8_t fan_number = 0;
    static uint16_t set_value = profile->fan_1;
    String serial_input;
    Serial.printf("Fan %i - Poti: %i RPM: %i\n", fan_number + 1, set_value, fanControl.getSpeed(fan_number));

    rp2040.wdt_begin(SERIAL_TIMEOUT_MS * 5);

    if (Serial.available() > 2)
    {
        serial_input = Serial.readStringUntil('\n');

        // Exit setup
        if (strstr(serial_input.c_str(), Commands::STORE))
        {
            state = State::RUNNING;
            conf.store();
            rp2040.wdt_begin(WATCHDOG_TIMEOUT);
        }
        // Switch Fan
        else if (strstr(serial_input.c_str(), Commands::SWITCH_FAN))
        {
            fan_number ^= 1;
        }
        else
        {
            set_value = atoi(serial_input.c_str());
            set_value = constrain(set_value, 0, 1023);
        }
        fanControl.setPwm(fan_number, set_value);
        if (fan_number == 0)
        {
            profile->fan_1 = set_value;
        }
        else
        {
            profile->fan_2 = set_value;
        }
    }
}

uint16_t reverseAnalogValue(int16_t analogValue)
{
    return 1023 - analogValue;
}

uint8_t analogToPercentage(int analogValue)
{
    return static_cast<uint8_t>(constrain(map(analogValue, 2, 1018, 0, 100), 0, 100));
}

void checkForErrors()
{
    // check Temperature sensor
    if (tempProbe.getTemp() < TEMP_MIN)
    {
        state = State::ERROR;
        screen.showError(Errors::TEMP_OPEN);
    }
    else if (tempProbe.getTemp() > TEMP_MAX)
    {
        state = State::ERROR;
        screen.showError(Errors::TEMP_SHORTED);
    }
    if (State::RUNNING == state && millis() / 1000 > STARTUP_TIME_S)
    {
        if (fanControl.getSpeed(0) <= 0)
        {
            state = State::ERROR;
            char text[26];
            strcpy(text, Errors::FAN_NO_TACHO_SIGNAL);
            text[24] = '1';
            text[25] = '\0';
            screen.showError(text);
        }
        else if (fanControl.getSpeed(1) <= 0)
        {
            state = State::ERROR;
            char text[26];
            strcpy(text, Errors::FAN_NO_TACHO_SIGNAL);
            text[24] = '2';
            text[25] = '\0';
            screen.showError(text);
        }
    }
}

void loop()
{
    rp2040.wdt_reset();
    button.read();
    tempProbe.update();
    analog.update();
    readProfile();
    updateScreen();
    runTask();

    // TODO: enable this check
    // checkForErrors();

    // TODO: check for TEMP_ANTIFREEZE_MIN
}
