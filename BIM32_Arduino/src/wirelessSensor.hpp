class WirelessSensor {
    #define WSENSORS 2

    public:
        void handleReceive();
        void receive();
        bool dataRelevance(uint8_t wsensNum);
        int get_updated(unsigned int num);
        String get_sensorType(unsigned int num, unsigned int sensor);
        String get_lightType(unsigned int num);
        String get_energyType();
        String get_co2Type();
        float get_temperature(unsigned int num, unsigned int sensor, bool corr);
        float get_humidity(unsigned int num, bool corr);
        float get_pressure(unsigned int num, bool corr);
        float get_light(unsigned int num, bool corr);
        float get_voltage(unsigned int num, bool corr);
        float get_current(unsigned int num, bool corr);
        float get_power(unsigned int num, bool corr);
        float get_energy(unsigned int num, bool corr);
        float get_frequency(unsigned int num, bool corr);
        float get_co2(unsigned int num, bool corr);
        int get_batteryAdc(unsigned int num);
        float get_batteryVoltage(unsigned int num);
        int get_batteryLevel(unsigned int num);
        float get_batteryPercentage(unsigned int num);

    private:
        uint16_t receivedIndex = 0;
        char receivedData[1024];
        int _updated[WSENSORS] = {-1, -1};               // Last update timestamp
        float _temperature[WSENSORS][5] = {              // Temperature
            {40400.0, 40400.0, 40400.0, 40400.0, 40400.0}, 
            {40400.0, 40400.0, 40400.0, 40400.0, 40400.0} 
        };
        float _humidity[WSENSORS] = {40400.0, 40400.0};  // Humidity
        float _pressure[WSENSORS] = {40400.0, 40400.0};  // Pressure
        char _sensorType[WSENSORS][20] = {"", ""};       // Temperature, Humidity, Pressure sensor type (name)
        float _light[WSENSORS] = {-1.0, -1.0};           // Ambient light
        char _lightType[WSENSORS][20] = {"", ""};        // Ambient light sensor type (name)
        float _voltage[WSENSORS] = {-1.0, -1.0};         // Voltage
        float _current[WSENSORS] = {-1.0, -1.0};         // Current
        float _power[WSENSORS] = {-1.0, -1.0};           // Power
        float _energy[WSENSORS] = {-1.0, -1.0};          // Energy
        float _frequency[WSENSORS] = {-1.0, -1.0};       // Frequency
        float _co2[WSENSORS] = {-1.0, -1.0};             // CO2
        int _adc[WSENSORS] = {-1, -1};                   // Battery ADC raw data
        float _batteryVoltage[WSENSORS] = {-1.0, -1.0};  // Battery voltage
        int _batteryLevel[WSENSORS] = {-1, -1};          // Battery level
        float _batteryPercentage[WSENSORS] = {-1, -1};   // Battery percentage
};

void WirelessSensor::handleReceive() {
    while(Serial2.available()) {
        char c = Serial2.read();
        receivedData[receivedIndex] = c;
        if(receivedIndex < 1023) receivedIndex++;
    }
}

void WirelessSensor::receive() {
    String wsensorStr = "";
    char lastChar = receivedData[receivedIndex];
    char last_char = receivedData[receivedIndex - 1];
    if(lastChar == '\n' or lastChar == '}' or last_char == '\n' or last_char == '}') {
        receivedIndex = 0;
        Serial.println(SEPARATOR);
        Serial.println("Wireless sensor receive... ");

        String received = String(receivedData);
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
                    _updated[number] = now();

                    strlcpy(_sensorType[number], root["s"] | "", sizeof(_sensorType[number]));
                    strlcpy(_lightType[number], root["a"] | "", sizeof(_lightType[number]));

                    _temperature[number][0] = root["t"] | 40400.0;
                    _temperature[number][1] = root["ds"][0] | 40400.0;
                    _temperature[number][2] = root["ds"][1] | 40400.0;
                    _temperature[number][3] = root["ds"][2] | 40400.0;
                    _temperature[number][4] = root["ds"][3] | 40400.0;

                    _humidity[number] = root["h"] | 40400.0;
                    _pressure[number] = root["p"] | 40400.0;

                    _light[number] = root["l"] | -1.0;

                    _voltage[number] = root["pzem"][0] | -1.0;
                    _current[number] = root["pzem"][1] | -1.0;
                    _power[number] = root["pzem"][2] | -1.0;
                    _energy[number] = root["pzem"][3] | -1.0;
                    _frequency[number] = root["pzem"][4] | -1.0;

                    _co2[number] = root["s8"] | -1.0;

                    _adc[number] = root["b"] | -1;
                    _batteryVoltage[number] = (float)_adc[number] / (300.0 - config.wsensor_bat_k(number));
                    if(_batteryVoltage[number] > 0.0) {
                        float umin = 3.75;
                        float umax = 3.9;
                        if(config.wsensor_bat_type(number) == 0) umax = 4.5;
                        float stp = (umax - umin) / 4;
                        if(_batteryVoltage[number] < (umin + stp)) _batteryLevel[number] = 1;
                        else if(_batteryVoltage[number] < (umin + stp * 2)) _batteryLevel[number] = 2;
                        else if(_batteryVoltage[number] < (umin + stp * 3)) _batteryLevel[number] = 3;
                        else _batteryLevel[number] = 4;
                        _batteryPercentage[number] = (_batteryVoltage[number] - umin) * 100.0 / (umax - umin); 
                        if(_batteryPercentage[number] > 100.0) _batteryPercentage[number] = 100.0;
                    }

                    Serial.printf("Sensor %d updated", number);
                    if(_updated > 0)
                        Serial.printf(
                            " at: %d:%02d:%02d\r\n", 
                            hour(_updated[number]), 
                            minute(_updated[number]), 
                            second(_updated[number])
                        );
                    else Serial.println(": never");
                }
            }

            int rc = wsensorStr.indexOf("OK+RC");
            if(rc != -1) {
                unsigned int ch = wsensorStr.substring(rc + 5, rc + 8).toInt();
                if(ch != config.wsensor_channel()) {
                    Serial.println("Changing channel number");
                    if(global.uart2_tx != HC12) {
                        Serial2.setPins(HC12_RX_PIN, HC12_TX_PIN);
                        global.uart2_tx = HC12;
                    }
                    digitalWrite(HC12_SET_PIN, LOW);
                    delay(50);
                    Serial2.printf("AT+C%03d\r\n", config.wsensor_channel());
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
    return (now() - _updated[wsensNum]) < (config.wsensor_expire(wsensNum) * 60);
}

/*
 * Getters
 */
 
int WirelessSensor::get_updated(unsigned int num) {
    if(num >= WSENSORS) return -1;
    return _updated[num];
}

String WirelessSensor::get_sensorType(unsigned int num, unsigned int sensor) {
    if(num >= WSENSORS or sensor > 4) return "";
    if(sensor == 0) return String(_sensorType[num]);
    else if(sensor >= 1 and sensor <= 4) return "DS18B20";
    else return "";
}

String WirelessSensor::get_lightType(unsigned int num) {
    if(num >= WSENSORS) return "";
    return String(_lightType[num]);
}

String WirelessSensor::get_energyType() {
    return "PZEM-004t";
}

String WirelessSensor::get_co2Type() {
    return "Senseair S8";
}

float WirelessSensor::get_temperature(unsigned int num, unsigned int sensor, bool corr) {
    if(num >= WSENSORS or sensor > 4) return 40400.0;
    return _temperature[num][sensor] + (corr ? config.wsensor_temp_corr(num, sensor) : 0.0);
}

float WirelessSensor::get_humidity(unsigned int num, bool corr) {
    if(num >= WSENSORS) return 40400.0;
    return _humidity[num] + (corr ? config.wsensor_hum_corr(num) : 0.0);
}

float WirelessSensor::get_pressure(unsigned int num, bool corr) {
    if(num >= WSENSORS) return 40400.0;
    return _pressure[num] + (corr ? config.wsensor_pres_corr(num) : 0.0);
}

float WirelessSensor::get_light(unsigned int num, bool corr) {
    if(num >= WSENSORS) return -1.0;
    return _light[num] + (corr ? config.wsensor_light_corr(num) : 0.0);
}

float WirelessSensor::get_voltage(unsigned int num, bool corr) {
    if(num >= WSENSORS) return -1.0;
    return _voltage[num] + (corr ? config.wsensor_volt_corr(num) : 0.0);
}

float WirelessSensor::get_current(unsigned int num, bool corr) {
    if(num >= WSENSORS) return -1.0;
    return _current[num] + (corr ? config.wsensor_curr_corr(num) : 0.0);
}

float WirelessSensor::get_power(unsigned int num, bool corr) {
    if(num >= WSENSORS) return -1.0;
    return _power[num] + (corr ? config.wsensor_pow_corr(num) : 0.0);
}

float WirelessSensor::get_energy(unsigned int num, bool corr) {
    if(num >= WSENSORS) return -1.0;
    return _energy[num] + (corr ? config.wsensor_enrg_corr(num) : 0.0);
}

float WirelessSensor::get_frequency(unsigned int num, bool corr) {
    if(num >= WSENSORS) return -1.0;
    return _frequency[num] + (corr ? config.wsensor_freq_corr(num) : 0.0);
}

float WirelessSensor::get_co2(unsigned int num, bool corr) {
    if(num >= WSENSORS) return -1.0;
    return _co2[num] + (corr ? config.wsensor_co2_corr(num) : 0.0);
}

int WirelessSensor::get_batteryAdc(unsigned int num) {
    if(num >= WSENSORS) return -1;
    return _adc[num];
}

float WirelessSensor::get_batteryVoltage(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return _batteryVoltage[num];
}

int WirelessSensor::get_batteryLevel(unsigned int num) {
    if(num >= WSENSORS) return -1;
    return _batteryLevel[num];
}

float WirelessSensor::get_batteryPercentage(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return _batteryPercentage[num];
}