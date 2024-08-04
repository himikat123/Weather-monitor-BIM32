class AgregateData {
    public:
        float lcdTempIn(float* tempSequence);
        float lcdTempOut();
        float lcdHumIn(float* humSequence);
        float lcdHumOut();
        float lcdPresOut();
        void lcdSequenceNames(String* nameSequence);
        String lcdVoltage();
        uint8_t lcdVoltageColor();
        int lcdBatteryLevel();
        String lcdComfort();
        uint8_t lcdWindDirection(int deg);
        float comfortTemp();
        float comfortHum();
        float comfortIaq();
        float comfortCo2();

    private:
        void _lcdTempSequence(float* tempSequence);
        void _lcdHumSequence(float* humSequence);
        String _lcdVoltageWsensor();
        String _lcdVoltageThingspeak();
};

float AgregateData::lcdTempIn(float* tempSequence) {
    float temp = 40400.0;
    switch(config.display_source_tempIn_sens()) {
        case 1: temp = weather.get_currentTemp(); break; /* temperature from weather forecast */
        case 2: { /* temperature from wireless sensor */
            if(wsensor.dataRelevance(config.display_source_tempIn_wsensNum())) 
                temp = wsensor.get_temperature(
                    config.display_source_tempIn_wsensNum(), 
                    config.display_source_tempIn_temp(), 
                    CORRECTED
                );
        }; break;
        case 3: { /* temperature from thingspeak */
            if(thingspeak.dataRelevance())
                temp = thingspeak.get_field(config.display_source_tempIn_thing());
        }; break;
        case 4: _lcdTempSequence(tempSequence); break;
        case 5: temp = sensors.get_bme280_temp(CORRECTED); break; /* temperature from BME280 */
        case 6: temp = sensors.get_bmp180_temp(CORRECTED); break; /* temperature from BMP180 */
        case 7: temp = sensors.get_sht21_temp(CORRECTED); break; /* temperature from SHT21 */
        case 8: temp = sensors.get_dht22_temp(CORRECTED); break; /* temperature from DHT22 */
        case 9: temp = sensors.get_ds18b20_temp(CORRECTED); break; /* temperature from DS18B20 */
        case 10: temp = sensors.get_bme680_temp(CORRECTED); break; /* temperature from BME680 */
        default: ; break;
    }
    return temp;
}

void AgregateData::_lcdTempSequence(float* tempSequence) {
    for(uint8_t i=0; i<4; i++) {
        switch(config.display_source_sequence_temp(i)) {
            case 1: tempSequence[i] = weather.get_currentTemp(); break; /* Forecast */
            case 2: { /* wireless sensor */
                if(wsensor.dataRelevance(config.display_source_sequence_wsenstemp(i, 0)))
                    tempSequence[i] = wsensor.get_temperature(
                        config.display_source_sequence_wsenstemp(i, 0),
                        config.display_source_sequence_wsenstemp(i, 1),
                        CORRECTED
                    );
            }; break;
            case 3: { /* thingspeak */
                if(thingspeak.dataRelevance())
                    tempSequence[i] = thingspeak.get_field(config.display_source_sequence_thngtemp(i));
            }; break;
            case 4: tempSequence[i] = sensors.get_bme280_temp(CORRECTED); break; /* BME280 */
            case 5: tempSequence[i] = sensors.get_bmp180_temp(CORRECTED); break; /* BMP180 */
            case 6: tempSequence[i] = sensors.get_sht21_temp(CORRECTED); break; /* SHT21 */
            case 7: tempSequence[i] = sensors.get_dht22_temp(CORRECTED); break; /* DHT22 */
            case 8: tempSequence[i] = sensors.get_ds18b20_temp(CORRECTED); break; /* DS18B20 */
            case 9: tempSequence[i] = sensors.get_bme680_temp(CORRECTED); break; /* BME680 */
            default: ; break;
        }
    }
}

void AgregateData::lcdSequenceNames(String* nameSequence) {
    for(uint8_t i=0; i<4; i++) {
        nameSequence[i] = config.display_source_sequence_name(i);
    }
}

float AgregateData::lcdTempOut() {
    float temp = 40400.0;;
    switch(config.display_source_tempOut_sens()) {
        case 1: temp = weather.get_currentTemp(); break; /* temperature from weather forecast */
        case 2: { /* temperature from wireless sensor */
            if(wsensor.dataRelevance(config.display_source_tempOut_wsensNum())) 
                temp = wsensor.get_temperature(
                    config.display_source_tempOut_wsensNum(), 
                    config.display_source_tempOut_temp(),
                    CORRECTED
                );
        }; break;
        case 3: { /* temperature from thingspeak */
            if(thingspeak.dataRelevance())
                temp = thingspeak.get_field(config.display_source_tempOut_thing());
        }; break;
        case 4: temp = sensors.get_bme280_temp(CORRECTED); break; /* temperature from BME280 */
        case 5: temp = sensors.get_bmp180_temp(CORRECTED); break; /* temperature from BMP180 */
        case 6: temp = sensors.get_sht21_temp(CORRECTED); break; /* temperature from SHT21 */
        case 7: temp = sensors.get_dht22_temp(CORRECTED); break; /* temperature from DHT22 */
        case 8: temp = sensors.get_ds18b20_temp(CORRECTED); break; /* temperature from DS18B20 */
        case 9: temp = sensors.get_bme680_temp(CORRECTED); break; /* temperature from BME680 */
        default: ; break;
    }
    return temp;
}

float AgregateData::lcdHumIn(float* humSequence) {
    float hum = 40400.0;
    switch(config.display_source_humIn_sens()) {
        case 1: hum = weather.get_currentHum(); break; /* humudity from weather forecast */
        case 2: { /* humidity from wireless sensor */
            if(wsensor.dataRelevance(config.display_source_humIn_wsensNum()))
                hum = wsensor.get_humidity(config.display_source_humIn_wsensNum(), CORRECTED);
        }; break;
        case 3: { /* humidity from thingspeak */
            if(thingspeak.dataRelevance()) hum = thingspeak.get_field(config.display_source_humIn_thing());
        }; break;
        case 4: _lcdHumSequence(humSequence); break;
        case 5: hum = sensors.get_bme280_hum(CORRECTED); break; /* humidity from BME280 */
        case 6: hum = sensors.get_sht21_hum(CORRECTED); break; /* humidity from SHT21 */
        case 7: hum = sensors.get_dht22_hum(CORRECTED); break; /* humidity from DHT22 */
        case 8: hum = sensors.get_bme680_hum(CORRECTED); break; /* humidity from BME680 */
        default: ; break;
    }
    return hum;
}

void AgregateData::_lcdHumSequence(float* humSequence) {
    for(uint8_t i=0; i<4; i++) {
        switch(config.display_source_sequence_hum(i)) {
            case 1: humSequence[i] = weather.get_currentHum(); break; /* Forecast */
            case 2: { /* wireless sensor */
                if(wsensor.dataRelevance(config.display_source_sequence_wsenshum(i)))
                    humSequence[i] = wsensor.get_humidity(config.display_source_sequence_wsenshum(i), CORRECTED);
            }; break;
            case 3: { /* thingspeak */
                if(thingspeak.dataRelevance())
                    humSequence[i] = thingspeak.get_field(config.display_source_sequence_thnghum(i));
            }; break;
            case 4: humSequence[i] = sensors.get_bme280_hum(CORRECTED); break; /* BME280 */
            case 5: humSequence[i] = sensors.get_sht21_hum(CORRECTED); break; /* SHT21 */
            case 6: humSequence[i] = sensors.get_dht22_hum(CORRECTED); break; /* DHT22 */
            case 7: humSequence[i] = sensors.get_bme680_hum(CORRECTED); break; /* BME680 */
            default: ; break;
        }
    }
}

float AgregateData::lcdHumOut() {
    float hum = 40400.0;
    switch(config.display_source_humOut_sens()) {
        case 1: hum = weather.get_currentHum(); break; /* humudity from weather forecast */
        case 2: { /* humidity from wireless sensor */
            if(wsensor.dataRelevance(config.display_source_humOut_wsensNum()))
                hum = wsensor.get_humidity(config.display_source_humOut_wsensNum(), CORRECTED);
        }; break;
        case 3: { /* humidity from thingspeak */
            if(thingspeak.dataRelevance()) hum = thingspeak.get_field(config.display_source_humOut_thing());
        }; break;
        case 6: hum = sensors.get_bme280_hum(CORRECTED); break; /* humidity from BME280 */
        case 7: hum = sensors.get_sht21_hum(CORRECTED); break; /* humidity from SHT21 */
        case 8: hum = sensors.get_dht22_hum(CORRECTED); break; /* humidity from DHT22 */
        case 9: hum = sensors.get_bme680_hum(CORRECTED); break; /* humidity from BME680 */
        default: ; break;
    }
    return hum;
}

float AgregateData::lcdPresOut() {
    float pres = 40400.0;
    switch(config.display_source_presOut_sens()) {
        case 1: pres = weather.get_currentPres(); break; // pressure outside from weather forecast
        case 2: { // presure outside from wireless sensor
            if(wsensor.dataRelevance(config.display_source_presOut_wsensNum()))
                pres = wsensor.get_pressure(config.display_source_presOut_wsensNum(), CORRECTED);
        }; break;
        case 3: { // presure outside from thingspeak
            if(thingspeak.dataRelevance()) pres = thingspeak.get_field(config.display_source_presOut_thing());
        }; break;
        case 4: pres = sensors.get_bme280_pres(CORRECTED); break; // pressure outside from BME280
        case 5: pres = sensors.get_bmp180_pres(CORRECTED); break; // pressure outside from BMP180
        case 6: pres = sensors.get_bme680_pres(CORRECTED); break; // pressure outside from BME680
    }
    return pres;
}

String AgregateData::lcdVoltage() {
    String value = "";
    switch(config.display_source_volt_sens()) {
        case 1: value = _lcdVoltageWsensor(); break; /* from wireless sensor */
        case 2: value = _lcdVoltageThingspeak(); break; /* from thingspeak */
        case 3: { // iaq from BME680
            float iaq = sensors.get_bme680_iaq(CORRECTED);
            if(validate.iaq(iaq)) value = "IAQ " + String((int)round(iaq));
            else value = ""; 
        }; break;
        default: ; break;
    }
    return value;
}

String AgregateData::_lcdVoltageWsensor() {
    String value = "";
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
                float hiVolt = wsensor.get_voltage(config.display_source_volt_wsensNum(), CORRECTED);
                if(validate.volt(hiVolt)) value = String(round(hiVolt * 100) / 100) + lang.v();
            }; break;
            case 3: { /* SenseAir S8 CO2 level */
                float co2 = wsensor.get_co2(config.display_source_volt_wsensNum(), CORRECTED);
                if(validate.co2(co2)) value = String((int)round(co2)) + "ppm";
            }; break;
            default: ; break;
        }
    }
    return value;
}

String AgregateData::_lcdVoltageThingspeak() {
    String value = "";
    if(thingspeak.dataRelevance()) {
        switch(config.display_source_volt_thingType()) {
            case 0: { /* battery voltage */
                float volt = thingspeak.get_field(config.display_source_volt_thing());
                if(validate.batVolt(volt)) value = String(round(volt * 100) / 100) + lang.v();
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
uint8_t AgregateData::lcdVoltageColor() {
    uint8_t type = 0;
    switch(config.display_source_volt_sens()) {
        case 1: { /* from wireless sensor */
            if(config.display_source_volt_volt() == 3) /* SenseAir S8 CO2 level */
                type = global.co2_level + 1;
        }; break; 
        case 3: { // iaq from BME680
            type = global.iaq_level + 1;
        }; break;
        default: ; break;
    }
    return type;
}

int AgregateData::lcdBatteryLevel() {
    int level = -1;
    if(config.display_source_bat_sens() == 1) { // battery symbol from wireless sensor
        if(wsensor.dataRelevance(config.display_source_bat_wsensNum()))
            level = wsensor.get_batteryLevel(config.display_source_bat_wsensNum());
    }
    else if(config.display_source_bat_sens() == 2) { // battery symbol from thingspeak
        if(thingspeak.dataRelevance())
            level = thingspeak.get_field(config.display_source_bat_thing());
    }
    return level;
}

String AgregateData::lcdComfort() {
    String comfort = "";
    if(config.display_source_descr() == 1) {
        comfort = lang.comfort(global.comfort);

        if(global.iaq_level) {
            if(comfort.length()) comfort += ". ";
            comfort += lang.airQuality(global.iaq_level);
        }

        else if(global.co2_level) {
            if(comfort.length()) comfort += ". ";
            comfort += lang.airQuality(global.co2_level);
        }
    }
    return comfort;
}

uint8_t AgregateData::lcdWindDirection(int deg) {
    uint8_t wind = 0;
    if((deg >= 338 && deg < 360) || (deg >= 0 && deg < 22)) wind = 0; // north
    else if(deg >= 22 && deg < 67) wind = 1; // north-east
    else if(deg >= 67 && deg < 112) wind = 2; // east
    else if(deg >= 112 && deg < 157) wind = 3; // south-east
    else if(deg >= 157 && deg < 202) wind = 4; // south
    else if(deg >= 202 && deg < 247) wind = 5; // south-west
    else if(deg >= 247 && deg < 292) wind = 6; // west
    else if(deg >= 292 && deg < 338) wind = 7; // north-west
    return wind;
}

float AgregateData::comfortTemp() {
    float temp = 40400.0;
    switch(config.comfort_temp_source()) {
        case 1: temp = weather.get_currentTemp(); break; // temperature from weather forecast
        case 2: // temperature from wireless sensor
            if(wsensor.dataRelevance(config.comfort_temp_wsensNum()))
                temp = wsensor.get_temperature(config.comfort_temp_wsensNum(), config.comfort_temp_sens(), CORRECTED);
            break;
        case 3: // temperature from thingspeak
            if(thingspeak.dataRelevance()) 
                temp = thingspeak.get_field(config.comfort_temp_thing());
            break;
        case 4: temp = sensors.get_bme280_temp(CORRECTED); break; // temperature from BME280
        case 5: temp = sensors.get_bmp180_temp(CORRECTED); break; // temperature from BMP180
        case 6: temp = sensors.get_sht21_temp(CORRECTED); break; // temperature from SHT21
        case 7: temp = sensors.get_dht22_temp(CORRECTED); break; // temperature from DHT22
        case 8: temp = sensors.get_ds18b20_temp(CORRECTED); break; // temperature from DS18B20
        case 9: temp = sensors.get_bme680_temp(CORRECTED); break; // temperature from BME680
        default: ; break;
    }
    return temp;
}

float AgregateData::comfortHum() {
    float hum = 40400.0;
    switch(config.comfort_hum_source()) {
        case 1: hum = weather.get_currentHum(); break; // humidity from weather forecast
        case 2: // humidity from wireless sensor
            if(wsensor.dataRelevance(config.comfort_hum_wsensNum()))
                hum = wsensor.get_humidity(config.comfort_hum_wsensNum(), CORRECTED);
            break;
        case 3: // humidity from thingspeak
            if(thingspeak.dataRelevance())
                hum = thingspeak.get_field(config.comfort_hum_thing());
            break;
        case 4: hum = sensors.get_bme280_hum(CORRECTED); break; // humidity from BME280
        case 5: hum = sensors.get_sht21_hum(CORRECTED); break; // humidity from SHT21
        case 6: hum = sensors.get_dht22_hum(CORRECTED); break; // humidity from DHT22
        case 7: hum = sensors.get_bme680_hum(CORRECTED); break; // humidity from BME680
        default: ; break;
    }
    return hum;
}

float AgregateData::comfortIaq() {
    float iaq = 40400.0;
    if(config.comfort_iaq_source() == 1) {
        iaq = sensors.get_bme680_iaq(CORRECTED);
    }
    return iaq;
}

float AgregateData::comfortCo2() {
    int co2 = 40400.0;
    if(config.comfort_co2_source() == 1) {
        if(wsensor.dataRelevance(config.comfort_co2_wsensNum()))
            co2 = wsensor.get_co2(config.comfort_co2_wsensNum(), CORRECTED);
    }
    return co2;
}