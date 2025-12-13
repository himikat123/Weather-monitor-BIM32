class WirelessSensor {
    #define WSENSORS 2

    public:
        void handleReceive();
        void receive();
        bool dataRelevance(uint8_t wsensNum);
        time_t get_updated(unsigned int num);
        float get_temperature(unsigned int num, unsigned int sensor);
        float get_humidity(unsigned int num);
        float get_pressure(unsigned int num);
        float get_windSpeed(unsigned int num);
        int get_windDir(unsigned int num);
        float get_light(unsigned int num);
        float get_voltage(unsigned int num);
        float get_current(unsigned int num);
        float get_power(unsigned int num);
        float get_energy(unsigned int num);
        float get_frequency(unsigned int num);
        float get_co2(unsigned int num);
        int get_batteryAdc(unsigned int num);
        float get_batteryVoltage(unsigned int num);
        int get_batteryLevel(unsigned int num);
        float get_batteryPercentage(unsigned int num);
        char* get_rawData();

    private:
        uint16_t _receivedIndex = 0;
        char _receivedData[1024];
};

void WirelessSensor::handleReceive() {
    while(Serial2.available()) {
        char c = Serial2.read();
        _receivedData[_receivedIndex] = c;
        if(_receivedIndex < 1023) _receivedIndex++;
    }
}

void WirelessSensor::receive() {
    String wsensorStr = "";
    char lastChar = _receivedData[_receivedIndex];
    char last_char = _receivedData[_receivedIndex - 1];
    if(lastChar == '\n' or lastChar == '}' or last_char == '\n' or last_char == '}') {
        _receivedIndex = 0;
        Serial.println(SEPARATOR);
        Serial.println("Wireless sensor receive... ");

        String received = String(_receivedData);
        int index = received.indexOf('\n');
        wsensorStr = received.substring(0, index);
        Serial.println(wsensorStr);
        for(uint8_t i=0; i<3; i++) {
            int startJson = wsensorStr.indexOf("{");
            int endJson = wsensorStr.indexOf("}");
            if(startJson != -1 and endJson != -1) {
                JsonDocument root;
                String js = wsensorStr.substring(startJson, endJson + 1);
                wsensorStr = wsensorStr.substring(endJson + 1);
                DeserializationError error = deserializeJson(root, js);
                if(error) {
                    Serial.println("Wireless sensor deserialization error");
                    return;
                }
                int number = root["num"];
                if(number >= 0 and number < WSENSORS) {
                    state.wsensor.time[number] = now();
                    // temperature
                    state.wsensor.temp.data[0][number] = root["t"] | 40400.0;
                    strlcpy(state.wsensor.temp.name[0][number], root["s"] | "", sizeof(state.wsensor.temp.name[0][number]));
                    for(int i=1; i<5; i++) {
                        state.wsensor.temp.data[i][number] = root["ds"][i - 1] | 40400.0;
                        strlcpy(state.wsensor.temp.name[i][number], "DS18B20", sizeof(state.wsensor.temp.name[i][number]));
                    }
                    // humidity
                    state.wsensor.hum.data[number] = root["h"] | 40400.0;
                    strlcpy(state.wsensor.hum.name[number], root["s"] | "", sizeof(state.wsensor.hum.name[number]));
                    // pressure
                    state.wsensor.pres.data[number] = root["p"] | 40400.0;
                    strlcpy(state.wsensor.pres.name[number], root["s"] | "", sizeof(state.wsensor.pres.name[number]));
                    // wind
                    state.wsensor.wind.speed.data[number] = root["wind"][0] | -1;
                    strlcpy(state.wsensor.wind.speed.name[number], "RS485", sizeof(state.wsensor.wind.speed.name[number]));
                    state.wsensor.wind.dir.data[number] = root["wind"][1] | -1;
                    strlcpy(state.wsensor.wind.dir.name[number], "RS485", sizeof(state.wsensor.wind.dir.name[number]));
                    // ambient light
                    state.wsensor.light.data[number] = root["l"] | -1.0;
                    strlcpy(state.wsensor.light.name[number], root["a"] | "", sizeof(state.wsensor.light.name[number]));
                    // PZEM-004t
                    state.wsensor.voltage.data[number] = root["pzem"][0] | -1.0;
                    strlcpy(state.wsensor.voltage.name[number], "PZEM-004t", sizeof(state.wsensor.voltage.name[number]));
                    state.wsensor.current.data[number] = root["pzem"][1] | -1.0;
                    strlcpy(state.wsensor.current.name[number], "PZEM-004t", sizeof(state.wsensor.current.name[number]));
                    state.wsensor.power.data[number] = root["pzem"][2] | -1.0;
                    strlcpy(state.wsensor.power.name[number], "PZEM-004t", sizeof(state.wsensor.power.name[number]));
                    state.wsensor.energy.data[number] = root["pzem"][3] | -1.0;
                    strlcpy(state.wsensor.energy.name[number], "PZEM-004t", sizeof(state.wsensor.energy.name[number]));
                    state.wsensor.freq.data[number] = root["pzem"][4] | -1.0;
                    strlcpy(state.wsensor.freq.name[number], "PZEM-004t", sizeof(state.wsensor.freq.name[number]));
                    // Senseair S8
                    state.wsensor.co2.data[number] = root["s8"] | -1.0;
                    strlcpy(state.wsensor.co2.name[number], "Senseair S8", sizeof(state.wsensor.co2.name[number]));
                    // Battery
                    state.wsensor.battery.adc[number] = root["b"] | -1;
                    state.wsensor.battery.voltage[number] = (float)state.wsensor.battery.adc[number] / (300.0 - config.wsensor.batK(number));
                    if(state.wsensor.battery.voltage[number] > 0.0) {
                        float umin = 3.75;
                        float umax = 3.9;
                        if(config.wsensor.batType(number) == 0) umax = 4.5;
                        float stp = (umax - umin) / 4;
                        if(state.wsensor.battery.voltage[number] < (umin + stp)) state.wsensor.battery.level[number] = 1;
                        else if(state.wsensor.battery.voltage[number] < (umin + stp * 2)) state.wsensor.battery.level[number] = 2;
                        else if(state.wsensor.battery.voltage[number] < (umin + stp * 3)) state.wsensor.battery.level[number] = 3;
                        else state.wsensor.battery.level[number] = 4;
                        state.wsensor.battery.percentage[number] = (state.wsensor.battery.voltage[number] - umin) * 100.0 / (umax - umin); 
                        if(state.wsensor.battery.percentage[number] > 100.0) state.wsensor.battery.percentage[number] = 100.0;
                    }

                    Serial.printf("Sensor %d updated", number);
                    time_t updTime = state.wsensor.time[number];
                    if(updTime > 0) Serial.printf(" at: %d:%02d:%02d\r\n", hour(updTime), minute(updTime), second(updTime));
                    else Serial.println(": never");
                }
                state.wsensor.updated = true;
            }

            int rc = wsensorStr.indexOf("OK+RC");
            if(rc != -1) {
                unsigned int ch = wsensorStr.substring(rc + 5, rc + 8).toInt();
                if(ch != config.wsensor.channel()) {
                    Serial.println("Changing channel number");
                    if(state.uart2_tx != HC12) {
                        Serial2.setPins(HC12_RX_PIN, HC12_TX_PIN);
                        state.uart2_tx = HC12;
                    }
                    digitalWrite(HC12_SET_PIN, LOW);
                    delay(50);
                    Serial2.printf("AT+C%03d\r\n", config.wsensor.channel());
                    Serial2.flush();
                    vTaskDelay(100);
                    digitalWrite(HC12_SET_PIN, HIGH);
                }
            }
        }
    }
  
    vTaskDelay(1);
    wsensorStr = "";
}

/**
 * check if data is not expired
 */
bool WirelessSensor::dataRelevance(uint8_t wsensNum) {
    return (now() - state.wsensor.time[wsensNum]) < (config.wsensor.expire(wsensNum) * 60);
}

/*
 * Getters
 */
 
time_t WirelessSensor::get_updated(unsigned int num) {
    if(num >= WSENSORS) return -1;
    return state.wsensor.time[num];
}

float WirelessSensor::get_temperature(unsigned int num, unsigned int sensor) {
    if(num >= WSENSORS or sensor > 4) return 40400.0;
    return state.wsensor.temp.data[sensor][num] + config.wsensor.tempCorr(num, sensor);
}

float WirelessSensor::get_humidity(unsigned int num) {
    if(num >= WSENSORS) return 40400.0;
    return state.wsensor.hum.data[num] + config.wsensor.humCorr(num);
}

float WirelessSensor::get_pressure(unsigned int num) {
    if(num >= WSENSORS) return 40400.0;
    return state.wsensor.pres.data[num] + config.wsensor.presCorr(num);
}

float WirelessSensor::get_windSpeed(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.wind.speed.data[num] + config.wsensor.windSpeedCorr(num);
}

int WirelessSensor::get_windDir(unsigned int num) {
    if(num >= WSENSORS) return 40400.0;
    return state.wsensor.wind.dir.data[num] + config.wsensor.windDirCorr(num); 
}

float WirelessSensor::get_light(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.light.data[num] + config.wsensor.lightCorr(num);
}

float WirelessSensor::get_voltage(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.voltage.data[num] + config.wsensor.voltCorr(num);
}

float WirelessSensor::get_current(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.current.data[num] + config.wsensor.currCorr(num);
}

float WirelessSensor::get_power(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.power.data[num] + config.wsensor.powCorr(num);
}

float WirelessSensor::get_energy(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.energy.data[num] + config.wsensor.enrgCorr(num);
}

float WirelessSensor::get_frequency(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.freq.data[num] + config.wsensor.freqCorr(num);
}

float WirelessSensor::get_co2(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.co2.data[num] + config.wsensor.co2Corr(num);
}

int WirelessSensor::get_batteryAdc(unsigned int num) {
    if(num >= WSENSORS) return -1;
    return state.wsensor.battery.adc[num];
}

float WirelessSensor::get_batteryVoltage(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.battery.voltage[num];
}

int WirelessSensor::get_batteryLevel(unsigned int num) {
    if(num >= WSENSORS) return -1;
    return state.wsensor.battery.level[num];
}

float WirelessSensor::get_batteryPercentage(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.battery.percentage[num];
}

char* WirelessSensor::get_rawData() {
    return _receivedData;
}