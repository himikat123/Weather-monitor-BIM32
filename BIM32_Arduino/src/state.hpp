#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include <ArduinoJson.h>
#include <cstring>


#define PROPERTY(type, name)                                 \
private:                                                     \
    type name;                                               \
public:                                                      \
    const type& get##name() const { return name; }           \
    void set##name(const type& v) { name = v; }

#define PROPERTY_ARRAY(type, name, size)                     \
private:                                                     \
    type name[size];                                         \
public:                                                      \
    type get##name(int i) const { return name[i]; }          \
    void set##name(int i, type v) { name[i] = v; }

#define PROPERTY_ARRAY_STR(name, size, len)                  \
private:                                                     \
    char name[size][len];                                    \
public:                                                      \
    const char* get##name(int i) const { return name[i]; }   \
    void set##name(int i, const char* v) {                   \
        strncpy(name[i], v, len);                            \
        name[i][len-1] = 0;                                  \
    }

#define PROPERTY_STR(name, len)                              \
private:                                                     \
    char name[len];                                          \
public:                                                      \
    const char* get##name() const { return name; }           \
    void set##name(const char* v) {                          \
        strncpy(name, v, len);                               \
        name[len-1] = 0;                                     \
    }

#define SSID_COUNT 30
#define SSID_LEN   33
#define IP_LEN     16
#define MAC_LEN    18
#define W_NAME_LEN 32


class DataModel {
public:
    class Network {
    public:
        PROPERTY_ARRAY_STR(ssids, SSID_COUNT, SSID_LEN)
        PROPERTY_ARRAY(int, rssis, SSID_COUNT)

        PROPERTY_STR(ssid, SSID_LEN)

        PROPERTY(unsigned int, ch)
        PROPERTY(int, sig)

        PROPERTY_STR(mac, MAC_LEN)
        PROPERTY_STR(ip, IP_LEN)
        PROPERTY_STR(mask, IP_LEN)
        PROPERTY_STR(gw, IP_LEN)
        PROPERTY_STR(dns1, IP_LEN)
        PROPERTY_STR(dns2, IP_LEN)

        PROPERTY(bool, updated)

        void toJson(JsonObject o) const {
            JsonArray a_ssids = o["ssids"].to<JsonArray>();
            JsonArray a_rssis = o["rssis"].to<JsonArray>();

            for(int i=0; i<SSID_COUNT; i++) {
                a_ssids.add(getssids(i));
                a_rssis.add(getrssis(i));
            }

            o["ssid"] = getssid();
            o["ch"]   = getch();
            o["sig"]  = getsig();
            o["mac"]  = getmac();
            o["ip"]   = getip();
            o["mask"] = getmask();
            o["gw"]   = getgw();
            o["dns1"] = getdns1();
            o["dns2"] = getdns2();
        }
    };

    class BME280 {
        PROPERTY(float, temp)
        PROPERTY(float, hum)
        PROPERTY(float, pres)
        PROPERTY(bool, updated)
    public:
        void toJson(JsonObject o) const {
            o["temp"] = gettemp();
            o["hum"] = gethum();
            o["pres"] = getpres();
        }
    };

    class BMP180 {
        PROPERTY(float, temp)
        PROPERTY(float, pres)
        PROPERTY(bool, updated)
    public:
        void toJson(JsonObject o) const {
            o["temp"] = gettemp();
            o["pres"] = getpres();
        }
    };

    class SHT21 {
        PROPERTY(float, temp)
        PROPERTY(float, hum)
        PROPERTY(bool, updated)
    public:
        void toJson(JsonObject o) const {
            o["temp"] = gettemp();
            o["hum"] = gethum();
        }
    };

    class DHT22 {
        PROPERTY(float, temp)
        PROPERTY(float, hum)
        PROPERTY(bool, updated)
    public:
        void toJson(JsonObject o) const {
            o["temp"] = gettemp();
            o["hum"] = gethum();
        }
    };

    class ESP32Temp {
        PROPERTY(float, temp)
        PROPERTY(bool, updated)
    public:
        void toJson(JsonObject o) const { o["temp"] = gettemp(); }
    };

    class DS18B20 {
        PROPERTY(float, temp)
        PROPERTY(bool, updated)
    public:
        void toJson(JsonObject o) const { o["temp"] = gettemp(); }
    };

    class MAX44009 {
        PROPERTY(float, light)
        PROPERTY(bool, updated)
    public:
        void toJson(JsonObject o) const { o["light"] = getlight(); }
    };

    class BH1750 {
        PROPERTY(float, light)
        PROPERTY(bool, updated)
    public:
        void toJson(JsonObject o) const { o["light"] = getlight(); }
    };

    class Analog {
        PROPERTY(float, volt)
        PROPERTY(bool, updated)
    public:
        void toJson(JsonObject o) const { o["volt"] = getvolt(); }
    };

    class BME680 {
        PROPERTY(float, temp)
        PROPERTY(float, hum)
        PROPERTY(float, pres)
        PROPERTY(float, iaq)
        PROPERTY(unsigned int, iaqAccr)
        PROPERTY(bool, updated)
    public:
        void toJson(JsonObject o) const {
            o["temp"] = gettemp();
            o["hum"]  = gethum();
            o["pres"] = getpres();
            o["iaq"]  = getiaq();
            o["iaqAccr"] = getiaqAccr();
        }
    };

    class Thing {
        PROPERTY(unsigned int, time)
        PROPERTY_ARRAY(float, data, 8)
        PROPERTY(bool, updated)
    public:
        void toJson(JsonObject o) const {
            o["time"] = gettime();
            JsonArray a = o["data"].to<JsonArray>();
            for(int i=0; i<8; i++) a.add(getdata(i));
        }
    };

    class Weather {
        PROPERTY(float, temp)
        PROPERTY(float, hum)
        PROPERTY(float, pres)

        class Wind {
            PROPERTY(float, speed)
            PROPERTY(int, dir)
        public:
            void toJson(JsonObject o) const {
                o["speed"]=getspeed();
                o["dir"]=getdir();
            }
        } wind;

        PROPERTY_STR(descript, 64)
        PROPERTY(unsigned int, time)
        PROPERTY(unsigned int, icon)
        PROPERTY(bool, isDay)
        PROPERTY(bool, updated)

        class Daily {
            PROPERTY_ARRAY(float, tMax, 5)
            PROPERTY_ARRAY(float, tMin, 5)
            PROPERTY_ARRAY(float, wind, 5)
            PROPERTY_ARRAY(unsigned int, icon, 5)
        public:
            void toJson(JsonObject o) const {
                JsonArray A = o["tMax"].to<JsonArray>();
                JsonArray B = o["tMin"].to<JsonArray>();
                JsonArray C = o["wind"].to<JsonArray>();
                JsonArray D = o["icon"].to<JsonArray>();

                for(int i=0; i<5; i++){
                    A.add(gettMax(i));
                    B.add(gettMin(i));
                    C.add(getwind(i));
                    D.add(geticon(i));
                }
            }
        } daily;

    public:
        Wind& getWind(){ return wind; }
        Daily& getDaily(){ return daily; }

        void toJson(JsonObject o) const {
            o["temp"] = gettemp();
            o["hum"] = gethum();
            o["pres"] = getpres();

            wind.toJson(o["wind"].to<JsonObject>());
            o["descript"] = getdescript();
            o["time"] = gettime();
            o["icon"] = geticon();
            o["isDay"] = getisDay();

            daily.toJson(o["daily"].to<JsonObject>());
        }
    };

    class WSensor {
        PROPERTY(bool, updated)
        PROPERTY_ARRAY(unsigned int, time, 2)

        class Block2D {
            PROPERTY_ARRAY(float, data_row0, 2)
            PROPERTY_ARRAY(float, data_row1, 2)
            PROPERTY_ARRAY(float, data_row2, 2)
            PROPERTY_ARRAY(float, data_row3, 2)
            PROPERTY_ARRAY(float, data_row4, 2)

            PROPERTY_ARRAY_STR(name_row0, 2, W_NAME_LEN)
            PROPERTY_ARRAY_STR(name_row1, 2, W_NAME_LEN)
            PROPERTY_ARRAY_STR(name_row2, 2, W_NAME_LEN)
            PROPERTY_ARRAY_STR(name_row3, 2, W_NAME_LEN)
            PROPERTY_ARRAY_STR(name_row4, 2, W_NAME_LEN)

        public:
            void toJson(JsonObject o) const {
                JsonArray D = o["data"].to<JsonArray>();
                JsonArray N = o["name"].to<JsonArray>();

                auto addRow = [&](auto& getterVal, auto& getterName){
                    JsonArray rd = D.add<JsonArray>();
                    JsonArray rn = N.add<JsonArray>();
                    rd.add(getterVal(0));
                    rd.add(getterVal(1));
                    rn.add(getterName(0));
                    rn.add(getterName(1));
                };

                addRow([&](int i){return getdata_row0(i);}, [&](int i){return getname_row0(i);} );
                addRow([&](int i){return getdata_row1(i);}, [&](int i){return getname_row1(i);} );
                addRow([&](int i){return getdata_row2(i);}, [&](int i){return getname_row2(i);} );
                addRow([&](int i){return getdata_row3(i);}, [&](int i){return getname_row3(i);} );
                addRow([&](int i){return getdata_row4(i);}, [&](int i){return getname_row4(i);} );
            }
        };

        class Block1D {
            PROPERTY_ARRAY(float, data, 2)
            PROPERTY_ARRAY_STR(name, 2, W_NAME_LEN)
        public:
            void toJson(JsonObject o) const {
                JsonArray D = o["data"].to<JsonArray>();
                JsonArray N = o["name"].to<JsonArray>();
                for(int i=0; i<2; i++){
                    D.add(getdata(i));
                    N.add(getname(i));
                }
            }
        };

        Block2D temp;
        Block1D hum, pres, light, voltage, current, power, energy, freq, co2;

        PROPERTY_ARRAY(float, bat, 2)
        PROPERTY(bool, updated)

    public:
        Block2D& gettemp(){ return temp; }
        Block1D& gethum(){ return hum; }
        Block1D& getpres(){ return pres; }
        Block1D& getlight(){ return light; }
        Block1D& getvoltage(){ return voltage; }
        Block1D& getcurrent(){ return current; }
        Block1D& getpower(){ return power; }
        Block1D& getenergy(){ return energy; }
        Block1D& getfreq(){ return freq; }
        Block1D& getco2(){ return co2; }

        void toJson(JsonObject o) const {
            JsonArray t = o["time"].to<JsonArray>();
            t.add(gettime(0));
            t.add(gettime(1));

            temp.toJson(o["temp"].to<JsonObject>());
            hum.toJson(o["hum"].to<JsonObject>());
            pres.toJson(o["pres"].to<JsonObject>());
            light.toJson(o["light"].to<JsonObject>());
            voltage.toJson(o["voltage"].to<JsonObject>());
            current.toJson(o["current"].to<JsonObject>());
            power.toJson(o["power"].to<JsonObject>());
            energy.toJson(o["energy"].to<JsonObject>());
            freq.toJson(o["freq"].to<JsonObject>());
            co2.toJson(o["co2"].to<JsonObject>());

            JsonArray b = o["bat"].to<JsonArray>();
            b.add(getbat(0));
            b.add(getbat(1));
        }
    };

private:
    Network network;
    BME280 bme280;
    BMP180 bmp180;
    SHT21 sht21;
    DHT22 dht22;
    ESP32Temp esp32;
    DS18B20 ds18b20;
    MAX44009 max44009;
    BH1750 bh1750;
    Analog analog;
    BME680 bme680;
    Thing thing;
    Weather weather;
    WSensor wsensor;

public:
    Network& getnetwork() { return network; }
    BME280& getbme280() { return bme280; }
    BMP180& getbmp180() { return bmp180; }
    SHT21& getsht21() { return sht21; }
    DHT22& getdht22() { return dht22; }
    ESP32Temp& getesp32() { return esp32; }
    DS18B20& getds18b20() { return ds18b20; }
    MAX44009& getmax44009() { return max44009; }
    BH1750& getbh1750() { return bh1750; }
    Analog& getanalog() { return analog; }
    BME680& getbme680() { return bme680; }
    Thing& getthing() { return thing; }
    Weather& getweather() { return weather; }
    WSensor& getwsensor() { return wsensor; }

    void toJson(JsonDocument& doc) {
        network.toJson(doc["network"].to<JsonObject>());
        bme280.toJson(doc["bme280"].to<JsonObject>());
        bmp180.toJson(doc["bmp180"].to<JsonObject>());
        sht21.toJson(doc["sht21"].to<JsonObject>());
        dht22.toJson(doc["dht22"].to<JsonObject>());
        esp32.toJson(doc["esp32"].to<JsonObject>());
        ds18b20.toJson(doc["ds18b20"].to<JsonObject>());
        max44009.toJson(doc["max44009"].to<JsonObject>());
        bh1750.toJson(doc["bh1750"].to<JsonObject>());
        analog.toJson(doc["analog"].to<JsonObject>());
        bme680.toJson(doc["bme680"].to<JsonObject>());
        thing.toJson(doc["thing"].to<JsonObject>());
        weather.toJson(doc["weather"].to<JsonObject>());
        wsensor.toJson(doc["wsensor"].to<JsonObject>());
    }
};

#endif


//////////////////////////////
//пример

//DataModel model;

// ... записываем данные ...
//model.getbme280().settemp(23.5);

//StaticJsonDocument<5000> doc;
//model.toJson(doc);

//String json;
//serializeJson(doc, json);

//Serial.println(json);   // ← JSON-строка готова

//////////////////////////////////
//main.ino
//#include "data_model.h"

//DataModel model;

/////////////////////////////////
//class.hpp
//#
//#pragma once
//#include "sensor_manager.h"

//extern DataModel model;