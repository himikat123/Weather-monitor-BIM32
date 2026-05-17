#include "nextion.hpp"
#include "../../languages/languages.hpp"
#include "../../config/config.hpp"

void Nextion::init() {
    _nextion.writeStr("Hourly.MM.txt", lang.mm());
    _nextion.writeStr("Hourly.MS.txt", lang.ms());
    _nextion.writeStr("HistoryIn.In.txt", lang.historyIn());
    _nextion.writeStr("HistoryOut.Out.txt", lang.historyOut());
    
    _nextion.writeStr("Network.NETWORK.txt", lang.network());
    _nextion.writeStr("Network.SIGNALLEVEL.txt", lang.signalLevel());
    _nextion.writeStr("Network.IPADDRESS.txt", lang.ipAddr());
    _nextion.writeStr("Network.MACADDRESS.txt", lang.macAddr());
    _nextion.writeStr("Network.ESP32TEMP.txt", lang.esp32Temp());
    _nextion.writeStr("Network.FIRMWARE.txt", lang.firmware());
    _nextion.writeNum("BigClock.clockFormat.val", config.clock.format());

    for(unsigned int i=0; i<4; i++) {
        _nextion.writeStr("Main.nameSeq" + String(i) + ".txt", config.display.source.sequence.name(i));
    }
    _nextion.writeNum("Main.sequence.tim", config.display.source.sequence.dur() * 1000);

    /* Alarm */
    _nextion.writeStr("Texts.ALARM.txt", lang.alarm());
    
    /* Initialize NX4832K035 display  */
    if(config.display.model(0) == D_NX4832K035) {
        // config
        unsigned int langCode = 0;
        if(config.lang() == "de") langCode = 1;
        if(config.lang() == "ru") langCode = 2;
        if(config.lang() == "pl") langCode = 3;
        if(config.lang() == "ua") langCode = 4;
        if(config.lang() == "bg") langCode = 5;
        if(config.lang() == "es") langCode = 6;
        _nextion.writeNum("Main.lang.val", langCode);
        _nextion.writeNum("Main.weather.val", config.weather.provider());

        // texts
        String monthFullName[12] = {"JAN_", "FEB_", "MAR_", "APR_", "MAY_", "JUN_", "JUL_", "AUG_", "SEP_", "OCT_", "NOV_", "DEC_"};
        String monthDay[12] = {"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
        String monthShortName[12] = {"JAN", "FEB", "MAR", "APR", "MAI", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
        String weekdayFullName[7] = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};
        String weekdayShortName[7] = {"SU", "MO", "TU", "WE", "TH", "FR", "SA"};

        for(uint8_t i=0; i<12; i++) {
            _nextion.writeStr("Texts." + monthFullName[i] + ".txt", lang.monthFullName(i + 1));
            _nextion.writeStr("Texts." + monthDay[i] + ".txt", lang.monthDay(i + 1));
            _nextion.writeStr("Texts." + monthShortName[i] + ".txt", lang.monthShortName(i + 1));
        }

        for(uint8_t i=0; i<7; i++) {
            _nextion.writeStr("Texts." + weekdayFullName[i] + ".txt", lang.weekdayFullName(i + 1));
            _nextion.writeStr("Texts." + weekdayShortName[i] + ".txt", lang.weekdayShortName(i + 1));
        }
    }

    _nextion.writeNum("thup", 1);
    vTaskDelay(100);
    _nextion.writeNum("sleep", 0);
    vTaskDelay(100);
    _nextion.writeNum("dim", _prevBright > 0 ? _prevBright : 100);
    vTaskDelay(2000);
    _nextion.writeStr("page Main");
}

void Nextion::showLogo() {
    _nextion.writeNum("sleep", 0);
    _nextion.writeNum("dim", 100);
    _nextion.writeStr("page Logo");
}