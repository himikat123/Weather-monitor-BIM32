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
         * Handle server & websockets
         */
        server.handleClient();
        websocket.loop();

        if(websocket.hasClient()) {
            if(millis() - ws_update >= 1000) {
                ws_update = millis();
                JsonDocument doc;
                JsonObject root = doc.to<JsonObject>();

                if(websocket.available()) {
                    String msg = websocket.read();
                    if(msg.length()) {
                        doc["state"] = (String(state.secretCode) == msg) ? "OK" : "LOGIN";
                        Serial.println(String(state.secretCode));
                        Serial.println(msg);
                    }
                }

                doc["runtime"] = esp_timer_get_time() / 1000ULL;
                doc["heap"] = ESP.getFreeHeap();
                doc["time"] = now();

                if(state.wsensor.updated) doc["wsensRaw"] = String(wsensor.get_rawData());
                
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
                updateIfNeeded(state.thing, root);
                updateIfNeeded(state.weather, root);
                updateIfNeeded(state.wsensor, root);
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
         * Automatic daily restart of the device at the specified time.
         * Uncomment this line and specify the time if you need autorestart. 
         */
        // if(hour() == 17 and minute() == 0 and second() == 0) ESP.restart();
        
        vTaskDelay(10);
    }
}