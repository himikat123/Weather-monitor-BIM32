void display1_toggle();
void display2_toggle();
void alarm_button();
void mp3_busy();
void comfortCalculate();
void get_time();
boolean is_summertime();

void TaskSensors(void *pvParameters) {
    (void) pvParameters;

    sensors.init();

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
    attachInterrupt(MP3_BUSY_PIN, mp3_busy, FALLING);

    // HC12 wireless module channel number request
    digitalWrite(SET_HC12_PIN, LOW);
    delay(50);
    Serial2.println("AT+RC");
    delay(100);
    digitalWrite(SET_HC12_PIN, HIGH);

    sound.init();

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
            sensors.read();
            comfort.calculate();
            comfort.soundNotify();
        }

        /**
         * BME680 sensor update
         * try as often as possible, the sensor will update when it decides
         */
        sensors.BME680Read();

        /**
         * List of available networks update
         * every 20 seconds
         */
        if(millis() - networks_update > 20000) {
            networks_update = millis();
            network.scanNetworks();
        }

        wsensor.receive(); /* Receive from wireless sensor */
        sound.hourlySignal(); /* Hourly signal */
        sound.alarm(); /* Alarm */

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
             * Weather update
             * every 20 minutes
             */
            if(now() - weather.get_currentUpdated() > 1200) {
                Serial.println(SEPARATOR);
                Serial.println("Current weather update... ");
                if(network.isConnected()) weather.update();
                else Serial.println("No internet connection");
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

        network.setNeedToPing();

        vTaskDelay(50);
    }
}

/**
 * Interrupt from display 1 button
 */
void display1_toggle() {
    global.display_but_pressed[0] = true;
}

/**
 * Interrupt from display 2 button
 */
void display2_toggle() {
    global.display_but_pressed[1] = true;
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