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
        //JsonObject o = obj.createNestedObject("wsensor");
        JsonObject o = obj["wsensor"].add<JsonObject>();
        //JsonArray aTime = o.createNestedArray("time");
        JsonArray aTime = o["time"].add<JsonArray>();
        //aTime.add(time[0]);
        //aTime.add(time[1]);
        aTime.add(time[0]);
        aTime.add(time[1]);

        //temp.toJson(o.createNestedObject("temp"));
        temp.toJson(o["temp"].add<JsonObject>());
        //hum.toJson(o.createNestedObject("hum"));
        hum.toJson(o["hum"].add<JsonObject>());
        //pres.toJson(o.createNestedObject("pres"));
        pres.toJson(o["pres"].add<JsonObject>());
        //light.toJson(o.createNestedObject("light"));
        light.toJson(o["light"].add<JsonObject>());
        //voltage.toJson(o.createNestedObject("voltage"));
        voltage.toJson(o["voltage"].add<JsonObject>());
        //current.toJson(o.createNestedObject("current"));
        current.toJson(o["current"].add<JsonObject>());
        //power.toJson(o.createNestedObject("power"));
        power.toJson(o["power"].add<JsonObject>());
        //energy.toJson(o.createNestedObject("energy"));
        energy.toJson(o["energy"].add<JsonObject>());
        //freq.toJson(o.createNestedObject("freq"));
        freq.toJson(o["freq"].add<JsonObject>());
        //co2.toJson(o.createNestedObject("co2"));
        co2.toJson(o["co2"].add<JsonObject>());
        //wind.toJson(o.createNestedObject("wind")); 
        wind.toJson(o["wind"].add<JsonObject>());

        //JsonArray b = o.createNestedArray("bat");
        JsonArray b = o["bat"].add<JsonArray>();
        //for(int i=0; i<2; i++) b.add(battery.adc[i]);
        for(int i=0; i<2; i++) b.add(battery.adc[i]);
    }
};