class AgregateLcdData {
    public:
        static constexpr int WEATHER_TEMP_IN       = 1;
        static constexpr int WSENSOR_TEMP_IN       = 2;
        static constexpr int THING_TEMP_IN         = 3;
        static constexpr int SEQUENCE_TEMP_IN      = 4;
        static constexpr int BME280_TEMP_IN        = 5;
        static constexpr int BMP180_TEMP_IN        = 6;
        static constexpr int SHT21_TEMP_IN         = 7;
        static constexpr int DHT22_TEMP_IN         = 8;
        static constexpr int DS18B20_TEMP_IN       = 9;
        static constexpr int BME680_TEMP_IN        = 10;

        static constexpr int WEATHER_TEMP_OUT = 1;
        static constexpr int WSENSOR_TEMP_OUT = 2;
        static constexpr int THING_TEMP_OUT   = 3;
        static constexpr int BME280_TEMP_OUT  = 4;
        static constexpr int BMP180_TEMP_OUT  = 5;
        static constexpr int SHT21_TEMP_OUT   = 6;
        static constexpr int DHT22_TEMP_OUT   = 7;
        static constexpr int DS18B20_TEMP_OUT = 8;
        static constexpr int BME680_TEMP_OUT  = 9;

        static constexpr int WEATHER_TEMP_SEQUENCE = 1;
        static constexpr int WSENSOR_TEMP_SEQUENCE = 2;
        static constexpr int THING_TEMP_SEQUENCE   = 3;
        static constexpr int BME280_TEMP_SEQUENCE  = 4;
        static constexpr int BMP180_TEMP_SEQUENCE  = 5;
        static constexpr int SHT21_TEMP_SEQUENCE   = 6;
        static constexpr int DHT22_TEMP_SEQUENCE   = 7;
        static constexpr int DS18B20_TEMP_SEQUENCE = 8;
        static constexpr int BME680_TEMP_SEQUENCE  = 9;

        static constexpr int WEATHER_HUM_IN  = 1;
        static constexpr int WSENSOR_HUM_IN  = 2;
        static constexpr int THING_HUM_IN    = 3;
        static constexpr int SEQUENCE_HUM_IN = 4;
        static constexpr int BME280_HUM_IN   = 5;
        static constexpr int SHT21_HUM_IN    = 6;
        static constexpr int DHT22_HUM_IN    = 7;
        static constexpr int BME680_HUM_IN   = 8;

        static constexpr int WEATHER_HUM_SEQUENCE = 1;
        static constexpr int WSENSOR_HUM_SEQUENCE = 2;
        static constexpr int THING_HUM_SEQUENCE   = 3;
        static constexpr int BME280_HUM_SEQUENCE  = 4;
        static constexpr int SHT21_HUM_SEQUENCE   = 5;
        static constexpr int DHT22_HUM_SEQUENCE   = 6;
        static constexpr int BME680_HUM_SEQUENCE  = 7;

        static constexpr int WEATHER_HUM_OUT = 1;
        static constexpr int WSENSOR_HUM_OUT = 2;
        static constexpr int THING_HUM_OUT   = 3;
        static constexpr int BME280_HUM_OUT  = 4;
        static constexpr int SHT21_HUM_OUT   = 5;
        static constexpr int DHT22_HUM_OUT   = 6;
        static constexpr int BME680_HUM_OUT  = 7;

        static constexpr int WEATHER_PRES_OUT = 1;
        static constexpr int WSENSOR_PRES_OUT = 2;
        static constexpr int THING_PRES_OUT   = 3;
        static constexpr int BME280_PRES_OUT  = 4;
        static constexpr int BMP180_PRES_OUT  = 5;
        static constexpr int BME680_PRES_OUT  = 6;

        static constexpr int WEATHER_WIND_SPEED = 1;
        static constexpr int WSENSOR_WIND_SPEED = 2;
        static constexpr int THING_WIND_SPEED   = 3;

        static constexpr int WEATHER_WIND_DIR = 1;
        static constexpr int WSENSOR_WIND_DIR = 2;
        static constexpr int THING_WIND_DIR   = 3;

        static constexpr int WSENSOR_VOLT      = 1;
        static constexpr int THING_VOLT        = 2;
        static constexpr int DATE              = 3;
        static constexpr int BME680_IAQ        = 4;
        static constexpr int BME680_ABS_HUM    = 5;
        static constexpr int BME680_DEW_POINT  = 6;
        static constexpr int BME280_ABS_HUM    = 7;
        static constexpr int BME280_DEW_POINT  = 8;
        static constexpr int DHT22_ABS_HUM     = 9;
        static constexpr int DHT22_DEW_POINT   = 10;
        static constexpr int SHT21_ABS_HUM     = 11;
        static constexpr int SHT21_DEW_POINT   = 12;
        static constexpr int WEATHER_ABS_HUM   = 13;
        static constexpr int WEATHER_DEW_POINT = 14;

        static constexpr int WSENSOR_BAT_VOLT    = 0;
        static constexpr int WSENSOR_BAT_PERCENT = 1;
        static constexpr int WSENSOR_PZEM_VOLT   = 2;
        static constexpr int WSENSOR_S8_CO2      = 3;

        static constexpr int THING_BAT_VOLT    = 0;
        static constexpr int THING_BAT_PERCENT = 1;

        static constexpr int WSENSOR_S8_COLOR = 1;
        static constexpr int BME680_IAQ_COLOR = 4;

        static constexpr int WSENSOR_BAT_SYMB = 1;
        static constexpr int THING_BAT_SYMB   = 2;

        float tempIn(float* tempSequence);
        float tempOut();
        float humIn(float* humSequence);
        float humOut();
        float presOut();
        float windSpeed();
        int windDir();
        void sequenceNames(String* nameSequence);
        String voltage();
        uint8_t voltageColor();
        int batteryLevel();
        String comfort();
        uint8_t windDirection(int deg);
        String localDate();

    private:
        void _tempSequence(float* tempSequence);
        void _humSequence(float* humSequence);
        String _voltageWsensor();
        String _voltageThingspeak();
        String _iaq();
        String _absoluteHum(float temp, float hum);
        String _dewPoint(float temp, float hum);
};

float AgregateLcdData::tempIn(float* tempSequence) {
    float temp = UNDEFINED_FLOAT;
    switch(config.display.source.tempIn.sens()) {
        case WEATHER_TEMP_IN: temp = weather.get_currentTemp(); break; /* temperature from weather forecast */
        case WSENSOR_TEMP_IN: { /* temperature from wireless sensor */
            if(wsensor.dataRelevance(config.display.source.tempIn.wsensNum())) 
                temp = wsensor.get_temperature(
                    config.display.source.tempIn.wsensNum(), 
                    config.display.source.tempIn.temp()
                );
        }; break;
        case THING_TEMP_IN: { /* temperature from thingspeak */
            if(thingspeak.dataRelevance())
                temp = thingspeak.get_field(config.display.source.tempIn.thing());
        }; break;
        case SEQUENCE_TEMP_IN: _tempSequence(tempSequence); break;
        case BME280_TEMP_IN: temp = sensors.get_bme280_temp(); break; /* temperature from BME280 */
        case BMP180_TEMP_IN: temp = sensors.get_bmp180_temp(); break; /* temperature from BMP180 */
        case SHT21_TEMP_IN: temp = sensors.get_sht21_temp(); break; /* temperature from SHT21 */
        case DHT22_TEMP_IN: temp = sensors.get_dht22_temp(); break; /* temperature from DHT22 */
        case DS18B20_TEMP_IN: temp = sensors.get_ds18b20_temp(); break; /* temperature from DS18B20 */
        case BME680_TEMP_IN: temp = sensors.get_bme680_temp(); break; /* temperature from BME680 */
        default: ; break;
    }
    return temp;
}

void AgregateLcdData::_tempSequence(float* tempSequence) {
    for(uint8_t i=0; i<4; i++) {
        tempSequence[i] = UNDEFINED_FLOAT;
        switch(config.display.source.sequence.temp(i)) {
            case WEATHER_TEMP_SEQUENCE: tempSequence[i] = weather.get_currentTemp(); break; /* Forecast */
            case WSENSOR_TEMP_SEQUENCE: { /* wireless sensor */
                if(wsensor.dataRelevance(config.display.source.sequence.wsenstemp(i, 0)))
                    tempSequence[i] = wsensor.get_temperature(
                        config.display.source.sequence.wsenstemp(i, 0),
                        config.display.source.sequence.wsenstemp(i, 1)
                    );
            }; break;
            case THING_TEMP_SEQUENCE: { /* thingspeak */
                if(thingspeak.dataRelevance())
                    tempSequence[i] = thingspeak.get_field(config.display.source.sequence.thngtemp(i));
            }; break;
            case BME280_TEMP_SEQUENCE: tempSequence[i] = sensors.get_bme280_temp(); break; /* BME280 */
            case BMP180_TEMP_SEQUENCE: tempSequence[i] = sensors.get_bmp180_temp(); break; /* BMP180 */
            case SHT21_TEMP_SEQUENCE: tempSequence[i] = sensors.get_sht21_temp(); break; /* SHT21 */
            case DHT22_TEMP_SEQUENCE: tempSequence[i] = sensors.get_dht22_temp(); break; /* DHT22 */
            case DS18B20_TEMP_SEQUENCE: tempSequence[i] = sensors.get_ds18b20_temp(); break; /* DS18B20 */
            case BME680_TEMP_SEQUENCE: tempSequence[i] = sensors.get_bme680_temp(); break; /* BME680 */
            default: ; break;
        }
    }
}

void AgregateLcdData::sequenceNames(String* nameSequence) {
    for(uint8_t i=0; i<4; i++) {
        nameSequence[i] = config.display.source.sequence.name(i);
    }
}

float AgregateLcdData::tempOut() {
    float temp = UNDEFINED_FLOAT;
    switch(config.display.source.tempOut.sens()) {
        case WEATHER_TEMP_OUT: temp = weather.get_currentTemp(); break; /* temperature from weather forecast */
        case WSENSOR_TEMP_OUT: { /* temperature from wireless sensor */
            if(wsensor.dataRelevance(config.display.source.tempOut.wsensNum())) 
                temp = wsensor.get_temperature(
                    config.display.source.tempOut.wsensNum(), 
                    config.display.source.tempOut.temp()
                );
        }; break;
        case THING_TEMP_OUT: { /* temperature from thingspeak */
            if(thingspeak.dataRelevance())
                temp = thingspeak.get_field(config.display.source.tempOut.thing());
        }; break;
        case BME280_TEMP_OUT: temp = sensors.get_bme280_temp(); break; /* temperature from BME280 */
        case BMP180_TEMP_OUT: temp = sensors.get_bmp180_temp(); break; /* temperature from BMP180 */
        case SHT21_TEMP_OUT: temp = sensors.get_sht21_temp(); break; /* temperature from SHT21 */
        case DHT22_TEMP_OUT: temp = sensors.get_dht22_temp(); break; /* temperature from DHT22 */
        case DS18B20_TEMP_OUT: temp = sensors.get_ds18b20_temp(); break; /* temperature from DS18B20 */
        case BME680_TEMP_OUT: temp = sensors.get_bme680_temp(); break; /* temperature from BME680 */
        default: ; break;
    }
    return temp;
}

float AgregateLcdData::humIn(float* humSequence) {
    float hum = UNDEFINED_FLOAT;
    switch(config.display.source.humIn.sens()) {
        case WEATHER_HUM_IN: hum = weather.get_currentHum(); break; /* humudity from weather forecast */
        case WSENSOR_HUM_IN: { /* humidity from wireless sensor */
            if(wsensor.dataRelevance(config.display.source.humIn.wsensNum()))
                hum = wsensor.get_humidity(config.display.source.humIn.wsensNum());
        }; break;
        case THING_HUM_IN: { /* humidity from thingspeak */
            if(thingspeak.dataRelevance()) 
                hum = thingspeak.get_field(config.display.source.humIn.thing());
        }; break;
        case SEQUENCE_HUM_IN: _humSequence(humSequence); break;
        case BME280_HUM_IN: hum = sensors.get_bme280_hum(); break; /* humidity from BME280 */
        case SHT21_HUM_IN: hum = sensors.get_sht21_hum(); break; /* humidity from SHT21 */
        case DHT22_HUM_IN: hum = sensors.get_dht22_hum(); break; /* humidity from DHT22 */
        case BME680_HUM_IN: hum = sensors.get_bme680_hum(); break; /* humidity from BME680 */
        default: ; break;
    }
    return hum;
}

void AgregateLcdData::_humSequence(float* humSequence) {
    for(uint8_t i=0; i<4; i++) {
        humSequence[i] = UNDEFINED_FLOAT;
        switch(config.display.source.sequence.hum(i)) {
            case WEATHER_HUM_SEQUENCE: humSequence[i] = weather.get_currentHum(); break; /* Forecast */
            case WSENSOR_HUM_SEQUENCE: { /* wireless sensor */
                if(wsensor.dataRelevance(config.display.source.sequence.wsenshum(i)))
                    humSequence[i] = wsensor.get_humidity(config.display.source.sequence.wsenshum(i));
            }; break;
            case THING_HUM_SEQUENCE: { /* thingspeak */
                if(thingspeak.dataRelevance())
                    humSequence[i] = thingspeak.get_field(config.display.source.sequence.thnghum(i));
            }; break;
            case BME280_HUM_SEQUENCE: humSequence[i] = sensors.get_bme280_hum(); break; /* BME280 */
            case SHT21_HUM_SEQUENCE: humSequence[i] = sensors.get_sht21_hum(); break; /* SHT21 */
            case DHT22_HUM_SEQUENCE: humSequence[i] = sensors.get_dht22_hum(); break; /* DHT22 */
            case BME680_HUM_SEQUENCE: humSequence[i] = sensors.get_bme680_hum(); break; /* BME680 */
            default: ; break;
        }
    }
}

float AgregateLcdData::humOut() {
    float hum = UNDEFINED_FLOAT;
    switch(config.display.source.humOut.sens()) {
        case WEATHER_HUM_OUT: hum = weather.get_currentHum(); break; /* humudity from weather forecast */
        case WSENSOR_HUM_OUT: { /* humidity from wireless sensor */
            if(wsensor.dataRelevance(config.display.source.humOut.wsensNum()))
                hum = wsensor.get_humidity(config.display.source.humOut.wsensNum());
        }; break;
        case THING_HUM_OUT: { /* humidity from thingspeak */
            if(thingspeak.dataRelevance()) 
                hum = thingspeak.get_field(config.display.source.humOut.thing());
        }; break;
        case BME280_HUM_OUT: hum = sensors.get_bme280_hum(); break; /* humidity from BME280 */
        case SHT21_HUM_OUT: hum = sensors.get_sht21_hum(); break; /* humidity from SHT21 */
        case DHT22_HUM_OUT: hum = sensors.get_dht22_hum(); break; /* humidity from DHT22 */
        case BME680_HUM_OUT: hum = sensors.get_bme680_hum(); break; /* humidity from BME680 */
        default: ; break;
    }
    return hum;
}

float AgregateLcdData::presOut() {
    float pres = UNDEFINED_FLOAT;
    switch(config.display.source.presOut.sens()) {
        case WEATHER_PRES_OUT: pres = weather.get_currentPres(); break; // pressure outside from weather forecast
        case WSENSOR_PRES_OUT: { // presure outside from wireless sensor
            if(wsensor.dataRelevance(config.display.source.presOut.wsensNum()))
                pres = wsensor.get_pressure(config.display.source.presOut.wsensNum());
        }; break;
        case THING_PRES_OUT: { // presure outside from thingspeak
            if(thingspeak.dataRelevance()) 
                pres = thingspeak.get_field(config.display.source.presOut.thing());
        }; break;
        case BME280_PRES_OUT: pres = sensors.get_bme280_pres(); break; // pressure outside from BME280
        case BMP180_PRES_OUT: pres = sensors.get_bmp180_pres(); break; // pressure outside from BMP180
        case BME680_PRES_OUT: pres = sensors.get_bme680_pres(); break; // pressure outside from BME680
    }
    return pres;
}

float AgregateLcdData::windSpeed() {
    float windSpeed = UNDEFINED_FLOAT;
    switch(config.display.source.wind.speed.sens()) {
        case WEATHER_WIND_SPEED: windSpeed = weather.get_currentWindSpeed(); break; // wind speed from weather forecast
        case WSENSOR_WIND_SPEED: { // wind speed from wireless sensor
            if(wsensor.dataRelevance(config.display.source.wind.speed.wsensNum()))
                windSpeed = wsensor.get_windSpeed(config.display.source.wind.speed.wsensNum());
        }; break;
        case THING_WIND_SPEED: { // wind speed from thingspeak
            if(thingspeak.dataRelevance()) 
                windSpeed = thingspeak.get_field(config.display.source.wind.speed.thing());
        }; break;
    }
    return windSpeed;
}

int AgregateLcdData::windDir() {
    int windDir = UNDEFINED_FLOAT;
    switch(config.display.source.wind.dir.sens()) {
        case WEATHER_WIND_DIR: windDir = weather.get_currentWindDir(); break; // wind directory from weather forecast
        case WSENSOR_WIND_DIR: { // wind directory from wireless sensor
            if(wsensor.dataRelevance(config.display.source.wind.dir.wsensNum()))
                windDir = wsensor.get_windDir(config.display.source.wind.dir.wsensNum());
        }; break;
        case THING_WIND_DIR: { // wind directory from thingspeak
            if(thingspeak.dataRelevance()) 
                windDir = round(thingspeak.get_field(config.display.source.wind.dir.thing()));
        }; break;
    }
    return windDir;
}

String AgregateLcdData::voltage() {
    String value = "";
    switch(config.display.source.volt.sens()) {
        case WSENSOR_VOLT: value = _voltageWsensor(); break; /* from wireless sensor */
        case THING_VOLT: value = _voltageThingspeak(); break; /* from thingspeak */
        case DATE: value = localDate(); break; /* date */
        case BME680_IAQ: value = _iaq(); break; // iaq from BME680
        case BME680_ABS_HUM: value = _absoluteHum(sensors.get_bme680_temp(), sensors.get_bme680_hum()); break; // absolute humidity from BME680
        case BME680_DEW_POINT: value = _dewPoint(sensors.get_bme680_temp(), sensors.get_bme680_hum()); break; // dew point from BME680
        case BME280_ABS_HUM: value = _absoluteHum(sensors.get_bme280_temp(), sensors.get_bme280_hum()); break; // absolute humidity from BME280
        case BME280_DEW_POINT: value = _dewPoint(sensors.get_bme280_temp(), sensors.get_bme280_hum()); break; // dew point from BME280
        case DHT22_ABS_HUM: value = _absoluteHum(sensors.get_dht22_temp(), sensors.get_dht22_hum()); break; // absolute humidity from DHT22
        case DHT22_DEW_POINT: value = _dewPoint(sensors.get_dht22_temp(), sensors.get_dht22_hum()); break; // dew point from DHT22
        case SHT21_ABS_HUM: value = _absoluteHum(sensors.get_sht21_temp(), sensors.get_sht21_hum()); break; // absolute humidity from SHT21
        case SHT21_DEW_POINT: value = _dewPoint(sensors.get_sht21_temp(), sensors.get_sht21_hum()); break; // dew point from SHT21
        case WEATHER_ABS_HUM: value = _absoluteHum(weather.get_currentTemp(), weather.get_currentHum()); break; // absolute humidity from weather forecast
        case WEATHER_DEW_POINT: value = _dewPoint(weather.get_currentTemp(), weather.get_currentHum()); break; // dew point from weather forecast
        default: ; break;
    }
    return value;
}

String AgregateLcdData::_voltageWsensor() {
    String value = "--";
    if(wsensor.dataRelevance(config.display.source.volt.wsensNum())) {
        switch(config.display.source.volt.volt()) {
            case WSENSOR_BAT_VOLT: { /* battery voltage */
                float volt = wsensor.get_batteryVoltage(config.display.source.volt.wsensNum());
                if(validate.batVolt(volt)) value = String(round(volt * 100) / 100) + lang.v();
            }; break;
            case WSENSOR_BAT_PERCENT: { /* battery percentage */
                float prc = wsensor.get_batteryPercentage(config.display.source.volt.wsensNum());
                if(validate.batPercent(prc)) value = String(prc, 0) + "%";
            }; break;
            case WSENSOR_PZEM_VOLT: { /* PZEM-004t voltage */
                float hiVolt = wsensor.get_voltage(config.display.source.volt.wsensNum());
                if(validate.volt(hiVolt)) value = String(round(hiVolt * 100) / 100) + lang.v();
            }; break;
            case WSENSOR_S8_CO2: { /* SenseAir S8 CO2 level */
                float co2 = wsensor.get_co2(config.display.source.volt.wsensNum());
                if(validate.co2(co2)) value = String((int)round(co2)) + "ppm";
            }; break;
            default: ; break;
        }
    }
    return value;
}

String AgregateLcdData::_voltageThingspeak() {
    String value = "--";
    if(thingspeak.dataRelevance()) {
        switch(config.display.source.volt.thingType()) {
            case THING_BAT_VOLT: { /* battery voltage */
                float volt = thingspeak.get_field(config.display.source.volt.thing());
                if(validate.thingVolt(volt)) value = String(round(volt * 100) / 100) + lang.v();
            }; break;
            case THING_BAT_PERCENT: { /* battery percentage */
                float prc = thingspeak.get_field(config.display.source.volt.thing());
                if(validate.batPercent(prc)) value = String(prc, 0) + "%";
            }; break;
            default: ; break;
        }
    }
    return value;
}

/**
 * Voltage color
 * @return 0: default color, 1-3: air quality level
 */
uint8_t AgregateLcdData::voltageColor() {
    uint8_t type = 0;
    switch(config.display.source.volt.sens()) {
        case WSENSOR_S8_COLOR: { /* from wireless sensor */
            if(config.display.source.volt.volt() == 3) /* SenseAir S8 CO2 level */
                type = state.co2_level;
        }; break; 
        case BME680_IAQ_COLOR: { // iaq from BME680
            type = state.iaq_level;
        }; break;
        default: ; break;
    }
    return type;
}

int AgregateLcdData::batteryLevel() {
    int level = -1;
    if(config.display.source.bat.sens() == WSENSOR_BAT_SYMB) { // battery symbol from wireless sensor
        if(wsensor.dataRelevance(config.display.source.bat.wsensNum())) {
            level = wsensor.get_batteryLevel(config.display.source.bat.wsensNum());
        }
    }
    else if(config.display.source.bat.sens() == THING_BAT_SYMB) { // battery symbol from thingspeak
        if(thingspeak.dataRelevance()) {
            level = thingspeak.get_field(config.display.source.bat.thing());
            if(level == 5) level = 4;
        }
    }
    return level;
}

String AgregateLcdData::comfort() {
    String comfort = "";
    if(config.display.source.descr() == 1) {
        comfort = lang.comfort(state.comfort);

        if(state.iaq_level) {
            if(comfort.length()) comfort += ". ";
            comfort += lang.airQuality(state.iaq_level);
        }

        else if(state.co2_level) {
            if(comfort.length()) comfort += ". ";
            comfort += lang.airQuality(state.co2_level);
        }
    }
    return comfort;
}

uint8_t AgregateLcdData::windDirection(int deg) {
    uint8_t wind = 0;
    if((deg >= 338 && deg <= 360) || (deg >= 0 && deg < 22)) wind = 0; // north
    else if(deg >= 22 && deg < 67) wind = 1; // north-east
    else if(deg >= 67 && deg < 112) wind = 2; // east
    else if(deg >= 112 && deg < 157) wind = 3; // south-east
    else if(deg >= 157 && deg < 202) wind = 4; // south
    else if(deg >= 202 && deg < 247) wind = 5; // south-west
    else if(deg >= 247 && deg < 292) wind = 6; // west
    else if(deg >= 292 && deg < 338) wind = 7; // north-west
    return wind;
}

String AgregateLcdData::_iaq() {
    float iaq = sensors.get_bme680_iaq();
    if(validate.iaq(iaq)) return ("IAQ " + String((int)round(iaq)));
    else return "--";
}

String AgregateLcdData::_absoluteHum(float temp, float hum) {
    float ah = sensors.absoluteHum(temp, hum);
    if(validate.absoluteHum(ah)) {
        ah = round(ah * 10) / 10.0;
        String buf = String(ah, 1);
        buf += lang.gpm();
        return buf;
    }
    else return "--";
}

String AgregateLcdData::_dewPoint(float temp, float hum) {
    float dp = sensors.dewPoint(temp, hum);
    if(validate.dewPoint(dp, temp)) {
        float value = dp;
        value = round(value * 10) / 10.0;
        String buf = String(value, 1) + "°C";
        return buf;
    }
    else return "--";
}

String AgregateLcdData::localDate() {
    char buf[20];
    if(config.lang() == "en") sprintf(buf, "%s %02d, %d", lang.monthShortName(month()), day(), year()); 
    else if(config.lang() == "de") sprintf(buf, "%02d. %s %d", day(), lang.monthShortName(month()), year());
    else sprintf(buf, "%02d %s %d", day(), lang.monthShortName(month()), year());

    return String(buf);
}