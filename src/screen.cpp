#include "screen.hpp"

namespace airPi
{
    Screen::Screen()
    {
        display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R2, U8X8_PIN_NONE);
    }

    Screen::~Screen()
    {
        display->clearBuffer();
        delete display;
    }

    void Screen::begin()
    {
        display->begin();
    }

    void Screen::on()
    {
        display->setPowerSave(0);
    }

    void Screen::off()
    {
        display->setPowerSave(1);
    }

    void Screen::setTemperature(float temp)
    {
        if (temperature == temp)
            return;
        temperature = temp;
        updateBuffer();
    }

    void Screen::setProfile(uint8_t profile)
    {

        if (this->profile == profile)
            return;
        this->profile = profile;
        updateBuffer();
    }

    void Screen::setRpm(uint8_t fan, uint16_t value)
    {
        switch (fan)
        {
        case 0:
            if (rpm_1 == value)
                return;
            rpm_1 = value;
            break;
        case 1:
            if (rpm_2 == value)
                return;
            rpm_2 = value;
            break;
        default:
            break;
        }
        updateBuffer();
    }

    void Screen::setSpeed(uint8_t fan, uint8_t value)
    {
        switch (fan)
        {
        case 0:
            if (speed_1 == value)
                return;
            speed_1 = value;
            break;
        case 1:
            if (speed_2 == value)
                return;
            speed_2 = value;
            break;
        default:
            break;
        }
        updateBuffer();
    }

    void Screen::setFanSetup(uint8_t fan)
    {
        if (setup_fan == fan)
            return;
        setup_fan = fan;
        updateBuffer();
    }

    void Screen::showError(const char error_msg[])
    {
        error_occured = true;
        display->clearBuffer();
        display->setFont(u8g2_font_profont15_tr);
        printWords(error_msg, 0, display->getMaxCharHeight());
        display->sendBuffer();
    }

    void Screen::updateBuffer()
    {
        // TODO: this should not be necessary, check & remove
        if (error_occured)
            return;

        uint screen_width = display->getDisplayWidth();
        display->clearBuffer();

        // draw lines
        display->drawVLine(64, 0, 44);
        display->drawHLine(0, 50, screen_width);

        // draw fan values
        char text[4];
        display->setFont(u8g2_font_profont22_mf);
        int x_offset = ((screen_width / 2) - (display->getMaxCharWidth() * 4)) / 2;
        int y_offset = display->getMaxCharHeight();
        // RPM 1
        sprintf(text, "%*i", sizeof(text), rpm_1);
        display->drawStr(x_offset, y_offset, text);
        // RPM 2
        sprintf(text, "%*i", sizeof(text), rpm_2);
        display->drawStr(x_offset + (screen_width / 2) + 1, y_offset, text);
        // Speed 1
        sprintf(text, "%*i%%", sizeof(text) - 1, speed_1); // padding to 3 characters because of % sign
        display->drawStr(x_offset, y_offset * 2, text);
        // Speed 2
        sprintf(text, "%*i%%", sizeof(text) - 1, speed_2);
        display->drawStr(x_offset + (screen_width / 2) + 1, y_offset * 2, text);

        // draw status bar
        updateStatusbar();

        display->sendBuffer();
    }

    void Screen::updateStatusbar()
    {
        char profile_string[10];
        char temp[10];
        // Set Fan 1
        display->setFont(u8g2_font_profont11_mf);
        int x_offset = display->getDisplayWidth() - 1 - (sizeof(temp) * display->getMaxCharWidth());
        // Profile
        sprintf(profile_string, "Profile: %i", profile);
        display->drawStr(0, 63, profile_string);

        switch (setup_fan)
        {
        case 0:
            // Temp
            sprintf(temp, "T: %5.1f\xb0"
                          "C",
                    temperature);
            break;
        case 3:
            sprintf(temp, "Setup USB");
            break;
        default:
            // Setup
            sprintf(temp, "Set Fan %i", setup_fan);
        }
        display->drawStr(x_offset, 63, temp);
    }

    // Source https://github.com/olikraus/u8g2/discussions/1479#discussioncomment-6232867
    void Screen::printWords(const char *msg, int xloc, int yloc /*bottom*/)
    {
        int dspwidth = display->getDisplayWidth();
        int strwidth = 0; // string width in pixels
        char glyph[2];
        glyph[1] = 0;

        for (const char *ptr = msg, *lastblank = NULL; *ptr; ++ptr)
        {
            while (xloc == 0 && *msg == ' ')
                if (ptr == msg++)
                    ++ptr; // skip blanks at the left edge

            glyph[0] = *ptr;
            strwidth += display->getStrWidth(glyph); // accumulate the pixel width
            if (*ptr == ' ')
                lastblank = ptr; // remember where the last blank was
            else
                ++strwidth; // non-blanks will be separated by one additional pixel

            if (xloc + strwidth > dspwidth)
            { // if we ran past the right edge of the display
                int starting_xloc = xloc;
                while (msg < (lastblank ? lastblank : ptr))
                { // print to the last blank, or a full line
                    glyph[0] = *msg++;
                    xloc += display->drawStr(xloc, yloc, glyph);
                }

                strwidth -= xloc - starting_xloc;    // account for what we printed
                yloc += display->getMaxCharHeight(); // advance to the next line
                xloc = 0;
                lastblank = NULL;
            }
        }
        while (*msg)
        { // print any characters left over
            glyph[0] = *msg++;
            xloc += display->drawStr(xloc, yloc, glyph);
        }
    }
}
