void TaskServer(void *pvParameters) {
    (void) pvParameters;

    while(1) {
        /**
         * BME680 sensor update
         * try as often as possible, the sensor will update when it decides
         */
        sensors.BME680Read();

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