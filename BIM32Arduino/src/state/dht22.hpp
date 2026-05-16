#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct DHT22State {
    float temp = 40400.0;
    float hum = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["dht22"]["temp"] = temp;
        o["dht22"]["hum"] = hum;
    }
};