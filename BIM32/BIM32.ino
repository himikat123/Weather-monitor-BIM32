/**
 *  Weather Monitor BIM32 v3.6
 *  https://github.com/himikat123/Weather-monitor-BIM32
 *
 *  © himikat123@gmail.com, Nürnberg, Deutschland, 2020-2024
 *
 *  ESP32 Dev Module
 *  1.2 MB APP / 1.5 MB SPIFFS
 *
 *  Arduino IDE v1.8.19
 *  ESP32 board v1.0.5
 */

/* Arduino libraries */
#include <Arduino.h>
#include <ArduinoOTA.h>
#include "FS.h"
#include <SPIFFS.h>
#include <Update.h>
#include <WiFi.h>
#include <HTTPClient.h>
WiFiClient client;
#include <ESPmDNS.h>

/* External Libraries */
#include <ArduinoJson.h> // v6.19.3 https://github.com/bblanchon/ArduinoJson
#include <TimeLib.h> // v1.6.0 https://playground.arduino.cc/Code/Time/
#include <ESP32Ping.h> // v1.5 https://github.com/marian-craciunescu/ESP32Ping
#include <AsyncTCP.h> // v1.1.1 https://github.com/me-no-dev/AsyncTCP
#include <ESPAsyncWebServer.h> // v1.2.3 https://github.com/me-no-dev/ESPAsyncWebServer
AsyncWebServer server(80);
#include <SoftwareSerial.h> // v6.16.1 https://github.com/plerup/espsoftwareserial/
SoftwareSerial Serial3;
#include "EasyNextionLibrary.h" // v1.0.3 https://github.com/Seithan/EasyNextionLibrary
EasyNex myNex(Serial1);
#include "DFRobotDFPlayerMini.h" // v1.0.5 https://github.com/DFRobot/DFRobotDFPlayerMini
DFRobotDFPlayerMini mp3player;
#include "ESP32SSDP.h" // v1.2.0 https://github.com/luc-github/ESP32SSDP

/* Own classes */
#include "globals.hpp"
Config config;
#include "sensors.hpp"
Sensors sensors;
#include "languages.hpp"
Lang lang;
#include "wirelessSensor.hpp"
WirelessSensor wsensor;
#include "weather.hpp"
Weather weather;
#include "sound.hpp"
Sound sound;
#include "timentp.hpp"
TimeNTP timentp;
#include "thingspeak.hpp"
Thingspeak thingspeak;
#include "narodmon.hpp"
Narodmon narodmon;
#include "fonts.hpp"
#include "nextion.hpp"
Nextion nextion;
#include "ws2812b.hpp"
WS2812b ws2812b;
#include "network.hpp"
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
  pinMode(AIR_HUMIDIFIER_PIN, OUTPUT);
  pinMode(AIR_DRYER_PIN, OUTPUT);
  pinMode(AIR_HEATER_PIN, OUTPUT);
  pinMode(AIR_COOLER_PIN, OUTPUT);
  digitalWrite(AIR_HUMIDIFIER_PIN, LOW);
  digitalWrite(AIR_DRYER_PIN, LOW);
  digitalWrite(AIR_HEATER_PIN, LOW);
  digitalWrite(AIR_COOLER_PIN, LOW);

  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, NEXTION_RX_PIN, NEXTION_TX_PIN);
  Serial2.begin(9600);
  Serial3.begin(9600, SWSERIAL_8N1, MP3_RX_PIN, MP3_TX_PIN, false);
  myNex.writeNum("sleep", 0);
  myNex.writeStr("page Logo");
  Serial.println(SEPARATOR);
  Serial.println(SEPARATOR);
  Serial.println("*  Weather Monitor BIM32 " + String(global.fw) + "    © himikat123@gmail.com   2020-2024  *");
  Serial.println(SEPARATOR);
  Serial.println();

  SPIFFS.begin();
  config.readConfig();

  ws2812b.init(); // Initialize WS2812b display

  network.connect();
  
  webInterface_init();

  delay(45);

  xTaskCreatePinnedToCore(TaskDisplay, "TaskDisplay", 32768, NULL, 1, &task_display_handle, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskSensors, "TaskSensors", 32768, NULL, 1, &task_sensors_handle, ARDUINO_RUNNING_CORE);
}

void loop() {

}
