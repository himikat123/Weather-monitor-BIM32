#include "lcdDisplay.hpp"
#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "../config/config.hpp"
#include "../agregateData/lcdData/agregateLcdData.hpp"
#include "../weather/weather.hpp"
#include "../cloud/thingspeak/thingspeak.hpp"
#include "../state/state.hpp"

void LcdDisplay::_getData() {
    _tHour = config.clock.format() > 1 ? hour() : hourFormat12();
    _tMinute = minute();
    _tWeekday = weekday();
    _tDay = day();
    _tMonth = month();
    _tYear = year();
    _rssi = WiFi.RSSI();
    _netLogo = state.apMode ? "Access Point" : "WiFi";
    _netSsid = state.apMode ? config.accessPoint.ssid() : WiFi.SSID();
    _netRssi = state.apMode ? "100%" : String(_rssi) + "dBm";
    _netIp = state.apMode ? config.accessPoint.ip() : WiFi.localIP().toString();
    _netMac = state.apMode ? WiFi.softAPmacAddress() : WiFi.macAddress();
    _netTemp = sensors.get_esp32_temp();
    _netFw = FW;
    _tempIn = agregateLcdData.tempIn(_tempSequence);
    _tempOut = agregateLcdData.tempOut();
    _humIn = agregateLcdData.humIn(_humSequence);
    _humOut = agregateLcdData.humOut();
    _presOut = agregateLcdData.presOut();
    agregateLcdData.sequenceNames(_nameSequence);
    _volt = agregateLcdData.voltage();
    _voltColor = agregateLcdData.voltageColor();
    _batLevel = agregateLcdData.batteryLevel();
    _isApMode = state.apMode;
    _comfort = agregateLcdData.comfort();
    _comfortType = config.display.source.descr();
    _description = weather.get_description();
    _currIcon = weather.get_currentIcon();
    _isDay = weather.get_isDay();
    _windSpd = weather.get_currentWindSpeed();
    _windDir = agregateLcdData.windDirection(weather.get_currentWindDir());
    _weatherUpdated = weather.get_currentUpdated();
    for(uint8_t i=0; i<4; i++) {
        _icons[i] = weather.get_dailyIcon(i);
        _dTemps[i] = weather.get_dailyDayTemp(i);
        _nTemps[i] = weather.get_dailyNightTemp(i);
        _winds[i] = weather.get_dailyWindSpeed(i);
    }

    for(uint8_t i=0; i<40; i++) {
        _crc.update(weather.get_hourlyTemp(i));
        _crc.update(weather.get_hourlyPres(i));
        _crc.update(weather.get_hourlyIcon(i));
        _crc.update(weather.get_hourlyDate(i));
        _crc.update(weather.get_hourlyWindSpeed(i));
        _crc.update(weather.get_hourlyWindDir(i));
        _crc.update(weather.get_hourlyPrec(i));
    }
    _hourlyChecksum = _crc.finalize();
    _crc.reset();

    for(uint8_t i=0; i<24; i++) {
        _crc.update(thingspeak.get_historyField(0, i));
        _crc.update(thingspeak.get_historyField(1, i));
        _crc.update(thingspeak.get_historyField(2, i));
        _crc.update(thingspeak.get_historyUpdated(i));
    }
    _historyOutChecksum = _crc.finalize();
    _crc.reset();

    for(uint8_t i=0; i<24; i++) {
        _crc.update(thingspeak.get_historyField(3, i));
        _crc.update(thingspeak.get_historyField(4, i));
        _crc.update(thingspeak.get_historyUpdated(i));
    }
    _historyInChecksum = _crc.finalize();
    _crc.reset();

    for(uint8_t i=0; i<12; i++) {
        _crc.update(config.alarm.time(i, 0));
        _crc.update(config.alarm.time(i, 1));
        _crc.update(config.alarm.state(i));
        for(uint8_t w=0; w<7; w++) {
            _crc.update(config.alarm.weekday(i, w));
        }
    }
    _alarmChecksum = _crc.finalize();
    _crc.reset();
}