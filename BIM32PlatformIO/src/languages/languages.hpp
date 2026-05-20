#pragma once
#include <Arduino.h>

class Lang {
    private:
        Lang() {}

        uint8_t _lang();

    public:
         static Lang& getInstance() {
            static Lang instance;
            return instance;
        }

        Lang(const Lang&) = delete;
        void operator=(const Lang&) = delete;

        const char* monthFullName(uint8_t num);
        const char* monthDay(uint8_t num);
        const char* monthShortName(uint8_t num);
        const char* weekdayShortName(uint8_t num);
        const char* weekdayFullName(uint8_t num);
        const char* alarm();
        const char* network();
        const char* signalLevel();
        const char* ipAddr();
        const char* macAddr();
        const char* esp32Temp();
        const char* firmware();
        const char* mm();
        const char* hpa();
        const char* ms();
        const char* gpm();
        const char* v();
        const char* historyIn();
        const char* historyOut();
        const char* comfort(unsigned int level);
        const char* airQuality(unsigned int level);
        const char* weatherDescription(unsigned int level);
        const char* touchCalibrate();
        const char* calibrationDone();
};

inline Lang& lang = Lang::getInstance();