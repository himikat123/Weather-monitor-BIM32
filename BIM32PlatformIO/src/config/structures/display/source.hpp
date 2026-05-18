#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"

class Config;

struct SourceStruct {
    struct BaseSensor {
        private:
            unsigned int _sens = 0;
            unsigned int _wsensNum = 0;
            unsigned int _thing = 0;
            friend class Config;

        public:
            const unsigned int sens() const { return _sens; }
            const unsigned int wsensNum() const { if(_wsensNum >= WSENSORS) return 0; return _wsensNum; }
            const unsigned int thing() const { if(_thing >= THNG_FIELDS) return 0; return _thing; }
    };

    struct TempSensor : public BaseSensor {
        private: 
            unsigned int _temp = 0; friend class Config;
        public: 
            const unsigned int temp() const { if(_temp >= WSENSOR_TEMPS) return 0; return _temp; }
    };

    struct VoltSensor : public BaseSensor {
        private:
            unsigned int _volt = 0;
            unsigned int _thingType = 0;
            friend class Config;

        public:
            const unsigned int volt() const { if(_volt > 3) return 0; return _volt; }
            const unsigned int thingType() const { return _thingType; }
    };

    struct Wind {
        BaseSensor speed;
        BaseSensor dir;
    };

    struct Sequence {
        private:
            unsigned int _dur = 2; // Sequence data display duration (seconds)
            char _name[SEQUENCES][33] = {"", "", "", ""}; // Sequence data names
            unsigned int _temp[SEQUENCES] = { 0 }; // Sequence data sources for the temperature sequence: 0-Nothing, 1-Thingspeak, 2-Wireless sensor, 3-BME280, 4-BMP180, 5-SHT21, 6-DHT22, 7-DS18B20, 8-ESP32, 9-Forecast, 10-BME680
            unsigned int _thngtemp[SEQUENCES] = { 0 }; // Thingspeak field number for the temperature sequence
            unsigned int _wsenstemp[SEQUENCES][WSENSORS] = { 0 }; // Wireless sensor number and its sensor for the temperature sequence
            unsigned int _hum[SEQUENCES] = { 0 }; // Sequence data sources for the humidity sequence: 0-Nothing, 1-Thingspeak, 2-Wireless sensor, 3-BME280, 4-SHT21, 5-DHT22, 6-Forecast, 7-BME680
            unsigned int _thnghum[SEQUENCES] = { 0 }; // Thingspeak field number for the humidity sequence
            unsigned int _wsenshum[SEQUENCES] =  { 0 }; // Wireless sensor number for the humidity sequence
            friend class Config;

        public:
            const unsigned int dur() const { return _dur; }
            const char* name(unsigned int slot) const { if(slot >= SEQUENCES) return ""; return _name[slot]; }
            const unsigned int temp(unsigned int slot) const { if(slot >= SEQUENCES) return 0; if(_temp[slot] > 10) return 0; return _temp[slot]; }
            const unsigned int thngtemp(unsigned int slot) const { if(slot >= SEQUENCES) return 0; if(_thngtemp[slot] >= THNG_FIELDS) return 0; return _thngtemp[slot]; }
            const unsigned int wsenstemp(unsigned int slot, unsigned int ws) const { if(slot >= SEQUENCES) return 0; if(ws > 1) return 0; if(ws == 0 and _wsenstemp[slot][0] >= WSENSORS) return 0; if(ws == 1 and _wsenstemp[slot][1] >= WSENSOR_TEMPS) return 0; return _wsenstemp[slot][ws]; }
            const unsigned int hum(unsigned int slot) const { if(slot >= SEQUENCES) return 0; if(_hum[slot] > 7) return 0; return _hum[slot]; }
            const unsigned int thnghum(unsigned int slot) const { if(slot >= SEQUENCES) return 0; if(_thnghum[slot] >= THNG_FIELDS) return 0; return _thnghum[slot]; }
            const unsigned int wsenshum(unsigned int slot) const { if(slot >= SEQUENCES) return 0; if(_wsenshum[slot] >= WSENSORS) return 0; return _wsenshum[slot]; }
    };


    private:
        unsigned int _descr = 0; // Additional description data source: 0-Nothing, 1-Comfort level, 2-Sequence
        friend class Config;

    public:
        BaseSensor bat;
        BaseSensor humOut;
        BaseSensor humIn;
        BaseSensor presOut;
        TempSensor tempOut;
        TempSensor tempIn;
        VoltSensor volt;
        Wind wind;
        Sequence sequence;
        const unsigned int descr() const { if(_descr > 2) return 0; return _descr; }
};