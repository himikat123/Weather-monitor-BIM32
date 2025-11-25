#pragma once

#define SSID_COUNT 30
#define SSID_LEN   33
#define IP_LEN     16
#define MAC_LEN    18
#define W_NAME_LEN 32

struct ESP32 {
    float temp;
    void toJson(JsonObject o) const {
        o["esp32"]["temp"] = temp;
    }
};

struct Network {
    char ssid[SSID_LEN];
    unsigned int ch;
    int sig;
    char mac[MAC_LEN];
    char ip[IP_LEN];
    char mask[IP_LEN];
    char gw[IP_LEN];
    char dns1[IP_LEN];
    char dns2[IP_LEN];

    char ssids[SSID_COUNT][SSID_LEN];
    int rssis[SSID_COUNT];

    void toJson(JsonObject o) const {
        o["network"]["ssid"] = ssid;
        o["network"]["ch"] = ch;
        o["network"]["sig"] = sig;
        o["network"]["mac"] = mac;
        o["network"]["ip"] = ip;
        o["network"]["mask"] = mask;
        o["network"]["gw"] = gw;
        o["network"]["dns1"] = dns1;
        o["network"]["dns2"] = dns2;

        JsonArray a_ssids = o.createNestedArray("ssids");
        for(int i=0; i<SSID_COUNT; i++) {
            JsonArray entry = a_ssids.createNestedArray();
            entry.add(ssids[i]);
            entry.add(rssis[i]);
        }
    }
};

struct BME680 {
    float temp;
    float hum;
    float pres;
    float iaq;
    unsigned int iaqAccr;
    bool updated;

    void toJson(JsonObject o) const {
        o["bme680"]["temp"] = temp;
        o["bme680"]["hum"] = hum;
        o["bme680"]["pres"] = pres;
        o["bme680"]["iaq"] = iaq;
        o["bme680"]["iaqAccr"] = iaqAccr;
    }
};

struct BME280 {
    float temp;
    float hum;
    float pres;
    bool updated;

    void toJson(JsonObject o) const {
        o["bme280"]["temp"] = temp;
        o["bme280"]["hum"] = hum;
        o["bme280"]["pres"] = pres;
    }
};

struct BMP180 {
    float temp;
    float pres;
    bool updated;

    void toJson(JsonObject o) const {
        o["bmp180"]["temp"] = temp;
        o["bmp180"]["pres"] = pres;
    }
};

struct SHT21 {
    float temp;
    float hum;
    bool updated;

    void toJson(JsonObject o) const {
        o["sht21"]["temp"] = temp;
        o["sht21"]["hum"] = hum;
    }
};

struct DHT22 {
    float temp;
    float hum;
    bool updated;

    void toJson(JsonObject o) const {
        o["dht22"]["temp"] = temp;
        o["dht22"]["hum"] = hum;
    }
};

struct DS18B20 {
    float temp;
    bool updated;

    void toJson(JsonObject o) const {
        o["ds18b20"]["temp"] = temp;
    }
};

struct MAX44009 {
    float light;
    bool updated;

    void toJson(JsonObject o) const {
        o["max44009"]["light"] = light;
    }
};

struct BH1750 {
    float light;
    bool updated;

    void toJson(JsonObject o) const {
        o["bh1750"]["light"] = light;
    }
};

struct Analog {
    float volt;
    bool updated;

    void toJson(JsonObject o) const {
        o["analog"]["volt"] = volt;
    }
};

struct Thing {
    unsigned int time;
    float data[8];
    bool updated;

    void toJson(JsonObject obj) const {
        JsonObject o = obj.createNestedObject("thing");
        o["time"] = time;
        JsonArray a = o.createNestedArray("data");
        for(int i=0;i<8;i++) a.add(data[i]);
    }
};

struct Weather {
    float temp;
    float hum;
    float pres;
    unsigned int icon;
    bool isDay;
    char descript[64];
    unsigned int time;
    bool updated;

    struct Wind {
        float speed;
        int dir;

        void toJson(JsonObject o) const {
            o["speed"] = speed;
            o["dir"] = dir;
        }
    } wind;

    struct Daily {
        float tMax[5];
        float tMin[5];
        float wind[5];
        unsigned int icon[5];

        void toJson(JsonObject o) const {
            JsonArray a_tMax = o.createNestedArray("tMax");
            JsonArray a_tMin = o.createNestedArray("tMin");
            JsonArray a_wind = o.createNestedArray("wind");
            JsonArray a_icon = o.createNestedArray("icon");
            for(int i=0; i<5; i++){
                a_tMax.add(tMax[i]);
                a_tMin.add(tMin[i]);
                a_wind.add(wind[i]);
                a_icon.add(icon[i]);
            }
        }
    } daily;

    void toJson(JsonObject obj) const {
        JsonObject o = obj.createNestedObject("weather");
        o["temp"] = temp;
        o["hum"] = hum;
        o["pres"] = pres;
        o["icon"] = icon;
        o["isDay"] = isDay;
        o["descript"] = descript;
        o["time"] = time;

        JsonObject windObj = o.createNestedObject("wind");
        wind.toJson(windObj);

        JsonObject dailyObj = o.createNestedObject("daily");
        daily.toJson(dailyObj);
    }
};

struct WSensor {
    float time[2];
    bool updated;

    struct Block1D {
        float data[2];
        char name[2][W_NAME_LEN];

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
        float data[5][2];
        char name[5][2][W_NAME_LEN];

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

        JsonArray b = o.createNestedArray("bat");
        for(int i=0;i<2;i++) b.add(time[i]);
    }
};

struct FS {
    unsigned int total;
    unsigned int free;
    char list[2048];
    bool updated;

    void toJson(JsonObject obj) const {
        JsonObject o = obj.createNestedObject("fs");
        o["total"] = total;
        o["free"] = free;
        o["list"] = list;
    }
};

struct State {
    ESP32 esp32;
    Network network;
    BME680 bme680;
    BME280 bme280;
    BMP180 bmp180;
    SHT21 sht21;
    DHT22 dht22;
    DS18B20 ds18b20;
    MAX44009 max44009;
    BH1750 bh1750;
    Analog analog;
    Thing thing;
    Weather weather;
    WSensor wsensor;
    FS fs;

    void toJson(JsonDocument doc) const {
        esp32.toJson(doc);
        network.toJson(doc);
        bme680.toJson(doc);
        bme280.toJson(doc);
        bmp180.toJson(doc);
        sht21.toJson(doc);
        dht22.toJson(doc);
        ds18b20.toJson(doc);
        max44009.toJson(doc);
        bh1750.toJson(doc);
        analog.toJson(doc);
        thing.toJson(doc);
        weather.toJson(doc);
        wsensor.toJson(doc);
        fs.toJson(doc);
    }
};