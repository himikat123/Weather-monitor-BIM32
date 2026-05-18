#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct DHT22State {
    float temp = 40400.0;
    float hum = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        JsonObject dht = o["dht22"].to<JsonObject>();
        dht["temp"] = temp;
        dht["hum"] = hum;
    }
};