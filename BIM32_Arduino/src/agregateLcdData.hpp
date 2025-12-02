class AgregateLcdData {
    public:
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
    float temp = 40400.0;
    switch(config.display_source_tempIn_sens()) {
        case 1: temp = weather.get_currentTemp(); break; /* temperature from weather forecast */
        case 2: { /* temperature from wireless sensor */
            if(wsensor.dataRelevance(config.display_source_tempIn_wsensNum())) 
                temp = wsensor.get_temperature(
                    config.display_source_tempIn_wsensNum(), 
                    config.display_source_tempIn_temp()
                );
        }; break;
        case 3: { /* temperature from thingspeak */
            if(thingspeak.dataRelevance())
                temp = thingspeak.get_field(config.display_source_tempIn_thing());
        }; break;
        case 4: _tempSequence(tempSequence); break;
        case 5: temp = sensors.get_bme280_temp(); break; /* temperature from BME280 */
        case 6: temp = sensors.get_bmp180_temp(); break; /* temperature from BMP180 */
        case 7: temp = sensors.get_sht21_temp(); break; /* temperature from SHT21 */
        case 8: temp = sensors.get_dht22_temp(); break; /* temperature from DHT22 */
        case 9: temp = sensors.get_ds18b20_temp(); break; /* temperature from DS18B20 */
        case 10: temp = sensors.get_bme680_temp(); break; /* temperature from BME680 */
        default: ; break;
    }
    return temp;
}

void AgregateLcdData::_tempSequence(float* tempSequence) {
    for(uint8_t i=0; i<4; i++) {
        switch(config.display_source_sequence_temp(i)) {
            case 1: tempSequence[i] = weather.get_currentTemp(); break; /* Forecast */
            case 2: { /* wireless sensor */
                if(wsensor.dataRelevance(config.display_source_sequence_wsenstemp(i, 0)))
                    tempSequence[i] = wsensor.get_temperature(
                        config.display_source_sequence_wsenstemp(i, 0),
                        config.display_source_sequence_wsenstemp(i, 1)
                    );
            }; break;
            case 3: { /* thingspeak */
                if(thingspeak.dataRelevance())
                    tempSequence[i] = thingspeak.get_field(config.display_source_sequence_thngtemp(i));
            }; break;
            case 4: tempSequence[i] = sensors.get_bme280_temp(); break; /* BME280 */
            case 5: tempSequence[i] = sensors.get_bmp180_temp(); break; /* BMP180 */
            case 6: tempSequence[i] = sensors.get_sht21_temp(); break; /* SHT21 */
            case 7: tempSequence[i] = sensors.get_dht22_temp(); break; /* DHT22 */
            case 8: tempSequence[i] = sensors.get_ds18b20_temp(); break; /* DS18B20 */
            case 9: tempSequence[i] = sensors.get_bme680_temp(); break; /* BME680 */
            default: ; break;
        }
    }
}

void AgregateLcdData::sequenceNames(String* nameSequence) {
    for(uint8_t i=0; i<4; i++) {
        nameSequence[i] = config.display_source_sequence_name(i);
    }
}

float AgregateLcdData::tempOut() {
    float temp = 40400.0;;
    switch(config.display_source_tempOut_sens()) {
        case 1: temp = weather.get_currentTemp(); break; /* temperature from weather forecast */
        case 2: { /* temperature from wireless sensor */
            if(wsensor.dataRelevance(config.display_source_tempOut_wsensNum())) 
                temp = wsensor.get_temperature(
                    config.display_source_tempOut_wsensNum(), 
                    config.display_source_tempOut_temp()
                );
        }; break;
        case 3: { /* temperature from thingspeak */
            if(thingspeak.dataRelevance())
                temp = thingspeak.get_field(config.display_source_tempOut_thing());
        }; break;
        case 4: temp = sensors.get_bme280_temp(); break; /* temperature from BME280 */
        case 5: temp = sensors.get_bmp180_temp(); break; /* temperature from BMP180 */
        case 6: temp = sensors.get_sht21_temp(); break; /* temperature from SHT21 */
        case 7: temp = sensors.get_dht22_temp(); break; /* temperature from DHT22 */
        case 8: temp = sensors.get_ds18b20_temp(); break; /* temperature from DS18B20 */
        case 9: temp = sensors.get_bme680_temp(); break; /* temperature from BME680 */
        default: ; break;
    }
    return temp;
}

float AgregateLcdData::humIn(float* humSequence) {
    float hum = 40400.0;
    switch(config.display_source_humIn_sens()) {
        case 1: hum = weather.get_currentHum(); break; /* humudity from weather forecast */
        case 2: { /* humidity from wireless sensor */
            if(wsensor.dataRelevance(config.display_source_humIn_wsensNum()))
                hum = wsensor.get_humidity(config.display_source_humIn_wsensNum());
        }; break;
        case 3: { /* humidity from thingspeak */
            if(thingspeak.dataRelevance()) 
                hum = thingspeak.get_field(config.display_source_humIn_thing());
        }; break;
        case 4: _humSequence(humSequence); break;
        case 5: hum = sensors.get_bme280_hum(); break; /* humidity from BME280 */
        case 6: hum = sensors.get_sht21_hum(); break; /* humidity from SHT21 */
        case 7: hum = sensors.get_dht22_hum(); break; /* humidity from DHT22 */
        case 8: hum = sensors.get_bme680_hum(); break; /* humidity from BME680 */
        default: ; break;
    }
    return hum;
}

void AgregateLcdData::_humSequence(float* humSequence) {
    for(uint8_t i=0; i<4; i++) {
        switch(config.display_source_sequence_hum(i)) {
            case 1: humSequence[i] = weather.get_currentHum(); break; /* Forecast */
            case 2: { /* wireless sensor */
                if(wsensor.dataRelevance(config.display_source_sequence_wsenshum(i)))
                    humSequence[i] = wsensor.get_humidity(config.display_source_sequence_wsenshum(i));
            }; break;
            case 3: { /* thingspeak */
                if(thingspeak.dataRelevance())
                    humSequence[i] = thingspeak.get_field(config.display_source_sequence_thnghum(i));
            }; break;
            case 4: humSequence[i] = sensors.get_bme280_hum(); break; /* BME280 */
            case 5: humSequence[i] = sensors.get_sht21_hum(); break; /* SHT21 */
            case 6: humSequence[i] = sensors.get_dht22_hum(); break; /* DHT22 */
            case 7: humSequence[i] = sensors.get_bme680_hum(); break; /* BME680 */
            default: ; break;
        }
    }
}

float AgregateLcdData::humOut() {
    float hum = 40400.0;
    switch(config.display_source_humOut_sens()) {
        case 1: hum = weather.get_currentHum(); break; /* humudity from weather forecast */
        case 2: { /* humidity from wireless sensor */
            if(wsensor.dataRelevance(config.display_source_humOut_wsensNum()))
                hum = wsensor.get_humidity(config.display_source_humOut_wsensNum());
        }; break;
        case 3: { /* humidity from thingspeak */
            if(thingspeak.dataRelevance()) 
                hum = thingspeak.get_field(config.display_source_humOut_thing());
        }; break;
        case 4: hum = sensors.get_bme280_hum(); break; /* humidity from BME280 */
        case 5: hum = sensors.get_sht21_hum(); break; /* humidity from SHT21 */
        case 6: hum = sensors.get_dht22_hum(); break; /* humidity from DHT22 */
        case 7: hum = sensors.get_bme680_hum(); break; /* humidity from BME680 */
        default: ; break;
    }
    return hum;
}

float AgregateLcdData::presOut() {
    float pres = 40400.0;
    switch(config.display_source_presOut_sens()) {
        case 1: pres = weather.get_currentPres(); break; // pressure outside from weather forecast
        case 2: { // presure outside from wireless sensor
            if(wsensor.dataRelevance(config.display_source_presOut_wsensNum()))
                pres = wsensor.get_pressure(config.display_source_presOut_wsensNum());
        }; break;
        case 3: { // presure outside from thingspeak
            if(thingspeak.dataRelevance()) 
                pres = thingspeak.get_field(config.display_source_presOut_thing());
        }; break;
        case 4: pres = sensors.get_bme280_pres(); break; // pressure outside from BME280
        case 5: pres = sensors.get_bmp180_pres(); break; // pressure outside from BMP180
        case 6: pres = sensors.get_bme680_pres(); break; // pressure outside from BME680
    }
    return pres;
}

float AgregateLcdData::windSpeed() {
    float windSpeed = -1;
    switch(config.display_source_wind_speed_sens()) {
        case 1: windSpeed = weather.get_currentWindSpeed(); break; // wind speed from weather forecast
        case 2: { // wind speed from wireless sensor
            if(wsensor.dataRelevance(config.display_source_wind_speed_wsensNum()))
                windSpeed = wsensor.get_windSpeed(config.display_source_wind_speed_wsensNum());
        }; break;
        case 3: { // wind speed from thingspeak
            if(thingspeak.dataRelevance()) 
                windSpeed = thingspeak.get_field(config.display_source_wind_speed_thing());
        }; break;
    }
    return windSpeed;
}

int AgregateLcdData::windDir() {
    int windDir = -1;
    switch(config.display_source_wind_dir_sens()) {
        case 1: windDir = weather.get_currentWindDir(); break; // wind directory from weather forecast
        case 2: { // wind directory from wireless sensor
            if(wsensor.dataRelevance(config.display_source_wind_dir_wsensNum()))
                windDir = wsensor.get_windDir(config.display_source_wind_dir_wsensNum());
        }; break;
        case 3: { // wind directory from thingspeak
            if(thingspeak.dataRelevance()) 
                windDir = round(thingspeak.get_field(config.display_source_wind_dir_thing()));
        }; break;
    }
    return windDir;
}

String AgregateLcdData::voltage() {
    String value = "";
    switch(config.display_source_volt_sens()) {
        case 1: value = _voltageWsensor(); break; /* from wireless sensor */
        case 2: value = _voltageThingspeak(); break; /* from thingspeak */
        case 3: value = localDate(); break; /* date */
        case 4: value = _iaq(); break; // iaq from BME680
        case 5: value = _absoluteHum(sensors.get_bme680_temp(), sensors.get_bme680_hum()); break; // absolute humidity from BME680
        case 6: value = _dewPoint(sensors.get_bme680_temp(), sensors.get_bme680_hum()); break; // dew point from BME680
        case 7: value = _absoluteHum(sensors.get_bme280_temp(), sensors.get_bme280_hum()); break; // absolute humidity from BME280
        case 8: value = _dewPoint(sensors.get_bme280_temp(), sensors.get_bme280_hum()); break; // dew point from BME280
        case 9: value = _absoluteHum(sensors.get_dht22_temp(), sensors.get_dht22_hum()); break; // absolute humidity from DHT22
        case 10: value = _dewPoint(sensors.get_dht22_temp(), sensors.get_dht22_hum()); break; // dew point from DHT22
        case 11: value = _absoluteHum(sensors.get_sht21_temp(), sensors.get_sht21_hum()); break; // absolute humidity from SHT21
        case 12: value = _dewPoint(sensors.get_sht21_temp(), sensors.get_sht21_hum()); break; // dew point from SHT21
        case 13: value = _absoluteHum(weather.get_currentTemp(), weather.get_currentHum()); break; // absolute humidity from weather forecast
        case 14: value = _dewPoint(weather.get_currentTemp(), weather.get_currentHum()); break; // dew point from weather forecast
        default: ; break;
    }
    return value;
}

String AgregateLcdData::_voltageWsensor() {
    String value = "--";
    if(wsensor.dataRelevance(config.display_source_volt_wsensNum())) {
        switch(config.display_source_volt_volt()) {
            case 0: { /* battery voltage */
                float volt = wsensor.get_batteryVoltage(config.display_source_volt_wsensNum());
                if(validate.batVolt(volt)) value = String(round(volt * 100) / 100) + lang.v();
            }; break;
            case 1: { /* battery percentage */
                float prc = wsensor.get_batteryPercentage(config.display_source_volt_wsensNum());
                if(validate.batPercent(prc)) value = String(prc, 0) + "%";
            }; break;
            case 2: { /* PZEM-004t voltage */
                float hiVolt = wsensor.get_voltage(config.display_source_volt_wsensNum());
                if(validate.volt(hiVolt)) value = String(round(hiVolt * 100) / 100) + lang.v();
            }; break;
            case 3: { /* SenseAir S8 CO2 level */
                float co2 = wsensor.get_co2(config.display_source_volt_wsensNum());
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
        switch(config.display_source_volt_thingType()) {
            case 0: { /* battery voltage */
                float volt = thingspeak.get_field(config.display_source_volt_thing());
                if(validate.thingVolt(volt)) value = String(round(volt * 100) / 100) + lang.v();
            }; break;
            case 1: { /* battery percentage */
                float prc = thingspeak.get_field(config.display_source_volt_thing());
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
    switch(config.display_source_volt_sens()) {
        case 1: { /* from wireless sensor */
            if(config.display_source_volt_volt() == 3) /* SenseAir S8 CO2 level */
                type = state.co2_level;
        }; break; 
        case 4: { // iaq from BME680
            type = state.iaq_level;
        }; break;
        default: ; break;
    }
    return type;
}

int AgregateLcdData::batteryLevel() {
    int level = -1;
    if(config.display_source_bat_sens() == 1) { // battery symbol from wireless sensor
        if(wsensor.dataRelevance(config.display_source_bat_wsensNum())) {
            level = wsensor.get_batteryLevel(config.display_source_bat_wsensNum());
        }
    }
    else if(config.display_source_bat_sens() == 2) { // battery symbol from thingspeak
        if(thingspeak.dataRelevance()) {
            level = thingspeak.get_field(config.display_source_bat_thing());
            if(level == 5) level = 4;
        }
    }
    return level;
}

String AgregateLcdData::comfort() {
    String comfort = "";
    if(config.display_source_descr() == 1) {
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