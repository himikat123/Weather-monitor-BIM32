void TaskServer(void *pvParameters) {
    (void) pvParameters;
    sensorsSemaphore = xSemaphoreCreateMutex();
    static unsigned long ws_update = 0;
    String web_filelist = "";
    size_t fsUsed = 0;
    size_t fsTotal = 0;

    while(1) {
        /**
         * BME680 sensor update
         * try as often as possible, the sensor will update when it decides
         */
        if(sensorsSemaphore != NULL) {
            if(xSemaphoreTake(sensorsSemaphore, (TickType_t)100) == pdTRUE) {
                sensors.BME680Read();
                xSemaphoreGive(sensorsSemaphore);
            }
        }

        /**
         * Handle server
         */
        server.handleClient();
        websocket.loop();

        // Example: send JSON if client connected
        if(websocket.hasClient()) {
            if(millis() - ws_update >= 1000) {
                ws_update = millis();
                JsonDocument doc;
                JsonObject root = doc.to<JsonObject>();

                //json["state"] = web_isLogged(false) ? "OK" : "LOGIN";
                doc["runtime"] = round(millis() / 1000);
                doc["heap"] = ESP.getFreeHeap();
                doc["time"] = now();

                //for(unsigned int i=0; i<global.nets; i++) {
                //    json["ssids"][i][0] = global.ssids[i];
                //    json["ssids"][i][1] = global.rssis[i];  
                //}
                //json["network"]["ssid"] = global.apMode ? config.accessPoint_ssid() : WiFi.SSID();
                //json["network"]["ch"] = WiFi.channel();
                //json["network"]["sig"] = WiFi.RSSI();
                //json["network"]["mac"] = WiFi.macAddress();
                //json["network"]["ip"] = WiFi.localIP().toString();
                //json["network"]["mask"] = WiFi.subnetMask().toString();
                //json["network"]["gw"] = WiFi.gatewayIP().toString();
                //json["network"]["dns1"] = WiFi.dnsIP().toString();
                //json["network"]["dns2"] = WiFi.dnsIP().toString();

                if(state.bme280.updated) {
                    state.bme280.toJson(root);
                    state.bme280.updated = false;
                }
                if(state.bmp180.updated) {
                    state.bmp180.toJson(root);
                    state.bmp180.updated = false;
                }
                if(state.sht21.updated) {
                    state.sht21.toJson(root);
                    state.sht21.updated = false;
                }
                if(state.dht22.updated) {
                    state.dht22.toJson(root);
                    state.dht22.updated = false;
                }
                if(state.esp32core.updated) {
                    state.esp32core.toJson(root);
                    state.esp32core.updated = false;
                }
                if(state.max44009.updated) {
                    state.max44009.toJson(root);
                    state.max44009.updated = false;
                }
                if(state.bh1750.updated) {
                    state.bh1750.toJson(root);
                    state.bh1750.updated = false;
                }
                if(state.analog.updated) {
                    state.analog.toJson(root);
                    state.analog.updated = false;
                }
                if(state.bme680.updated) {
                    state.bme680.toJson(root);
                    state.bme680.updated = false;
                }

                //json["thing"]["time"] = thingspeak.get_updated();
                //for(unsigned int i=0; i<THNG_FIELDS; i++) {
                //    json["thing"]["data"][i] = thingspeak.get_field(i);
                //}

                //json["weather"]["temp"] = weather.get_currentTemp(RAW);
                //json["weather"]["hum"] = weather.get_currentHum(RAW);
                //json["weather"]["pres"] = weather.get_currentPres(RAW);
                //json["weather"]["wind"]["speed"] = weather.get_currentWindSpeed();
                //json["weather"]["wind"]["dir"] = weather.get_currentWindDir();
                //json["weather"]["descript"] = weather.get_description();
                //json["weather"]["time"] = weather.get_currentUpdated();
                //json["weather"]["icon"] = weather.get_currentIcon();
                //json["weather"]["isDay"] = weather.get_isDay();
                //for(uint8_t i=0; i<4; i++) {
                //    json["weather"]["daily"]["tMax"][i] = weather.get_dailyDayTemp(i);
                //    json["weather"]["daily"]["tMin"][i] = weather.get_dailyNightTemp(i);
                //    json["weather"]["daily"]["wind"][i] = weather.get_dailyWindSpeed(i);
                //    json["weather"]["daily"]["icon"][i] = weather.get_dailyIcon(i);
               // }

                //for(unsigned int i=0; i<WSENSORS; i++) {
                //    json["wsensor"]["time"][i] = wsensor.get_updated(i);
                //    for(unsigned int n=0; n<5; n++) {
                //        json["wsensor"]["temp"]["data"][n][i] = wsensor.get_temperature(i, n, RAW);
                //        json["wsensor"]["temp"]["name"][n][i] = wsensor.get_sensorType(i, n);
                //    }
                //    json["wsensor"]["hum"]["data"][i] = wsensor.get_humidity(i, RAW);
                //    json["wsensor"]["hum"]["name"][i] = wsensor.get_sensorType(i, 0);
                //    json["wsensor"]["pres"]["data"][i] = wsensor.get_pressure(i, RAW);
                //    json["wsensor"]["pres"]["name"][i] = wsensor.get_sensorType(i, 0);
                //    json["wsensor"]["light"]["data"][i] = wsensor.get_light(i, RAW);
                //    json["wsensor"]["light"]["name"][i] = wsensor.get_lightType(i);
                //    json["wsensor"]["voltage"]["data"][i] = wsensor.get_voltage(i, RAW);
                //    json["wsensor"]["voltage"]["name"][i] = wsensor.get_energyType();
                //    json["wsensor"]["current"]["data"][i] = wsensor.get_current(i, RAW);
                //    json["wsensor"]["current"]["name"][i] = wsensor.get_energyType();
                //    json["wsensor"]["power"]["data"][i] = wsensor.get_power(i, RAW);
                //    json["wsensor"]["power"]["name"][i] = wsensor.get_energyType();
                //    json["wsensor"]["energy"]["data"][i] = wsensor.get_energy(i, RAW);
                //    json["wsensor"]["energy"]["name"][i] = wsensor.get_energyType();
                //    json["wsensor"]["freq"]["data"][i] = wsensor.get_frequency(i, RAW);
                //    json["wsensor"]["freq"]["name"][i] = wsensor.get_energyType();
                //    json["wsensor"]["co2"]["data"][i] = wsensor.get_co2(i, RAW);
                //    json["wsensor"]["co2"]["name"][i] = wsensor.get_co2Type();
                //    json["wsensor"]["bat"][i] = wsensor.get_batteryAdc(i);
                //}

                //if(global.fsInfoUpdate) {
                //    web_filelist = String();
                //    web_listAllFilesInDir("/");
                //    fsTotal = LittleFS.totalBytes();
                //    fsUsed = fsTotal - LittleFS.usedBytes();
                //    global.fsInfoUpdate = false;
                //}
                //json["fs"]["list"] = web_filelist;
                //json["fs"]["total"] = fsTotal;
                //json["fs"]["free"] = fsUsed;

                String data = "";
                serializeJson(root, data);
                websocket.sendJson(data);
            }
        }

        /**
         * Automatic daily restart of the device at the specified time.
         * Uncomment this line and specify the time if you need autorestart. 
         */
        // if(hour() == 17 and minute() == 0 and second() == 0) ESP.restart();
        
        vTaskDelay(10);
    }
}