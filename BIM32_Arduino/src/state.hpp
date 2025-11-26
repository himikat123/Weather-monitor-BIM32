#pragma once

#define SSID_COUNT 30
#define SSID_LEN   33
#define IP_LEN     16
#define MAC_LEN    18
#define W_NAME_LEN 32

struct ESP32State {
    float temp = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["esp32"]["temp"] = temp;
    }
};

struct NetworkState {
    char ssid[SSID_LEN] = "";
    unsigned int ch = 0;
    int sig = 0;
    char mac[MAC_LEN] = "";
    char ip[IP_LEN] = "";
    char mask[IP_LEN] = "";
    char gw[IP_LEN] = "";
    char dns1[IP_LEN] = "";
    char dns2[IP_LEN] = "";

    char ssids[SSID_COUNT][SSID_LEN] = { 0 };
    int rssis[SSID_COUNT] = { 0 };

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

struct BME680State {
    float temp = 40400.0;
    float hum = 40400.0;
    float pres = 40400.0;
    float iaq = -1.0;
    unsigned int iaqAccr = 0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["bme680"]["temp"] = temp;
        o["bme680"]["hum"] = hum;
        o["bme680"]["pres"] = pres;
        o["bme680"]["iaq"] = iaq;
        o["bme680"]["iaqAccr"] = iaqAccr;
    }
};

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

struct BMP180State {
    float temp = 40400.0;
    float pres = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["bmp180"]["temp"] = temp;
        o["bmp180"]["pres"] = pres;
    }
};

struct SHT21State {
    float temp = 40400.0;
    float hum = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["sht21"]["temp"] = temp;
        o["sht21"]["hum"] = hum;
    }
};

struct DHT22State {
    float temp = 40400.0;
    float hum = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["dht22"]["temp"] = temp;
        o["dht22"]["hum"] = hum;
    }
};

struct DS18B20State {
    float temp = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["ds18b20"]["temp"] = temp;
    }
};

struct MAX44009State {
    float light = -1.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["max44009"]["light"] = light;
    }
};

struct BH1750State {
    float light = -1.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["bh1750"]["light"] = light;
    }
};

struct AnalogState {
    float volt = -1.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["analog"]["volt"] = volt;
    }
};

struct ThingState {
    time_t time = 0;
    float data[8] = { -40400.0, -40400.0, -40400.0, -40400.0, -40400.0, -40400.0, -40400.0, -40400.0 };
    bool updated = false;

    void toJson(JsonObject obj) const {
        JsonObject o = obj.createNestedObject("thing");
        o["time"] = time;
        JsonArray a = o.createNestedArray("data");
        for(int i=0;i<8;i++) a.add(data[i]);
    }
};

struct WeatherState {
    float temp = 40400.0;
    float hum = 40400.0;
    float pres = 40400.0;
    unsigned int icon = 1;
    bool isDay = true;
    char descript[64] = "----";
    time_t time = 0;
    bool updated = false;

    struct Wind {
        float speed = -1.0;
        int dir = 0;

        void toJson(JsonObject o) const {
            o["speed"] = speed;
            o["dir"] = dir;
        }
    } wind;

    struct Daily {
        float tMax[5] = { 40400.0, 40400.0, 40400.0, 40400.0, 40400.0 };
        float tMin[5] = { 40400.0, 40400.0, 40400.0, 40400.0, 40400.0 };
        float wind[5] = { -1.0, -1.0, -1.0, -1.0, -1.0 };
        unsigned int icon[5] = { 0, 0, 0, 0, 0 };

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

struct FilesystemState {
    unsigned int total = 0;
    unsigned int free = 0;
    char list[2048] = "";
    bool updated = false;

    void toJson(JsonObject obj) const {
        JsonObject o = obj.createNestedObject("fs");
        o["total"] = total;
        o["free"] = free;
        o["list"] = list;
    }
};

struct State {
    ESP32State esp32core;
    NetworkState network;
    BME680State bme680;
    BME280State bme280;
    BMP180State bmp180;
    SHT21State sht21;
    DHT22State dht22;
    DS18B20State ds18b20;
    MAX44009State max44009;
    BH1750State bh1750;
    AnalogState analog;
    ThingState thing;
    WeatherState weather;
    WSensorState wsensor;
    FilesystemState filesystem;

    void toJson(JsonDocument& doc) const {
        JsonObject root = doc.to<JsonObject>();

        esp32core.toJson(root);
        network.toJson(root);
        bme680.toJson(root);
        bme280.toJson(root);
        bmp180.toJson(root);
        sht21.toJson(root);
        dht22.toJson(root);
        ds18b20.toJson(root);
        max44009.toJson(root);
        bh1750.toJson(root);
        analog.toJson(root);
        thing.toJson(root);
        weather.toJson(root);
        wsensor.toJson(root);
        filesystem.toJson(root);
    }
};