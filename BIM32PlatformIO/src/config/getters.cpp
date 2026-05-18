#include "./config.hpp"

uint16_t Config::calData(uint8_t num) {
    if(num > 4) return 0;
    return _calData[num];
}

String Config::lang() {
    if(String(_lang) == "") return "en";
    return String(_lang);
}

uint8_t Config::units_pres() {
    return _units_pres ? 1 : 0;
}