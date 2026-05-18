#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct BME280State {
    float temp = 40400.0;
    float hum = 40400.0;
    float pres = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        JsonObject bme = o["bme280"].to<JsonObject>();
        bme["temp"] = temp;
        bme["hum"] = hum;
        bme["pres"] = pres;
    }
};