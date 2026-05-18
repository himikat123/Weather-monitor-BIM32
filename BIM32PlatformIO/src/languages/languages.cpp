#include <Arduino.h>

#include "./languages.hpp"
#include "../config/config.hpp"
#include "./translates/comfort.hpp"
#include "./translates/monthFullName.hpp"
#include "./translates/monthShortName.hpp"
#include "./translates/monthDay.hpp"
#include "./translates/weekdayFullName.hpp"
#include "./translates/weekdayShortName.hpp"
#include "./translates/weatherDescription.hpp"
#include "./translates/airQuality.hpp"
#include "./translates/texts.hpp"

Lang lang;

uint8_t Lang::_lang() {
    if(config.lang() == "de") return 1;
    if(config.lang() == "ru") return 2;
    if(config.lang() == "pl") return 3;
    if(config.lang() == "ua") return 4;
    if(config.lang() == "bg") return 5;
    if(config.lang() == "es") return 6;
    return 0;
}

const char* Lang::monthFullName(uint8_t num) {
    if(num < 1 || num > 12) return unknownTxt;
    return monthFullNameTxt[num - 1][_lang()];
}

const char* Lang::monthDay(uint8_t num) {
    if(num < 1 || num > 12) return unknownTxt;
    return monthDayTxt[num - 1][_lang()];
}

const char* Lang::monthShortName(uint8_t num) {
    if(num < 1 || num > 12) return unknownTxt;
    return monthShortNameTxt[num - 1][_lang()];
}

const char* Lang::weekdayShortName(uint8_t num) {
    if(num < 1 || num > 7) return unknownTxt;
    return weekdayShortNameTxt[num - 1][_lang()];
}

const char* Lang::weekdayFullName(uint8_t num) {
    if(num < 1 || num > 7) return unknownTxt;
    return weekdayFullNameTxt[num - 1][_lang()];
}

const char* Lang::alarm() {
    return alarmTxt[_lang()];
}

const char* Lang::network() {
    return networkTxt[_lang()];
}

const char* Lang::signalLevel() {
    return signalLevelTxt[_lang()];
}

const char* Lang::ipAddr() {
    return ipAddrTxt[_lang()];
}

const char* Lang::macAddr() {
    return macAddrTxt[_lang()];
}

const char* Lang::esp32Temp() {
    return esp32TempTxt[_lang()];
}

const char* Lang::firmware() {
    return firmwareTxt[_lang()];
}

const char* Lang::mm() {
    return mmTxt[_lang()];
}

const char* Lang::hpa() {
    return hpaTxt[_lang()];
}

const char* Lang::ms() {
    return msTxt[_lang()];
}

const char* Lang::gpm() {
    return gpmTxt[_lang()];
}

const char* Lang::v() {
    return vTxt[_lang()];
}

const char* Lang::historyIn() {
    return historyInTxt[_lang()];
}

const char* Lang::historyOut() {
    return historyOutTxt[_lang()];
}

const char* Lang::comfort(unsigned int level) {
    if(level < 1 || level > 9) return unknownTxt;
    return comfortTxt[level - 1][_lang()];
}

const char* Lang::airQuality(unsigned int level) {
    if(level < 1 || level > 3) return unknownTxt;
    return airQualityTxt[level - 1][_lang()];
}

const char* Lang::weatherDescription(unsigned int level) {
    if(level > 15) return unknownTxt;
    return weatherDescriptionTxt[level][_lang()];
}

const char* Lang::touchCalibrate() {
    return touchCalibrateTxt[_lang()];
}

const char* Lang::calibrationDone() {
    return calibrationDoneTxt[_lang()];
}