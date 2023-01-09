class WirelessSensor {
  #define WSENSORS 2

  public:
    void receive(void);
    bool checkVolt(float volt);
    bool checkCurr(float curr);
    bool checkPwr(float pwr);
    bool checkEnrg(float enrg);
    bool checkFrq(float frq);
    bool checkBatVolt(float volt);
    bool checkBatLvl(int lvl);
    bool checkBatPercent(int percent);
    unsigned int get_updated(unsigned int num);
    float get_temperature(unsigned int num, unsigned int sensor, float corr);
    String get_sensorType(unsigned int num, unsigned int sensor);
    float get_humidity(unsigned int num, float corr);
    float get_pressure(unsigned int num, float corr);
    float get_light(unsigned int num, float corr);
    String get_lightType(unsigned int num);
    float get_voltage(unsigned int num, float corr);
    float get_current(unsigned int num, float corr);
    float get_power(unsigned int num, float corr);
    float get_energy(unsigned int num, float corr);
    float get_frequency(unsigned int num, float corr);
    String get_energyType(unsigned int num);
    int get_batteryAdc(unsigned int num);
    float get_batteryVoltage(unsigned int num);
    int get_batteryLevel(unsigned int num);
    int get_batteryPercentage(unsigned int num);

  private:
    unsigned int _updated[WSENSORS] = {0, 0};        // Last update timestamp
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
    int _adc[WSENSORS] = {-1, -1};                   // Battery ADC raw data
    float _batteryVoltage[WSENSORS] = {-1.0, -1.0};  // Battery voltage
    int _batteryLevel[WSENSORS] = {-1, -1};          // Battery level
    int _batteryPercentage[WSENSORS] = {-1, -1};     // Battery percentage
};

void WirelessSensor::receive() {
  String wsensorStr = "";
  while(Serial2.available()) {
    Serial.println(SEPARATOR);
    Serial.println("Wireless sensor receive... ");
    
    wsensorStr = Serial2.readStringUntil('\n');
    Serial.println(wsensorStr);
    if(wsensorStr.lastIndexOf("{") != -1) {
      StaticJsonDocument<2048> root;
      DeserializationError error = deserializeJson(root, wsensorStr);
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
          _batteryPercentage[number] = (_batteryVoltage[number] - umin) * 100 / (umax - umin); 
          if(_batteryPercentage[number] > 100) _batteryPercentage[number] = 100;
        }
            
        Serial.printf("Sensor %d updated at: ", number);
        Serial.printf("%d:%02d:%02d\r\n", hour(_updated[number]), minute(_updated[number]), second(_updated[number]));
      }
    }

    if(wsensorStr.lastIndexOf("OK+RC") != -1) {
      unsigned int ch = wsensorStr.substring(5).toInt();
      if(ch != config.wsensor_channel()) {
        Serial.println("Change channel number");
        digitalWrite(SET_HC12_PIN, LOW);
        delay(50);
        Serial2.printf("AT+C%03d\r\n", config.wsensor_channel());
        delay(100);
        digitalWrite(SET_HC12_PIN, HIGH);
      }
    }
  }
  
  vTaskDelay(1);
  wsensorStr = "";
}

/**
 * Check if PZEM-004t voltage is within the normal range
 */
bool WirelessSensor::checkVolt(float volt) {
  return (volt >= 80.0 and volt <= 260.0);
}

/**
 * Check if PZEM-004t current is within the normal range
 */
bool WirelessSensor::checkCurr(float curr) {
  return (curr >= 0.0 and curr <= 100.0);
}

/**
 * Check if PZEM-004t power is within the normal range
 */
bool WirelessSensor::checkPwr(float pwr) {
  return (pwr >= 0.0 and pwr <= 23000.0);
}

/**
 * Check if PZEM-004t energy is within the normal range
 */
bool WirelessSensor::checkEnrg(float enrg) {
  return (enrg >= 0.0 and enrg <= 10000.0);
}

/**
 * Check if PZEM-004t frequency is within the normal range
 */
bool WirelessSensor::checkFrq(float frq) {
  return (frq >= 45.0 and frq <= 65.0);
}

/**
 * Check if battery voltage is within the normal range
 */
bool WirelessSensor::checkBatVolt(float volt) {
  return (volt >= 0.0 and volt <= 12.0);
}

/**
 * Check if battery level is within the normal range
 */
bool WirelessSensor::checkBatLvl(int lvl) {
  return (lvl >= 1 and lvl <= 4);
}

/**
 * Check if battery percentage is within the normal range
 */
bool WirelessSensor::checkBatPercent(int percent) {
  return (percent >= 0 and percent <= 100);
}
        
/*
 * Getters
 */
 
unsigned int WirelessSensor::get_updated(unsigned int num) {
  if(num >= WSENSORS) return 0;
  return _updated[num];
}

float WirelessSensor::get_temperature(unsigned int num, unsigned int sensor, float corr) {
  if(num >= WSENSORS or sensor > 4) return 40400.0;
  return _temperature[num][sensor] + corr;
}

String WirelessSensor::get_sensorType(unsigned int num, unsigned int sensor) {
  if(num >= WSENSORS or sensor > 4) return "";
  if(sensor == 0) return String(_sensorType[num]);
  else return "DS18B20";
}

float WirelessSensor::get_humidity(unsigned int num, float corr) {
  if(num >= WSENSORS) return 40400.0;
  return _humidity[num] + corr;
}

float WirelessSensor::get_pressure(unsigned int num, float corr) {
  if(num >= WSENSORS) return 40400.0;
  return _pressure[num] + corr;
}

float WirelessSensor::get_light(unsigned int num, float corr) {
  if(num >= WSENSORS) return -1.0;
  return _light[num] + corr;
}

String WirelessSensor::get_lightType(unsigned int num) {
  if(num >= WSENSORS) return "";
  return String(_lightType[num]);
}

float WirelessSensor::get_voltage(unsigned int num, float corr) {
  if(num >= WSENSORS) return -1.0;
  return _voltage[num] + corr;
}

float WirelessSensor::get_current(unsigned int num, float corr) {
  if(num >= WSENSORS) return -1.0;
  return _current[num] + corr;
}

float WirelessSensor::get_power(unsigned int num, float corr) {
  if(num >= WSENSORS) return -1.0;
  return _power[num] + corr;
}

float WirelessSensor::get_energy(unsigned int num, float corr) {
  if(num >= WSENSORS) return -1.0;
  return _energy[num] + corr;
}

float WirelessSensor::get_frequency(unsigned int num, float corr) {
  if(num >= WSENSORS) return -1.0;
  return _frequency[num] + corr;
}

String WirelessSensor::get_energyType(unsigned int num) {
  if(num >= WSENSORS) return "";
  return "PZEM-004t";
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

int WirelessSensor::get_batteryPercentage(unsigned int num) {
  if(num >= WSENSORS) return -1;
  return _batteryPercentage[num];
}
