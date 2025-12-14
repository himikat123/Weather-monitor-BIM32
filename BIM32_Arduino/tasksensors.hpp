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

    pinMode(ALARM_BUTTON_PIN, INPUT);
    attachInterrupt(ALARM_BUTTON_PIN, alarm_button, FALLING);
    attachInterrupt(MP3_BUSY_PIN, mp3_busy, FALLING);

    sound.init();
    mqtt.init();

    network.scanNetworks();

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

            if(state.apMode) strlcpy(state.network.ssid, config.accessPoint.ssid(), sizeof(state.network.ssid));
            else WiFi.SSID().toCharArray(state.network.ssid, sizeof(state.network.ssid));
            state.network.ch = WiFi.channel();
            state.network.sig = WiFi.RSSI();
            uint8_t mac[6];
            WiFi.macAddress(mac);
            snprintf(state.network.mac, sizeof(state.network.mac), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            IPAddress ip = WiFi.localIP();
            IPAddress mask = WiFi.subnetMask();
            IPAddress gw = WiFi.gatewayIP();
            IPAddress dns1 = WiFi.dnsIP();
            IPAddress dns2 = WiFi.dnsIP();
            snprintf(state.network.ip, sizeof(state.network.ip), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
            snprintf(state.network.mask, sizeof(state.network.mask), "%u.%u.%u.%u", mask[0], mask[1], mask[2], mask[3]);
            snprintf(state.network.gw, sizeof(state.network.gw), "%u.%u.%u.%u", gw[0], gw[1], gw[2], gw[3]);
            snprintf(state.network.dns1, sizeof(state.network.dns1), "%u.%u.%u.%u", dns1[0], dns1[1], dns1[2], dns1[3]);
            snprintf(state.network.dns2, sizeof(state.network.dns2), "%u.%u.%u.%u", dns2[0], dns2[1], dns2[2], dns2[3]);
            state.network.updated = true;
        }

        wsensor.receive(); /* Receive from wireless sensor */

        /**
         * Network connection if not connected and if not Access point mode
         */
        if(WiFi.localIP().toString() == "0.0.0.0" or !network.isConnected()) {
            state.net_connected = false;
            network.connect();
            vTaskDelay(1000);
        }
        else state.net_connected = true;
        
        if(!state.apMode) {
            /**
             * Time synchronization with NTP server
             */
            if(config.clock.ntpPeriod() > 0) {
                if((millis() - ntp_update) > config.clock.ntpPeriod() * 60000 or !state.clockSynchronized) {
                    ntp_update = millis();
                    Serial.println(SEPARATOR);
                    Serial.println("NTP synchronization... ");
                    if(network.isConnected()) get_time();
                    else {
                        state.clockSynchronized = false;
                        Serial.println("No internet connection");
                    }
                    vTaskDelay(100);
                }
            }

            /**
             * Receive data from thingspeak
             */
            if(config.cloud.thingspeakReceive.period() > 0) {
                if((millis() - thingspeakReceive) > (config.cloud.thingspeakReceive.period() * 60000) or thingspeakReceive == 0) {
                    thingspeakReceive = millis();
                    Serial.println(SEPARATOR);
                    Serial.println("Receive data from thingspeak.com... ");
                    if(network.isConnected()) thingspeak.receive();
                    else {
                        thingspeakReceive = 0;
                        Serial.println("No internet connection");
                    }
                }
            }

            /**
             * Send data to thingspeak
             */
            if(config.cloud.thingspeakSend.period() > 0) {
                if((millis() - thingspeakSend) > (config.cloud.thingspeakSend.period() * 60000) or thingspeakSend == 0) {
                    thingspeakSend = millis();
                    Serial.println(SEPARATOR);
                    Serial.println("Send data to thingspeak.com... ");
                    if(network.isConnected()) thingspeak.send();
                    else {
                        thingspeakSend = 0;
                        Serial.println("No internet connection");
                    }
                }
            }

            /**
             * Send data to narodmon
             */
            if(config.cloud.narodmonSend.period() > 0) {
                if((millis() - narodmonSend) > (config.cloud.narodmonSend.period() * 60000) or narodmonSend == 0) {
                    narodmonSend = millis();
                    Serial.println(SEPARATOR);
                    Serial.println("Send data to narodmon.ru... ");
                    if(network.isConnected()) narodmon.send();
                    else {
                        narodmonSend = 0;
                        Serial.println("No internet connection");
                    }
                }
            }

            /**
             * Send data via MQTT
             */
            if(config.cloud.mqttSend.period() > 0 and network.isConnected()) {
                if(mqtt.loop()) {
                    if((millis() - mqttSend) > (config.cloud.mqttSend.period() * 1000) or mqttSend == 0) {
                        mqttSend = millis();
                        Serial.println(SEPARATOR);
                        Serial.println("Send data via MQTT...");
                        if(network.isConnected()) mqtt.send();
                        else {
                            mqttSend = 0;
                            Serial.println("No internet connection");
                        }
                    }
                }
            }
    
            /**
             * Weather update
             * every 20 minutes, or every hour for weatherbit.io
             */
            uint32_t weatherUpd = config.weather.provider() == 1 ? 3600 : 1200;
            if(state.debugWether || (now() - weather.get_currentUpdated() > weatherUpd)) {
                Serial.println(SEPARATOR);
                Serial.println("Current weather update... ");
                if(network.isConnected()) weather.update();
                else Serial.println("No internet connection");
                sensors.get_ds3231_timeDate();
                state.debugWether = false;
            }

            /**
             * Update history repository
             */
            if(config.cloud.history.period() > 0) {
                if(now() - historyUpdate > config.cloud.history.period() * 60) {
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
    if(millis() - state.btnMillis[0] > 500) {
        state.btnMillis[0] = millis();
        state.display_btn_pressed[0] = true;
    }
}

/**
 * Interrupt from display 2 button
 */
void display2_toggle() {
    if(millis() - state.btnMillis[1] > 500) {
        state.btnMillis[1] = millis();
        state.display_btn_pressed[1] = true;
    }
}

/**
 * Interrupt from alarm button
 */
void alarm_button() {
    state.alarm_but_pressed = true;
    //sound.stopPlaying();
}

/**
 * Interrupt from mp3 player busy pin
 */
void mp3_busy() {
    state.mp3_busy = false;
}

/**
 * Time synchronization with NTP server
 */
void get_time(void) {
    if(state.net_connected) {
        configTime(config.clock.utc() * 3600, 0, config.clock.ntp());
        struct tm tmstruct;
        vTaskDelay(1000);
        tmstruct.tm_year = 0;
        getLocalTime(&tmstruct);
        if(tmstruct.tm_year >= 121) {
            setTime(tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec, tmstruct.tm_mday, tmstruct.tm_mon + 1, tmstruct.tm_year + 1900);
            unsigned int summertime = config.clock.dlst() ? is_summertime() ? 3600 : 0 : 0;
            unsigned int t = now() + summertime;
            setTime(t);
            state.clockSynchronize = true;
            state.clockSynchronized = true;
            nextion.setDisplayRTC();
            sensors.set_ds3231_timeDate();
            Serial.print("successful: ");
            Serial.printf("%d:%02d:%02d %02d.%02d.%d\r\n", hour(), minute(), second(), day(), month(), year());
        }
        else {
            Serial.println("failed");
            state.clockSynchronized = false;
        }
    }
    else state.clockSynchronized = false;
}

/**
 * Check the time and date for daylight saving time
 */
boolean is_summertime() {
    if(month() < 3 || month() > 10) return false;
    if(month() > 3 && month() < 10) return true;
    if((month() == 3 && (hour() + 24 * day()) >= (1 + 24 * (31 - (5 * year() / 4 + 4) % 7))) || (month() == 10 && (hour() + 24 * day()) < (1 + 24 * (31 - (5 * year() / 4 + 1) % 7)))) return true;
    else return false;
}