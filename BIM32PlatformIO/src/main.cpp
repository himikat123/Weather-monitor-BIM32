/**
 *  Weather Monitor BIM32 v6.0a
 *  https://github.com/himikat123/Weather-monitor-BIM32
 *
 *  © himikat123@gmail.com, Nürnberg, Deutschland, 2020-2026
 *
 */

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include "./globals.hpp"
#include "./pinout.hpp"
#include "./config/config.hpp"
#include "./wsensor/wirelessSensor.hpp"
#include "./displays/ili9341/ili9341.hpp"
#include "./displays/nextion/nextion.hpp"

#include "./taskDisplay.hpp"
#include "./taskSensors.hpp"
#include "./taskServer.hpp"
#include "./web.hpp"

/**
 * Arduino setup
 */
void setup() {
    pinMode(HC12_SET_PIN, OUTPUT);
    digitalWrite(HC12_SET_PIN, HIGH);
    pinMode(DISPLAY1_BUTTON_PIN, INPUT);
    pinMode(DISPLAY2_BUTTON_PIN, INPUT);
    pinMode(ALARM_BUTTON_PIN, INPUT);
    pinMode(MP3_BUSY_PIN, INPUT);

    Serial.begin(115200, SERIAL_8N1, -1, 1);
    Serial2.begin(9600);
    Serial2.onReceive([]() {
        wsensor.handleReceive();
    });

    Serial.println(SEPARATOR);
    Serial.println(SEPARATOR);
    Serial.printf("*  Weather Monitor BIM32 %s    © himikat123@gmail.com   2020-2026  *\r\n", FW);
    Serial.println(SEPARATOR);
    Serial.println();

    if(!LittleFS.begin()) {
        Serial.println("LittleFS initialisation failed!");
        while(1) yield();
    }
    config.readConfig();

    #if defined(BIM32_CYD)
        ili9341.init();
        ili9341.showLogo();
    #else
        if(config.display.type(DISPLAY_1) == LCD_DISPLAY) {
            if(config.display.model(DISPLAY_1) == D_NX4832K035 or config.display.model(0) == D_NX4832T035) {
                Serial1.begin(115200, SERIAL_8N1, NEXTION_RX_PIN, NEXTION_TX_PIN);
                nextion.showLogo();
            }
            if(config.display.model(DISPLAY_1) == D_ILI9341) {
                ili9341.init();
                ili9341.showLogo();
            }
        }
    #endif


    xTaskCreatePinnedToCore(TaskDisplay1, "TaskDisplay1", 32768, NULL, -1, &task_display1_handle, 1);
    xTaskCreatePinnedToCore(TaskDisplay2, "TaskDisplay2", 8192, NULL, -1, &task_display2_handle, 1);

    WiFi.mode(WIFI_STA);
    network.connect();

    xTaskCreatePinnedToCore(TaskSensors, "TaskSensors", 32768, NULL, 1, &task_sensors_handle, 1);
    webInterface_init();
    xTaskCreatePinnedToCore(TaskServer, "TaskServer", 16384, NULL, 1, &task_server_handle, 1);
}

void loop() {

}