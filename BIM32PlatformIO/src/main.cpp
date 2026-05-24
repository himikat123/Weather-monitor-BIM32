/**
 *  Weather Monitor BIM32 v6.0a
 *  https://github.com/himikat123/Weather-monitor-BIM32
 *
 *  © himikat123@gmail.com, Nürnberg, Deutschland, 2020-2026
 *
 */

#include <Arduino.h>
#include <WiFi.h>
#include "FS.h"
#include <LittleFS.h>
#include "./globals.hpp"
#include "./pinout.hpp"
#include "./config/config.hpp"
#include "./network/network.hpp"
#include "./wsensor/wirelessSensor.hpp"
#include "./displays/ili9341/ili9341.hpp"
#include "./displays/nextion/nextion.hpp"

#include "./taskDisplay/taskDisplay.hpp"
#include "./taskSensors/taskSensors.hpp"
#include "./taskServer/taskServer.hpp"

TaskHandle_t task_display1_handle = NULL;
TaskHandle_t task_display2_handle = NULL;
TaskHandle_t task_server_handle = NULL;
TaskHandle_t task_sensors_handle = NULL;
SemaphoreHandle_t sensorsSemaphore = NULL;
uint8_t dummy = 0;

TaskDisplay taskDisplay1;
TaskDisplay taskDisplay2;
TaskSensors taskSensors;
TaskServer taskServer;

/**
 * Arduino setup
 */
void setup() {
    sensorsSemaphore = xSemaphoreCreateMutex(); 

    pinMode(HC12_SET_PIN, OUTPUT);
    digitalWrite(HC12_SET_PIN, HIGH);
    pinMode(DISPLAY1_BUTTON_PIN, INPUT_PULLUP);
    pinMode(DISPLAY2_BUTTON_PIN, INPUT_PULLUP);
    pinMode(ALARM_BUTTON_PIN, INPUT_PULLUP);
    pinMode(MP3_BUSY_PIN, INPUT_PULLUP);

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

    int disp1type = config.display.type(DISPLAY_1);
    int disp1model = config.display.model(DISPLAY_1); 
    if(disp1type == LCD_DISPLAY) {
        if(disp1model == D_NX4832K035 or disp1model == D_NX4832T035 or disp1model == D_NX4827K043) {
            Serial1.begin(115200, SERIAL_8N1, NEXTION_RX_PIN, NEXTION_TX_PIN);
            nextion.showLogo();
        }
        if(disp1model == D_ILI9341) {
            ili9341.init();
            ili9341.showLogo();
        }
    }

    if(disp1type && taskDisplay1.start("TaskDisplay1", 32768, 1, 1, DISPLAY_1));
    else Serial.println("Failed to start Display1 task!");
    if(disp1type && taskDisplay2.start("TaskDisplay2", 8192, 1, 1, DISPLAY_2));
    else Serial.println("Failed to start Display2 task!");

    WiFi.mode(WIFI_STA);
    network.connect();

    if(taskSensors.start("TaskSensors", 32768, 1, 1));
    else Serial.println("Failed to start Sensors task!");
    if(taskServer.start("TaskServer", 16384, 1, 1));
    else Serial.println("Failed to start Server task!");
}

void loop() {}