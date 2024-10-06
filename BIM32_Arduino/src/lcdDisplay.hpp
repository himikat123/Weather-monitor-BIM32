#include <CRC32.h>

class LcdDisplay {
    protected:
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
        int _prevCalendarShiftSeconds = -1;
};

void LcdDisplay::_getData() {
    _tHour = config.clock_format() ? hour() : hourFormat12();
    _tMinute = minute();
    _tWeekday = weekday();
    _tDay = day();
    _tMonth = month();
    _tYear = year();
    _rssi = WiFi.RSSI();
    _netLogo = global.apMode ? "Access Point" : "WiFi";
    _netSsid = global.apMode ? config.accessPoint_ssid() : WiFi.SSID();
    _netRssi = global.apMode ? "100%" : String(_rssi) + "dBm";
    _netIp = global.apMode ? config.accessPoint_ip() : WiFi.localIP().toString();
    _netMac = global.apMode ? WiFi.softAPmacAddress() : WiFi.macAddress();
    _netTemp = sensors.get_esp32_temp(RAW);
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
    _isApMode = global.apMode;
    _comfort = agregateLcdData.comfort();
    _comfortType = config.display_source_descr();
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

    CRC32 crc;
    for(uint8_t i=0; i<40; i++) {
        crc.update(weather.get_hourlyTemp(i));
        crc.update(weather.get_hourlyPres(i));
        crc.update(weather.get_hourlyIcon(i));
        crc.update(weather.get_hourlyDate(i));
        crc.update(weather.get_hourlyWindSpeed(i));
        crc.update(weather.get_hourlyWindDir(i));
        crc.update(weather.get_hourlyPrec(i));
    }
    _hourlyChecksum = crc.finalize();

    crc.reset();
    for(uint8_t i=0; i<24; i++) {
        crc.update(thingspeak.get_historyField(0, i));
        crc.update(thingspeak.get_historyField(1, i));
        crc.update(thingspeak.get_historyField(2, i));
        crc.update(thingspeak.get_historyUpdated(i));
    }
    _historyOutChecksum = crc.finalize();

    crc.reset();
    for(uint8_t i=0; i<24; i++) {
        crc.update(thingspeak.get_historyField(3, i));
        crc.update(thingspeak.get_historyField(4, i));
        crc.update(thingspeak.get_historyUpdated(i));
    }
    _historyInChecksum = crc.finalize();

    crc.reset();
    for(uint8_t i=0; i<12; i++) {
        crc.update(config.alarm_time(i, 0));
        crc.update(config.alarm_time(i, 1));
        crc.update(config.alarm_state(i));
        for(uint8_t w=0; w<7; w++) {
            crc.update(config.alarm_weekday(i, w));
        }
    }
    _alarmChecksum = crc.finalize();
}
