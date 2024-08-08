class Narodmon {
    public: void send();
    private: String _fieldsPrepare(unsigned int fieldNum, String metrics, String mac);
};

/**
 * Send data to norodmon
 */
void Narodmon::send() {
    if(config.narodmonSend_lat() == "" or config.narodmonSend_lon() == "") {
        Serial.println("No coordinates");
        return;
    }

    String mac = WiFi.macAddress();
    mac.replace(":", "");
    String buf = "#BIM" + mac;
    buf += "#" + config.narodmonSend_name();
    buf += "#" + config.narodmonSend_lat();
    buf += "#" + config.narodmonSend_lon();
    for(unsigned int i=0; i<12; i++) {
        buf += _fieldsPrepare(i, config.narodmonSend_metrics(i), mac); 
    }
    buf += "\n##\r\n";
    if(!client.connect("narodmon.ru", 8283, 3000)) Serial.println("failed");
    else {
        //Serial.println(buf);
        client.print(buf);
        Serial.println("Done");
    }
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.println("Successfull. Server returned" + line);
    }
    client.stop();
}

/**
 * Prepare data for the desired field according to the settings
 * @param field number
 * @return a string with field name and data
 */
String Narodmon::_fieldsPrepare(unsigned int fieldNum, String metrics, String mac) {
    String fields = "";
    String field = "\n#" + metrics + "_" + mac + "#";
    switch(config.narodmonSend_sensors(fieldNum)) {
        case 0: ; break; // --

        case 1: { // Weather
            // Temperature
            if(config.narodmonSend_types(fieldNum) == 0 and validate.temp(weather.get_currentTemp()))
                fields = field + String(weather.get_currentTemp());
            // Humidity
            if(config.narodmonSend_types(fieldNum) == 1 and validate.hum(weather.get_currentHum()))
                fields = field + String(weather.get_currentHum());
            // Pressure
            if(config.narodmonSend_types(fieldNum) == 2 and validate.pres(weather.get_currentPres())) 
                fields = field + String(weather.get_currentPres());
        }; break;

        case 2:{ // Wireless sensor
            unsigned int wsensNum = config.narodmonSend_wsensors(fieldNum);
            unsigned int wsensType = config.narodmonSend_wtypes(fieldNum);
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
                if(wsensType <= 4 and validate.temp(temp)) fields = field + String(temp);
                if(wsensType == 5 and validate.hum(hum)) fields = field + String(hum);
                if(wsensType == 6 and validate.pres(pres)) fields = field + String(pres);
                if(wsensType == 7 and validate.light(light)) fields = field + String(light);
                if(wsensType == 8 and validate.hVolt(volt)) fields = field + String(volt);
                if(wsensType == 9 and validate.current(cur)) fields = field + String(cur);
                if(wsensType == 10 and validate.power(pwr)) fields = field + String(pwr);
                if(wsensType == 11 and validate.energy(enrg)) fields = field + String(enrg);
                if(wsensType == 12 and validate.frequence(freq)) fields = field + String(freq);
                if(wsensType == 13 and validate.batVolt(vbat)) fields = field + String(vbat);
                if(wsensType == 14 and validate.batPercent(batprc)) fields = field + String(batprc);
                if(wsensType == 15 and validate.batLvl(batlvl)) fields = field + String(batlvl);
                if(wsensType == 16 and validate.co2(co2)) fields = field + String(co2);
            }
        }; break;

        case 3: { // BME280
            // Temperature
            if(config.narodmonSend_types(fieldNum) == 0 and validate.temp(sensors.get_bme280_temp(CORRECTED))) 
                fields = field + String(sensors.get_bme280_temp(CORRECTED));
            // Humidity
            if(config.narodmonSend_types(fieldNum) == 1 and validate.hum(sensors.get_bme280_hum(CORRECTED)))
                fields = field + String(sensors.get_bme280_hum(CORRECTED));
            // Pressure
            if(config.narodmonSend_types(fieldNum) == 2 and validate.pres(sensors.get_bme280_pres(CORRECTED)))
                fields = field + String(sensors.get_bme280_pres(CORRECTED));
        }; break;
    
        case 4: { // BMP180
            // Temperature
            if(config.narodmonSend_types(fieldNum) == 0 and validate.temp(sensors.get_bmp180_temp(CORRECTED))) 
                fields = field + String(sensors.get_bmp180_temp(CORRECTED));
            // Pressure
            if(config.narodmonSend_types(fieldNum) == 1 and validate.pres(sensors.get_bmp180_pres(CORRECTED))) 
                fields = field + String(sensors.get_bmp180_pres(CORRECTED));
        }; break;

        case 5: { // SHT21
            // Temperature
            if(config.narodmonSend_types(fieldNum) == 0 and validate.temp(sensors.get_sht21_temp(CORRECTED))) 
                fields = field + String(sensors.get_sht21_temp(CORRECTED));
            // Humidity
            if(config.narodmonSend_types(fieldNum) == 1 and validate.hum(sensors.get_sht21_hum(CORRECTED)))
                fields = field + String(sensors.get_sht21_hum(CORRECTED));
        }; break;

        case 6: { // DHT22
            // Temperature
            if(config.narodmonSend_types(fieldNum) == 0 and validate.temp(sensors.get_dht22_temp(CORRECTED))) 
                fields = field + String(sensors.get_dht22_temp(CORRECTED));
            // Humidity
            if(config.narodmonSend_types(fieldNum) == 1 and validate.hum(sensors.get_dht22_hum(CORRECTED))) 
                fields = field + String(sensors.get_dht22_hum(CORRECTED));
        }; break;

        case 7: // DS18B20
            // Temperature
            if(validate.temp(sensors.get_ds18b20_temp(CORRECTED)))
                fields = field + String(sensors.get_ds18b20_temp(CORRECTED));
        break;

        case 8: // MAX44009
            // Ambient light
            if(validate.light(sensors.get_max44009_light(CORRECTED)))
                fields = field + String(sensors.get_max44009_light(CORRECTED));
        break;

        case 9: // BH1750
            // Ambient light
            if(validate.light(sensors.get_bh1750_light(CORRECTED)))
                fields = field + String(sensors.get_bh1750_light(CORRECTED));
        break;

        case 10: // Ananlog input
            // Voltage
            if(validate.volt(sensors.get_analog_voltage(CORRECTED)))
                fields = field + String(sensors.get_analog_voltage(CORRECTED));
        break;

        case 11: // ESP32
            if(config.narodmonSend_types(fieldNum) == 0 and validate.temp(sensors.get_esp32_temp(CORRECTED)))
                fields = field + String(sensors.get_esp32_temp(CORRECTED));
            if(config.narodmonSend_types(fieldNum) == 1) 
                fields = field + String(millis() / 1000);
        break;

        case 12: { // BME680
            // Temperature
            if(config.narodmonSend_types(fieldNum) == 0 and validate.temp(sensors.get_bme680_temp(CORRECTED))) 
                fields = field + String(sensors.get_bme680_temp(CORRECTED));
            // Humidity
            if(config.narodmonSend_types(fieldNum) == 1 and validate.hum(sensors.get_bme680_hum(CORRECTED)))
                fields = field + String(sensors.get_bme680_hum(CORRECTED));
            // Pressure
            if(config.narodmonSend_types(fieldNum) == 2 and validate.pres(sensors.get_bme680_pres(CORRECTED)))
                fields = field + String(sensors.get_bme680_pres(CORRECTED));
            // IAQ
            if(config.narodmonSend_types(fieldNum) == 3 and validate.iaq(sensors.get_bme680_iaq(CORRECTED)))
                fields = field + String(sensors.get_bme680_iaq(CORRECTED));
        }; break;

        default: ; break;
    }

    return fields;
}