#include <Arduino.h>
#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include <LittleFS.h> // v2.0.13
#include "./config.hpp"

void Config::set_lang(String lng) {
    lng.toCharArray(_lang, 3);
}

void Config::saveAlarmFile() {
    JsonDocument doc;
    JsonObject alarmObj = doc["alarm"].to<JsonObject>();
    JsonArray timeArr = alarmObj["time"].to<JsonArray>();
    for(uint8_t i=0; i<ALARMS; i++) {
        JsonArray t = timeArr.add<JsonArray>();
        t.add(alarm._time[i][0]);
        t.add(alarm._time[i][1]);
    }
    JsonArray weekdaysArr = alarmObj["weekdays"].to<JsonArray>();
    for(uint8_t i=0; i<ALARMS; i++) {
        JsonArray w = weekdaysArr.add<JsonArray>();
        for(uint8_t n=0; n<7; n++) w.add(alarm._weekdays[i][n]);
    }
    JsonArray statesArr = alarmObj["states"].to<JsonArray>();
    for(uint8_t i=0; i<ALARMS; i++) statesArr.add(alarm._states[i]);
    JsonArray melodiesArr = alarmObj["melodies"].to<JsonArray>();
    for(uint8_t i=0; i<ALARMS; i++) melodiesArr.add(alarm._melodies[i]);
    File file = LittleFS.open("/alarm.json", "w");
    if(!file) return;
    serializeJson(doc, file);
    file.close();
}