/**
 *  Weather Monitor BIM32 v4.6
 *  https://github.com/himikat123/Weather-monitor-BIM32
 *
 *  © himikat123@gmail.com, Nürnberg, Deutschland, 2020-2024
 *
 *  Board: ESP32 Dev Module
 *  Partition sheme: No OTA (2MB APP/2MB SPIFFS)
 *
 *  Arduino IDE v1.8.19
 *  ESP32 board by Espressif Systems v2.0.14
 */


/* Arduino libraries */
#include <Arduino.h>
#include <ArduinoOTA.h>
#include "FS.h"
#include <LittleFS.h>
#include <Update.h>
#include <WiFi.h>
#include <HTTPClient.h>
WiFiClient client;
#include <WebServer.h>
WebServer server(80);
#include <ESPmDNS.h>
#include <time.h>

/* External Libraries */
#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "ESP32SSDP.h" // v1.2.1 https://github.com/luc-github/ESP32SSDP

/* Pictures */
#include "src/picturesAnts.hpp"
#include "src/picturesBats.hpp"
#include "src/picturesIconsBig.hpp"
#include "src/picturesIconsSmall.hpp"
#include "src/picturesIconsTiny.hpp"
#include "src/picturesLogo.hpp"
#include "src/picturesNumbers.hpp"
#include "src/picturesSymbols.hpp"
#include "src/picturesWind.hpp"
#include "src/picturesWindTiny.hpp"

/* Own classes */
#include "src/validate.hpp"
Validate validate;
#include "src/globals.hpp"
Configuration config;
#include "src/sensors.hpp"
Sensors sensors;
#include "src/languages.hpp"
Lang lang;
#include "src/wirelessSensor.hpp"
WirelessSensor wsensor;
#include "src/weather.hpp"
Weather weather;
#include "src/sound.hpp"
Sound sound;
#include "src/agregateSendData.hpp"
AgregateSendData agregateSendData;
#include "src/thingspeak.hpp"
Thingspeak thingspeak;
#include "src/narodmon.hpp"
Narodmon narodmon;
#include "src/mqtt.hpp"
MQTT mqtt;
#include "src/fonts.hpp"
#include "src/agregateLcdData.hpp"
AgregateLcdData agregateLcdData;
#include "src/lcdDisplay.hpp"
#include "src/nextion.hpp"
Nextion nextion;
#include "src/ili9341.hpp"
ILI9341 ili9341;
#include "src/agregateSegmentData.hpp"
AgregateSegmentData agregateSegmentData;
#include "src/segmentAnimationsShifts.hpp"
#include "src/segmentDisplay.hpp"
#include "src/ws2812b.hpp"
WS2812b ws2812b_1;
WS2812b ws2812b_2;
#include "src/network.hpp"
Network network;
#include "src/agregateComfortData.hpp"
AgregateComfortData agregateComfortData;
#include "src/comfort.hpp"
Comfort comfort;

#include "taskdisplay.hpp"
#include "tasksensors.hpp"

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
    Serial.printf("*  Weather Monitor BIM32 %s    © himikat123@gmail.com   2020-2024  *\r\n", FW);
    Serial.println(SEPARATOR);
    Serial.println();

    if(!LittleFS.begin()) {
        Serial.println("LittleFS initialisation failed!");
        while(1) yield();
    }
    config.readConfig();

    if(config.display_type(0) == LCD) {
        if(config.display_model(0) == D_NX4832K035 or config.display_model(0) == D_NX4832T035) {
            Serial1.begin(115200, SERIAL_8N1, NEXTION_RX_PIN, NEXTION_TX_PIN);
            nextion.showLogo();
        }
        if(config.display_model(DISPLAY_1) == D_ILI9341) {
            ili9341.init();
            ili9341.showLogo();
        }
    }

    xTaskCreatePinnedToCore(TaskDisplay1, "TaskDisplay1", 32768, NULL, 1, &task_display1_handle, 1);
    xTaskCreatePinnedToCore(TaskDisplay2, "TaskDisplay2", 32768, NULL, 1, &task_display2_handle, 1);

    network.connect();

    webInterface_init();

    xTaskCreatePinnedToCore(TaskSensors, "TaskSensors", 32768, NULL, 1, &task_sensors_handle, 1);
}

void loop() {

}