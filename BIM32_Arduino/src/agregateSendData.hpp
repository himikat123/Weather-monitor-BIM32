class AgregateSendData {
    public:
        float sendingData(uint8_t field, uint8_t type, uint8_t wsensNum, uint8_t wsensType);
};

float AgregateSendData::sendingData(uint8_t field, uint8_t type, uint8_t wsensNum, uint8_t wsensType) {
    float data = -40400.0;
    switch(field) {
        case 1: { // Weather
            // Temperature
            if(type == 0 and validate.temp(weather.get_currentTemp()))
                data = weather.get_currentTemp();
             // Humidity
            if(type == 1 and validate.hum(weather.get_currentHum()))
                data = weather.get_currentHum();
            // Pressure
            if(type == 2 and validate.pres(weather.get_currentPres())) 
                data = weather.get_currentPres();
        }; break;
        case 2: { // Wireless sensor
            if(wsensor.dataRelevance(wsensNum)) {
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

                if(wsensType <= 4 and validate.temp(temp)) data = temp;
                if(wsensType == 5 and validate.hum(hum)) data = hum;
                if(wsensType == 6 and validate.pres(pres)) data = pres;
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
            }
        }; break;
        case 3: { // BME280
            // Temperature
            if(type == 0 and validate.temp(sensors.get_bme280_temp(CORRECTED))) 
                data = sensors.get_bme280_temp(CORRECTED);
            // Humidity
            if(type == 1 and validate.hum(sensors.get_bme280_hum(CORRECTED)))
                data = sensors.get_bme280_hum(CORRECTED);
            // Pressure
            if(type == 2 and validate.pres(sensors.get_bme280_pres(CORRECTED)))
                data = sensors.get_bme280_pres(CORRECTED);
        }; break;
        case 4: { // BMP180
            // Temperature
            if(type == 0 and validate.temp(sensors.get_bmp180_temp(CORRECTED))) 
                data = sensors.get_bmp180_temp(CORRECTED);
            // Pressure
            if(type == 1 and validate.pres(sensors.get_bmp180_pres(CORRECTED)))
                data = sensors.get_bmp180_pres(CORRECTED);
        }; break;
        case 5: { // SHT21
            // Temperature
            if(type == 0 and validate.temp(sensors.get_sht21_temp(CORRECTED))) 
                data = sensors.get_sht21_temp(CORRECTED);
            // Humidity
            if(type == 1 and validate.hum(sensors.get_sht21_hum(CORRECTED)))
                data = sensors.get_sht21_hum(CORRECTED);
        }; break;
        case 6: { // DHT22
            // Temperature
            if(type == 0 and validate.temp(sensors.get_dht22_temp(CORRECTED)))
                data = sensors.get_dht22_temp(CORRECTED);
            // Humidity
            if(type == 1 and validate.hum(sensors.get_dht22_hum(CORRECTED)))
                data = sensors.get_dht22_hum(CORRECTED);
        }; break;
        case 7: // DS18B20
            // Temperature
            if(validate.temp(sensors.get_ds18b20_temp(CORRECTED)))
                data = sensors.get_ds18b20_temp(CORRECTED);
        break;
        case 8: // MAX44009
            // Ambient light
            if(validate.light(sensors.get_max44009_light(CORRECTED)))
                data = sensors.get_max44009_light(CORRECTED);
        break;
        case 9: // BH1750
            // Ambient light
            if(validate.light(sensors.get_bh1750_light(CORRECTED)))
                data = sensors.get_bh1750_light(CORRECTED);
        break;
        case 10: // Ananlog input
            // Voltage
            if(validate.volt(sensors.get_analog_voltage(CORRECTED)))
                data = sensors.get_analog_voltage(CORRECTED);
        break;
        case 11: { // ESP32
            // Temperature
            if(type == 0 and validate.temp(sensors.get_esp32_temp(CORRECTED)))
                data = sensors.get_esp32_temp(CORRECTED);
            // Runtime
            if(type == 1) 
                data = millis() / 1000;
        }; break;
        case 12: { // BME680
            // Temperature
            if(type == 0 and validate.temp(sensors.get_bme680_temp(CORRECTED)))
                data = sensors.get_bme680_temp(CORRECTED);
            // Humidity
            if(type == 1 and validate.hum(sensors.get_bme680_hum(CORRECTED)))
                data = sensors.get_bme680_hum(CORRECTED);
            // Pressure
            if(type == 2 and validate.pres(sensors.get_bme680_pres(CORRECTED)))
                data = sensors.get_bme680_pres(CORRECTED);
            // IAQ
            if(type == 3 and validate.iaq(sensors.get_bme680_iaq(CORRECTED)))
                data = sensors.get_bme680_iaq(CORRECTED);
        }; break;
        default: ; break; 
    }

    return data;
}