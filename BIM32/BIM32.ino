/**
 *  Weather Monitor BIM32 v4.0
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
#include <ESPmDNS.h>
#include <time.h>

/* External Libraries */
#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include <AsyncTCP.h> // v1.1.4 https://github.com/dvarrel/AsyncTCP
#include <ESPAsyncWebServer.h> // v1.2.3 https://github.com/me-no-dev/ESPAsyncWebServer
AsyncWebServer server(80);
#include <SoftwareSerial.h> // v8.1.0 https://github.com/plerup/espsoftwareserial/
SoftwareSerial Serial3;
#include "ESP32SSDP.h" // v1.2.0 https://github.com/luc-github/ESP32SSDP

/* Own classes */
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
#include "src/thingspeak.hpp"
Thingspeak thingspeak;
#include "src/narodmon.hpp"
Narodmon narodmon;
#include "src/fonts.hpp"
#include "src/nextion.hpp"
Nextion nextion;
#include "src/ili9341.hpp"
ILI9341 ili9341;
#include "src/ws2812b.hpp"
WS2812b ws2812b_1;
WS2812b ws2812b_2;
#include "src/network.hpp"
Network network;

#include "taskdisplay.hpp"
#include "tasksensors.hpp"


/* FreeRTOS running cores */
#if CONFIG_FREERTOS_UNICORE
  #define ARDUINO_RUNNING_CORE 0
#else
  #define ARDUINO_RUNNING_CORE 1
#endif
#define FS_NO_GLOBALS


/**
   Arduino setup
*/
void setup() {
  pinMode(SET_HC12_PIN, OUTPUT);
  digitalWrite(SET_HC12_PIN, HIGH);
  pinMode(DISPLAY1_BUTTON_PIN, INPUT);
  pinMode(DISPLAY2_BUTTON_PIN, INPUT);
  pinMode(ALARM_BUTTON_PIN, INPUT);
  //pinMode(AIR_HUMIDIFIER_PIN, OUTPUT);
  //pinMode(AIR_DRYER_PIN, OUTPUT);
  //pinMode(AIR_HEATER_PIN, OUTPUT);
  //pinMode(AIR_COOLER_PIN, OUTPUT);
  //digitalWrite(AIR_HUMIDIFIER_PIN, LOW);
  //digitalWrite(AIR_DRYER_PIN, LOW);
  //digitalWrite(AIR_HEATER_PIN, LOW);
  //digitalWrite(AIR_COOLER_PIN, LOW);

  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, NEXTION_RX_PIN, NEXTION_TX_PIN);
  Serial2.begin(9600);
  Serial3.begin(9600, SWSERIAL_8N1, MP3_RX_PIN, MP3_TX_PIN, false);

  Serial.println(SEPARATOR);
  Serial.println(SEPARATOR);
  Serial.printf("*  Weather Monitor BIM32 %s    © himikat123@gmail.com   2020-2024  *\r\n", FW);
  Serial.println(SEPARATOR);
  Serial.println();

  if(!LittleFS.begin()) {
    Serial.println("Little FS initialisation failed!");
    while(1) yield();
  }
  config.readConfig();

  //myNex.writeNum("sleep", 0);
  //myNex.writeStr("page Logo");

  xTaskCreatePinnedToCore(TaskDisplay1, "TaskDisplay1", 32768, NULL, 1, &task_display1_handle, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskDisplay2, "TaskDisplay2", 32768, NULL, 1, &task_display2_handle, ARDUINO_RUNNING_CORE);

  network.connect();
  
  webInterface_init();

  xTaskCreatePinnedToCore(TaskSensors, "TaskSensors", 32768, NULL, 1, &task_sensors_handle, ARDUINO_RUNNING_CORE);
}

void loop() {

}
