#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct BME280State {
    float temp = 40400.0;
    float hum = 40400.0;
    float pres = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["bme280"]["temp"] = temp;
        o["bme280"]["hum"] = hum;
        o["bme280"]["pres"] = pres;
    }
};