#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include "../globals.hpp"

struct WSensorState {
    time_t time[2] = { 0 };
    bool updated = false;

    struct Block1D {
        float data[2] = { 0 };
        char name[2][W_NAME_LEN] = { 0 };

        void toJson(JsonObject o) const {
            JsonArray D = o.createNestedArray("data");
            JsonArray N = o.createNestedArray("name");
            for(int i=0; i<2; i++) {
                D.add(data[i]);
                N.add(name[i]);
            }
        }
    };

    struct Block2D {
        float data[5][2] = { 0 };
        char name[5][2][W_NAME_LEN] = { 0 };

        void toJson(JsonObject o) const {
            JsonArray dataArr = o.createNestedArray("data");
            JsonArray nameArr = o.createNestedArray("name");
            for(int r=0; r<5; r++){
                JsonArray rowD = dataArr.createNestedArray();
                JsonArray rowN = nameArr.createNestedArray();
                for(int c=0; c<2; c++){
                    rowD.add(data[r][c]);
                    rowN.add(name[r][c]);
                }
            }
        }
    };

    struct BlockWind {
        Block1D speed, dir;

        void toJson(JsonObject o) const {
            speed.toJson(o.createNestedObject("speed"));
            dir.toJson(o.createNestedObject("dir"));
        }
    };

    struct BlockBattery {
        int adc[2] = {-1, -1};
        float voltage[2] = {-1.0, -1.0};
        int level[2] = {-1, -1};
        float percentage[2] = {-1, -1};
    };

    BlockBattery battery;
    BlockWind wind;
    Block2D temp;
    Block1D hum, pres, light, voltage, current, power, energy, freq, co2;

    void toJson(JsonObject obj) const {
        JsonObject o = obj.createNestedObject("wsensor");
        JsonArray aTime = o.createNestedArray("time");
        aTime.add(time[0]);
        aTime.add(time[1]);

        temp.toJson(o.createNestedObject("temp"));
        hum.toJson(o.createNestedObject("hum"));
        pres.toJson(o.createNestedObject("pres"));
        light.toJson(o.createNestedObject("light"));
        voltage.toJson(o.createNestedObject("voltage"));
        current.toJson(o.createNestedObject("current"));
        power.toJson(o.createNestedObject("power"));
        energy.toJson(o.createNestedObject("energy"));
        freq.toJson(o.createNestedObject("freq"));
        co2.toJson(o.createNestedObject("co2"));
        wind.toJson(o.createNestedObject("wind")); 

        JsonArray b = o.createNestedArray("bat");
        for(int i=0; i<2; i++) b.add(battery.adc[i]);
    }
};