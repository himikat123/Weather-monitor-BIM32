class Thingspeak {
    public:
        void receive();
        void send();
        void sendHistory();
        void receiveHistory();
        bool dataRelevance();
        float get_field(unsigned int num);
        int get_updated();
        float get_historyField(unsigned int sensor, unsigned int slot);
        unsigned int get_historyUpdated(unsigned int slot);
        boolean is_summertime();

    private:
        String _fieldPrepare(unsigned int field);
        String _historyFieldPrepare(unsigned int fieldNum);
        
        float _field[8] = {-40400.0, -40400.0, -40400.0, -40400.0, -40400.0, -40400.0, -40400.0, -40400.0};
        float _historyFields[7][24] = {
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
        };
        unsigned int _historyUpdated[24] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        int _updated = -1;
};

/**
 * Receive data from thingspeak
 */
void Thingspeak::receive() {
    if(config.thingspeakReceive_channelID() == "") {
        Serial.println("No Channel ID");
        return;
    }
    if(config.thingspeakReceive_rdkey() == "") {
        Serial.println("No Read API Key");
        return;
    }
    String url = "http://api.thingspeak.com/channels/" + config.thingspeakReceive_channelID();
    url += "/feeds.json?api_key=" + config.thingspeakReceive_rdkey() + "&results=1";
    String httpData = "";
    HTTPClient client;
    Serial.println(url);
    client.begin(url);
    int httpCode = client.GET();
    if(httpCode == HTTP_CODE_OK) {
        httpData = client.getString();
        Serial.println(httpData);
        JsonDocument root;
        DeserializationError error = deserializeJson(root, httpData);
        if(error) {
            Serial.println("Deserialization error");
            return;
        }
        String thing_tm  = root["feeds"][0]["created_at"];
        _field[0] = root["feeds"][0]["field1"];
        _field[1] = root["feeds"][0]["field2"];
        _field[2] = root["feeds"][0]["field3"];
        _field[3] = root["feeds"][0]["field4"];    
        _field[4] = root["feeds"][0]["field5"];
        _field[5] = root["feeds"][0]["field6"];
        _field[6] = root["feeds"][0]["field7"];
        _field[7] = root["feeds"][0]["field8"];
        TimeElements tmth;
        char buf[22];
        thing_tm.toCharArray(buf, 22);
        tmth.Year = atoi(strtok(buf, "-")) - 1970;
        tmth.Month = atoi(strtok(NULL, "-"));
        tmth.Day = atoi(strtok(NULL, "T"));
        tmth.Hour = atoi(strtok(NULL, ":"));
        tmth.Minute = atoi(strtok(NULL, ":"));
        tmth.Second = atoi(strtok(NULL, ":"));
        _updated = makeTime(tmth);
        _updated += config.clock_utc() * 3600;
        _updated += config.clock_dlst() ? is_summertime() ? 3600 : 0 : 0;
        Serial.printf("successfully updated at %02d:%02d:%02d\r\n", hour(), minute(), second());
    }
    else Serial.println("error, code: " + String(httpCode));
    client.end();
}

/**
 * Send data to thingspeak
 */
void Thingspeak::send() {
    if(config.thingspeakSend_wrkey() == "") {
        Serial.println("No Write API Key");
        return;
    }

    String url = "http://api.thingspeak.com/update?api_key=" + config.thingspeakSend_wrkey();
    for(unsigned int i=0; i<8; i++) {
        url += _fieldPrepare(i);
    }

    String httpData = "";
    HTTPClient client;
    Serial.println(url);
    client.begin(url);
    int httpCode = client.GET();
    if(httpCode == HTTP_CODE_OK) {
        httpData = client.getString();
        Serial.println(httpData);
        Serial.println("successfull");
    }
    else Serial.println("error, code: " + String(httpCode));
    client.end();
    httpData = "";
}

/**
 * Send data to weather history repository
 */
void Thingspeak::sendHistory() {
    if(config.history_wrkey() == "") {
        Serial.println("No Write API Key");
        return;
    }

    String url = "http://api.thingspeak.com/update?api_key=" + config.history_wrkey();
    for(unsigned int i=0; i<7; i++) {
        url += _historyFieldPrepare(i);
    }

    String httpData = "";
    HTTPClient client;
    Serial.println(url);
    client.begin(url);
    int httpCode = client.GET();
    if(httpCode == HTTP_CODE_OK) {
        httpData = client.getString();
        Serial.println(httpData);
        Serial.println("successfull");
    }
    else Serial.println("error, code: " + String(httpCode));
    client.end();
    httpData = "";
}

/**
 * Receive data from weather history repository
 */
void Thingspeak::receiveHistory() {
    if(config.history_channelID() == "") {
        Serial.println("No Channel ID");
        return;
    }
    if(config.history_rdkey() == "") {
        Serial.println("No Read API Key");
        return;
    }
    String url = "http://api.thingspeak.com/channels/" + config.history_channelID();
    url += "/feeds.json?api_key=" + config.history_rdkey() + "&results=24";
    String httpData = "";
    HTTPClient client;
    Serial.println(url);
    client.begin(url);
    int httpCode = client.GET();
    if(httpCode == HTTP_CODE_OK) {
        httpData = client.getString();
        Serial.println(httpData);
        JsonDocument root;
        DeserializationError error = deserializeJson(root, httpData);
        if(error) {
            Serial.println("Deserialization error");
            return;
        }
        String thing_tm  = root["feeds"][0]["created_at"];
        for(int i=0; i<24; i++) {
            _historyFields[0][i] = root["feeds"][i]["field1"].isNull() ? -99.0 : root["feeds"][i]["field1"];
            _historyFields[1][i] = root["feeds"][i]["field2"].isNull() ? -99.0 : root["feeds"][i]["field2"];
            _historyFields[2][i] = root["feeds"][i]["field3"].isNull() ? -99.0 : root["feeds"][i]["field3"];
            _historyFields[3][i] = root["feeds"][i]["field4"].isNull() ? -99.0 : root["feeds"][i]["field4"];
            _historyFields[4][i] = root["feeds"][i]["field5"].isNull() ? -99.0 : root["feeds"][i]["field5"];
            _historyFields[5][i] = root["feeds"][i]["field6"].isNull() ? -99.0 : root["feeds"][i]["field6"];
            _historyFields[6][i] = root["feeds"][i]["field7"].isNull() ? -99.0 : root["feeds"][i]["field7"];

            String thing_tm  = root["feeds"][i]["created_at"];
            TimeElements tmth;
            char buf[22];
            thing_tm.toCharArray(buf, 22);
            tmth.Year = atoi(strtok(buf, "-")) - 1970;
            tmth.Month = atoi(strtok(NULL, "-"));
            tmth.Day = atoi(strtok(NULL, "T"));
            tmth.Hour = atoi(strtok(NULL, ":"));
            tmth.Minute = atoi(strtok(NULL, ":"));
            tmth.Second = atoi(strtok(NULL, ":"));
            _historyUpdated[i] = makeTime(tmth);
            _historyUpdated[i] += config.clock_utc() * 3600;
            _historyUpdated[i] += config.clock_dlst() ? is_summertime() ? 3600 : 0 : 0;
        }
        Serial.println("successfull");
    }
    else Serial.println("error, code: " + String(httpCode));
    client.end();
}

/**
 * check if data is not expired
 */
bool Thingspeak::dataRelevance() {
    return (now() - _updated) < (config.thingspeakReceive_expire() * 60);
}

/**
 * Get data from a field
 * @param field number
 * @return field data or obviously erroneous value
 */
float Thingspeak::get_field(unsigned int num) {
    if(num > 7) return -40400.0;
    return _field[num];
}

/**
 * Get timestamp of last update
 * @return timestamp
 */
int Thingspeak::get_updated() {
    return _updated;
}

/**
 * Get data from a history repository field
 * @param sensor number
 * @param timeslot number
 * @return field data or obviously erroneous value
 */
float Thingspeak::get_historyField(unsigned int sensor, unsigned int slot) {
    if(sensor >= 7) return -99.0;
    if(slot >= 24) return -99.0;
    return _historyFields[sensor][slot];
}

/**
 * Get timestamp of history repository timeslot update
 * @return timestamp
 */
unsigned int Thingspeak::get_historyUpdated(unsigned int slot) {
    if(slot >= 24) return 0;
    return _historyUpdated[slot];
}

/**
 * Prepare data for the desired field according to the settings
 * @param field number
 * @return a string with field name and data
 */
String Thingspeak::_fieldPrepare(unsigned int fieldNum) {
    String fields = "";
    String field = "&field" + String(fieldNum + 1) + "=";
    switch(config.thingspeakSend_fields(fieldNum)) {
        case 0: ; break; // --
        case 1: { // Weather
            // Temperature
            if(config.thingspeakSend_types(fieldNum) == 0 and validate.temp(weather.get_currentTemp()))
                fields = field + String(weather.get_currentTemp());
             // Humidity
            if(config.thingspeakSend_types(fieldNum) == 1 and validate.hum(weather.get_currentHum()))
                fields = field + String(weather.get_currentHum());
            // Pressure
            if(config.thingspeakSend_types(fieldNum) == 2 and validate.pres(weather.get_currentPres())) 
                fields = field + String(weather.get_currentPres());
        }; break;
        case 2:{ // Wireless sensor
            unsigned int wsensNum = config.thingspeakSend_wsensors(fieldNum);
            unsigned int wsensType = config.thingspeakSend_wtypes(fieldNum);
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
            if(config.thingspeakSend_types(fieldNum) == 0 and validate.temp(sensors.get_bme280_temp(CORRECTED))) 
                fields = field + String(sensors.get_bme280_temp(CORRECTED));
            // Humidity
            if(config.thingspeakSend_types(fieldNum) == 1 and validate.hum(sensors.get_bme280_hum(CORRECTED)))
                fields = field + String(sensors.get_bme280_hum(CORRECTED));
            // Pressure
            if(config.thingspeakSend_types(fieldNum) == 2 and validate.pres(sensors.get_bme280_pres(CORRECTED)))
                fields = field + String(sensors.get_bme280_pres(CORRECTED));
        }; break;
        case 4: { // BMP180
            // Temperature
            if(config.thingspeakSend_types(fieldNum) == 0 and validate.temp(sensors.get_bmp180_temp(CORRECTED))) 
                fields = field + String(sensors.get_bmp180_temp(CORRECTED));
            // Pressure
            if(config.thingspeakSend_types(fieldNum) == 1 and validate.pres(sensors.get_bmp180_pres(CORRECTED)))
                fields = field + String(sensors.get_bmp180_pres(CORRECTED));
        }; break;
        case 5: { // SHT21
            // Temperature
            if(config.thingspeakSend_types(fieldNum) == 0 and validate.temp(sensors.get_sht21_temp(CORRECTED))) 
                fields = field + String(sensors.get_sht21_temp(CORRECTED));
            // Humidity
            if(config.thingspeakSend_types(fieldNum) == 1 and validate.hum(sensors.get_sht21_hum(CORRECTED)))
                fields = field + String(sensors.get_sht21_hum(CORRECTED));
        }; break;
        case 6: { // DHT22
            // Temperature
            if(config.thingspeakSend_types(fieldNum) == 0 and validate.temp(sensors.get_dht22_temp(CORRECTED)))
                fields = field + String(sensors.get_dht22_temp(CORRECTED));
            // Humidity
            if(config.thingspeakSend_types(fieldNum) == 1 and validate.hum(sensors.get_dht22_hum(CORRECTED)))
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
            // Temperature
            if(config.thingspeakSend_types(fieldNum) == 0 and validate.temp(sensors.get_esp32_temp(CORRECTED)))
                fields = field + String(sensors.get_esp32_temp(CORRECTED));
            // Runtime
            if(config.thingspeakSend_types(fieldNum) == 1) 
                fields = field + String(millis() / 1000);
        break;
        case 12: { // BME680
            // Temperature
            if(config.thingspeakSend_types(fieldNum) == 0 and validate.temp(sensors.get_bme680_temp(CORRECTED)))
                fields = field + String(sensors.get_bme680_temp(CORRECTED));
            // Humidity
            if(config.thingspeakSend_types(fieldNum) == 1 and validate.hum(sensors.get_bme680_hum(CORRECTED)))
                fields = field + String(sensors.get_bme680_hum(CORRECTED));
            // Pressure
            if(config.thingspeakSend_types(fieldNum) == 2 and validate.pres(sensors.get_bme680_pres(CORRECTED)))
                fields = field + String(sensors.get_bme680_pres(CORRECTED));
            // IAQ
            if(config.thingspeakSend_types(fieldNum) == 3 and validate.iaq(sensors.get_bme680_iaq(CORRECTED)))
                fields = field + String(sensors.get_bme680_iaq(CORRECTED));
        }; break;
        default: ; break; 
    }
    return fields;
}

/**
 * Prepare weather history data for the desired field according to the settings
 * @param field number
 * @return a string with field name and data
 */
String Thingspeak::_historyFieldPrepare(unsigned int fieldNum) {
    String fields = "";
    String field = "&field" + String(fieldNum + 1) + "=";

    if(config.history_fields(fieldNum) == 1 and (fieldNum == 0 or fieldNum == 3)) { // Weather Temperature
        if(validate.temp(weather.get_currentTemp()))
            fields = field + String(weather.get_currentTemp());
    }
    if(config.history_fields(fieldNum) == 1 and (fieldNum == 1 or fieldNum == 4)) { // Weather Humidity
        if(validate.hum(weather.get_currentHum()))
            fields = field + String(weather.get_currentHum());
    }
    if(config.history_fields(fieldNum) == 1 and fieldNum == 2) {                    // Weather Pressure
        if(validate.pres(weather.get_currentPres())) 
            fields = field + String(weather.get_currentPres());
    }
                                                                                    // Wireless sensor
    if((config.history_fields(fieldNum) == 2 and fieldNum <= 4) or (config.history_fields(fieldNum) == 1 and fieldNum == 6)) {
        unsigned int wsensNum = config.history_wSensors(fieldNum);
        unsigned int wsensType = config.history_wTypes(fieldNum);
        if(wsensor.dataRelevance(wsensNum)) {
            float temp = wsensor.get_temperature(wsensNum, wsensType, CORRECTED);
            float hum = wsensor.get_humidity(wsensNum, CORRECTED);
            float pres = wsensor.get_pressure(wsensNum, CORRECTED);
            float co2 = wsensor.get_co2(wsensNum, CORRECTED);
            if((fieldNum == 0 or fieldNum == 3) and validate.temp(temp)) fields = field + String(temp);
            if((fieldNum == 1 or fieldNum == 4) and validate.hum(hum)) fields = field + String(hum);
            if(fieldNum == 2 and validate.pres(pres)) fields = field + String(pres);
            if(fieldNum == 6 and validate.co2(co2)) fields = field + String(co2);
        }
    }

    if(config.history_fields(fieldNum) == 3) {                                      // Thingspeak
        float value = get_field(config.history_tFields(fieldNum));
        if(value >= -50.0 and value <= 1200.0) fields = field + String(value);
    }
    
    if(config.history_fields(fieldNum) == 4 and (fieldNum == 0 or fieldNum == 3)) { // BME280 Temperature
        if(validate.temp(sensors.get_bme280_temp(CORRECTED))) 
            fields = field + String(sensors.get_bme280_temp(CORRECTED));
    }
    if(config.history_fields(fieldNum) == 4 and (fieldNum == 1 or fieldNum == 4)) { // BME280 Humidity 
        if(validate.hum(sensors.get_bme280_hum(CORRECTED)))
            fields = field + String(sensors.get_bme280_hum(CORRECTED));
    }
    if(config.history_fields(fieldNum) == 4 and fieldNum == 2) {                    // BME280 Pressure
        if(validate.pres(sensors.get_bme280_pres(CORRECTED)))
            fields = field + String(sensors.get_bme280_pres(CORRECTED));
    }

    if(config.history_fields(fieldNum) == 5 and (fieldNum == 0 or fieldNum == 3)) { // BMP180 Temperature
        if(validate.temp(sensors.get_bmp180_temp(CORRECTED))) 
            fields = field + String(sensors.get_bmp180_temp(CORRECTED));
    }
    if(config.history_fields(fieldNum) == 5 and fieldNum == 2) {                    // BMP180 Pressure
        if(validate.pres(sensors.get_bmp180_pres(CORRECTED))) 
            fields = field + String(sensors.get_bmp180_pres(CORRECTED));
    }

    if(config.history_fields(fieldNum) == 5 and (fieldNum == 1 or fieldNum == 4)) { // SHT21 Humidity
        if(validate.hum(sensors.get_sht21_hum(CORRECTED)))
            fields = field + String(sensors.get_sht21_hum(CORRECTED));
    }

    if(config.history_fields(fieldNum) == 6 and (fieldNum == 0 or fieldNum == 3)) { // SHT21 Temperature
        if(validate.temp(sensors.get_sht21_temp(CORRECTED))) 
            fields = field + String(sensors.get_sht21_temp(CORRECTED));
    }

    if(config.history_fields(fieldNum) == 6 and (fieldNum == 1 or fieldNum == 4)) { // DHT22 Humidity
        if(validate.hum(sensors.get_dht22_hum(CORRECTED))) 
            fields = field + String(sensors.get_dht22_hum(CORRECTED));
    }

    if(config.history_fields(fieldNum) == 7 and (fieldNum == 0 or fieldNum == 3)) { // DHT22 Temperature
        if(validate.temp(sensors.get_dht22_temp(CORRECTED))) 
            fields = field + String(sensors.get_dht22_temp(CORRECTED));
    }
  
    if(config.history_fields(fieldNum) == 8 and (fieldNum == 0 or fieldNum == 3)) { // DS18B20 Temperature
        if(validate.temp(sensors.get_ds18b20_temp(CORRECTED)))
            fields = field + String(sensors.get_ds18b20_temp(CORRECTED));
    }

    if(config.history_fields(fieldNum) == 9 and (fieldNum == 0 or fieldNum == 3)) { // BME680 Temperature
        if(validate.temp(sensors.get_bme680_temp(CORRECTED))) 
            fields = field + String(sensors.get_bme680_temp(CORRECTED));
    }
    if(config.history_fields(fieldNum) == 7 and (fieldNum == 1 or fieldNum == 4)) { // BME680 Humidity 
        if(validate.hum(sensors.get_bme680_hum(CORRECTED)))
            fields = field + String(sensors.get_bme680_hum(CORRECTED));
    }
    if(config.history_fields(fieldNum) == 6 and fieldNum == 2) {                    // BME680 Pressure
        if(validate.pres(sensors.get_bme680_pres(CORRECTED)))
            fields = field + String(sensors.get_bme680_pres(CORRECTED));
    }
    if(config.history_fields(fieldNum) == 1 and fieldNum == 5) {                    // BME680 IAQ
        if(validate.iaq(sensors.get_bme680_iaq(CORRECTED))) 
            fields = field + String(sensors.get_bme680_iaq(CORRECTED));
    }
 
    return fields;
}

/**
 * Check the time and date for daylight saving time
 * (works with an error, changes the time at 1:00)
 */
boolean Thingspeak::is_summertime(void) {
    if(month() < 3 || month() > 10) return false;
    if(month() > 3 && month() < 10) return true;
    if((month() == 3 && (hour() + 24 * day()) >= (1 + 24 * (31 - (5 * year() / 4 + 4) % 7))) || (month() == 10 && (hour() + 24 * day()) < (1 + 24 * (31 - (5 * year() / 4 + 1) % 7)))) return true;
    else return false;
}