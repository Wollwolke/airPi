#pragma once
#include <stdint.h>
#include "configuration.hpp"

namespace airPi
{
    class ConfigStore
    {
    public:
        struct Profile
        {
            uint16_t fan_1;
            uint16_t fan_2;
        };

        ConfigStore();
        ~ConfigStore();
        void begin();
        ConfigStore::Profile *getProfile(uint8_t profileNumber);
        bool store();

    private:
        Profile configStore[PROFILE_COUNT];
        void readFromFlash();
    };
}
