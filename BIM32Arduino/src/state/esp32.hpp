#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct ESP32State {
    float temp = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["esp32"]["temp"] = temp;
    }
};