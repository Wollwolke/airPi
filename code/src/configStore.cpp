#include "configStore.hpp"
#include <EEPROM.h>

namespace airPi
{
    ConfigStore::ConfigStore() {}

    ConfigStore::~ConfigStore()
    {
        EEPROM.end();
    }

    void ConfigStore::begin()
    {
        EEPROM.begin(sizeof(Profile) * PROFILE_COUNT);
        readFromFlash();
    }

    ConfigStore::Profile *ConfigStore::getProfile(uint8_t profileNumber)
    {
        if (profileNumber < 1 || profileNumber > PROFILE_COUNT)
        {
            profileNumber = 1;
        }
        return &configStore[profileNumber - 1];
    }

    bool ConfigStore::store()
    {
        for (size_t i = 0; i < PROFILE_COUNT; ++i)
        {
            EEPROM.put(i * sizeof(Profile), configStore[i]);
        }

        return EEPROM.commit();
    }

    void ConfigStore::readFromFlash()
    {
        for (size_t i = 0; i < PROFILE_COUNT; ++i)
        {
            EEPROM.get(i * sizeof(Profile), configStore[i]);
        }
    }
}
