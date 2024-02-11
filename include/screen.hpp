#pragma once
#include <stdint.h>
#include <U8g2lib.h>

namespace airPi
{

    class Screen
    {
    private:
        U8G2_SSD1306_128X64_NONAME_F_HW_I2C *display;

        uint16_t rpm_1 = 0;
        uint16_t rpm_2 = 0;
        uint8_t speed_1 = 0;
        uint8_t speed_2 = 0;
        uint8_t profile = 0;
        uint8_t setup_fan = 0;
        float temperature = 0;
        bool error_occured = false;


        void updateBuffer();
        void updateStatusbar();
        void printWords(const char *msg, int xloc, int yloc /*bottom*/);

    public:
        Screen();
        ~Screen();
        void begin();
        void on();
        void off();
        void setTemperature(float temp);
        void setProfile(uint8_t profile);
        void setRpm(uint8_t fan, uint16_t value);
        void setSpeed(uint8_t fan, uint8_t value);
        void setFanSetup(uint8_t fan);
        void showError(const char error_msg[]);
    };
}
