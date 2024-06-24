#include <EasyNextionLibrary.h> // v1.0.6 https://github.com/Seithan/EasyNextionLibrary
EasyNex nex(Serial1);

class Nextion {
    #define NX4832K035 0
    #define NX4832T035 1

    public:
        void init();
        void refresh();
        void displayToggle();
        void displayOn(bool doinit = true);
        void displayOff();
        bool isDisplayOn();
        void brightness(unsigned int bright, bool reduc);
        void setDisplayRTC();
        void dataReceive();

    private:
        bool _power = true; // display on/off flag
        int _customData = -1; // flag of key symbols of receive from the display
        String _receivedData = "";
        uint16_t _air_color[4] = { 2016, 65520, 64512, 63488 };

        void _NX4832K035_setRTC();
        void _NX4832T035_timeDate();
        void _networkPage();
        void _voltage(String voltage, uint8_t voltageColor);
        void _battery(int level);
        void _antenna(int rssi, bool isApMode);
        void _temp(float temp, String field);
        void _hum(float hum, String field);
        void _sequence(float* tempSequence, float* humSequence, String* nameSequence);
        void _comfortLevel();
        void _weatherDescription();
        void _currentIcon();
        void _thermometer(float temp);
        void _pres();
        void _windSpeed();
        void _windDirection();
        void _updated();
        void _weatherForecast();
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

    for(unsigned int i=0; i<4; i++) {
        nex.writeStr("Main.nameSeq" + String(i) + ".txt", config.display_source_sequence_name(i));
    }
    nex.writeNum("Main.sequence.tim", config.display_source_sequence_dur() * 1000);

    /* Alarm */
    nex.writeStr("Texts.ALARM.txt", lang.alarm());
    
    /* Initialize NX4832K035 display  */
    if(config.display_model(0) == NX4832K035) {
        // config
        nex.writeNum("BigClock.clockFormat.val", config.clock_format());
        unsigned int langCode = 0;
        if(config.lang() == "de") langCode = 1;
        if(config.lang() == "ru") langCode = 2;
        if(config.lang() == "pl") langCode = 3;
        if(config.lang() == "ua") langCode = 4;
        nex.writeNum("Main.lang.val", langCode);

        // texts
        nex.writeStr("Texts.JANUARY.txt", lang.january());
        nex.writeStr("Texts.FEBRUARY.txt", lang.february());
        nex.writeStr("Texts.MARCH.txt", lang.march());
        nex.writeStr("Texts.APRIL.txt", lang.april());
        nex.writeStr("Texts.MAY.txt", lang.may());
        nex.writeStr("Texts.JUNE.txt", lang.june());
        nex.writeStr("Texts.JULY.txt", lang.july());
        nex.writeStr("Texts.AUGUST.txt", lang.august());
        nex.writeStr("Texts.SEPTEMBER.txt", lang.september());
        nex.writeStr("Texts.OCTOBER.txt", lang.october());
        nex.writeStr("Texts.NOVEMBER.txt", lang.november());
        nex.writeStr("Texts.DECEMBER.txt", lang.december());
        nex.writeStr("Texts.JAN_.txt", lang.january_());
        nex.writeStr("Texts.FEB_.txt", lang.february_());
        nex.writeStr("Texts.MAR_.txt", lang.march_());
        nex.writeStr("Texts.APR_.txt", lang.april_());
        nex.writeStr("Texts.MAY_.txt", lang.may_());
        nex.writeStr("Texts.JUN_.txt", lang.june_());
        nex.writeStr("Texts.JUL_.txt", lang.july_());
        nex.writeStr("Texts.AUG_.txt", lang.august_());
        nex.writeStr("Texts.SEP_.txt", lang.september_());
        nex.writeStr("Texts.OCT_.txt", lang.october_());
        nex.writeStr("Texts.NOV_.txt", lang.november_());
        nex.writeStr("Texts.DEC_.txt", lang.december_());
        nex.writeStr("Texts.JAN.txt", lang.jan());
        nex.writeStr("Texts.FEB.txt", lang.feb());
        nex.writeStr("Texts.MAR.txt", lang.mar());
        nex.writeStr("Texts.APR.txt", lang.apr());
        nex.writeStr("Texts.MAI.txt", lang.mai());
        nex.writeStr("Texts.JUN.txt", lang.jun());
        nex.writeStr("Texts.JUL.txt", lang.jul());
        nex.writeStr("Texts.AUG.txt", lang.aug());
        nex.writeStr("Texts.SEP.txt", lang.sep());
        nex.writeStr("Texts.OCT.txt", lang.oct());
        nex.writeStr("Texts.NOV.txt", lang.nov());
        nex.writeStr("Texts.DEC.txt", lang.dec());

        nex.writeStr("Texts.MONDAY.txt", lang.monday());
        nex.writeStr("Texts.TUESDAY.txt", lang.tuesday());
        nex.writeStr("Texts.WEDNESDAY.txt", lang.wednesday());
        nex.writeStr("Texts.THURSDAY.txt", lang.thursday());
        nex.writeStr("Texts.FRIDAY.txt", lang.friday());
        nex.writeStr("Texts.SATURDAY.txt", lang.saturday());
        nex.writeStr("Texts.SUNDAY.txt", lang.sunday());
        nex.writeStr("Texts.MO.txt", lang.mo());
        nex.writeStr("Texts.TU.txt", lang.tu());
        nex.writeStr("Texts.WE.txt", lang.we());
        nex.writeStr("Texts.TH.txt", lang.th());
        nex.writeStr("Texts.FR.txt", lang.fr());
        nex.writeStr("Texts.SA.txt", lang.sa());
        nex.writeStr("Texts.SU.txt", lang.su());
    }

    nex.writeNum("thup", 1);
    nex.writeStr("page Main");
}

void Nextion::refresh() {
    nex.writeNum("sleep", _power ? 0 : 1);

    if(_power) {
        if(config.display_model(0) == NX4832T035) _NX4832T035_timeDate();
        if(config.display_model(0) == NX4832K035) {
            if(global.clockSynchronize) {
                _NX4832K035_setRTC();
                global.clockSynchronize = false;
            }
        }

        float tempSequence[4] = {404.0, 404.0, 404.0, 404.0};
        float humSequence[4] = {404.0, 404.0, 404.0, 404.0};
        String nameSequence[4] = {"", "", "", ""};
        float tempIn = agregateData.lcdTempIn(tempSequence);
        float tempOut = agregateData.lcdTempOut();
        float humIn = agregateData.lcdHumIn(humSequence);
        float humOut = agregateData.lcdHumOut();
        agregateData.lcdSequenceNames(nameSequence);
        String voltage = agregateData.lcdVoltage();
        uint8_t voltageColor = agregateData.lcdVoltageColor();
        int batLevel = agregateData.lcdBatteryLevel();
        int rssi = WiFi.RSSI();
        bool isApMode = global.apMode;

        _networkPage();
        _comfortLevel();
        _voltage(voltage, voltageColor);
        _battery(batLevel);
        _antenna(rssi, isApMode);

        _temp(tempIn, "Main.tempInside.txt");
        _hum(humIn, "Main.humInside.txt");
        _sequence(tempSequence, humSequence, nameSequence);
  
        _weatherDescription();
        _currentIcon();
        //_thermometer(temp);
        _temp(tempOut, "Main.tempOutside.txt");
        _hum(humOut, "Main.humOutside.txt");
        _pres();
        _windSpeed();
        _windDirection();
        _updated();
        _weatherForecast();
        _hourlyData();
        _daily2hourly();
        _historyOut();
        _historyIn();
        _alarms();
    }
}

/**
 * Change display brightness
 */
void Nextion::brightness(unsigned int bright, bool reduc) {
    if(_power) nex.writeNum("dim", reduc ? round(bright / 2) : bright);
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
    if(now() > 1665606321) {
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
    String buf = "";
    nex.writeStr("BigClock.hour.txt", String(config.clock_format() ? hour() : hourFormat12()));
    nex.writeStr("BigClock.minute.txt", String(minute()));
    unsigned int wd = weekday();
    nex.writeStr("BigClock.weekday.txt", lang.weekdayShortName(wd));
    if(config.lang() == "en") buf = lang.monthName(month()) + " " + String(day()) + ", " + String(year());
    else buf = String(day()) + " " + lang.monthName(month()) + String(year());
    nex.writeStr("BigClock.date.txt", buf);
    nex.writeStr("Main.weekday0.txt", lang.weekdayShortName(wd));
    if(++wd > 7) wd = 1;
    nex.writeStr("Main.weekday2.txt", lang.weekdayShortName(wd));
    if(++wd > 7) wd = 1;
    nex.writeStr("Main.weekday3.txt", lang.weekdayShortName(wd));
    if(++wd > 7) wd = 1;
    nex.writeStr("Main.weekday4.txt", lang.weekdayShortName(wd));
    buf = String();
}

/**
 * Sending data to Network page
 */
void Nextion::_networkPage() {
    if(global.apMode) {
        nex.writeStr("Network.Logo.txt", "Access Point");
        nex.writeStr("Network.ssid.txt", config.accessPoint_ssid());
        nex.writeStr("Network.rssi.txt", "100%");
        nex.writeStr("Network.ip.txt", config.accessPoint_ip());
        nex.writeStr("Network.mac.txt", WiFi.softAPmacAddress());
    }
    else {
        nex.writeStr("Network.Logo.txt", "WiFi");
        nex.writeStr("Network.ssid.txt", WiFi.SSID());
        nex.writeStr("Network.rssi.txt", String(WiFi.RSSI()) + "dBm");
        nex.writeStr("Network.ip.txt", WiFi.localIP().toString());
        nex.writeStr("Network.mac.txt", WiFi.macAddress());
    }
    nex.writeStr("Network.temp.txt", String(int(round(sensors.get_esp32_temp(0)))) + "°C");
    nex.writeStr("Network.frmw.txt", FW);
}

/**
 * Display voltage, percentage, CO2 or IAQ
 */
void Nextion::_voltage(String volt, uint8_t voltageColor) {
    if(voltageColor > 3) voltageColor = 0;
    nex.writeStr("Main.uBat.txt", volt);
    nex.writeNum("Main.uBat.pco", _air_color[voltageColor]);
}

/**
 * Display battery symbol
 */
void Nextion::_battery(int level) {
    if(validate.batLvl(level)) nex.writeNum("Main.bat.pic", level + 35);
    else nex.writeNum("Main.bat.pic", 35);
}

/**
 * Display antenna symbol
 */
void Nextion::_antenna(int rssi, bool isApMode) {
    if(isApMode) nex.writeNum("Main.ant.pic", 70);
    else {
        uint8_t ant = 30;
        if(rssi > -51) ant = 34;
        if(rssi < -50 && rssi > -76) ant = 33;
        if(rssi <- 75 && rssi > -96) ant = 32;
        if(rssi < -95) ant = 31;
        if(rssi >= 0) ant = 30;
        nex.writeNum("Main.ant.pic", ant);
    }
}

/**
 * Display temperature
 */
void Nextion::_temp(float temp, String field) {
    if(validate.temp(temp)) nex.writeStr(field, String(int(round(temp))) + "°C");
    else nex.writeStr(field, "--");
}

/**
 * Display humidity
 */
void Nextion::_hum(float hum, String field) {
    if(validate.hum(hum)) nex.writeStr(field, String(int(round(hum))) + "%");
    else nex.writeStr(field, "--");
}

/**
 * Display sequence
 */
void Nextion::_sequence(float* tempSequence, float* humSequence, String* nameSequence) {
    for(unsigned int i=0; i<4; i++) {
        if(validate.temp(tempSequence[i])) 
            nex.writeStr("Main.tempSeq" + String(i) + ".txt", String(int(round(tempSequence[i]))) + "°C");
        else nex.writeStr("Main.tempSeq" + String(i) + ".txt", "--");

        if(validate.hum(humSequence[i]))
            nex.writeStr("Main.humSeq" + String(i) + ".txt", String(int(round(humSequence[i]))) + "%");
        else nex.writeStr("Main.humSeq" + String(i) + ".txt", "--");

        nex.writeStr("Main.txtSeq" + String(i) + ".txt", nameSequence[i]);
    }
}

/**
 * Display comfort level
 */
void Nextion::_comfortLevel() {
    /* -- */
    if(config.display_source_descr() == 0) {
        nex.writeNum("Main.seq.val", 0);
        nex.writeStr("Main.comfort.txt", "");
    }

    /* comfort level */
    if(config.display_source_descr() == 1) {
        nex.writeNum("Main.seq.val", 0);
        String comfort = lang.comfort(global.comfort);

        if(global.iaq_level) {
            if(comfort.length()) comfort += ". ";
            comfort += lang.airQuality(global.iaq_level);
        }

        else if(global.co2_level) {
            if(comfort.length()) comfort += ". ";
            comfort += lang.airQuality(global.co2_level);
        }
        nex.writeStr("Main.comfort.txt", comfort);
    }

    /* sequence */
    if(config.display_source_descr() == 2) {
        nex.writeNum("Main.seq.val", 1);
    }
}

/**
 * Display weather description
 */
void Nextion::_weatherDescription() {
    if(weather.get_description() == "") nex.writeStr("Main.description.txt", "--");
    nex.writeStr("Main.description.txt", weather.get_description());
}

/**
 * Display current weather icon
 */
void Nextion::_currentIcon() {
    if(weather.get_currentIcon() == 0) nex.writeNum("Main.icon0.pic", 64);
    else nex.writeNum("Main.icon0.pic", weather.get_currentIcon() + 9);
    switch(weather.get_currentIcon()) {
        case 1: {
            if(weather.get_isDay()) nex.writeNum("Main.icon0.pic", 10);
            else nex.writeNum("Main.icon0.pic", 11);
        }; break;
        case 2: {
            if(weather.get_isDay()) nex.writeNum("Main.icon0.pic", 12);
            else nex.writeNum("Main.icon0.pic", 13);
        }; break;
        case 3: nex.writeNum("Main.icon0.pic", 14); break;
        case 4: nex.writeNum("Main.icon0.pic", 15); break;
        case 5: nex.writeNum("Main.icon0.pic", 16); break;
        case 6: {
            if(weather.get_isDay()) nex.writeNum("Main.icon0.pic", 17);
            else nex.writeNum("Main.icon0.pic", 18);
        }; break;
        case 7: nex.writeNum("Main.icon0.pic", 19); break;
        case 8: nex.writeNum("Main.icon0.pic", 20); break;
        default: nex.writeNum("Main.icon0.pic", 64); break;
    }
}

/**
 * Display thermometer icon (red or blue)
 * @param temp 
 */
void Nextion::_thermometer(float temp) {
    nex.writeNum("Main.thermometer.pic", temp < 0.0 ? 40 : 41);
}

/**
 * Display pressure
 */
void Nextion::_pres() {
    float pres = 40400.0;
    if(config.display_source_presOut_sens() == 1) // pressure outside from weather forecast
        pres = weather.get_currentPres();
    if(config.display_source_presOut_sens() == 2) // presure outside from wireless sensor
        if(wsensor.dataRelevance(config.display_source_presOut_wsensNum()))
            pres = wsensor.get_pressure(config.display_source_presOut_wsensNum(), CORRECTED);
    if(config.display_source_presOut_sens() == 3) // presure outside from thingspeak
        if(thingspeak.dataRelevance())
            pres = thingspeak.get_field(config.display_source_presOut_thing());
    if(config.display_source_presOut_sens() == 4) // pressure outside from BME280
        pres = sensors.get_bme280_pres(CORRECTED);
    if(config.display_source_presOut_sens() == 5) // pressure outside from BMP180
        pres = sensors.get_bmp180_pres(CORRECTED);
    if(config.display_source_presOut_sens() == 6) // pressure outside from BME680
        pres = sensors.get_bme680_pres(CORRECTED);
    if(validate.pres(pres)) 
        nex.writeStr("Main.presOutside.txt", String(int(round(pres * 0.75))) + lang.mm());
    else nex.writeStr("Main.presOutside.txt", "--");
}

/**
 * Display wind speed
 */
void Nextion::_windSpeed() {
    if(validate.wind(weather.get_currentWindSpeed()))
        nex.writeStr("Main.wind0.txt", String(int(round(weather.get_currentWindSpeed()))) + lang.ms());
    else nex.writeStr("Main.wind0.txt", "--");
}

/**
 * Display wind direction
 */
void Nextion::_windDirection() {
    unsigned int deg = round(float(weather.get_currentWindDir()) / 45.0);
    if(deg > 7) deg = 0;
    nex.writeNum("Main.windDir0.pic", deg + 42);
}

/**
 * Display the time and date of the last weather update
 */
void Nextion::_updated() {
    char buf[32] = "";
    sprintf(buf, "⭮ %02d.%02d.%d %d:%02d:%02d",
        day(weather.get_currentUpdated()),
        month(weather.get_currentUpdated()),
        year(weather.get_currentUpdated()),
        hour(weather.get_currentUpdated()),
        minute(weather.get_currentUpdated()),
        second(weather.get_currentUpdated())
    );
    if(weather.get_currentUpdated() == 0) nex.writeStr("Main.updatedTime.txt", "--");
    else nex.writeStr("Main.updatedTime.txt", buf);
}

/**
 * Display daily weather forecast
 */
void Nextion::_weatherForecast() {
    for(unsigned int i=0; i<4; i++) {
        // icons
        if(weather.get_dailyIcon(i) == 0) nex.writeNum("Main.icon" + String(i + 1) + ".pic", 29);
        else nex.writeNum("Main.icon" + String(i + 1) + ".pic", weather.get_dailyIcon(i) + 20);

        // max temp
        if(validate.temp(weather.get_dailyDayTemp(i)))
            nex.writeStr("Main.tempMax" + String(i + 1) + ".txt", String(int(round(weather.get_dailyDayTemp(i)))) + "°C");
        else nex.writeStr("Main.tempMax" + String(i + 1) + ".txt", "--");

        // min temp
        if(validate.temp(weather.get_dailyNightTemp(i)))
            nex.writeStr("Main.tempMin" + String(i + 1) + ".txt", String(int(round(weather.get_dailyNightTemp(i)))) + "°C");
        else nex.writeStr("Main.tempMin" + String(i + 1) + ".txt", "--");

        // wind speed
        if(validate.wind(weather.get_dailyWindSpeed(i)))
            nex.writeStr("Main.wind" + String(i + 1) + ".txt", String(int(round(weather.get_dailyWindSpeed(i)))) + lang.ms());
        else nex.writeStr("Main.wind" + String(i + 1) + ".txt", "--");
    }
}

/**
 * Sending data to display hourly forecast
 */
void Nextion::_hourlyData() {
    char dat[22] = "";
    char buf[20] = "";
    Serial1.print("Hourly.data0.txt=\"");
    if(config.weather_provider() == OPENWEATHERMAP) {
        for(unsigned int i=0; i<40; i++) {
            // temp
            int t = round(weather.get_hourlyTemp(i) * 10);
            sprintf(buf, "%04d", t);
            for(unsigned int k = 0; k < 4; k++) dat[k] = buf[k];
            // pres
            unsigned int p = round(weather.get_hourlyPres(i) * 0.75);
            sprintf(buf, "%03d", p);
            for(unsigned int k = 0; k < 3; k++) dat[4 + k] = buf[k];
            // icon
            sprintf(buf, "%02d", weather.get_hourlyIcon(i));
            for(unsigned int k = 0; k < 2; k++) dat[7 + k] = buf[k];
            // weekday
            sprintf(buf, "%d", weekday(weather.get_hourlyDate(i)) - 1);
            dat[9] = buf[0];
            // day
            sprintf(buf, "%02d", day(weather.get_hourlyDate(i)));
            for(unsigned int k = 0; k < 2; k++) dat[10 + k] = buf[k];
            // month
            sprintf(buf, "%02d", month(weather.get_hourlyDate(i)) - 1);
            for(unsigned int k = 0; k < 2; k++) dat[12 + k] = buf[k];
            // hour
            sprintf(buf, "%02d", hour(weather.get_hourlyDate(i)));
            for(unsigned int k = 0; k < 2; k++) dat[14 + k] = buf[k];
            // wind speed
            unsigned int wind = round(weather.get_hourlyWindSpeed(i));
            sprintf(buf, "%02d", wind);
            for(unsigned int k = 0; k < 2; k++) dat[16 + k] = buf[k];
            // wind direction
            unsigned int deg = round(float(weather.get_hourlyWindDir(i)) / 45.0);
            if(deg > 7) deg = 0;
            sprintf(buf, "%d", deg);
            dat[18] = buf[0];
            // precipitation
            int pr = round(weather.get_hourlyPrec(i) * 100);
            sprintf(buf, "%03d", pr);
            for(unsigned int k=0; k<3; k++) dat[19 + k] = buf[k];

            // send all to display
            for(unsigned int k=0; k<22; k++) Serial1.print(dat[k]);
        }
    }
    Serial1.print("\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);
}

/**
 * Initialize daily forecast links to hourly forecast
 */
void Nextion::_daily2hourly() {
    unsigned int fd = 2;
    for(unsigned int i=0; i<40; i++) {
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
    char dat[18] = "";
    char buf[20] = "";
    Serial1.print("HistoryOut.data0.txt=\"");
    for(unsigned int i = 0; i < 24; i++) {
        // temperature
        int t = round(thingspeak.get_historyField(0, i) * 10);
        sprintf(buf, "%04d", t);
        for(unsigned int k = 0; k < 4; k++) dat[k] = buf[k];
        // humidity
        int h = thingspeak.get_historyField(1, i);
        sprintf(buf, "%03d", h);
        for(unsigned int k = 0; k < 3; k++) dat[4 + k] = buf[k];
        // pressure
        int p = round(thingspeak.get_historyField(2, i) * 0.75);
        sprintf(buf, "%03d", p);
        for(unsigned int k = 0; k < 3; k++) dat[7 + k] = buf[k];
        // day
        sprintf(buf, "%02d", day(thingspeak.get_historyUpdated(i)));
        for(unsigned int k = 0; k < 2; k++) dat[10 + k] = buf[k];
        // month
        sprintf(buf, "%02d", month(thingspeak.get_historyUpdated(i)) - 1);
        for(unsigned int k = 0; k < 2; k++) dat[12 + k] = buf[k];
        // hour
        sprintf(buf, "%02d", hour(thingspeak.get_historyUpdated(i)));
        for(unsigned int k = 0; k < 2; k++) dat[14 + k] = buf[k];
        // minute
        sprintf(buf, "%02d", minute(thingspeak.get_historyUpdated(i)));
        for(unsigned int k = 0; k < 2; k++) dat[16 + k] = buf[k];
        // send current slot
        for(unsigned int k=0; k<18; k++) Serial1.print(dat[k]);
    }
    Serial1.print("\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);
}

/**
 * Sending data to display indoor weather history
 */
void Nextion::_historyIn() {
    char dat[15] = "";
    char buf[20] = "";
    Serial1.print("HistoryIn.data0.txt=\"");
    for(unsigned int i = 0; i < 24; i++) {
        // temperature
        int t = round(thingspeak.get_historyField(3, i) * 10);
        sprintf(buf, "%04d", t);
        for(unsigned int k = 0; k < 4; k++) dat[k] = buf[k];
        // humidity
        int h = thingspeak.get_historyField(4, i);
        sprintf(buf, "%03d", h);
        for(unsigned int k = 0; k < 3; k++) dat[4 + k] = buf[k];
        // day
        sprintf(buf, "%02d", day(thingspeak.get_historyUpdated(i)));
        for(unsigned int k = 0; k < 2; k++) dat[7 + k] = buf[k];
        // month
        sprintf(buf, "%02d", month(thingspeak.get_historyUpdated(i)) - 1);
        for(unsigned int k = 0; k < 2; k++) dat[9 + k] = buf[k];
        // hour
        sprintf(buf, "%02d", hour(thingspeak.get_historyUpdated(i)));
        for(unsigned int k = 0; k < 2; k++) dat[11 + k] = buf[k];
        // minute
        sprintf(buf, "%02d", minute(thingspeak.get_historyUpdated(i)));
        for(unsigned int k = 0; k < 2; k++) dat[13 + k] = buf[k];
        // send current slot
        for(unsigned int k=0; k<15; k++) Serial1.print(dat[k]);
    }
    Serial1.print("\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);
}

/**
 * Sending alarms data
 */
void Nextion::_alarms() {
    char alarmData[144];
    char buf[3];
    unsigned int alarmOn = 0;
    Serial1.print("Alarm.alarms.txt=\"");
    for(unsigned int i=0; i<12; i++) {
        sprintf(buf, "%02d", config.alarm_time(i, 0));
        for(unsigned int j=0; j<2; j++) alarmData[i * 4 + j] = buf[j];
        sprintf(buf, "%02d", config.alarm_time(i, 1));
        for(unsigned int j=0; j<2; j++) alarmData[i * 4 + j + 2] = buf[j];
        for(unsigned int w=0; w<7; w++) {
            sprintf(buf, "%d", config.alarm_weekday(i, w));
            alarmData[i * 7 + w + 48] = buf[0];
        }
        sprintf(buf, "%d", config.alarm_state(i));
        alarmData[i + 132] = buf[0];
        alarmOn |= config.alarm_state(i);
    }
    for(unsigned int k=0; k<144; k++) Serial1.print(alarmData[k]);
    Serial1.print("\"");
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    Serial1.write(0xFF);
    nex.writeNum("Main.alarm.pic", alarmOn ? 71 : 72);
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
                if(_receivedData.lastIndexOf("{\"h\":") != -1)
                    setTime(
                        root["h"].as<int>(), 
                        root["i"].as<int>(), 
                        root["s"].as<int>(), 
                        root["d"].as<int>(), 
                        root["m"].as<int>(), 
                        root["y"].as<int>()
                    );
            }
            _receivedData = "";
            _customData = -1;
        }
    }
}