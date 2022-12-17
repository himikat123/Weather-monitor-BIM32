class Narodmon {
  public: void send();
  private: String _fieldsPrepare(unsigned int fieldNum, String metrics, String mac);
};

/**
 * Send data to norodmon
 */
void Narodmon::send() {
  Serial.println(SEPARATOR);
  Serial.println("Send data to narodmon.ru... ");
  
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
  if(!client.connect("narodmon.ru", 8283)) Serial.println("failed");
  else {
    client.print(buf);
    Serial.println("Done");
  }
  while(client.available()) {
    String line = client.readStringUntil('\r');
    Serial.printf("Successfull. Server returned %s\r\n", line);
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
  String field = "\n#" + metrics + mac + "#";
  switch(config.narodmonSend_sensors(fieldNum)) {
    case 0: ; break; // --
    
    case 1: { // Weather
      // Temperature
      if(config.narodmonSend_types(fieldNum) == 0 and sensors.checkTemp(weather.get_currentTemp()))
        fields = field + String(weather.get_currentTemp());
      // Humidity
      if(config.narodmonSend_types(fieldNum) == 1 and sensors.checkHum(weather.get_currentHum()))
        fields = field + String(weather.get_currentHum());
      // Pressure
      if(config.narodmonSend_types(fieldNum) == 2 and sensors.checkPres(weather.get_currentPres())) 
        fields = field + String(weather.get_currentPres());
    }; break;
    
    case 2: { // BME280
      // Temperature
      if(config.narodmonSend_types(fieldNum) == 0 and 
        sensors.checkTemp(sensors.get_bme280_temp(config.bme280_temp_corr()))) 
          fields = field + String(sensors.get_bme280_temp(config.bme280_temp_corr()));
      // Humidity
      if(config.narodmonSend_types(fieldNum) == 1 and 
        sensors.checkHum(sensors.get_bme280_hum(config.bme280_hum_corr())))
          fields = field + String(sensors.get_bme280_hum(config.bme280_hum_corr()));
      // Pressure
      if(config.narodmonSend_types(fieldNum) == 2 and
        sensors.checkPres(sensors.get_bme280_pres(config.bme280_pres_corr())))
          fields = field + String(sensors.get_bme280_pres(config.bme280_pres_corr()));
    }; break;
    
    case 3: { // BMP180
      // Temperature
      if(config.narodmonSend_types(fieldNum) == 0 and
        sensors.checkTemp(sensors.get_bmp180_temp(config.bmp180_temp_corr()))) 
          fields = field + String(sensors.get_bmp180_temp(config.bmp180_temp_corr()));
      // Pressure
      if(config.narodmonSend_types(fieldNum) == 1 and
        sensors.checkPres(sensors.get_bmp180_pres(config.bmp180_pres_corr()))) 
          fields = field + String(sensors.get_bmp180_pres(config.bmp180_pres_corr()));
    }; break;
    
    case 4: { // SHT21
      // Temperature
      if(config.narodmonSend_types(fieldNum) == 0 and
        sensors.checkTemp(sensors.get_sht21_temp(config.sht21_temp_corr()))) 
          fields = field + String(sensors.get_sht21_temp(config.sht21_temp_corr()));
      // Humidity
      if(config.narodmonSend_types(fieldNum) == 1 and
        sensors.checkHum(sensors.get_sht21_hum(config.sht21_hum_corr())))
          fields = field + String(sensors.get_sht21_hum(config.sht21_hum_corr()));
    }; break;
    
    case 5: { // DHT22
      // Temperature
      if(config.narodmonSend_types(fieldNum) == 0 and
        sensors.checkTemp(sensors.get_dht22_temp(config.dht22_temp_corr()))) 
          fields = field + String(sensors.get_dht22_temp(config.dht22_temp_corr()));
      // Humidity
      if(config.narodmonSend_types(fieldNum) == 1 and
        sensors.checkHum(sensors.get_dht22_hum(config.dht22_hum_corr()))) 
          fields = field + String(sensors.get_dht22_hum(config.dht22_hum_corr()));
    }; break;
    
    case 6: // DS18B20
      // Temperature
      if(sensors.checkTemp(sensors.get_ds18b20_temp(config.ds18b20_temp_corr())))
          fields = field + String(sensors.get_ds18b20_temp(config.ds18b20_temp_corr()));
      break;
    
    case 7: // MAX44009
      // Ambient light
      if(sensors.checkLight(sensors.get_max44009_light(config.max44009_light_corr())))
          fields = field + String(sensors.get_max44009_light(config.max44009_light_corr()));
      break;

    case 8: // BH1750
      // Ambient light
      if(sensors.checkLight(sensors.get_bh1750_light(config.bh1750_light_corr())))
          fields = field + String(sensors.get_bh1750_light(config.bh1750_light_corr()));
      break;

    case 9: // Ananlog input
      // Voltage
      if(sensors.checkVolt(sensors.get_analog_voltage(config.analog_voltage_corr())))
          fields = field + String(sensors.get_analog_voltage(config.analog_voltage_corr()));
      break;

    case 10: // ESP32
      if(config.narodmonSend_types(fieldNum) == 0 and
        sensors.checkTemp(sensors.get_esp32_temp(config.esp32_temp_corr())))
          fields = field + String(sensors.get_esp32_temp(config.esp32_temp_corr()));
      if(config.narodmonSend_types(fieldNum) == 1) 
        fields = field + String(millis() / 1000);
      break;

    case 11:{ // Wireless sensor
      unsigned int wsensNum = config.narodmonSend_wsensors(fieldNum);
      unsigned int wsensType = config.narodmonSend_wtypes(fieldNum);
      if((now() - wsensor.get_updated(wsensNum)) < (config.wsensor_expire(wsensNum) * 60)) {
        float temp = wsensor.get_temperature(wsensNum, wsensType, config.wsensor_temp_corr(wsensNum, wsensType));
        float hum = wsensor.get_humidity(wsensNum, config.wsensor_hum_corr(wsensNum));
        float pres = wsensor.get_pressure(wsensNum, config.wsensor_pres_corr(wsensNum));
        float light = wsensor.get_light(wsensNum, config.wsensor_light_corr(wsensNum));
        float volt = wsensor.get_voltage(wsensNum, config.wsensor_volt_corr(wsensNum));
        float cur = wsensor.get_current(wsensNum, config.wsensor_curr_corr(wsensNum));
        float pwr = wsensor.get_power(wsensNum, config.wsensor_pow_corr(wsensNum));
        float enrg = wsensor.get_energy(wsensNum, config.wsensor_enrg_corr(wsensNum));
        float freq = wsensor.get_frequency(wsensNum, config.wsensor_freq_corr(wsensNum));
        float vbat = wsensor.get_batteryVoltage(wsensNum);
        unsigned int batlvl = wsensor.get_batteryLevel(wsensNum);
        if(wsensType >= 0 and wsensType <= 4 and sensors.checkTemp(temp)) fields = field + String(temp);
        if(wsensType == 5 and sensors.checkHum(hum)) fields = field + String(hum);
        if(wsensType == 6 and sensors.checkPres(pres)) fields = field + String(pres);
        if(wsensType == 7 and sensors.checkLight(light)) fields = field + String(light);
        if(wsensType == 8 and wsensor.checkVolt(volt)) fields = field + String(volt);
        if(wsensType == 9 and wsensor.checkCurr(cur)) fields = field + String(cur);
        if(wsensType == 10 and wsensor.checkPwr(pwr)) fields = field + String(pwr);
        if(wsensType == 11 and wsensor.checkEnrg(enrg)) fields = field + String(enrg);
        if(wsensType == 12 and wsensor.checkFrq(freq)) fields = field + String(freq);
        if(wsensType == 13 and wsensor.checkBatVolt(vbat)) fields = field + String(vbat);
        if(wsensType == 14 and wsensor.checkBatLvl(batlvl)) fields = field + String(batlvl);
      }
    }; break;
    
    default: ; break; 
  }
  return fields;
}
