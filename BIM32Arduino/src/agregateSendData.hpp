class AgregateSendData {
    private:
        static constexpr int WEATHER_SOURCE = 1;
            static constexpr int WEATHER_TEMP = 0;
            static constexpr int WEATHER_HUM = 1;
            static constexpr int WEATHER_PRES = 2;
            static constexpr int WEATHER_WIND_SPEED = 3;
            static constexpr int WEATHER_WIND_DIR = 4;
            static constexpr int WEATHER_ABS_HUM = 5;
            static constexpr int WEATHER_DEW_POINT = 6;
        static constexpr int WSENSOR_SOURCE = 2;
            static constexpr int WSENSOR_TEMP = 4;
            static constexpr int WSENSOR_HUM = 5;
            static constexpr int WSENSOR_PRES = 6;
            static constexpr int WSENSOR_WIND_SPEED = 7;
            static constexpr int WSENSOR_WIND_DIR = 8;
            static constexpr int WSENSOR_LIGHT = 9;
            static constexpr int WSENSOR_VOLT = 10;
            static constexpr int WSENSOR_CURR = 11;
            static constexpr int WSENSOR_PWR = 12;
            static constexpr int WSENSOR_ENRG = 13;
            static constexpr int WSENSOR_FREQ = 14;
            static constexpr int WSENSOR_VBAT = 15;
            static constexpr int WSENSOR_BATPRC = 16;
            static constexpr int WSENSOR_BATLVL = 17;
            static constexpr int WSENSOR_CO2 = 18;
            static constexpr int WSENSOR_ABS_HUM = 19;
            static constexpr int WSENSOR_DEW_POINT = 20;
        static constexpr int BME280_SOURCE = 3;
            static constexpr int BME280_TEMP = 0;
            static constexpr int BME280_HUM = 1;
            static constexpr int BME280_PRES = 2;
            static constexpr int BME280_ABS_HUM = 3;
            static constexpr int BME280_DEW_POINT = 4;
        static constexpr int BMP180_SOURCE = 4;
            static constexpr int BMP180_TEMP = 0;
            static constexpr int BMP180_PRES = 1;
        static constexpr int SHT21_SOURCE = 5;
            static constexpr int SHT21_TEMP = 0;
            static constexpr int SHT21_HUM = 1;
            static constexpr int SHT21_ABS_HUM = 2;
            static constexpr int SHT21_DEW_POINT = 3;
        static constexpr int DHT22_SOURCE = 6;
            static constexpr int DHT22_TEMP = 0;
            static constexpr int DHT22_HUM = 1;
            static constexpr int DHT22_ABS_HUM = 2;
            static constexpr int DHT22_DEW_POINT = 3;
        static constexpr int DS18B20_SOURCE = 7;
        static constexpr int MAX44009_SOURCE = 8;
        static constexpr int BH1750_SOURCE = 9;
        static constexpr int ANALOG_SOURCE = 10;
        static constexpr int ESP32_SOURCE = 11;
            static constexpr int ESP32_TEMP = 0;
            static constexpr int ESP32_RUNTIME = 1;
        static constexpr int BME680_SOURCE = 12;
            static constexpr int BME680_TEMP = 0;
            static constexpr int BME680_HUM = 1;
            static constexpr int BME680_PRES = 2;
            static constexpr int BME680_IAQ = 3;
            static constexpr int BME680_ABS_HUM = 4;
            static constexpr int BME680_DEW_POINT = 5;
    public:
        float sendingData(uint8_t field, uint8_t type, uint8_t wsensNum, uint8_t wsensType);
};

float AgregateSendData::sendingData(uint8_t field, uint8_t type, uint8_t wsensNum, uint8_t wsensType) {
    float data = UNDEFINED_FLOAT;
    switch(field) {
        case WEATHER_SOURCE: { // Weather
            float t = weather.get_currentTemp();
            float h = weather.get_currentHum();
            float p = weather.get_currentPres();
            float ws = weather.get_currentWindSpeed();
            int wd = weather.get_currentWindDir();
            float ah = sensors.absoluteHum(t, h);
            float dp = sensors.dewPoint(t, h);
            if(type == WEATHER_TEMP and validate.temp(t)) data = t;
            if(type == WEATHER_HUM and validate.hum(h)) data = h;
            if(type == WEATHER_PRES and validate.pres(p)) data = p;
            if(type == WEATHER_WIND_SPEED and validate.windSpeed(ws)) data = ws;
            if(type == WEATHER_WIND_DIR and validate.windDir(wd)) data = wd;
            if(type == WEATHER_ABS_HUM and validate.absoluteHum(ah)) data = ah;
            if(type == WEATHER_DEW_POINT and validate.dewPoint(dp, t)) data = dp;
        }; break;
        case WSENSOR_SOURCE: { // Wireless sensor
            if(wsensor.dataRelevance(wsensNum)) {
                float t0 = wsensor.get_temperature(wsensNum, 0);
                float temp = wsensor.get_temperature(wsensNum, wsensType);
                float hum = wsensor.get_humidity(wsensNum);
                float pres = wsensor.get_pressure(wsensNum);
                float windSpeed = wsensor.get_windSpeed(wsensNum);
                int windDir = wsensor.get_windDir(wsensNum);
                float light = wsensor.get_light(wsensNum);
                float volt = wsensor.get_voltage(wsensNum);
                float cur = wsensor.get_current(wsensNum);
                float pwr = wsensor.get_power(wsensNum);
                float enrg = wsensor.get_energy(wsensNum);
                float freq = wsensor.get_frequency(wsensNum);
                float co2 = wsensor.get_co2(wsensNum);
                float vbat = wsensor.get_batteryVoltage(wsensNum);
                int batlvl = wsensor.get_batteryLevel(wsensNum);
                float batprc = wsensor.get_batteryPercentage(wsensNum);
                float ah = sensors.absoluteHum(t0, hum);
                float dp = sensors.dewPoint(t0, hum);
                if(wsensType <= WSENSOR_TEMP and validate.temp(temp)) data = temp;
                if(wsensType == WSENSOR_HUM and validate.hum(hum)) data = hum;
                if(wsensType == WSENSOR_PRES and validate.pres(pres)) data = pres;
                if(wsensType == WSENSOR_WIND_SPEED and validate.windSpeed(windSpeed)) data = windSpeed;
                if(wsensType == WSENSOR_WIND_DIR and validate.windDir(pres)) data = windDir;
                if(wsensType == WSENSOR_LIGHT and validate.light(light)) data = light;
                if(wsensType == WSENSOR_VOLT and validate.hVolt(volt)) data = volt;
                if(wsensType == WSENSOR_CURR and validate.current(cur)) data = cur;
                if(wsensType == WSENSOR_PWR and validate.power(pwr)) data = pwr;
                if(wsensType == WSENSOR_ENRG and validate.energy(enrg)) data = enrg;
                if(wsensType == WSENSOR_FREQ and validate.frequence(freq)) data = freq;
                if(wsensType == WSENSOR_VBAT and validate.batVolt(vbat)) data = vbat;
                if(wsensType == WSENSOR_BATPRC and validate.batPercent(batprc)) data = batprc;
                if(wsensType == WSENSOR_BATLVL and validate.batLvl(batlvl)) data = batlvl;
                if(wsensType == WSENSOR_CO2 and validate.co2(co2)) data = co2;
                if(wsensType == WSENSOR_ABS_HUM and validate.absoluteHum(ah)) data = ah;
                if(wsensType == WSENSOR_DEW_POINT and validate.dewPoint(dp, t0)) data = dp;
            }
        }; break;
        case BME280_SOURCE: { // BME280
            float t = sensors.get_bme280_temp();
            float h = sensors.get_bme280_hum();
            float p = sensors.get_bme280_pres();
            float ah = sensors.absoluteHum(t, h);
            float dp = sensors.dewPoint(t, h);
            if(type == BME280_TEMP and validate.temp(t)) data = t;
            if(type == BME280_HUM and validate.hum(h)) data = h;
            if(type == BME280_PRES and validate.pres(p)) data = p;
            if(type == BME280_ABS_HUM and validate.absoluteHum(ah)) data = ah;
            if(type == BME280_DEW_POINT and validate.dewPoint(dp, t)) data = dp;
        }; break;
        case BMP180_SOURCE: { // BMP180
            float t = sensors.get_bmp180_temp();
            float p = sensors.get_bmp180_pres();
            if(type == BMP180_TEMP and validate.temp(t)) data = t;
            if(type == BMP180_PRES and validate.pres(p)) data = p;
        }; break;
        case SHT21_SOURCE: { // SHT21
            float t = sensors.get_sht21_temp();
            float h = sensors.get_sht21_hum();
            float ah = sensors.absoluteHum(t, h);
            float dp = sensors.dewPoint(t, h);
            if(type == SHT21_TEMP and validate.temp(t)) data = t;
            if(type == SHT21_HUM and validate.hum(h)) data = h;
            if(type == SHT21_ABS_HUM and validate.absoluteHum(ah)) data = ah;
            if(type == SHT21_DEW_POINT and validate.dewPoint(dp, t)) data = dp;
        }; break;
        case DHT22_SOURCE: { // DHT22
            float t = sensors.get_dht22_temp();
            float h = sensors.get_dht22_hum();
            float ah = sensors.absoluteHum(t, h);
            float dp = sensors.dewPoint(t, h);
            if(type == DHT22_TEMP and validate.temp(t)) data = t;
            if(type == DHT22_HUM and validate.hum(h)) data = h;
            if(type == DHT22_ABS_HUM and validate.absoluteHum(ah)) data = ah;
            if(type == DHT22_DEW_POINT and validate.dewPoint(dp, t)) data = dp;
        }; break;
        case DS18B20_SOURCE: { // DS18B20
            float t = sensors.get_ds18b20_temp();
            if(validate.temp(t)) data = t;
        }; break;
        case MAX44009_SOURCE: { // MAX44009
            float l = sensors.get_max44009_light();
            if(validate.light(l)) data = l;
        }; break;
        case BH1750_SOURCE: { // BH1750
            float l = sensors.get_bh1750_light();
            if(validate.light(l)) data = l;
        }; break;
        case ANALOG_SOURCE: { // Ananlog input
            float a = sensors.get_analog_voltage();
            if(validate.volt(a)) data = a;
        }; break;
        case ESP32_SOURCE: { // ESP32
            float t = sensors.get_esp32_temp();
            if(type == ESP32_TEMP and validate.temp(t)) data = t;
            if(type == ESP32_RUNTIME) data = esp_timer_get_time() / 1000ULL;
        }; break;
        case BME680_SOURCE: { // BME680
            float t = sensors.get_bme680_temp();
            float h = sensors.get_bme680_hum();
            float p = sensors.get_bme680_pres();
            float i = sensors.get_bme680_iaq();
            float ah = sensors.absoluteHum(t, h);
            float dp = sensors.dewPoint(t, h);
            if(type == BME680_TEMP and validate.temp(t)) data = t;
            if(type == BME680_HUM and validate.hum(h)) data = h;
            if(type == BME680_PRES and validate.pres(p)) data = p;
            if(type == BME680_IAQ and validate.iaq(i)) data = i;
            if(type == BME680_ABS_HUM and validate.absoluteHum(ah)) data = ah;
            if(type == BME680_DEW_POINT and validate.dewPoint(dp, t)) data = dp;
        }; break;
        default: ; break; 
    }

    return data;
}