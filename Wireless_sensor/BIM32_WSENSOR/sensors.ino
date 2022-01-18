/**
 * Sensors initialization
 */
void sensors_init(void){
  //BME280
  #ifdef USE_BME280
    if(bme280.begin(0x77, &Wire)) detected.bme280 = true;
    else if(bme280.begin(0x76, &Wire)) detected.bme280 = true;
  #endif

  //SHT21
  #ifdef USE_SHT21
    SHT21.begin();
    Wire.beginTransmission(SHT21_ADDRESS);
    Wire.write(0xE7);
    Wire.endTransmission();
    delay(50);
    Wire.requestFrom(SHT21_ADDRESS, 1);
    if(Wire.available() == 1){
      Wire.read();
      detected.sht21 = true;
    }
  #endif

  //DS18B20
  #ifdef USE_DS18B20
    ds18b20.setResolution(10);
    ds18b20.requestTemp();
  #endif
  
  //MAX44009
  #ifdef USE_MAX44009
    if(!max44009.begin()) detected.max44009 = true;
  #endif
}


/**
 * Get sensor data
 */
void get_datas(void){
  //BME280
  #ifdef USE_BME280
    if(detected.bme280){
      datas.temp = bme280.readTemperature();
      datas.hum = bme280.readHumidity();
      datas.pres = bme280.readPressure() / 100.0F;
    }
  #endif

  //SHT21
  #ifdef USE_SHT21
    if(detected.sht21){
      datas.temp = SHT21.getTemperature();
      datas.hum = SHT21.getHumidity();
    }
  #endif

  //DS18B20
  #ifdef USE_DS18B20
    delay(200);
    if(ds18b20.readTemp()) detected.ds18b20 = true;
    if(detected.ds18b20) datas.temp = ds18b20.getTemp();
  #endif
  
  //MAX44009
  #ifdef USE_MAX44009
    if(detected.max44009) datas.light = max44009.get_lux();
  #endif
    
  //Battery
  datas.adc = analogRead(BAT);
}
