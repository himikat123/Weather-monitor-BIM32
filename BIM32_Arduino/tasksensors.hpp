void display1_toggle();
void display2_toggle();
void alarm_button();
void mp3_busy();
void get_time();
boolean is_summertime();

void TaskSensors(void *pvParameters) {
    (void) pvParameters;
    attachInterrupt(DISPLAY1_BUTTON_PIN, display1_toggle, FALLING);
    attachInterrupt(DISPLAY2_BUTTON_PIN, display2_toggle, FALLING);

    sensors.init();

    unsigned int ntp_update = 0;
    unsigned int sensors_update = 0;
    unsigned int networks_update = 0;
    unsigned int thingspeakReceive = 0;
    unsigned int thingspeakSend = 0;
    unsigned int narodmonSend = 0;
    unsigned int mqttSend = 0;
    unsigned int historyUpdate = 0;

    // HC12 wireless module channel number request
    digitalWrite(HC12_SET_PIN, LOW);
    delay(50);
    Serial2.println("AT+RC");
    Serial2.flush();
    delay(100);
    digitalWrite(HC12_SET_PIN, HIGH);

    attachInterrupt(ALARM_BUTTON_PIN, alarm_button, FALLING);
    attachInterrupt(MP3_BUSY_PIN, mp3_busy, FALLING);

    sound.init();
    mqtt.init();

    while(1) {
        // Enter access point mode if "Settings" button is pressed
        if(digitalRead(SETTINGS_BUTTON_PIN) == 0) network.runAccessPoint();

        /**
         * Wired sensors update
         * & comfort calculate. 
         * every 5 seconds
         */
        if(millis() - sensors_update > 5000) {
            sensors_update = millis();
            if(sensorsSemaphore != NULL) {
                if(xSemaphoreTake(sensorsSemaphore, (TickType_t)100) == pdTRUE) {
                    sensors.read();
                    xSemaphoreGive(sensorsSemaphore);
                }
            }
            comfort.calculate();
            comfort.devicesControl();
        }

        /**
         * List of available networks update
         * every 20 seconds
         */
        if(millis() - networks_update > 20000) {
            networks_update = millis();
            network.scanNetworks();
        }

        wsensor.receive(); /* Receive from wireless sensor */

        if(!global.apMode) {
            /**
             * Network connection if not connected and if not Access point mode
             */
            if(WiFi.localIP().toString() == "0.0.0.0" or !network.isConnected()) {
                global.net_connected = false;
                network.connect();
                vTaskDelay(1000);
            }
            else global.net_connected = true;

            /**
             * Time synchronization with NTP server
             */
            if(config.clock_ntp_period() > 0) {
                if((millis() - ntp_update) > config.clock_ntp_period() * 60000 or !global.clockSynchronized) {
                    ntp_update = millis();
                    Serial.println(SEPARATOR);
                    Serial.print("NTP synchronization... ");
                    if(network.isConnected()) get_time();
                    else Serial.println("No internet connection");
                    vTaskDelay(100);
                }
            }

            /**
             * Receive data from thingspeak
             */
            if(config.thingspeakReceive_period() > 0) {
                if((millis() - thingspeakReceive) > (config.thingspeakReceive_period() * 60000) or thingspeakReceive == 0) {
                    thingspeakReceive = millis();
                    Serial.println(SEPARATOR);
                    Serial.println("Receive data from thingspeak.com... ");
                    if(network.isConnected()) thingspeak.receive();
                    else Serial.println("No internet connection");
                }
            }

            /**
             * Send data to thingspeak
             */
            if(config.thingspeakSend_period() > 0) {
                if((millis() - thingspeakSend) > (config.thingspeakSend_period() * 60000) or thingspeakSend == 0) {
                    thingspeakSend = millis();
                    Serial.println(SEPARATOR);
                    Serial.println("Send data to thingspeak.com... ");
                    if(network.isConnected()) thingspeak.send();
                    else Serial.println("No internet connection");
                }
            }

            /**
             * Send data to narodmon
             */
            if(config.narodmonSend_period() > 0) {
                if((millis() - narodmonSend) > (config.narodmonSend_period() * 60000) or narodmonSend == 0) {
                    narodmonSend = millis();
                    Serial.println(SEPARATOR);
                    Serial.println("Send data to narodmon.ru... ");
                    if(network.isConnected()) narodmon.send();
                    else Serial.println("No internet connection");
                }
            }

            /**
             * Send data via MQTT
             */
            if(config.mqttSend_period() > 0 and network.isConnected()) {
                if(mqtt.loop()) {
                    if((millis() - mqttSend) > (config.mqttSend_period() * 1000) or mqttSend == 0) {
                        mqttSend = millis();
                        Serial.println(SEPARATOR);
                        Serial.println("Send data via MQTT...");
                        if(network.isConnected()) mqtt.send();
                        else Serial.println("No internet connection");
                    }
                }
            }
    
            /**
             * Weather update
             * every 20 minutes
             */
            if(now() - weather.get_currentUpdated() > 1200) {
                Serial.println(SEPARATOR);
                Serial.println("Current weather update... ");
                if(network.isConnected()) weather.update();
                else Serial.println("No internet connection");
                sensors.get_ds3231_timeDate();
            }

            /**
             * Update history repository
             */
            if(config.history_period() > 0) {
                if(now() - historyUpdate > config.history_period() * 60) {
                    historyUpdate = now();
                    Serial.println(SEPARATOR);
                    Serial.println("Send data to weather history repository... ");
                    if(network.isConnected()) thingspeak.sendHistory();
                    else Serial.println("No internet connection");
                    Serial.println(SEPARATOR);
                    Serial.println("Receive data from weather history repository... ");
                    if(network.isConnected()) thingspeak.receiveHistory();
                    else Serial.println("No internet connection");
                }
            }
        }

        comfort.soundNotify();
        sound.hourlySignal(); /* Hourly signal */
        sound.alarm(); /* Alarm */
        network.setNeedToPing();

        vTaskDelay(50);
    }
}

/**
 * Interrupt from display 1 button
 */
void display1_toggle() {
    if(millis() - global.btnMillis[0] > 500) {
        global.btnMillis[0] = millis();
        global.display_but_pressed[0] = true;
    }
}

/**
 * Interrupt from display 2 button
 */
void display2_toggle() {
    if(millis() - global.btnMillis[1] > 500) {
        global.btnMillis[1] = millis();
        global.display_but_pressed[1] = true;
    }
}

/**
 * Interrupt from alarm button
 */
void alarm_button() {
    global.alarm_but_pressed = true;
    sound.stopPlaying();
}

/**
 * Interrupt from mp3 player busy pin
 */
void mp3_busy() {
    global.mp3_busy = false;
}

/**
 * Time synchronization with NTP server
 */
void get_time(void) {
    if(global.net_connected) {
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
            nextion.setDisplayRTC();
            sensors.set_ds3231_timeDate();
            Serial.print("successful: ");
            Serial.printf("%d:%02d:%02d %02d.%02d.%d\r\n", hour(), minute(), second(), day(), month(), year());
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