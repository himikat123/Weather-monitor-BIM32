#pragma once

#include <Arduino.h>
#include <WiFi.h> 
#include <CRC32.h>
#include <TimeLib.h>
#include "config.hpp"
#include "sensors.hpp"
#include "agregateLcdData.hpp"
#include "weather.hpp"
#include "thingspeak.hpp"

class LcdDisplay {
    protected:
        CRC32 _crc;
        AgregateLcdData agregateLcdData;
        virtual void _getData();

        bool _power = true;
        bool _forced = false;
        uint8_t _tHour = 24;
        uint8_t _tMinute = 60;
        uint8_t _tWeekday = 7;
        uint8_t _tDay = 32;
        uint8_t _tMonth = 13;
        uint16_t _tYear = 3024;
        String _netLogo = "";
        String _netSsid = "";
        String _netRssi = "";
        String _netIp = "";
        String _netMac = "";
        float _netTemp = 404.0;
        String _netFw = "";
        float _tempSequence[4] = {404.0, 404.0, 404.0, 404.0};
        float _humSequence[4] = {404.0, 404.0, 404.0, 404.0};
        String _nameSequence[4] = {"", "", "", ""};
        uint8_t _icons[4] = {40, 40, 40, 40};
        float _dTemps[4] = {404.0, 404.0, 404.0, 404.0};
        float _nTemps[4] = {404.0, 404.0, 404.0, 404.0};
        float _winds[4] = {404.0, 404.0, 404.0, 404.0};
        float _tempIn = 404.0;
        float _tempOut = 404.0;
        float _humIn = 404.0;
        float _humOut = 404.0;
        float _presOut = 404.0;
        String _volt = "";
        uint8_t _voltColor = 4;
        int _batLevel = 404;
        int _rssi = 404;
        bool _isApMode = false;
        String _comfort = "";
        uint8_t _comfortType = 4;
        String _description = "";
        uint8_t _currIcon = 40;
        bool _isDay = true;
        float _windSpd = -1;
        uint8_t _windDir = 40;
        time_t _weatherUpdated = 0;
        uint32_t _hourlyChecksum = 0;
        uint32_t _historyOutChecksum = 0;
        uint32_t _historyInChecksum = 0;
        uint32_t _alarmChecksum = 0;
        int _hourlyShift = 0;
        int _historyOutShift = 16;
        int _historyInShift = 16;

        bool _prevPower = false;
        time_t _prevForced = 0;
        uint8_t _prevBright = 0;
        uint8_t _prevTHour = 0;
        uint8_t _prevTMinute = 0;
        uint8_t _prevTSecond = 0;
        uint8_t _prevTWeekday = 0;
        uint8_t _prevTDay = 0;
        uint8_t _prevTMonth = 0;
        uint16_t _prevTYear = 0;
        String _prevNetLogo = "--";
        String _prevNetSsid = "--";
        String _prevNetRssi = "--";
        String _prevNetIp = "--";
        String _prevNetMac = "--";
        float _prevNetTemp = 504.0;
        String _prevNetFw = "--";
        float _prevTempSequence[4] = {504.0, 504.0, 504.0, 504.0};
        float _prevHumSequence[4] = {504.0, 504.0, 504.0, 504.0};
        String _prevNameSequence[4] = {"--", "--", "--", "--"};
        uint8_t _prevIcons[4] = {50, 50, 50, 50};
        float _prevDTemps[4] = {504.0, 504.0, 504.0, 504.0};
        float _prevNTemps[4] = {504.0, 504.0, 504.0, 504.0};
        float _prevWinds[4] = {504.0, 504.0, 504.0, 504.0};
        float _prevTempIn = 504.0;
        float _prevTempOut = 504.0;
        float _prevHumIn = 504.0;
        float _prevHumOut = 504.0;
        float _prevPresOut = 504.0;
        String _prevVolt = "--";
        uint8_t _prevVoltColor = 50;
        int _prevBatLevel = 504;
        int _prevRssi = 504;
        bool _prevIsApMode = true;
        String _prevComfort = "--";
        uint8_t _prevComfortType = 50;
        String _prevDescription = "--";
        uint8_t _prevCurrIcon = 50;
        bool _prevIsDay = false;
        float _prevWindSpd = -2;
        uint8_t _prevWindDir = 50;
        time_t _prevWeatherUpdated = 1;
        uint32_t _prevHourlyChecksum = 1;
        uint32_t _prevHistoryOutChecksum = 1;
        uint32_t _prevHistoryInChecksum = 1;
        uint32_t _prevAlarmChecksum = 1;
        uint8_t _prevAlarmOn = 2;
        int _prevHourlyShift = -1;
        int _prevHistoryOutShift = -1;
        int _prevHistoryInShift = -1;
        int _prevCalendarShiftSeconds = -1;
};