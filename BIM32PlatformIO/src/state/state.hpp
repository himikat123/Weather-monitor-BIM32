#pragma once

#include <Arduino.h>
#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include "./esp32.hpp"
#include "./network.hpp"
#include "./bme680.hpp"
#include "./bme280.hpp"
#include "./bmp180.hpp"
#include "./sht21.hpp"
#include "./dht22.hpp"
#include "./ds18b20.hpp"
#include "./max44009.hpp"
#include "./bh1750.hpp"
#include "./analog.hpp"
#include "./thingspeak..hpp"
#include "./weather.hpp"
#include "./wsensor.hpp"
#include "./fs.hpp"

struct State {
    private:
        State() {}

    public:
        static State& getInstance() {
            static State instance;
            return instance;
        }

        State(const State&) = delete;
        void operator=(const State&) = delete;

        bool clockSynchronized = false; // Is the time synchronized with the ntp server?
        bool clockSynchronize = false; // Should the display RTC be updated?
        bool net_connected = false; // Is the device connected to the network?
        bool disp_on_off[2] = {true, true}; // Display on/off state flag
        uint32_t disp_autoOff[2] = {0, 0}; // Displays auto off interval counter
        bool reduc[2] = {false, false}; // Display brightness reduction flag
        bool apMode = false; // Access point mode
        uint8_t display_state[2] = {1, 1}; // display on/off: 0: nothing do, 1: turn on, 2: turn off
        volatile bool display_btn_pressed[2] = {false, false}; // display (1, 2) button pressed flag
        volatile bool alarm_but_pressed = false; // alarm button pressed flag
        volatile bool mp3_busy = true; // mp3 player busy pin
        volatile unsigned int btnMillis[2] = {0, 0}; // contact debounce time variable
        unsigned int comfort = 0; // Comfort level code: 1-Comfortable, 2-Hot, 3-Cold, 4-Humid, 5-Dry, 6-Hot & Humid, 7-Hot & Dry, 8-Cold & Humid, 9-Cold & Dry
        unsigned int iaq_level = 0; // IAQ level code: 0-undefined, 1-Air clean, 2-Air pulluted, 3-Air heavily polluted
        unsigned int co2_level = 0; // CO2 level code: 0-undefined, 1-Air clean, 2-Air pulluted, 3-Air heavily polluted
        bool clockPoints = false; // ILI9341 clock points state
        int8_t disp_night_state[2] = {-1, -1}; // state of displays night off: -1-undefined, 0-off, 1-on
        bool touch_calibrate = false; // touch calibrate modus
        bool colorChanged = false; // timeslot color has been changed from web interface
        int segmentDispOrder[2][9] = { 0 }; // 7 segment display digits order for test
        bool debugTouch = false;
        bool debugWether = false;
        double secretCode = esp_random();

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

inline State& state = State::getInstance();