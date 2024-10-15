void TaskServer(void *pvParameters) {
    (void) pvParameters;
    sensorsSemaphore = xSemaphoreCreateMutex();

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

        /**
         * Automatic daily restart of the device at the specified time.
         * Uncomment this line and specify the time if you need autorestart. 
         */
        // if(hour() == 12 and minute() == 0 and second() == 0) ESP.restart();
        
        vTaskDelay(10);
    }
}