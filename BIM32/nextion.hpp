class Nextion {
  #define NX4832K035 0
  #define NX4832T035 1

  public:
    void init(void);
    void tick(void);
    void displayToggle(void);
    void displayOn(void);
    void displayOff(void);
    bool isDisplayOn(void);
    void setIntensity(unsigned int brightness, bool reduc);
    void setDisplayRTC(void);
    void dataReceive(void);

  private:
    bool _power = true; // display on/off flag
    int _customData = -1; // flag of key symbols of receive from the display
    String _receivedData = "";

    void _NX4832K035_setRTC(void);
    void _NX4832T035_timeDate(void);
    void _networkPage(void);
    void _voltage(void);
    void _battery(void);
    void _antenna(void);
    float _temp(unsigned int sens, unsigned int wsensNum, unsigned int tempSensor, unsigned int thing, String field);
    float _hum(unsigned int sens, unsigned int wsensNum, unsigned int thing, String field);
    void _comfortLevel(float temp, float hum);
    void _weatherDescription(void);
    void _currentIcon(void);
    void _thermometer(float temp);
    void _pres(void);
    void _windSpeed(void);
    void _windDirection(void);
    void _updated(void);
    void _weatherForecast(void);
    void _hourlyData(void);
    void _daily2hourly(void);
    void _historyOut(void);
    void _historyIn(void);
    void _alarms(void);
};

/**
 * Initialization of display variables 
 * Sending settings and texts
 */
void Nextion::init(void) {
  myNex.writeStr("Hourly.MM.txt", lang.mm());
  myNex.writeStr("Hourly.MS.txt", lang.ms());
  myNex.writeStr("HistoryIn.In.txt", lang.historyIn());
  myNex.writeStr("HistoryOut.Out.txt", lang.historyOut());
 
  myNex.writeStr("Network.NETWORK.txt", lang.network());
  myNex.writeStr("Network.SIGNALLEVEL.txt", lang.signalLevel());
  myNex.writeStr("Network.IPADDRESS.txt", lang.ipAddr());
  myNex.writeStr("Network.MACADDRESS.txt", lang.macAddr());
  myNex.writeStr("Network.ESP32TEMP.txt", lang.esp32Temp());
  myNex.writeStr("Network.FIRMWARE.txt", lang.firmware());

  for(unsigned int i=0; i<4; i++) {
    myNex.writeStr("Main.nameSeq" + String(i) + ".txt", config.display_source_sequence_name(i));
  }
  myNex.writeNum("Main.sequence.tim", config.display_source_sequence_dur() * 1000);

  /* Alarm */
  myNex.writeStr("Texts.ALARM.txt", lang.alarm());
  
  /* Initialize NX4832K035 display  */
  if(config.display_type(0) == NX4832K035) {
    // config
    myNex.writeNum("BigClock.clockFormat.val", config.clock_format());
    unsigned int langCode = 0;
    if(config.lang() == "de") langCode = 1;
    if(config.lang() == "ru") langCode = 2;
    if(config.lang() == "pl") langCode = 3;
    if(config.lang() == "ua") langCode = 4;
    myNex.writeNum("Main.lang.val", langCode);

    // texts
    myNex.writeStr("Texts.JANUARY.txt", lang.january());
    myNex.writeStr("Texts.FEBRUARY.txt", lang.february());
    myNex.writeStr("Texts.MARCH.txt", lang.march());
    myNex.writeStr("Texts.APRIL.txt", lang.april());
    myNex.writeStr("Texts.MAY.txt", lang.may());
    myNex.writeStr("Texts.JUNE.txt", lang.june());
    myNex.writeStr("Texts.JULY.txt", lang.july());
    myNex.writeStr("Texts.AUGUST.txt", lang.august());
    myNex.writeStr("Texts.SEPTEMBER.txt", lang.september());
    myNex.writeStr("Texts.OCTOBER.txt", lang.october());
    myNex.writeStr("Texts.NOVEMBER.txt", lang.november());
    myNex.writeStr("Texts.DECEMBER.txt", lang.december());
    myNex.writeStr("Texts.JAN_.txt", lang.january_());
    myNex.writeStr("Texts.FEB_.txt", lang.february_());
    myNex.writeStr("Texts.MAR_.txt", lang.march_());
    myNex.writeStr("Texts.APR_.txt", lang.april_());
    myNex.writeStr("Texts.MAY_.txt", lang.may_());
    myNex.writeStr("Texts.JUN_.txt", lang.june_());
    myNex.writeStr("Texts.JUL_.txt", lang.july_());
    myNex.writeStr("Texts.AUG_.txt", lang.august_());
    myNex.writeStr("Texts.SEP_.txt", lang.september_());
    myNex.writeStr("Texts.OCT_.txt", lang.october_());
    myNex.writeStr("Texts.NOV_.txt", lang.november_());
    myNex.writeStr("Texts.DEC_.txt", lang.december_());
    myNex.writeStr("Texts.JAN.txt", lang.jan());
    myNex.writeStr("Texts.FEB.txt", lang.feb());
    myNex.writeStr("Texts.MAR.txt", lang.mar());
    myNex.writeStr("Texts.APR.txt", lang.apr());
    myNex.writeStr("Texts.MAI.txt", lang.mai());
    myNex.writeStr("Texts.JUN.txt", lang.jun());
    myNex.writeStr("Texts.JUL.txt", lang.jul());
    myNex.writeStr("Texts.AUG.txt", lang.aug());
    myNex.writeStr("Texts.SEP.txt", lang.sep());
    myNex.writeStr("Texts.OCT.txt", lang.oct());
    myNex.writeStr("Texts.NOV.txt", lang.nov());
    myNex.writeStr("Texts.DEC.txt", lang.dec());

    myNex.writeStr("Texts.MONDAY.txt", lang.monday());
    myNex.writeStr("Texts.TUESDAY.txt", lang.tuesday());
    myNex.writeStr("Texts.WEDNESDAY.txt", lang.wednesday());
    myNex.writeStr("Texts.THURSDAY.txt", lang.thursday());
    myNex.writeStr("Texts.FRIDAY.txt", lang.friday());
    myNex.writeStr("Texts.SATURDAY.txt", lang.saturday());
    myNex.writeStr("Texts.SUNDAY.txt", lang.sunday());
    myNex.writeStr("Texts.MO.txt", lang.mo());
    myNex.writeStr("Texts.TU.txt", lang.tu());
    myNex.writeStr("Texts.WE.txt", lang.we());
    myNex.writeStr("Texts.TH.txt", lang.th());
    myNex.writeStr("Texts.FR.txt", lang.fr());
    myNex.writeStr("Texts.SA.txt", lang.sa());
    myNex.writeStr("Texts.SU.txt", lang.su());
  }

  myNex.writeNum("thup", 1);

  myNex.writeStr("page Main");
}

/**
 * Sending all data to display
 */
void Nextion::tick(void) {
  myNex.writeNum("sleep", _power ? 0 : 1);

  if(_power) {
    if(config.display_type(0) == NX4832T035) _NX4832T035_timeDate();
    if(config.display_type(0) == NX4832K035) {
      if(global.clockSynchronize) {
        _NX4832K035_setRTC();
        global.clockSynchronize = false;
      }
    }
  
    _networkPage();
    _voltage();
    _battery();
    _antenna();
  
    float temp = _temp(
      config.display_source_tempIn_sens(),
      config.display_source_tempIn_wsensNum(),
      config.display_source_tempIn_temp(),
      config.display_source_tempIn_thing(),
      "Main.tempInside.txt"
    );
  
    float hum =_hum(
      config.display_source_humIn_sens(),
      config.display_source_humIn_wsensNum(),
      config.display_source_humIn_thing(),
      "Main.humInside.txt"
    );
  
    _comfortLevel(temp, hum);
    _weatherDescription();
    _currentIcon();
  
    temp = _temp(
      config.display_source_tempOut_sens(),
      config.display_source_tempOut_wsensNum(),
      config.display_source_tempOut_temp(),
      config.display_source_tempOut_thing(),
      "Main.tempOutside.txt"
    );
  
    _thermometer(temp);
  
    hum =_hum(
      config.display_source_humOut_sens(),
      config.display_source_humOut_wsensNum(),
      config.display_source_humOut_thing(),
      "Main.humOutside.txt"
    );
  
    _pres();
    _windSpeed();
    _windDirection();
    _updated();
    _weatherForecast();
    _hourlyData();
    _daily2hourly();
    _historyOut();
    _historyIn();
    _alarms();
  }
}

/**
 * Change display brightness
 */
void Nextion::setIntensity(unsigned int brightness, bool reduc) {
  if(_power) myNex.writeNum("dim", reduc ? round(brightness / 2) : brightness);
}

/**
 * Toggles display (on/off)
 */
void Nextion::displayToggle() {
  if(_power) myNex.writeNum("dim", 0);
  else init();
  _power = !_power;
}

/**
 * Turns on the display
 */
void Nextion::displayOn() {
  _power = true;
  init();
}

/*
 * Turns off the display
 */
void Nextion::displayOff() {
  myNex.writeNum("dim", 0);
  _power = false;
}

/**
 * Returns true if display is on
 */
bool Nextion::isDisplayOn() {
  return _power;
}

/**
 * Set the time and date of the display with built-in RTC
 */
void Nextion::setDisplayRTC(void) {
  if(config.display_type(0) == NX4832K035) _NX4832K035_setRTC();
}

/**
 * Set the time and date of the NX4832K035 display
 */
void Nextion::_NX4832K035_setRTC(void) {
  if(now() > 1665606321) {
    myNex.writeNum("rtc0", year());
    myNex.writeNum("rtc1", month());
    myNex.writeNum("rtc2", day());
    myNex.writeNum("rtc3", hour());
    myNex.writeNum("rtc4", minute());
    myNex.writeNum("rtc5", second());
  }
}

/**
 * Sending time and date to the NX4832T035 display that does not have a built-in RTC
 */
void Nextion::_NX4832T035_timeDate(void) {
  char buf[32] = "";
  myNex.writeStr("BigClock.hour.txt", String(config.clock_format() ? hour() : hourFormat12()));
  myNex.writeStr("BigClock.minute.txt", String(minute()));
  unsigned int wd = weekday();
  myNex.writeStr("BigClock.weekday.txt", lang.weekdayShortName(wd));
  if(config.lang() == "en") sprintf(buf, "%s %d, %d", lang.monthName(month()), day(), year());
  else sprintf(buf, "%d %s %d", day(), lang.monthName(month()), year());
  myNex.writeStr("BigClock.date.txt", String(buf));
  myNex.writeStr("Main.weekday0.txt", lang.weekdayShortName(wd));
  if(++wd > 7) wd = 1;
  myNex.writeStr("Main.weekday2.txt", lang.weekdayShortName(wd));
  if(++wd > 7) wd = 1;
  myNex.writeStr("Main.weekday3.txt", lang.weekdayShortName(wd));
  if(++wd > 7) wd = 1;
  myNex.writeStr("Main.weekday4.txt", lang.weekdayShortName(wd));
}

/**
 * Sending data to Network page
 */
void Nextion::_networkPage(void) {
  if(global.apMode) {
    myNex.writeStr("Network.Logo.txt", "Access Point");
    myNex.writeStr("Network.ssid.txt", config.accessPoint_ssid());
    myNex.writeStr("Network.rssi.txt", "100%");
    myNex.writeStr("Network.ip.txt", "192.168.4.1");
    myNex.writeStr("Network.mac.txt", WiFi.softAPmacAddress());
  }
  else {
    myNex.writeStr("Network.Logo.txt", "WiFi");
    myNex.writeStr("Network.ssid.txt", WiFi.SSID());
    myNex.writeStr("Network.rssi.txt", String(WiFi.RSSI()) + "dBm");
    myNex.writeStr("Network.ip.txt", WiFi.localIP().toString());
    myNex.writeStr("Network.mac.txt", WiFi.macAddress());
  }
  myNex.writeStr("Network.temp.txt", String(int(round(sensors.get_esp32_temp(0)))) + "°C");
  myNex.writeStr("Network.frmw.txt", global.fw);
}

/**
 * Display voltage
 */
void Nextion::_voltage(void) {
  if(config.display_source_volt_sens() == 1) { // voltage from wireless sensor
    if(now() - wsensor.get_updated(config.display_source_volt_wsensNum()) < 
    config.wsensor_expire(config.display_source_volt_wsensNum()) * 60) {
      if(config.display_source_volt_volt() == 0) { // battery voltage
        float voltage = wsensor.get_batteryVoltage(config.display_source_volt_wsensNum());
        if(wsensor.checkBatVolt(voltage)) 
          myNex.writeStr("Main.uBat.txt", String(round(voltage * 100) / 100) + lang.v());
        else myNex.writeStr("Main.uBat.txt", "");
      }
      else if(config.display_source_volt_volt() == 1) { // PZEM-004t voltage
        float voltage = wsensor.get_voltage(config.display_source_volt_wsensNum(), config.wsensor_volt_corr(config.display_source_volt_wsensNum()));
        if(wsensor.checkVolt(voltage))
          myNex.writeStr("Main.uBat.txt", String(round(voltage * 100) / 100) + lang.v());
        else myNex.writeStr("Main.uBat.txt", "");
      }
      else myNex.writeStr("Main.uBat.txt", "");
    }
    else myNex.writeStr("Main.uBat.txt", "");
  }
  else if(config.display_source_volt_sens() == 2) { // voltage from thingspeak
    float voltage = thingspeak.get_field(config.display_source_volt_thing());
    if((wsensor.checkVolt(voltage) or wsensor.checkBatVolt(voltage)) and ((now() - thingspeak.get_updated()) < (config.thingspeakReceive_expire() * 60)))
      myNex.writeStr("Main.uBat.txt", String(round(voltage * 100) / 100) + lang.v());
    else myNex.writeStr("Main.uBat.txt", "");
  }
  else myNex.writeStr("Main.uBat.txt", "");
}

/**
 * Display battery symbol
 */
void Nextion::_battery(void) {
  if(config.display_source_bat_sens() == 1) { // battery symbol from wireless sensor
    int level = wsensor.get_batteryLevel(config.display_source_bat_wsensNum());
    if(wsensor.checkBatLvl(level) and ((now() - wsensor.get_updated(config.display_source_bat_wsensNum())) < 
    (config.wsensor_expire(config.display_source_bat_wsensNum()) * 60))) 
      myNex.writeNum("Main.bat.pic", level + 35);
    else myNex.writeNum("Main.bat.pic", 35);
  }
  else if(config.display_source_bat_sens() == 2) { // battery symbol from thingspeak
    unsigned int level = thingspeak.get_field(config.display_source_bat_thing());
    if(wsensor.checkBatLvl(level) and ((now() - thingspeak.get_updated()) < (config.thingspeakReceive_expire() * 60)))
      myNex.writeNum("Main.bat.pic", level + 35);
    else myNex.writeNum("Main.bat.pic", 35);
  }
  else myNex.writeNum("Main.bat.pic", 35);
}

/**
 * Display antenna symbol
 */
void Nextion::_antenna(void) {
  if(global.apMode) myNex.writeNum("Main.ant.pic", 70);
  else {
    unsigned int ant = 30;
    int rssi = WiFi.RSSI();
    if(rssi > -51) ant = 34;
    if(rssi < -50 && rssi > -76) ant = 33;
    if(rssi <- 75 && rssi > -96) ant = 32;
    if(rssi < -95) ant = 31;
    if(rssi >= 0) ant = 30;
    myNex.writeNum("Main.ant.pic", ant);
  }
}

/**
 * Display temperature
 * @param sens temperature source
 * @param wsensNum wireless sensor number
 * @param tempSensor temperature sensor number (for wireless sensor)
 * @param thing thinspeak field number
 * @param field name of the temperature field in the display
 * @return displayed temperature
 */
float Nextion::_temp(unsigned int sens, unsigned int wsensNum, unsigned int tempSensor, unsigned int thing, String field) {
  float temp = 40400.0;
  if(sens == 0) temp = weather.get_currentTemp(); // temperature from weather forecast
  if(sens == 1) { // temperature from wireless sensor
    if(now() - wsensor.get_updated(wsensNum) < config.wsensor_expire(wsensNum) * 60) {
      temp = wsensor.get_temperature(wsensNum, tempSensor, config.wsensor_temp_corr(wsensNum, tempSensor));
    }
  }
  if(sens == 2) { // temperature from thingspeak
    if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60) {
      temp = thingspeak.get_field(thing);
    }
  }
  if(sens == 3) { // temperature from BME280
    temp = sensors.get_bme280_temp(config.bme280_temp_corr());
  }
  if(sens == 4) { // temperature from BMP180
    temp = sensors.get_bmp180_temp(config.bmp180_temp_corr());
  }
  if(sens == 5) { // temperature from SHT21
    temp = sensors.get_sht21_temp(config.sht21_temp_corr());
  }
  if(sens == 6) { // temperature from DHT22
    temp = sensors.get_dht22_temp(config.dht22_temp_corr());
  }
  if(sens == 7) { // temperature from DS18B20
    temp = sensors.get_ds18b20_temp(config.ds18b20_temp_corr());
  }
  if(sens == 8) { // temperature from sequence
    float tempSequence[4] = { 40400.0, 40400.0, 40400.0, 40400.0 }; 
    for(unsigned int i=0; i<4; i++) {
      if(config.display_source_sequence_temp(i) == 1) { // thingspeak
        if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60) {
          tempSequence[i] = thingspeak.get_field(config.display_source_sequence_thngtemp(i));
        }
      }
      if(config.display_source_sequence_temp(i) == 2) { // wireless sensor
        if(now() - wsensor.get_updated(config.display_source_sequence_wsenstemp(i, 0)) < 
        config.wsensor_expire(config.display_source_sequence_wsenstemp(i, 0)) * 60) {
          tempSequence[i] = wsensor.get_temperature(
            config.display_source_sequence_wsenstemp(i, 0),
            config.display_source_sequence_wsenstemp(i, 1),
            config.wsensor_temp_corr(
              config.display_source_sequence_wsenstemp(i, 0),
              config.display_source_sequence_wsenstemp(i, 1)
            )
          );
        }
      }
      if(config.display_source_sequence_temp(i) == 3) { // BME280
        tempSequence[i] = sensors.get_bme280_temp(config.bme280_temp_corr());
      }
      if(config.display_source_sequence_temp(i) == 4) { // BMP180
        tempSequence[i] = sensors.get_bmp180_temp(config.bmp180_temp_corr());
      }
      if(config.display_source_sequence_temp(i) == 5) { // SHT21
        tempSequence[i] = sensors.get_sht21_temp(config.sht21_temp_corr());
      }
      if(config.display_source_sequence_temp(i) == 6) { // DHT22
        tempSequence[i] = sensors.get_dht22_temp(config.dht22_temp_corr());
      }
      if(config.display_source_sequence_temp(i) == 7) { // DS18B20
        tempSequence[i] = sensors.get_ds18b20_temp(config.ds18b20_temp_corr());
      }
      if(config.display_source_sequence_temp(i) == 8) { // ESP32
        tempSequence[i] = sensors.get_esp32_temp(config.esp32_temp_corr());
      }
      if(config.display_source_sequence_temp(i) == 9) { // Forecast
        tempSequence[i] = weather.get_currentTemp();
      }
      
      if(sensors.checkTemp(tempSequence[i])) myNex.writeStr("Main.tempSeq" + String(i) + ".txt", String(int(round(tempSequence[i]))) + "°C");
      else myNex.writeStr("Main.tempSeq" + String(i) + ".txt", "--");
      
      myNex.writeStr("Main.txtSeq" + String(i) + ".txt", config.display_source_sequence_name(i));
    }
  }
  else {
    if(sensors.checkTemp(temp)) myNex.writeStr(field, String(int(round(temp))) + "°C");
    else myNex.writeStr(field, "--");
    if(field == "Main.tempInside.txt") {
      for(unsigned int i=0; i<4; i++) {
        myNex.writeStr("Main.txtSeq" + String(i) + ".txt", "--");
      }
    }
  }
  return temp;
}

/**
 * Display humidity
 * @param sens humidity source
 * @param wsensNum wireless sensor number
 * @param thing thinspeak field number
 * @param field name of the humidity field in the display
 * @return displayed humidity
 */
float Nextion::_hum(unsigned int sens, unsigned int wsensNum, unsigned int thing, String field) {
  float hum = 40400.0;
  if(sens == 0) hum = weather.get_currentHum(); // humudity from weather forecast
  if(sens == 1) { // humidity from wireless sensor
    if(now() - wsensor.get_updated(wsensNum) < config.wsensor_expire(wsensNum) * 60) {
      hum = wsensor.get_humidity(wsensNum, config.wsensor_hum_corr(wsensNum));
    }
  }
  if(sens == 2) { // humidity from thingspeak
    if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60) {
      hum = thingspeak.get_field(thing);
    }
  }
  if(sens == 3) { // humidity from BME280
    hum = sensors.get_bme280_hum(config.bme280_hum_corr());
  }
  if(sens == 4) { // humidity from SHT21
    hum = sensors.get_sht21_hum(config.sht21_hum_corr());
  }
  if(sens == 5) { // humidity from DHT22
    hum = sensors.get_dht22_hum(config.dht22_hum_corr());
  }
  if(sens == 6) { // humidity from sequence
    float humSequence[4] = { 40400.0, 40400.0, 40400.0, 40400.0 }; 
    for(unsigned int i=0; i<4; i++) {
      if(config.display_source_sequence_hum(i) == 1) { // thingspeak
        if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60) {
          humSequence[i] = thingspeak.get_field(config.display_source_sequence_thnghum(i));
        }
      }
      if(config.display_source_sequence_hum(i) == 2) { // wireless sensor
        if(now() - wsensor.get_updated(config.display_source_sequence_wsenshum(i)) < 
        config.wsensor_expire(config.display_source_sequence_wsenshum(i)) * 60) {
          humSequence[i] = wsensor.get_humidity(
            config.display_source_sequence_wsenshum(i),
            config.wsensor_hum_corr(config.display_source_sequence_wsenshum(i))
          );
        }
      }
      if(config.display_source_sequence_hum(i) == 3) { // BME280
        humSequence[i] = sensors.get_bme280_hum(config.bme280_hum_corr());
      }
      if(config.display_source_sequence_hum(i) == 4) { // SHT21
        humSequence[i] = sensors.get_sht21_hum(config.sht21_hum_corr());
      }
      if(config.display_source_sequence_hum(i) == 5) { // DHT22
        humSequence[i] = sensors.get_dht22_hum(config.dht22_hum_corr());
      }
      if(config.display_source_sequence_hum(i) == 6) { // Forecast
        humSequence[i] = weather.get_currentHum();
      }
      
      if(sensors.checkHum(humSequence[i])) { 
        myNex.writeStr("Main.humSeq" + String(i) + ".txt", String(int(round(humSequence[i]))) + "%");
      }
      else myNex.writeStr("Main.humSeq" + String(i) + ".txt", "--");
    }
  }
  else {
    if(sensors.checkHum(hum)) myNex.writeStr(field, String(int(round(hum))) + "%");
    else myNex.writeStr(field, "--");
  }
  return hum;
}

/**
 * Display comfort level
 * @param temp temperature
 * @param hum humidity
 */
void Nextion::_comfortLevel(float temp, float hum) {
  #define COMFORTABLE 0
  #define HOT         1
  #define COLD        2
  #define DRY         3
  #define HUMID       4
  #define HOT_HUMID   5
  #define HOT_DRY     6
  #define COLD_HUMID  7
  #define COLD_DRY    8
  
  if(config.display_source_descr() == 0) { // --
    myNex.writeNum("Main.seq.val", 0);
    myNex.writeStr("Main.comfort.txt", "");
  }
  
  if(config.display_source_descr() == 1) { // comfort level
    myNex.writeNum("Main.seq.val", 0);
    switch(global.comfort) {
      case 0: myNex.writeStr("Main.comfort.txt", "--"); break;
      case 1: myNex.writeStr("Main.comfort.txt", lang.comfort(COMFORTABLE)); break;
      case 2: myNex.writeStr("Main.comfort.txt", lang.comfort(HOT)); break;
      case 3: myNex.writeStr("Main.comfort.txt", lang.comfort(COLD)); break;
      case 4: myNex.writeStr("Main.comfort.txt", lang.comfort(DRY)); break;
      case 5: myNex.writeStr("Main.comfort.txt", lang.comfort(HUMID)); break;
      case 6: myNex.writeStr("Main.comfort.txt", lang.comfort(HOT_HUMID)); break;
      case 7: myNex.writeStr("Main.comfort.txt", lang.comfort(HOT_DRY)); break;
      case 8: myNex.writeStr("Main.comfort.txt", lang.comfort(COLD_HUMID)); break;
      case 9: myNex.writeStr("Main.comfort.txt", lang.comfort(COLD_DRY)); break;
      default: myNex.writeStr("Main.comfort.txt", "--"); break;
    }
  }
  if(config.display_source_descr() == 2) { // sequence
    myNex.writeNum("Main.seq.val", 1);
  }
}

/**
 * Display weather description
 */
void Nextion::_weatherDescription(void) {
  if(weather.get_description() == "") myNex.writeStr("Main.description.txt", "--");
  myNex.writeStr("Main.description.txt", weather.get_description());
}

/**
 * Display current weather icon
 */
void Nextion::_currentIcon(void) {
  if(weather.get_currentIcon() == 0) myNex.writeNum("Main.icon0.pic", 64);
  else myNex.writeNum("Main.icon0.pic", weather.get_currentIcon() + 9);
  switch(weather.get_currentIcon()) {
    case 1: {
      if(weather.get_isDay()) myNex.writeNum("Main.icon0.pic", 10);
      else myNex.writeNum("Main.icon0.pic", 11);
    }; break;
    case 2: {
      if(weather.get_isDay()) myNex.writeNum("Main.icon0.pic", 12);
      else myNex.writeNum("Main.icon0.pic", 13);
    }; break;
    case 3: myNex.writeNum("Main.icon0.pic", 14); break;
    case 4: myNex.writeNum("Main.icon0.pic", 15); break;
    case 5: myNex.writeNum("Main.icon0.pic", 16); break;
    case 6: {
      if(weather.get_isDay()) myNex.writeNum("Main.icon0.pic", 17);
      else myNex.writeNum("Main.icon0.pic", 18);
    }; break;
    case 7: myNex.writeNum("Main.icon0.pic", 19); break;
    case 8: myNex.writeNum("Main.icon0.pic", 20); break;
    default: myNex.writeNum("Main.icon0.pic", 64); break;
  }
}

/**
 * Display thermometer icon (red or blue)
 * @param temp 
 */
void Nextion::_thermometer(float temp) {
  myNex.writeNum("Main.thermometer.pic", temp < 0.0 ? 40 : 41);
}

/**
 * Display pressure
 */
void Nextion::_pres(void) {
  float pres = 40400.0;
  if(config.display_source_presOut_sens() == 0) // pressure outside from weather forecast
    pres = weather.get_currentPres();
  if(config.display_source_presOut_sens() == 1) // presure outside from wireless sensor
    if(now() - wsensor.get_updated(config.display_source_presOut_wsensNum()) < 
    config.wsensor_expire(config.display_source_presOut_wsensNum()) * 60)
      pres = wsensor.get_pressure(config.display_source_presOut_wsensNum(), config.wsensor_pres_corr(config.display_source_presOut_wsensNum()));
  if(config.display_source_presOut_sens() == 2) // presure outside from thingspeak
    if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60)
      pres = thingspeak.get_field(config.display_source_presOut_thing());
  if(config.display_source_presOut_sens() == 3) // pressure outside from BME280
    pres = sensors.get_bme280_pres(config.bme280_pres_corr());
  if(config.display_source_presOut_sens() == 4) // pressure outside from BMP180
    pres = sensors.get_bmp180_pres(config.bmp180_pres_corr());
  if(sensors.checkPres(pres)) 
  myNex.writeStr("Main.presOutside.txt", String(int(round(pres * 0.75))) + lang.mm());
  else myNex.writeStr("Main.presOutside.txt", "--");
}

/**
 * Display wind speed
 */
void Nextion::_windSpeed(void) {
  if(weather.checkWind(weather.get_currentWindSpeed()))
    myNex.writeStr("Main.wind0.txt", String(int(round(weather.get_currentWindSpeed()))) + lang.ms());
  else myNex.writeStr("Main.wind0.txt", "--");
}

/**
 * Display wind direction
 */
void Nextion::_windDirection(void) {
  unsigned int deg = round(float(weather.get_currentWindDir()) / 45.0);
  if(deg > 7) deg = 0;
  myNex.writeNum("Main.windDir0.pic", deg + 42);
}

/**
 * Display the time and date of the last weather update
 */
void Nextion::_updated(void) {
  char buf[32] = "";
  if(config.lang() == "en") {
    sprintf(buf, "⭮ %d-%02d-%02d %d:%02d:%02d", 
      year(weather.get_currentUpdated()),
      month(weather.get_currentUpdated()),
      day(weather.get_currentUpdated()),
      config.clock_format() ? hour(weather.get_currentUpdated()) : hourFormat12(),
      minute(weather.get_currentUpdated()),
      second(weather.get_currentUpdated())
    );
  }
  else {
    sprintf(buf, "⭮ %02d.%02d.%d %d:%02d:%02d", 
      day(weather.get_currentUpdated()),
      month(weather.get_currentUpdated()),
      year(weather.get_currentUpdated()),
      config.clock_format() ? hour(weather.get_currentUpdated()) : hourFormat12(),
      minute(weather.get_currentUpdated()),
      second(weather.get_currentUpdated())
    );
  }
  if(weather.get_currentUpdated() == 0) myNex.writeStr("Main.updatedTime.txt", "--");
  else myNex.writeStr("Main.updatedTime.txt", buf);
}

/**
 * Display daily weather forecast
 */
void Nextion::_weatherForecast(void) {
  for(unsigned int i=0; i<4; i++) {
    // icons
    if(weather.get_dailyIcon(i) == 0) myNex.writeNum("Main.icon" + String(i + 1) + ".pic", 29);
    else myNex.writeNum("Main.icon" + String(i + 1) + ".pic", weather.get_dailyIcon(i) + 20);

    // max temp
    if(sensors.checkTemp(weather.get_dailyDayTemp(i)))
      myNex.writeStr("Main.tempMax" + String(i + 1) + ".txt", String(int(round(weather.get_dailyDayTemp(i)))) + "°C");
    else myNex.writeStr("Main.tempMax" + String(i + 1) + ".txt", "--");

    // min temp
    if(sensors.checkTemp(weather.get_dailyNightTemp(i)))
      myNex.writeStr("Main.tempMin" + String(i + 1) + ".txt", String(int(round(weather.get_dailyNightTemp(i)))) + "°C");
    else myNex.writeStr("Main.tempMin" + String(i + 1) + ".txt", "--");

    // wind speed
    if(weather.checkWind(weather.get_dailyWindSpeed(i)))
      myNex.writeStr("Main.wind" + String(i + 1) + ".txt", String(int(round(weather.get_dailyWindSpeed(i)))) + lang.ms());
    else myNex.writeStr("Main.wind" + String(i + 1) + ".txt", "--");
  }
}

/**
 * Sending data to display hourly forecast
 */
void Nextion::_hourlyData(void) {
  char dat[22] = "";
  char buf[5] = "";
  Serial1.print("Hourly.data0.txt=\"");
  if(config.weather_provider() == OPENWEATHERMAP) {
    for(unsigned int i=0; i<40; i++) {
      // temp
      int t = round(weather.get_hourlyTemp(i) * 10);
      sprintf(buf, "%04d", t);
      for(unsigned int k = 0; k < 4; k++) dat[k] = buf[k];
      // pres
      unsigned int p = round(weather.get_hourlyPres(i) * 0.75);
      sprintf(buf, "%03d", p);
      for(unsigned int k = 0; k < 3; k++) dat[4 + k] = buf[k];
      // icon
      sprintf(buf, "%02d", weather.get_hourlyIcon(i));
      for(unsigned int k = 0; k < 2; k++) dat[7 + k] = buf[k];
      // weekday
      sprintf(buf, "%d", weekday(weather.get_hourlyDate(i)) - 1);
      dat[9] = buf[0];
      // day
      sprintf(buf, "%02d", day(weather.get_hourlyDate(i)));
      for(unsigned int k = 0; k < 2; k++) dat[10 + k] = buf[k];
      // month
      sprintf(buf, "%02d", month(weather.get_hourlyDate(i)) - 1);
      for(unsigned int k = 0; k < 2; k++) dat[12 + k] = buf[k];
      // hour
      sprintf(buf, "%02d", hour(weather.get_hourlyDate(i)));
      for(unsigned int k = 0; k < 2; k++) dat[14 + k] = buf[k];
      // wind speed
      unsigned int wind = round(weather.get_hourlyWindSpeed(i));
      sprintf(buf, "%02d", wind);
      for(unsigned int k = 0; k < 2; k++) dat[16 + k] = buf[k];
      // wind direction
      unsigned int deg = round(float(weather.get_hourlyWindDir(i)) / 45.0);
      if(deg > 7) deg = 0;
      sprintf(buf, "%d", deg);
      dat[18] = buf[0];
      // precipitation
      int pr = round(weather.get_hourlyPrec(i) * 100);
      sprintf(buf, "%03d", pr);
      for(unsigned int k=0; k<3; k++) dat[19 + k] = buf[k];
      
      // send all to display
      for(unsigned int k=0; k<22; k++) Serial1.print(dat[k]);
    }
  }
  Serial1.print("\"");
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  Serial1.write(0xFF);
}

/**
 * Initialize daily forecast links to hourly forecast
 */
void Nextion::_daily2hourly(void) {
  unsigned int fd = 2;
  for(unsigned int i=0; i<40; i++) {
    if(hour(weather.get_hourlyDate(i)) == 0) {
      if(i != 0) myNex.writeNum("Hourly.day" + String(fd++) + ".val", i);
      if(fd > 4) break;
    }
  }
}

/**
 * Sending data to display outdoor weather history
 */
void Nextion::_historyOut(void) {
  char dat[18] = "";
  char buf[5] = "";
  Serial1.print("HistoryOut.data0.txt=\"");
  for(unsigned int i = 0; i < 24; i++) {
    // temperature
    int t = round(thingspeak.get_historyField(0, i) * 10);
    sprintf(buf, "%04d", t);
    for(unsigned int k = 0; k < 4; k++) dat[k] = buf[k];
    // humidity
    int h = thingspeak.get_historyField(1, i);
    sprintf(buf, "%03d", h);
    for(unsigned int k = 0; k < 3; k++) dat[4 + k] = buf[k];
    // pressure
    int p = round(thingspeak.get_historyField(2, i) * 0.75);
    sprintf(buf, "%03d", p);
    for(unsigned int k = 0; k < 3; k++) dat[7 + k] = buf[k];
    // day
    sprintf(buf, "%02d", day(thingspeak.get_historyUpdated(i)));
    for(unsigned int k = 0; k < 2; k++) dat[10 + k] = buf[k];
    // month
    sprintf(buf, "%02d", month(thingspeak.get_historyUpdated(i)) - 1);
    for(unsigned int k = 0; k < 2; k++) dat[12 + k] = buf[k];
    // hour
    sprintf(buf, "%02d", hour(thingspeak.get_historyUpdated(i)));
    for(unsigned int k = 0; k < 2; k++) dat[14 + k] = buf[k];
    // minute
    sprintf(buf, "%02d", minute(thingspeak.get_historyUpdated(i)));
    for(unsigned int k = 0; k < 2; k++) dat[16 + k] = buf[k];
    // send current slot
    for(unsigned int k=0; k<18; k++) Serial1.print(dat[k]);
  }
  Serial1.print("\"");
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  Serial1.write(0xFF);
}

/**
 * Sending data to display indoor weather history
 */
void Nextion::_historyIn(void) {
  char dat[15] = "";
  char buf[5] = "";
  Serial1.print("HistoryIn.data0.txt=\"");
  for(unsigned int i = 0; i < 24; i++) {
    // temperature
    int t = round(thingspeak.get_historyField(3, i) * 10);
    sprintf(buf, "%04d", t);
    for(unsigned int k = 0; k < 4; k++) dat[k] = buf[k];
    // humidity
    int h = thingspeak.get_historyField(4, i);
    sprintf(buf, "%03d", h);
    for(unsigned int k = 0; k < 3; k++) dat[4 + k] = buf[k];
    // day
    sprintf(buf, "%02d", day(thingspeak.get_historyUpdated(i)));
    for(unsigned int k = 0; k < 2; k++) dat[7 + k] = buf[k];
    // month
    sprintf(buf, "%02d", month(thingspeak.get_historyUpdated(i)) - 1);
    for(unsigned int k = 0; k < 2; k++) dat[9 + k] = buf[k];
    // hour
    sprintf(buf, "%02d", hour(thingspeak.get_historyUpdated(i)));
    for(unsigned int k = 0; k < 2; k++) dat[11 + k] = buf[k];
    // minute
    sprintf(buf, "%02d", minute(thingspeak.get_historyUpdated(i)));
    for(unsigned int k = 0; k < 2; k++) dat[13 + k] = buf[k];
    // send current slot
    for(unsigned int k=0; k<15; k++) Serial1.print(dat[k]);
  }
  Serial1.print("\"");
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  Serial1.write(0xFF);
}

/**
 * Sending alarms data
 */
void Nextion::_alarms(void) {
  char alarmData[144];
  char buf[3];
  unsigned int alarmOn = 0;
  Serial1.print("Alarm.alarms.txt=\"");
  for(unsigned int i=0; i<12; i++) {
    sprintf(buf, "%02d", config.alarm_time(i, 0));
    for(unsigned int j=0; j<2; j++) alarmData[i * 4 + j] = buf[j];
    sprintf(buf, "%02d", config.alarm_time(i, 1));
    for(unsigned int j=0; j<2; j++) alarmData[i * 4 + j + 2] = buf[j];
    for(unsigned int w=0; w<7; w++) {
      sprintf(buf, "%d", config.alarm_weekday(i, w));
      alarmData[i * 7 + w + 48] = buf[0];
    }
    sprintf(buf, "%d", config.alarm_state(i));
    alarmData[i + 132] = buf[0];
    alarmOn |= config.alarm_state(i);
  }
  for(unsigned int k=0; k<144; k++) Serial1.print(alarmData[k]);
  Serial1.print("\"");
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  myNex.writeNum("Main.alarm.pic", alarmOn ? 71 : 72);
}

/**
 * Receive date and time from display
 */
void Nextion::dataReceive(void) {
  while(Serial1.available()) {
    char nextionChar = Serial1.read();
    if(nextionChar == '{' or nextionChar == '\x87') {
      global.disp_autoOff[0] = millis();
      if(!isDisplayOn()) displayOn();
    }
    if(nextionChar == '{') _customData = 1;
    if(nextionChar == '}') {
      _customData = 0;
      _receivedData.concat(nextionChar);
    }
    if(_customData == 1) _receivedData.concat(nextionChar);
    if(_customData == 0) {
      StaticJsonDocument<1024> root;
      DeserializationError error = deserializeJson(root, _receivedData);
      if(!error) {
        if(_receivedData.lastIndexOf("{\"h\":") != -1)
          setTime(
            root["h"].as<int>(), 
            root["i"].as<int>(), 
            root["s"].as<int>(), 
            root["d"].as<int>(), 
            root["m"].as<int>(), 
            root["y"].as<int>()
          );
      }
      _receivedData = "";
      _customData = -1;
    }
  }
}
