#include <EasyNextionLibrary.h> // v1.0.6 https://github.com/Seithan/EasyNextionLibrary
EasyNex nex(Serial1);

class Nextion : LcdDisplay {
    #define NX4832K035 0
    #define NX4832T035 1
    #define OPENWEATHERMAP 0
    #define WEATHERBIT 1
    #define OPEN_METEO 2

    public:
        void init();
        void showLogo();
        void refresh();
        void displayToggle();
        void displayOn(bool doinit = true);
        void displayOff();
        bool isDisplayOn();
        void brightness(unsigned int bright);
        void setDisplayRTC();
        void dataReceive();

    private:
        int _customData = -1; // flag of key symbols of receive from the display
        String _receivedData = "";
        uint16_t _air_color[4] = { 2016, 65520, 64512, 63488 };

        void _NX4832K035_setRTC();
        void _NX4832T035_timeDate();
        void _networkPage();
        void _showVoltage();
        void _showBattery();
        void _showAntenna();
        void _showTempIn();
        void _showTempOut();
        void _showHumIn();
        void _showHumOut();
        void _showPres();
        void _showSequence();
        void _showComfortLevel();
        void _showWeatherDescription();
        void _showCurrentIcon();
        void _showThermometer();
        void _showWindSpeed();
        void _showWindDirection();
        void _showUpdated();
        void _showWeatherForecast();
        void _hourlyData();
        void _daily2hourly();
        void _historyOut();
        void _historyIn();
        void _alarms();
};

void Nextion::init() {
    nex.writeStr("Hourly.MM.txt", lang.mm());
    nex.writeStr("Hourly.MS.txt", lang.ms());
    nex.writeStr("HistoryIn.In.txt", lang.historyIn());
    nex.writeStr("HistoryOut.Out.txt", lang.historyOut());
    
    nex.writeStr("Network.NETWORK.txt", lang.network());
    nex.writeStr("Network.SIGNALLEVEL.txt", lang.signalLevel());
    nex.writeStr("Network.IPADDRESS.txt", lang.ipAddr());
    nex.writeStr("Network.MACADDRESS.txt", lang.macAddr());
    nex.writeStr("Network.ESP32TEMP.txt", lang.esp32Temp());
    nex.writeStr("Network.FIRMWARE.txt", lang.firmware());
    nex.writeNum("BigClock.clockFormat.val", config.clock_format());

    for(unsigned int i=0; i<4; i++) {
        nex.writeStr("Main.nameSeq" + String(i) + ".txt", config.display_source_sequence_name(i));
    }
    nex.writeNum("Main.sequence.tim", config.display_source_sequence_dur() * 1000);

    /* Alarm */
    nex.writeStr("Texts.ALARM.txt", lang.alarm());
    
    /* Initialize NX4832K035 display  */
    if(config.display_model(0) == NX4832K035) {
        // config
        unsigned int langCode = 0;
        if(config.lang() == "de") langCode = 1;
        if(config.lang() == "ru") langCode = 2;
        if(config.lang() == "pl") langCode = 3;
        if(config.lang() == "ua") langCode = 4;
        if(config.lang() == "bg") langCode = 5;
        nex.writeNum("Main.lang.val", langCode);
        nex.writeNum("Main.weather.val", config.weather_provider());

        // texts
        String monthFullName[12] = {"JAN_", "FEB_", "MAR_", "APR_", "MAY_", "JUN_", "JUL_", "AUG_", "SEP_", "OCT_", "NOV_", "DEC_"};
        String monthDay[12] = {"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
        String monthShortName[12] = {"JAN", "FEB", "MAR", "APR", "MAI", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
        String weekdayFullName[7] = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};
        String weekdayShortName[7] = {"SU", "MO", "TU", "WE", "TH", "FR", "SA"};

        for(uint8_t i=0; i<12; i++) {
            nex.writeStr("Texts." + monthFullName[i] + ".txt", lang.monthFullName(i + 1));
            nex.writeStr("Texts." + monthDay[i] + ".txt", lang.monthDay(i + 1));
            nex.writeStr("Texts." + monthShortName[i] + ".txt", lang.monthShortName(i + 1));
        }

        for(uint8_t i=0; i<7; i++) {
            nex.writeStr("Texts." + weekdayFullName[i] + ".txt", lang.weekdayFullName(i + 1));
            nex.writeStr("Texts." + weekdayShortName[i] + ".txt", lang.weekdayShortName(i + 1));
        }
    }

    nex.writeNum("thup", 1);
    vTaskDelay(100);
    nex.writeNum("sleep", 0);
    vTaskDelay(100);
    nex.writeNum("dim", _prevBright > 0 ? _prevBright : 100);
    vTaskDelay(2000);
    nex.writeStr("page Main");
}

void Nextion::showLogo() {
    nex.writeNum("sleep", 0);
    nex.writeNum("dim", 100);
    nex.writeStr("page Logo");
}

void Nextion::refresh() {
    if(millis() - _prevForced > 10000) {
        _forced = true;
        _prevForced = millis();
        nex.writeNum("BigClock.clockFormat.val", config.clock_format());
    }

    if(_prevPower != _power or _forced) {
        nex.writeNum("sleep", _power ? 0 : 1);
        _prevPower = _power;
    }

    if(_power) {
        _getData();

        if(config.display_model(0) == NX4832T035) _NX4832T035_timeDate();
        if(config.display_model(0) == NX4832K035) {
            if(global.clockSynchronize) {
                _NX4832K035_setRTC();
                global.clockSynchronize = false;
            }
        }

        _networkPage();
        _showComfortLevel();
        _showVoltage();
        _showBattery();
        _showAntenna();

        _showTempIn();
        _showHumIn();
        _showSequence();
  
        _showWeatherDescription();
        _showCurrentIcon();
        _showTempOut();
        _showHumOut();
        _showPres();
        _showWindSpeed();
        _showWindDirection();
        _showUpdated();
        _showWeatherForecast();
        _hourlyData();
        _historyOut();
        _historyIn();
        _alarms();
    }
}

/**
 * Change display brightness
 */
void Nextion::brightness(unsigned int bright) {
    uint8_t br = global.reduc[0] ? round(bright / 2) : bright;
    if(br < config.display_brightness_min(0)) br = config.display_brightness_min(0);
    if(br > config.display_brightness_max(0)) br = config.display_brightness_max(0); 
    if(_prevBright != br or _forced) {
        if(_power) nex.writeNum("dim", br);
        _prevBright = br;
        _forced = false;
    }
}

/**
 * Toggles display (on/off)
 */
void Nextion::displayToggle() {
    if(_power) nex.writeNum("dim", 0);
    else init();
    _power = !_power;
}

/**
 * Turns on the display
 */
void Nextion::displayOn(bool doinit) {
    _power = true;
    if(doinit) init();
}

/*
 * Turns off the display
 */
void Nextion::displayOff() {
    nex.writeNum("dim", 0);
    _power = false;
}

/**
 * Returns true if display is on
 */
bool Nextion::isDisplayOn() {
    return _power;
}

/**
 * Set the time and date of the display with built-in RTC
 */
void Nextion::setDisplayRTC() {
    if(config.display_model(0) == NX4832K035) _NX4832K035_setRTC();
}

/**
 * Set the time and date of the NX4832K035 display
 */
void Nextion::_NX4832K035_setRTC() {
    if(now() > 1700000000) {
        nex.writeNum("rtc5", second());
        nex.writeNum("rtc4", minute());
        nex.writeNum("rtc3", hour());
        nex.writeNum("rtc2", day());
        nex.writeNum("rtc1", month());
        nex.writeNum("rtc0", year());
    }
}

/**
 * Sending time and date to the NX4832T035 display that does not have a built-in RTC
 */
void Nextion::_NX4832T035_timeDate() {
    if(_prevTHour != _tHour or _forced) {
        char buf[4];
        if(config.clock_format() % 2 == 0) sprintf(buf, "%d", _tHour);
        else sprintf(buf, "%02d", _tHour);
        nex.writeStr("BigClock.hour.txt", String(buf));
        _prevTHour = _tHour;
    }
    if(_prevTMinute != _tMinute or _forced) {
        char buf[4];
        sprintf(buf, "%02d", _tMinute);
        nex.writeStr("BigClock.minute.txt", String(buf));
        _prevTMinute = _tMinute;
    }
    if(_prevTDay != _tDay or _prevTMonth != _tMonth or _prevTYear != _tYear or _forced) {
        nex.writeStr("BigClock.date.txt", String(_tDay) + " " + lang.monthDay(_tMonth) + " " + String(_tYear));
        _prevTDay = _tDay;
        _prevTMonth = _tMonth;
        _prevTYear = _tYear;
    }
    if(_prevTWeekday != _tWeekday or _forced) {
        unsigned int wd = _tWeekday;
        nex.writeStr("BigClock.weekday.txt", lang.weekdayFullName(wd));
        nex.writeStr("Main.weekday0.txt", lang.weekdayShortName(wd));
        if(++wd > 7) wd = 1;
        nex.writeStr("Main.weekday2.txt", lang.weekdayShortName(wd));
        if(++wd > 7) wd = 1;
        nex.writeStr("Main.weekday3.txt", lang.weekdayShortName(wd));
        if(++wd > 7) wd = 1;
        nex.writeStr("Main.weekday4.txt", lang.weekdayShortName(wd));
        _prevTWeekday = _tWeekday;
    }
}

/**
 * Sending data to Network page
 */
void Nextion::_networkPage() {
    if(_prevNetLogo != _netLogo or _forced) {
        nex.writeStr("Network.Logo.txt", _netLogo);
        _prevNetLogo = _netLogo;
    }
    if(_prevNetSsid != _netSsid or _forced) {
        nex.writeStr("Network.ssid.txt", _netSsid);
        _prevNetSsid = _netSsid;
    }
    if(_prevNetRssi != _netRssi or _forced) {
        nex.writeStr("Network.rssi.txt", _netRssi);
        _prevNetRssi = _netRssi;
    }
    if(_prevNetIp != _netIp or _forced) {
        nex.writeStr("Network.ip.txt", _netIp);
        _prevNetIp = _netIp;
    }
    if(_prevNetMac != _netMac or _forced) {
        nex.writeStr("Network.mac.txt", _netMac);
        _prevNetMac = _netMac;
    }
    if(_prevNetTemp != _netTemp or _forced) {
        nex.writeStr("Network.temp.txt", validate.temp(_netTemp) ? String(int(round(_netTemp))) + "°C" : "--");
        _prevNetTemp = _netTemp;
    }
    if(_prevNetFw != _netFw or _forced) {
        nex.writeStr("Network.frmw.txt", _netFw);
        _prevNetFw = _netFw;
    }
}

/**
 * Display voltage, percentage, CO2 or IAQ
 */
void Nextion::_showVoltage() {
    if(_prevVolt != _volt or _forced) {
        nex.writeStr("Main.uBat.txt", _volt);
        _prevVolt = _volt;
    } 
    if(_prevVoltColor != _voltColor or _forced) {
        if(_voltColor > 3) _voltColor = 0;
        nex.writeNum("Main.uBat.pco", _air_color[_voltColor]);
        _prevVoltColor = _voltColor;
    }
}

/**
 * Display battery symbol
 */
void Nextion::_showBattery() {
    if(_prevBatLevel != _batLevel or _forced) {
        nex.writeNum("Main.bat.pic", validate.batLvl(_batLevel) ? (_batLevel + 35) : 35);
        _prevBatLevel = _batLevel;
    }
}

/**
 * Display antenna symbol
 */
void Nextion::_showAntenna() {
    if(_prevRssi != _rssi or _prevIsApMode != _isApMode or _forced) {
        uint8_t ant = 0;
        if(_isApMode) ant = 70;
        else {
            ant = 30;
            if(_rssi > -51) ant = 34;
            if(_rssi < -50 && _rssi > -76) ant = 33;
            if(_rssi < -75 && _rssi > -96) ant = 32;
            if(_rssi < -95) ant = 31;
            if(_rssi >= 0) ant = 30;
        }
        _prevRssi = _rssi;
        _prevIsApMode = _isApMode;
        nex.writeNum("Main.ant.pic", ant);
    }
}

/**
 * Display temperature inside
 */
void Nextion::_showTempIn() {
    if(_prevTempIn != _tempIn or _forced) {
        if(config.display_source_tempIn_sens() != 4) {
            nex.writeStr("Main.tempInside.txt", validate.temp(_tempIn) 
                ? (String(int(round(_tempIn))) + "°C") : "--"
            );
        }
        _prevTempIn = _tempIn;
    }
}

/**
 * Display temperature outside
 */
void Nextion::_showTempOut() {
    if(_prevTempOut != _tempOut or _forced) {
        _showThermometer();
        nex.writeStr("Main.tempOutside.txt", validate.temp(_tempOut) 
            ? (String(int(round(_tempOut))) + "°C") : "--"
        );
        _prevTempOut = _tempOut;
    }
}

/**
 * Display humidity inside
 */
void Nextion::_showHumIn() {
    if(_prevHumIn != _humIn or _forced) {
        if(config.display_source_humIn_sens() != 4) {
            nex.writeStr("Main.humInside.txt", validate.hum(_humIn) 
                ? (String(int(round(_humIn))) + "%") : "--"
            );
        }
        _prevHumIn = _humIn;
    }
}

/**
 * Display humidity outside
 */
void Nextion::_showHumOut() {
    if(_prevHumOut != _humOut or _forced) {
        nex.writeStr("Main.humOutside.txt", validate.hum(_humOut) 
            ? (String(int(round(_humOut))) + "%") : "--"
        );
        _prevHumOut = _humOut;
    }
}

/**
 * Display pressure
 */
void Nextion::_showPres() {
    if(_prevPresOut != _presOut or _forced) {
        nex.writeStr("Main.presOutside.txt", validate.pres(_presOut) 
            ? (String(int(round(_presOut * 0.75))) + lang.mm()) : "--"
        );
        _prevPresOut = _presOut;
    }
}

/**
 * Display sequence
 */
void Nextion::_showSequence() {
    if(_forced) nex.writeNum("Main.sequence.en", config.display_source_tempIn_sens() == 4 ? 1 : 0);
    for(unsigned int i=0; i<4; i++) {
        if(_prevTempSequence[i] != _tempSequence[i] or _forced) {
            nex.writeStr("Main.tempSeq" + String(i) + ".txt", validate.temp(_tempSequence[i]) 
                ? (String(int(round(_tempSequence[i]))) + "°C") : "--"
            );
            _prevTempSequence[i] = _tempSequence[i];
        }
        if(_prevHumSequence[i] != _humSequence[i] or _forced) {
            nex.writeStr("Main.humSeq" + String(i) + ".txt", validate.hum(_humSequence[i])
                ? (String(int(round(_humSequence[i]))) + "%") : "--"
            );
            _prevHumSequence[i] = _humSequence[i];
        }
        if(_prevNameSequence[i] != _nameSequence[i] or _forced) {
            nex.writeStr("Main.txtSeq" + String(i) + ".txt", _nameSequence[i]);
            _prevNameSequence[i] = _nameSequence[i];
        }
    }
}

/**
 * Display comfort level
 */
void Nextion::_showComfortLevel() {
    if(_prevComfortType != _comfortType or _forced) {
        nex.writeNum("Main.seq.val", _comfortType == 2 ? 1 : 0);
        _prevComfortType = _comfortType;
    }
    if(_prevComfort != _comfort or _forced) {
        if(config.display_source_tempIn_sens() != 4) {
            nex.writeStr("Main.comfort.txt", _comfort);
        }
        _prevComfort = _comfort;
    }
}

/**
 * Display weather description
 */
void Nextion::_showWeatherDescription() {
    if(_prevDescription != _description or _forced) {
        nex.writeStr("Main.description.txt", _description == "" ? "--" : _description);
        _prevDescription = _description;
    }
}

/**
 * Display current weather icon
 */
void Nextion::_showCurrentIcon() {
    if(_prevCurrIcon != _currIcon or _prevIsDay != _isDay or _forced) {
        uint8_t icon = 64;
        switch(_currIcon) {
            case 1: icon = _isDay ? 10 : 11; break;
            case 2: icon = _isDay ? 12 : 13; break;
            case 3: icon = 14; break;
            case 4: icon = 15; break;
            case 5: icon = 16; break;
            case 6: icon = _isDay ? 17 : 18; break;
            case 7: icon = 19; break;
            case 8: icon = 20; break;
            default: ; break;
        }
        nex.writeNum("Main.icon0.pic", icon);
        _prevCurrIcon = _currIcon;
        _prevIsDay = _isDay;
    }
}

/**
 * Display thermometer icon (red or blue)
 */
void Nextion::_showThermometer() {
    nex.writeNum("Main.thermometer.pic", _tempOut < 0.0 ? 40 : 41);
}

/**
 * Display wind speed
 */
void Nextion::_showWindSpeed() {
    if(_prevWindSpd != _windSpd or _forced) {
        nex.writeStr("Main.wind0.txt", validate.wind(_windSpd) 
            ? (String(int(round(_windSpd))) + lang.ms()) : "--"
        );
        _prevWindSpd = _windSpd;
    }
}

/**
 * Display wind direction
 */
void Nextion::_showWindDirection() {
    if(_prevWindDir != _windDir or _forced) {
        nex.writeNum("Main.windDir0.pic", _windDir > 7 ? 42 : _windDir + 42);
        _prevWindDir = _windDir;
    }
}

/**
 * Display the time and date of the last weather update
 */
void Nextion::_showUpdated() {
    if(_prevWeatherUpdated != _weatherUpdated or _forced) {
        time_t t = _weatherUpdated;
        char buf[32] = "";
        unsigned int hr = config.clock_format() > 1 ? hour(t) : hourFormat12(t);
        if(config.clock_format() % 2 == 0) sprintf(buf, "⭮ %02d.%02d.%d %d:%02d:%02d", day(t), month(t), year(t), hr, minute(t), second(t)); 
        else sprintf(buf, "⭮ %02d.%02d.%d %02d:%02d:%02d", day(t), month(t), year(t), hr, minute(t), second(t));
        nex.writeStr("Main.updatedTime.txt", t > 0 ? buf : "--");
        _prevWeatherUpdated = _weatherUpdated;
    }
}

/**
 * Display daily weather forecast
 */
void Nextion::_showWeatherForecast() {
    for(uint8_t i=0; i<4; i++) {
        if(_prevIcons[i] != _icons[i] or _forced) {
            nex.writeNum("Main.icon" + String(i + 1) + ".pic", _icons[i] == 0 ? 29 : (_icons[i] + 20));
            _prevIcons[i] = _icons[i];
        }
        if(_prevDTemps[i] != _dTemps[i] or _forced) {
            nex.writeStr("Main.tempMax" + String(i + 1) + ".txt", validate.temp(_dTemps[i]) 
                ? (String(int(round(_dTemps[i]))) + "°C") : "--"
            );
            _prevDTemps[i] = _dTemps[i];
        }
        if(_prevNTemps[i] != _nTemps[i] or _forced) {
            nex.writeStr("Main.tempMin" + String(i + 1) + ".txt", validate.temp(_nTemps[i]) 
                ? (String(int(round(_nTemps[i]))) + "°C") : "--"
            );
            _prevNTemps[i] = _nTemps[i];
        }
        if(_prevWinds[i] != _winds[i] or _forced) {
            nex.writeStr("Main.wind" + String(i + 1) + ".txt", validate.wind(_winds[i])
                ? (String(int(round(_winds[i]))) + lang.ms()) : "--"
            );
            _prevWinds[i] = _winds[i];
        }
    }
}

/**
 * Sending data to display hourly forecast
 */
void Nextion::_hourlyData() {
    if((_prevHourlyChecksum != _hourlyChecksum) or _forced) {
        char dat[22] = "";
        char buf[20] = "";
        Serial1.print("Hourly.data0.txt=\"");

        for(uint8_t i=0; i<40; i++) {
            // temp
            int t = round(weather.get_hourlyTemp(i) * 10);
            sprintf(buf, "%04d", t);
            for(uint8_t k=0; k<4; k++) dat[k] = buf[k];
            // pres
            unsigned int p = round(weather.get_hourlyPres(i) * 0.75);
            sprintf(buf, "%03d", p);
            for(uint8_t k=0; k<3; k++) dat[4 + k] = buf[k];
            // icon
            sprintf(buf, "%02d", weather.get_hourlyIcon(i));
            for(uint8_t k=0; k<2; k++) dat[7 + k] = buf[k];
            // weekday
            sprintf(buf, "%d", weekday(weather.get_hourlyDate(i)) - 1);
            dat[9] = buf[0];
            // day
            sprintf(buf, "%02d", day(weather.get_hourlyDate(i)));
            for(uint8_t k=0; k<2; k++) dat[10 + k] = buf[k];
            // month
            sprintf(buf, "%02d", month(weather.get_hourlyDate(i)) - 1);
            for(uint8_t k=0; k<2; k++) dat[12 + k] = buf[k];
            // hour
            unsigned int hr = config.clock_format() > 1 
                ? hour(weather.get_hourlyDate(i)) 
                : hourFormat12(weather.get_hourlyDate(i));
            sprintf(buf, "%02d", hr);
            for(uint8_t k=0; k<2; k++) dat[14 + k] = buf[k];
            // wind speed
            unsigned int wind = round(weather.get_hourlyWindSpeed(i));
            sprintf(buf, "%02d", wind);
            for(uint8_t k=0; k<2; k++) dat[16 + k] = buf[k];
            // wind direction
            unsigned int deg = agregateLcdData.windDirection(weather.get_hourlyWindDir(i));
            if(deg > 7) deg = 0;
            sprintf(buf, "%d", deg);
            dat[18] = buf[0];
            // precipitation
            int pr = 0;
            if(config.weather_provider() == OPEN_METEO) pr = weather.get_hourlyPrec(i);
            else pr = round(weather.get_hourlyPrec(i) * 100);
            sprintf(buf, "%03d", pr);
            for(uint8_t k=0; k<3; k++) dat[19 + k] = buf[k];

            // send all to display
            for(uint8_t k=0; k<22; k++) Serial1.print(dat[k]);
        }
        Serial1.print("\"");
        Serial1.write(0xFF);
        Serial1.write(0xFF);
        Serial1.write(0xFF);

        _daily2hourly();
        _prevHourlyChecksum = _hourlyChecksum;
    }
}

/**
 * Initialize daily forecast links to hourly forecast
 */
void Nextion::_daily2hourly() {
    unsigned int fd = 2;
    for(uint8_t i=0; i<40; i++) {
        if(hour(weather.get_hourlyDate(i)) == 0) {
            if(i != 0) nex.writeNum("Hourly.day" + String(fd++) + ".val", i);
            if(fd > 4) break;
        }
    }
}

/**
 * Sending data to display outdoor weather history
 */
void Nextion::_historyOut() {
    if(_prevHistoryOutChecksum != _historyOutChecksum or _forced) {
        char dat[18] = "";
        char buf[20] = "";
        Serial1.print("HistoryOut.data0.txt=\"");
        for(uint8_t i=0; i<24; i++) {
            // temperature
            int t = round(thingspeak.get_historyField(0, i) * 10);
            sprintf(buf, "%04d", t);
            for(uint8_t k=0; k<4; k++) dat[k] = buf[k];
            // humidity
            int h = thingspeak.get_historyField(1, i);
            sprintf(buf, "%03d", h);
            for(uint8_t k=0; k<3; k++) dat[4 + k] = buf[k];
            // pressure
            int p = round(thingspeak.get_historyField(2, i) * 0.75);
            sprintf(buf, "%03d", p);
            for(uint8_t k=0; k<3; k++) dat[7 + k] = buf[k];
            // day
            sprintf(buf, "%02d", day(thingspeak.get_historyUpdated(i)));
            for(uint8_t k=0; k<2; k++) dat[10 + k] = buf[k];
            // month
            sprintf(buf, "%02d", month(thingspeak.get_historyUpdated(i)) - 1);
            for(uint8_t k=0; k<2; k++) dat[12 + k] = buf[k];
            // hour
            unsigned int hr = config.clock_format() > 1 
                ? hour(thingspeak.get_historyUpdated(i)) 
                : hourFormat12(thingspeak.get_historyUpdated(i));
            sprintf(buf, "%02d", hr);
            for(uint8_t k=0; k<2; k++) dat[14 + k] = buf[k];
            // minute
            sprintf(buf, "%02d", minute(thingspeak.get_historyUpdated(i)));
            for(uint8_t k=0; k<2; k++) dat[16 + k] = buf[k];
            // send current slot
            for(uint8_t k=0; k<18; k++) Serial1.print(dat[k]);
        }
        Serial1.print("\"");
        Serial1.write(0xFF);
        Serial1.write(0xFF);
        Serial1.write(0xFF);

        _prevHistoryOutChecksum = _historyOutChecksum;
    }
}

/**
 * Sending data to display indoor weather history
 */
void Nextion::_historyIn() {
    if(_prevHistoryInChecksum != _historyInChecksum or _forced) {
        char dat[15] = "";
        char buf[20] = "";
        Serial1.print("HistoryIn.data0.txt=\"");
        for(uint8_t i=0; i<24; i++) {
            // temperature
            int t = round(thingspeak.get_historyField(3, i) * 10);
            sprintf(buf, "%04d", t);
            for(uint8_t k=0; k<4; k++) dat[k] = buf[k];
            // humidity
            int h = thingspeak.get_historyField(4, i);
            sprintf(buf, "%03d", h);
            for(uint8_t k=0; k<3; k++) dat[4 + k] = buf[k];
            // day
            sprintf(buf, "%02d", day(thingspeak.get_historyUpdated(i)));
            for(uint8_t k=0; k<2; k++) dat[7 + k] = buf[k];
            // month
            sprintf(buf, "%02d", month(thingspeak.get_historyUpdated(i)) - 1);
            for(uint8_t k=0; k<2; k++) dat[9 + k] = buf[k];
            // hour
            unsigned int hr = config.clock_format() > 1 
                ? hour(thingspeak.get_historyUpdated(i)) 
                : hourFormat12(thingspeak.get_historyUpdated(i));
            sprintf(buf, "%02d", hr);
            for(uint8_t k=0; k<2; k++) dat[11 + k] = buf[k];
            // minute
            sprintf(buf, "%02d", minute(thingspeak.get_historyUpdated(i)));
            for(uint8_t k=0; k<2; k++) dat[13 + k] = buf[k];
            // send current slot
            for(uint8_t k=0; k<15; k++) Serial1.print(dat[k]);
        }
        Serial1.print("\"");
        Serial1.write(0xFF);
        Serial1.write(0xFF);
        Serial1.write(0xFF);

        _prevHistoryInChecksum = _historyInChecksum;
    }
}

/**
 * Sending alarms data
 */
void Nextion::_alarms() {
    if(_prevAlarmChecksum != _alarmChecksum or _forced) {
        unsigned int alarmOn = 0;
        char alarmData[144];
        char buf[3];
        Serial1.print("Alarm.alarms.txt=\"");
        for(uint8_t i=0; i<12; i++) {
            sprintf(buf, "%02d", config.alarm_time(i, 0));
            for(uint8_t j=0; j<2; j++) alarmData[i * 4 + j] = buf[j];
            sprintf(buf, "%02d", config.alarm_time(i, 1));
            for(uint8_t j=0; j<2; j++) alarmData[i * 4 + j + 2] = buf[j];
            for(uint8_t w=0; w<7; w++) {
                sprintf(buf, "%d", config.alarm_weekday(i, w));
                alarmData[i * 7 + w + 48] = buf[0];
            }
            sprintf(buf, "%d", config.alarm_state(i));
            alarmData[i + 132] = buf[0];
            alarmOn |= config.alarm_state(i);
        }
        for(uint8_t k=0; k<144; k++) Serial1.print(alarmData[k]);
        Serial1.print("\"");
        Serial1.write(0xFF);
        Serial1.write(0xFF);
        Serial1.write(0xFF);

        nex.writeNum("Alarm.alarmUpd.en", 1);
        nex.writeNum("Main.alarm.pic", alarmOn ? 71 : 72);

        _prevAlarmChecksum = _alarmChecksum;
    }
}

/**
 * Receive date and time from display
 */
void Nextion::dataReceive() {
    while(Serial1.available()) {
        char nextionChar = Serial1.read();
        if(nextionChar == '{' or nextionChar == '\x87') {
            global.disp_autoOff[0] = millis();
            if(!isDisplayOn()) displayOn();
        }
        if(nextionChar == '{') _customData = 1;
        if(nextionChar == '}') {
            _customData = 0;
            _receivedData.concat(nextionChar);
        }
        if(_customData == 1) _receivedData.concat(nextionChar);
        if(_customData == 0) {
            JsonDocument root;
            DeserializationError error = deserializeJson(root, _receivedData);
            if(!error) {
                /* Time from display */
                if(_receivedData.lastIndexOf("{\"h\":") != -1) {
                    setTime(
                        root["h"].as<int>(), 
                        root["i"].as<int>(), 
                        root["s"].as<int>(), 
                        root["d"].as<int>(), 
                        root["m"].as<int>(), 
                        root["y"].as<int>()
                    );
                }

                /* Alarm settings from display */
                if(_receivedData.lastIndexOf("{\"alarms\":") != -1) {
                    uint8_t times[ALARMS][2], weekdays[ALARMS][7], states[ALARMS]; 
                    for(uint8_t i=0; i<ALARMS; i++) {
                        for(uint8_t n=0; n<10; n++) {
                            uint8_t val = root["alarms"][i][n].as<int>();
                            if(n == 0) config.set_alarm_state(i, val);
                            if(n == 1) config.set_alarm_time(i, 0, val);
                            if(n == 2) config.set_alarm_time(i, 1, val);
                            if(n >= 3) config.set_alarm_weekday(i, n - 3, val);
                        }
                    }
                    _forced = true;
                    config.save_alarm_file();
                }
            }
            _receivedData = "";
            _customData = -1;
        }
    }
}