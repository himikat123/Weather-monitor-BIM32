#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct BME680State {
    float temp = 40400.0;
    float hum = 40400.0;
    float pres = 40400.0;
    float iaq = -1.0;
    unsigned int iaqAccr = 0;
    bool updated = false;

    void toJson(JsonObject o) const {
        JsonObject bme = o["bme680"].to<JsonObject>();
        bme["temp"] = temp;
        bme["hum"] = hum;
        bme["pres"] = pres;
        bme["iaq"] = iaq;
        bme["iaqAccr"] = iaqAccr;
    }
};