/**
 * Sensors initialization
 */
void sensorsInit(void){
  
  //BME280 initialization
  #ifdef USE_BME280
    if(bme280.begin(0x77, &Wire)) detected.bme280 = true;
    else if(bme280.begin(0x76, &Wire)) detected.bme280 = true;
  #endif

  #ifndef USE_BME280
  
    //SHT21 initialization
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
    
  #endif

  uint8_t ds_address[8];
  //DS18B20 initialization
  #ifdef USE_DS18B20_1
    if(ds18b20_1.readAddress(ds_address)){
      detected.ds18b20_1 = true;
      ds18b20_1.setResolution(DS18B20_RESOLUTION);
      ds18b20_1.requestTemp();
    }
  #endif
  #ifdef USE_DS18B20_2
    if(ds18b20_2.readAddress(ds_address)){
      detected.ds18b20_2 = true;
      ds18b20_2.setResolution(DS18B20_RESOLUTION);
      ds18b20_2.requestTemp();
    }
  #endif
  #ifdef USE_DS18B20_3
    if(ds18b20_3.readAddress(ds_address)){
      detected.ds18b20_3 = true;
      ds18b20_3.setResolution(DS18B20_RESOLUTION);
      ds18b20_3.requestTemp();
    }
  #endif
  #ifdef USE_DS18B20_4
    if(ds18b20_4.readAddress(ds_address)){
      detected.ds18b20_4 = true;
      ds18b20_4.setResolution(DS18B20_RESOLUTION);
      ds18b20_4.requestTemp();
    }
  #endif
  
  //MAX44009 initialization
  #ifdef USE_MAX44009
    if(!max44009.begin()) detected.max44009 = true;
  #endif

   //S8 initialization
  #ifdef USE_S8
    sensor_S8->get_firmware_version(sensor.firm_version);
    int len = strlen(sensor.firm_version);
    if(len) detected.s8 = true;
  #endif
}

/**
 * Get sensor data
 */
void getData(void){
  
  //BME280 read
  #ifdef USE_BME280
    if(detected.bme280){
      datas.temp = bme280.readTemperature();
      datas.hum = bme280.readHumidity();
      datas.pres = bme280.readPressure() / 100.0F;
    }
  #endif

  //SHT21 read
  #ifdef USE_SHT21
    if(detected.sht21){
      datas.temp = SHT21.getTemperature();
      datas.hum = SHT21.getHumidity();
    }
  #endif

  //DS18B20 read
  #ifdef USE_DS18B20_1
    if(DS18B20_RESOLUTION == 9) delay(94);
    if(DS18B20_RESOLUTION == 10) delay(188);
    if(DS18B20_RESOLUTION == 11) delay(375);
    if(DS18B20_RESOLUTION == 12) delay(750);
    if(detected.ds18b20_1) datas.temp_1 = ds18b20_1.getTemp();

    #ifdef USE_DS18B20_2
      if(detected.ds18b20_2) datas.temp_2 = ds18b20_2.getTemp();

      #ifdef USE_DS18B20_3
        if(detected.ds18b20_3) datas.temp_3 = ds18b20_3.getTemp();

        #ifdef USE_DS18B20_4
          if(detected.ds18b20_4) datas.temp_4 = ds18b20_4.getTemp();
        #endif
      #endif
    #endif
  #endif
  
  //MAX44009 read
  #ifdef USE_MAX44009
    if(detected.max44009) datas.light = max44009.get_lux();
  #endif
    
  //Battery voltage read
  datas.adc = analogRead(BAT);

  //PZEM-004T read
  #ifdef USE_PZEM_004T
    float voltage = pzem.voltage();
    if(!isnan(voltage)) datas.voltage = voltage;
    else datas.voltage = -1.0;
    float current = pzem.current();
    if(!isnan(current)) datas.current = current;
    else datas.current = -1.0;
    float power = pzem.power();
    if(!isnan(power)) datas.power = power;
    else datas.power = -1.0;
    float energy = pzem.energy();
    if(!isnan(energy)) datas.energy = energy;
    else datas.energy = -1.0;
    float frequency = pzem.frequency();
    if(!isnan(frequency)) datas.frequency = frequency;
    else datas.frequency = -1.0;
    float pf = pzem.pf();
    if(!isnan(pf)) datas.pf = pf;
    else datas.pf = -1.0;
  #endif

  //S8 read
  #ifdef USE_S8
    if(detected.s8) datas.co2 = sensor_S8->get_co2();
  #endif
}
