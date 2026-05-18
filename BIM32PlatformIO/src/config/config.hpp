#pragma once

#include <Arduino.h> 
#include "FS.h"
#include <LittleFS.h>
#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include "../globals.hpp"

#include "structures/comfort.hpp"
#include "structures/network.hpp"
#include "structures/accessPoint.hpp"
#include "structures/weather.hpp"
#include "structures/clock.hpp"
#include "structures/display/display.hpp"
#include "structures/sound.hpp"
#include "structures/sensors.hpp"
#include "structures/wsensor.hpp"
#include "structures/cloud/cloud.hpp"
#include "structures/alarm.hpp"
#include "structures/account.hpp"

class Config {
    private:
        uint16_t _calData[5] = { 0 }; // Touch calibration data
        char _lang[3] = "en"; // Localization
        uint8_t _units_pres = 0;

        template <size_t N>
        inline void _copy(JsonVariant from, char (&to)[N]) {
            strlcpy(to, from | (const char*)to, N);
        }

        template <typename T>
        inline void _copy(JsonVariant from, T& to) {
            to = from | to;
        }

        inline void _copy(JsonVariant from, bool& to) {
            to = from | to;
        }

    public:
        uint16_t calData(uint8_t num);
        String lang();
        uint8_t units_pres();
        void readConfig(void);
        void set_lang(String lng);
        void saveAlarmFile();

        ComfortStruct comfort;
        NetworkStruct network;
        AccessPointStruct accessPoint;
        WeatherStruct weather;
        ClockStruct clock;
        DisplayStruct display;
        SoundStruct sound;
        SensorsStruct sensors;
        WsensorStruct wsensor;
        CloudStruct cloud;
        AlarmStruct alarm;
        AccountStruct account;
};

extern Config config;