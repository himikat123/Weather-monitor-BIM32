void display1_toggle(void);
void display2_toggle(void);
void alarm_button(void);
void comfortCalculate(void);
void get_time(void);
boolean is_summertime(void);


void TaskSensors(void *pvParameters) {
  (void) pvParameters;
  
  sensors.init();
  sensors.read(); 
  sound.init();

  unsigned int ntp_update = 0;
  unsigned int sensors_update = 0;
  unsigned int networks_update = 0;
  unsigned int thingspeakReceive = 0;
  unsigned int thingspeakSend = 0;
  unsigned int narodmonSend = 0;
  unsigned int historyUpdate = 0;

  attachInterrupt(DISPLAY1_BUTTON_PIN, display1_toggle, FALLING);
  attachInterrupt(DISPLAY2_BUTTON_PIN, display2_toggle, FALLING);
  attachInterrupt(ALARM_BUTTON_PIN, alarm_button, FALLING);

  // HC12 wireless module channel number request
  digitalWrite(SET_HC12_PIN, LOW);
  delay(50);
  Serial2.println("AT+RC");
  delay(100);
  digitalWrite(SET_HC12_PIN, HIGH);
    
  while(1) {
    /**
     * Check if a button is pressed
     */
    // Enter access point mode if button is pressed
    if(digitalRead(SETTINGS_BUTTON_PIN) == 0) network.runAccessPoint();

    /**
     * Wired sensors update
     * every 5 seconds
     */
    if(millis() - sensors_update > 5000) {
      sensors_update = millis();
      sensors.read();
    }

    /**
     * BME680 sensor update
     * try as often as possible, the sensor will update when it decides
     */
    sensors.BME680Read();

    /**
     * Comfort calculate 
     */
    comfortCalculate();

    /**
     * List of available networks update
     * every 20 seconds
     */
    if(millis() - networks_update > 20000) {
      networks_update = millis();
      global.nets = WiFi.scanNetworks();
      if(global.nets > 29) global.nets = 29;
      for(unsigned int i=0; i<global.nets; i++) {
        WiFi.SSID(i).toCharArray(global.ssids[i], sizeof(global.ssids[i]));
        global.rssis[i] = abs(WiFi.RSSI(i));
      }
    }

    /* Receive from wireless sensor */
    wsensor.receive();

    /* Hourly signal */
    sound.hourlySignal();

    /* Alarm */
    sound.alarm();
    
    
    if(!global.apMode) {
      /**
       * Network connection if not connected and if not Access point mode
       */
      if(WiFi.localIP().toString() == "0.0.0.0" or WiFi.status() != WL_CONNECTED) {
        global.net_connected = false;
        network.connect();
        vTaskDelay(1000);
      }
      else global.net_connected = true;
    
      /**
       * Time synchronization with NTP server
       */
      if(config.clock_ntp_period()) {
        if((millis() - ntp_update) > config.clock_ntp_period() * 60000 or !global.clockSynchronized) {
          ntp_update = millis();
          if(network.isConnected()) get_time();
          else Serial.println("No internet connection");
          vTaskDelay(100);
        }
      }
    
      /**
       * Receive data from thingspeak
       */
      if(config.thingspeakReceive_period()) {
        if((millis() - thingspeakReceive) > (config.thingspeakReceive_period() * 60000) or thingspeakReceive == 0) {
          thingspeakReceive = millis();
          if(network.isConnected()) thingspeak.receive();
          else Serial.println("No internet connection");
        }
      }

      /**
       * Send data to thingspeak
       */
      if(config.thingspeakSend_period()) {
        if((millis() - thingspeakSend) > (config.thingspeakSend_period() * 60000) or thingspeakSend == 0) {
          thingspeakSend = millis();
          if(network.isConnected()) thingspeak.send();
          else Serial.println("No internet connection");
        }
      }

      /**
       * Send data to narodmon
       */
      if(config.narodmonSend_period()) {
        if((millis() - narodmonSend) > (config.narodmonSend_period() * 60000) or narodmonSend == 0) {
          narodmonSend = millis();
          if(network.isConnected()) narodmon.send();
          else Serial.println("No internet connection");
        }
      }
    
      /**
       * Weather update
       * every 20 minutes
       */
      if(now() - weather.get_currentUpdated() > 1200) {
        if(network.isConnected()) weather.update();
        else Serial.println("No internet connection");
      }

      /**
       * Update history repository
       */
      if(now() - historyUpdate > config.history_period() * 60) {
        historyUpdate = now();
        thingspeak.sendHistory();
        thingspeak.receiveHistory();
      }
    }

    network.setNeedToPing();

    vTaskDelay(50);
  }
}

/**
 * Check if display 1 button was pressed
 */
void display1_toggle() {
  global.display_but_pressed[0] = true;
}

/**
 * Check if display 2 button was pressed
 */
void display2_toggle() {
  global.display_but_pressed[1] = true;
}

/**
 * Check if alarm button was pressed
 */
void alarm_button() {
  global.alarm_but_pressed = true;
}

/**
 * Calculate comfort level 
 */
void comfortCalculate() {
  #define UNDEFINED            0
  #define COMFORTABLE          1
  #define HOT                  2
  #define COLD                 3
  #define HUMID                4
  #define DRY                  5
  #define HOT_HUMID            6
  #define HOT_DRY              7
  #define COLD_HUMID           8
  #define COLD_DRY             9
  #define AIR_CLEAN            1
  #define AIR_POLLUTED         2
  #define AIR_HEAVILY_POLLUTED 3
  
  int temp = 40400;
  int hum = 40400;
  int tempLevel = 0;
  int humLevel = 0;
    
  switch(config.comfort_temp_source()) {
    case 1: temp = round(weather.get_currentTemp()); break; // temperature from weather forecast
    case 2: { // temperature from wireless sensor
      if(now() - wsensor.get_updated(config.comfort_temp_wsensNum()) < config.wsensor_expire(config.comfort_temp_wsensNum()) * 60)
        temp = round(wsensor.get_temperature(config.comfort_temp_wsensNum(), config.comfort_temp_sens(), config.wsensor_temp_corr(config.comfort_temp_wsensNum(), config.comfort_temp_sens())));
    }; break;
    case 3: { // temperature from thingspeak
      if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60)
        temp = round(thingspeak.get_field(config.comfort_temp_thing()));
    }; break;
    case 4: temp = round(sensors.get_bme280_temp(config.bme280_temp_corr())); break; // temperature from BME280
    case 5: temp = round(sensors.get_bmp180_temp(config.bmp180_temp_corr())); break; // temperature from BMP180
    case 6: temp = round(sensors.get_sht21_temp(config.sht21_temp_corr())); break; // temperature from SHT21
    case 7: temp = round(sensors.get_dht22_temp(config.dht22_temp_corr())); break; // temperature from DHT22
    case 8: temp = round(sensors.get_ds18b20_temp(config.ds18b20_temp_corr())); break; // temperature from DS18B20
    case 9: temp = round(sensors.get_bme680_temp(config.bme680_temp_corr())); break; // temperature from BME680
    default: temp = 40400; break;
  }
  switch(config.comfort_hum_source()) {
    case 1: hum = round(weather.get_currentHum()); break; // humidity from weather forecast
    case 2: { // humidity from wireless sensor
      if(now() - wsensor.get_updated(config.comfort_hum_wsensNum()) < config.wsensor_expire(config.comfort_hum_wsensNum()) * 60)
        hum = round(wsensor.get_humidity(config.comfort_hum_wsensNum(), config.wsensor_hum_corr(config.comfort_hum_wsensNum())));
    }; break;
    case 3: { // humidity from thingspeak
      if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60)
        hum = round(thingspeak.get_field(config.comfort_hum_thing()));
    }; break;
    case 4: hum = round(sensors.get_bme280_hum(config.bme280_hum_corr())); break; // humidity from BME280
    case 5: hum = round(sensors.get_sht21_hum(config.sht21_hum_corr())); break; // humidity from SHT21
    case 6: hum = round(sensors.get_dht22_hum(config.dht22_hum_corr())); break; // humidity from DHT22
    case 7: hum = round(sensors.get_bme680_hum(config.bme680_hum_corr())); break; // humidity from BME680
    default: temp = 40400; break;
  }

  if(sensors.checkTemp(float(temp))) {
    if(temp > config.comfort_temp_max()) tempLevel = 1;
    if(temp < config.comfort_temp_min()) tempLevel = 2;
  }
  else tempLevel = -1;

  if(sensors.checkHum(float(hum))) {
    if(hum > config.comfort_hum_max()) humLevel = 1;
    if(hum < config.comfort_hum_min()) humLevel = 2;
  }
  else humLevel = -1;

  if(tempLevel == -1 && humLevel == -1) global.comfort = UNDEFINED;
  if(tempLevel == 1 && humLevel < 1) global.comfort = HOT;
  if(tempLevel == 2 && humLevel < 1) global.comfort = COLD;
  if(tempLevel < 1 && humLevel == 1) global.comfort = HUMID;
  if(tempLevel < 1 && humLevel == 2) global.comfort = DRY;
  if(tempLevel == 1 && humLevel == 1) global.comfort = HOT_HUMID;
  if(tempLevel == 1 && humLevel == 2) global.comfort = HOT_DRY;
  if(tempLevel == 2 && humLevel == 1) global.comfort = COLD_HUMID;
  if(tempLevel == 2 && humLevel == 2) global.comfort = COLD_DRY;

  if(config.comfort_iaq_source() == 1) {
    float iaq = sensors.get_bme680_iaq(config.bme680_iaq_corr());
    if(sensors.checkIaq(iaq)) {
      global.iaq_level = AIR_CLEAN;
      if(iaq > 100.0) global.iaq_level = AIR_POLLUTED;
      if(iaq > 200.0) global.iaq_level = AIR_HEAVILY_POLLUTED;
    }
    else global.iaq_level = 0;
  }

  if(config.comfort_co2_source() == 1) {
    if(now() - wsensor.get_updated(config.comfort_co2_wsensNum()) < config.wsensor_expire(config.comfort_co2_wsensNum()) * 60) {
      float co2 = wsensor.get_co2(config.comfort_co2_wsensNum(), config.wsensor_co2_corr(config.comfort_co2_wsensNum()));
      if(wsensor.checkCo2(co2)) {
        global.co2_level = AIR_CLEAN;
        if(co2 > 800.0) global.co2_level = AIR_POLLUTED;
        if(co2 > 1400.0) global.co2_level = AIR_HEAVILY_POLLUTED;
      }
      else global.co2_level = 0;
    } 
  }
}

/**
 * Time synchronization with NTP server
 */
void get_time(void) {
  if(global.net_connected) {
    Serial.println(SEPARATOR);
    Serial.print("NTP synchronization... ");
    configTime(config.clock_utc() * 3600, 0, config.clock_ntp());
    struct tm tmstruct;
    vTaskDelay(1000);
    tmstruct.tm_year = 0;
    getLocalTime(&tmstruct);
    if(tmstruct.tm_year >= 121) {
      setTime(tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec, tmstruct.tm_mday, tmstruct.tm_mon + 1, tmstruct.tm_year + 1900);
      unsigned int summertime = config.clock_dlst() ? is_summertime() ? 3600 : 0 : 0;
      unsigned int t = now() + summertime;
      setTime(t);
      global.clockSynchronize = true;
      global.clockSynchronized = true;
      Serial.print("successful: ");
      Serial.printf("%d:%02d:%02d %02d-%02d-%d\r\n", hour(), minute(), second(), day(), month(), year());
    }
    else {
      Serial.println("failed");
      global.clockSynchronized = false;
    }
  }
  else global.clockSynchronized = false;
}

/**
 * Check the time and date for daylight saving time
 * (works with an error, changes the time at 1:00)
 */
boolean is_summertime(void) {
  if(month() < 3 || month() > 10) return false;
  if(month() > 3 && month() < 10) return true;
  if((month() == 3 && (hour() + 24 * day()) >= (1 + 24 * (31 - (5 * year() / 4 + 4) % 7))) || (month() == 10 && (hour() + 24 * day()) < (1 + 24 * (31 - (5 * year() / 4 + 1) % 7)))) return true;
  else return false;
}
