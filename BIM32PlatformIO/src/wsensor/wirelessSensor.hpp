#pragma once
#include <Arduino.h>

class WirelessSensor {
    #define WSENSORS 2

    public:
        static WirelessSensor& getInstance() {
            static WirelessSensor instance;
            return instance;
        }

        WirelessSensor(const WirelessSensor&) = delete;
        void operator=(const WirelessSensor&) = delete;

        void receive();
        void parse();
        bool dataRelevance(uint8_t wsensNum);
        time_t get_updated(unsigned int num);
        float get_temperature(unsigned int num, unsigned int sensor);
        float get_humidity(unsigned int num);
        float get_pressure(unsigned int num);
        float get_windSpeed(unsigned int num);
        int get_windDir(unsigned int num);
        float get_light(unsigned int num);
        float get_voltage(unsigned int num);
        float get_current(unsigned int num);
        float get_power(unsigned int num);
        float get_energy(unsigned int num);
        float get_frequency(unsigned int num);
        float get_co2(unsigned int num);
        int get_batteryAdc(unsigned int num);
        float get_batteryVoltage(unsigned int num);
        int get_batteryLevel(unsigned int num);
        float get_batteryPercentage(unsigned int num);

    private:
        WirelessSensor() {}

        uint16_t _receivedIndex = 0;
        char _receivedData[1024];
};

inline WirelessSensor& wsensor = WirelessSensor::getInstance();