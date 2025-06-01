class AgregateSendData {
    public:
        float sendingData(uint8_t field, uint8_t type, uint8_t wsensNum, uint8_t wsensType);
};

float AgregateSendData::sendingData(uint8_t field, uint8_t type, uint8_t wsensNum, uint8_t wsensType) {
    float data = -40400.0;
    switch(field) {
        case 1: { // Weather
            float t = weather.get_currentTemp(CORRECTED);
            float h = weather.get_currentHum(CORRECTED);
            float p = weather.get_currentPres(CORRECTED);
            float ah = sensors.absoluteHum(t, h);
            float dp = sensors.dewPoint(t, h);
            if(type == 0 and validate.temp(t)) data = config.units_temp() ? sensors.fahrenheit(t) : t;
            if(type == 1 and validate.hum(h)) data = h;
            if(type == 2 and validate.pres(p)) data = config.units_pres() ? p : sensors.mmHg(p);
            if(type == 3 and validate.absoluteHum(ah)) data = ah;
            if(type == 4 and validate.dewPoint(dp, t)) data = config.units_temp() ? sensors.fahrenheit(dp) : dp;
        }; break;
        case 2: { // Wireless sensor
            if(wsensor.dataRelevance(wsensNum)) {
                float t0 = wsensor.get_temperature(wsensNum, 0, CORRECTED);
                float temp = wsensor.get_temperature(wsensNum, wsensType, CORRECTED);
                float hum = wsensor.get_humidity(wsensNum, CORRECTED);
                float pres = wsensor.get_pressure(wsensNum, CORRECTED);
                float light = wsensor.get_light(wsensNum, CORRECTED);
                float volt = wsensor.get_voltage(wsensNum, CORRECTED);
                float cur = wsensor.get_current(wsensNum, CORRECTED);
                float pwr = wsensor.get_power(wsensNum, CORRECTED);
                float enrg = wsensor.get_energy(wsensNum, CORRECTED);
                float freq = wsensor.get_frequency(wsensNum, CORRECTED);
                float co2 = wsensor.get_co2(wsensNum, CORRECTED);
                float vbat = wsensor.get_batteryVoltage(wsensNum);
                int batlvl = wsensor.get_batteryLevel(wsensNum);
                float batprc = wsensor.get_batteryPercentage(wsensNum);
                float ah = sensors.absoluteHum(t0, hum);
                float dp = sensors.dewPoint(t0, hum);
                if(wsensType <= 4 and validate.temp(temp)) data = config.units_temp() ? sensors.fahrenheit(temp) : temp;
                if(wsensType == 5 and validate.hum(hum)) data = hum;
                if(wsensType == 6 and validate.pres(pres)) data = config.units_pres() ? pres : sensors.mmHg(pres);
                if(wsensType == 7 and validate.light(light)) data = light;
                if(wsensType == 8 and validate.hVolt(volt)) data = volt;
                if(wsensType == 9 and validate.current(cur)) data = cur;
                if(wsensType == 10 and validate.power(pwr)) data = pwr;
                if(wsensType == 11 and validate.energy(enrg)) data = enrg;
                if(wsensType == 12 and validate.frequence(freq)) data = freq;
                if(wsensType == 13 and validate.batVolt(vbat)) data = vbat;
                if(wsensType == 14 and validate.batPercent(batprc)) data = batprc;
                if(wsensType == 15 and validate.batLvl(batlvl)) data = batlvl;
                if(wsensType == 16 and validate.co2(co2)) data = co2;
                if(wsensType == 17 and validate.absoluteHum(ah)) data = ah;
                if(wsensType == 18 and validate.dewPoint(dp, t0)) data = config.units_temp() ? sensors.fahrenheit(dp) : dp;
            }
        }; break;
        case 3: { // BME280
            float t = sensors.get_bme280_temp(CORRECTED);
            float h = sensors.get_bme280_hum(CORRECTED);
            float p = sensors.get_bme280_pres(CORRECTED);
            float ah = sensors.absoluteHum(t, h);
            float dp = sensors.dewPoint(t, h);
            if(type == 0 and validate.temp(t)) data = config.units_temp() ? sensors.fahrenheit(t) : t;
            if(type == 1 and validate.hum(h)) data = h;
            if(type == 2 and validate.pres(p)) data = config.units_pres() ? p : sensors.mmHg(p);
            if(type == 3 and validate.absoluteHum(ah)) data = ah;
            if(type == 4 and validate.dewPoint(dp, t)) data = config.units_temp() ? sensors.fahrenheit(dp) : dp;
        }; break;
        case 4: { // BMP180
            float t = sensors.get_bmp180_temp(CORRECTED);
            float p = sensors.get_bmp180_pres(CORRECTED);
            if(type == 0 and validate.temp(t)) data = config.units_temp() ? sensors.fahrenheit(t) : t;
            if(type == 1 and validate.pres(p)) data = config.units_pres() ? p : sensors.mmHg(p);
        }; break;
        case 5: { // SHT21
            float t = sensors.get_sht21_temp(CORRECTED);
            float h = sensors.get_sht21_hum(CORRECTED);
            float ah = sensors.absoluteHum(t, h);
            float dp = sensors.dewPoint(t, h);
            if(type == 0 and validate.temp(t)) data = config.units_temp() ? sensors.fahrenheit(t) : t;
            if(type == 1 and validate.hum(h)) data = h;
            if(type == 2 and validate.absoluteHum(ah)) data = ah;
            if(type == 3 and validate.dewPoint(dp, t)) data = config.units_temp() ? sensors.fahrenheit(dp) : dp;
        }; break;
        case 6: { // DHT22
            float t = sensors.get_dht22_temp(CORRECTED);
            float h = sensors.get_dht22_hum(CORRECTED);
            float ah = sensors.absoluteHum(t, h);
            float dp = sensors.dewPoint(t, h);
            if(type == 0 and validate.temp(t)) data = config.units_temp() ? sensors.fahrenheit(t) : t;
            if(type == 1 and validate.hum(h)) data = h;
            if(type == 2 and validate.absoluteHum(ah)) data = ah;
            if(type == 3 and validate.dewPoint(dp, t)) data = config.units_temp() ? sensors.fahrenheit(dp) : dp;
        }; break;
        case 7: { // DS18B20
            float t = sensors.get_ds18b20_temp(CORRECTED);
            if(validate.temp(t)) data = config.units_temp() ? sensors.fahrenheit(t) : t;
        }; break;
        case 8: { // MAX44009
            float l = sensors.get_max44009_light(CORRECTED);
            if(validate.light(l)) data = l;
        }; break;
        case 9: { // BH1750
            float l = sensors.get_bh1750_light(CORRECTED);
            if(validate.light(l)) data = l;
        }; break;
        case 10: { // Ananlog input
            float a = sensors.get_analog_voltage(CORRECTED);
            if(validate.volt(a)) data = a;
        }; break;
        case 11: { // ESP32
            float t = sensors.get_esp32_temp(CORRECTED);
            if(type == 0 and validate.temp(t)) data = config.units_temp() ? sensors.fahrenheit(t) : t;
            if(type == 1) data = millis() / 1000;
        }; break;
        case 12: { // BME680
            float t = sensors.get_bme680_temp(CORRECTED);
            float h = sensors.get_bme680_hum(CORRECTED);
            float p = sensors.get_bme680_pres(CORRECTED);
            float i = sensors.get_bme680_iaq(CORRECTED);
            float ah = sensors.absoluteHum(t, h);
            float dp = sensors.dewPoint(t, h);
            if(type == 0 and validate.temp(t)) data = config.units_temp() ? sensors.fahrenheit(t) : t;
            if(type == 1 and validate.hum(h)) data = h;
            if(type == 2 and validate.pres(p)) data = config.units_pres() ? p : sensors.mmHg(p);
            if(type == 3 and validate.iaq(i)) data = i;
            if(type == 4 and validate.absoluteHum(ah)) data = ah;
            if(type == 5 and validate.dewPoint(dp, t)) data = config.units_temp() ? sensors.fahrenheit(dp) : dp;
        }; break;
        default: ; break; 
    }

    return data;
}