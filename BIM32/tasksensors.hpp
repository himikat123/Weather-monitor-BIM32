void display1_toggle(void);
void display2_toggle(void);
void alarm_button(void);
void comfortCalculate(void);

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

  // HC12 wireless module channel nummer request
  Serial2.println("AT+RC");
    
  while(1) {
    /**
     * Check if a button is pressed
     */
    // Enter access point mode if button is pressed
    if(digitalRead(SETTINGS_BUTTON_PIN) == 0) network.runAccessPoint();
    
    // Display 1 toogle if display 1 button was pressed
    if(global.display1_but_pressed) {
      global.display1_but_pressed = false;
      global.disp_autoOff[NEXTION] = millis();
      nextion.displayToggle();
    }

    // Display 2 toogle if display 2 button was pressed
    if(global.display2_but_pressed) {
      global.display2_but_pressed = false;
      global.disp_autoOff[WS2812B] = millis();
      ws2812b.displayToggle();
    }
    
    /**
     * Network connection if not connected and if not Access point mode
     */
    if(!global.apMode) {
      if(WiFi.localIP().toString() == "0.0.0.0" or WiFi.status() != WL_CONNECTED) {
        global.net_connected = false;
        network.connect();
        vTaskDelay(1000);
      }
      else global.net_connected = true;
    }
    
    /**
     * Time synchronization with NTP server
     */
    if(config.clock_ntp_period()) {
      if((millis() - ntp_update) > config.clock_ntp_period() * 60000 or !global.clockSynchronized) {
        ntp_update = millis();
        if(network.isConnected()) timentp.get_time();
        else Serial.println("No internet connection");
        vTaskDelay(100);
      }
    }

    /**
     * Wired sensors update
     * every 2 seconds
     */
    if(millis() - sensors_update > 2000) {
      sensors_update = millis();
      sensors.read();
    }

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

    /**
     * Receive data from thingspeak
     */
    if(config.thingspeakReceive_turnOn()) {
      if((millis() - thingspeakReceive) > (config.thingspeakReceive_period() * 60000) or thingspeakReceive == 0) {
        thingspeakReceive = millis();
        if(network.isConnected()) thingspeak.receive();
        else Serial.println("No internet connection");
      }
    }

    /**
     * Send data to thingspeak
     */
    if(config.thingspeakSend_turnOn()) {
      if((millis() - thingspeakSend) > (config.thingspeakSend_period() * 60000) or thingspeakSend == 0) {
        thingspeakSend = millis();
        if(network.isConnected()) thingspeak.send();
        else Serial.println("No internet connection");
      }
    }

    /**
     * Send data to narodmon
     */
    if(config.narodmonSend_turnOn()) {
      if((millis() - narodmonSend) > (config.narodmonSend_period() * 60000) or narodmonSend == 0) {
        narodmonSend = millis();
        if(network.isConnected()) narodmon.send();
        else Serial.println("No internet connection");
      }
    }

    /* Receive from wireless sensor */
    wsensor.receive();

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
      vTaskDelay(500);
      thingspeak.receiveHistory();
    }
    
    vTaskDelay(50);
  }
}

/**
 * Check if display 1 button was pressed
 */
void display1_toggle() {
  global.display1_but_pressed = true;
}

/**
 * Check if display 2 button was pressed
 */
void display2_toggle() {
  global.display2_but_pressed = true;
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
  int temp = 40400;
  int hum = 40400;
  
  switch(config.comfort_temp_source()) {
    case 0: temp = round(weather.get_currentTemp()); break; // temperature from weather forecast
    case 1: { // temperature from wireless sensor
      if(now() - wsensor.get_updated(config.comfort_temp_wsensNum()) < config.wsensor_expire(config.comfort_temp_wsensNum()) * 60)
        temp = round(wsensor.get_temperature(config.comfort_temp_wsensNum(), config.comfort_temp_sens(), config.wsensor_temp_corr(config.comfort_temp_wsensNum(), config.comfort_temp_sens())));
    }; break;
    case 2: { // temperature from thingspeak
      if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60)
        temp = round(thingspeak.get_field(config.comfort_temp_thing()));
    }; break;
    case 3: temp = round(sensors.get_bme280_temp(config.bme280_temp_corr())); break; // temperature from BME280
    case 4: temp = round(sensors.get_bmp180_temp(config.bmp180_temp_corr())); break; // temperature from BMP180
    case 5: temp = round(sensors.get_sht21_temp(config.sht21_temp_corr())); break; // temperature from SHT21
    case 6: temp = round(sensors.get_dht22_temp(config.dht22_temp_corr())); break; // temperature from DHT22
    case 7: temp = round(sensors.get_ds18b20_temp(config.ds18b20_temp_corr())); break; // temperature from DS18B20
    default: temp = 40400; break;
  }
  switch(config.comfort_hum_source()) {
    case 0: hum = round(weather.get_currentHum()); break; // humidity from weather forecast
    case 1: { // humidity from wireless sensor
      if(now() - wsensor.get_updated(config.comfort_hum_wsensNum()) < config.wsensor_expire(config.comfort_hum_wsensNum()) * 60)
        hum = round(wsensor.get_humidity(config.comfort_hum_wsensNum(), config.wsensor_hum_corr(config.comfort_hum_wsensNum())));
    }; break;
    case 2: { // humidity from thingspeak
      if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60)
        hum = round(thingspeak.get_field(config.comfort_hum_thing()));
    }; break;
    case 3: hum = round(sensors.get_bme280_hum(config.bme280_hum_corr())); break; // humidity from BME280
    case 4: hum = round(sensors.get_sht21_hum(config.sht21_hum_corr())); break; // humidity from SHT21
    case 5: hum = round(sensors.get_dht22_hum(config.dht22_hum_corr())); break; // humidity from DHT22
    default: temp = 40400; break;
  }
  
  if(sensors.checkTemp(temp) and sensors.checkHum(hum)) {
    if(temp >= config.comfort_temp_min() and temp <= config.comfort_temp_max() and hum >= config.comfort_hum_min() and hum <= config.comfort_hum_max()) {
      digitalWrite(AIR_HUMIDIFIER_PIN, LOW);
      digitalWrite(AIR_DRYER_PIN, LOW);
      digitalWrite(AIR_HEATER_PIN, LOW);
      digitalWrite(AIR_COOLER_PIN, LOW);
      global.comfort = 1;
    }
    if(temp > config.comfort_temp_max() and hum >= config.comfort_hum_min() and hum <= config.comfort_hum_max()) {
      digitalWrite(AIR_HUMIDIFIER_PIN, LOW);
      digitalWrite(AIR_DRYER_PIN, LOW);
      digitalWrite(AIR_HEATER_PIN, LOW);
      digitalWrite(AIR_COOLER_PIN, HIGH);
      global.comfort = 2;
    }
    if(temp < config.comfort_temp_min() and hum >= config.comfort_hum_min() and hum <= config.comfort_hum_max()) {
      digitalWrite(AIR_HUMIDIFIER_PIN, LOW);
      digitalWrite(AIR_DRYER_PIN, LOW);
      digitalWrite(AIR_HEATER_PIN, HIGH);
      digitalWrite(AIR_COOLER_PIN, LOW);
      global.comfort = 3;
    }
    if(temp >= config.comfort_temp_min() and temp <= config.comfort_temp_max() and hum < config.comfort_hum_min()) {
      digitalWrite(AIR_HUMIDIFIER_PIN, HIGH);
      digitalWrite(AIR_DRYER_PIN, LOW);
      digitalWrite(AIR_HEATER_PIN, LOW);
      digitalWrite(AIR_COOLER_PIN, LOW);
      global.comfort = 4;
    }
    if(temp >= config.comfort_temp_min() and temp <= config.comfort_temp_max() and hum > config.comfort_hum_max()) {
      digitalWrite(AIR_HUMIDIFIER_PIN, LOW);
      digitalWrite(AIR_DRYER_PIN, HIGH);
      digitalWrite(AIR_HEATER_PIN, LOW);
      digitalWrite(AIR_COOLER_PIN, LOW);
      global.comfort = 5;
    }
    if(temp > config.comfort_temp_max() and hum > config.comfort_hum_max()) {
      digitalWrite(AIR_HUMIDIFIER_PIN, LOW);
      digitalWrite(AIR_DRYER_PIN, HIGH);
      digitalWrite(AIR_HEATER_PIN, LOW);
      digitalWrite(AIR_COOLER_PIN, HIGH);
      global.comfort = 6;
    }
    if(temp > config.comfort_temp_max() and hum < config.comfort_hum_min()) {
      digitalWrite(AIR_HUMIDIFIER_PIN, HIGH);
      digitalWrite(AIR_DRYER_PIN, LOW);
      digitalWrite(AIR_HEATER_PIN, LOW);
      digitalWrite(AIR_COOLER_PIN, HIGH);
      global.comfort = 7;
    }
    if(temp < config.comfort_temp_min() and hum > config.comfort_hum_max()) {
      digitalWrite(AIR_HUMIDIFIER_PIN, LOW);
      digitalWrite(AIR_DRYER_PIN, HIGH);
      digitalWrite(AIR_HEATER_PIN, HIGH);
      digitalWrite(AIR_COOLER_PIN, LOW);
      global.comfort = 8;
    }
    if(temp < config.comfort_temp_min() and hum < config.comfort_hum_min()) {
      digitalWrite(AIR_HUMIDIFIER_PIN, HIGH);
      digitalWrite(AIR_DRYER_PIN, LOW);
      digitalWrite(AIR_HEATER_PIN, HIGH);
      digitalWrite(AIR_COOLER_PIN, LOW);
      global.comfort = 9;
    }
  }
  else {
    digitalWrite(AIR_HUMIDIFIER_PIN, LOW);
    digitalWrite(AIR_DRYER_PIN, LOW);
    digitalWrite(AIR_HEATER_PIN, LOW);
    digitalWrite(AIR_COOLER_PIN, LOW);
    global.comfort = 0;
  }
}
