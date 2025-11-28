template<typename T>
void updateIfNeeded(T& sensor, JsonObject& root) {
    if(sensor.updated) {
        sensor.toJson(root);
        sensor.updated = false;
    }
}

/**
 * Update filelist
 */
void appendToFsList(const char* text) {
    strncat(state.filesystem.list, text, sizeof(state.filesystem.list) - strlen(state.filesystem.list) - 1);
}
void listAllFilesInDir(const char* dirname) {
    File root = LittleFS.open(dirname);
    if(!root) return;
    if(!root.isDirectory()) return;

    File file = root.openNextFile();
    while(file) {
        if(file.isDirectory()) {
            char sub[128];
            snprintf(sub, sizeof(sub), "%s/", file.path());
            listAllFilesInDir(sub);
        } 
        else {
            const char* filename = file.name();
            if(strcmp(filename, "user.us") != 0) {
                appendToFsList(dirname);
                appendToFsList(filename);
                appendToFsList(":");
                char sizeBuf[16];
                snprintf(sizeBuf, sizeof(sizeBuf), "%u", file.size());
                appendToFsList(sizeBuf);
                appendToFsList(",");
            }
        }
        file = root.openNextFile();
    }
}


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

        if(websocket.hasClient()) {
            if(millis() - ws_update >= 1000) {
                ws_update = millis();
                JsonDocument doc;
                JsonObject root = doc.to<JsonObject>();

                doc["runtime"] = round(millis() / 1000);
                doc["heap"] = ESP.getFreeHeap();
                doc["time"] = now();
                
                updateIfNeeded(state.network, root);
                updateIfNeeded(state.bme280, root);
                updateIfNeeded(state.bmp180, root);
                updateIfNeeded(state.sht21, root);
                updateIfNeeded(state.dht22, root);
                updateIfNeeded(state.esp32core, root);
                updateIfNeeded(state.max44009, root);
                updateIfNeeded(state.bh1750, root);
                updateIfNeeded(state.analog, root);
                updateIfNeeded(state.bme680, root);

                //json["thing"]["time"] = thingspeak.get_updated();
                //for(unsigned int i=0; i<THNG_FIELDS; i++) {
                //    json["thing"]["data"][i] = thingspeak.get_field(i);
                //}

                updateIfNeeded(state.weather, root);

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

                updateIfNeeded(state.filesystem, root);

                String data = "";
                serializeJson(root, data);
                websocket.sendJson(data);
            }
        }

        if(state.filesystem.fsInfoUpdate) {
            state.filesystem.list[0] = '\0';
            listAllFilesInDir("/");
            state.filesystem.total = LittleFS.totalBytes();
            state.filesystem.free = state.filesystem.total - LittleFS.usedBytes();
            state.filesystem.fsInfoUpdate = false;
            state.filesystem.updated = true;
        }

        /**
         * Automatic daily restart of the device at the specified time.
         * Uncomment this line and specify the time if you need autorestart. 
         */
        // if(hour() == 17 and minute() == 0 and second() == 0) ESP.restart();
        
        vTaskDelay(10);
    }
}