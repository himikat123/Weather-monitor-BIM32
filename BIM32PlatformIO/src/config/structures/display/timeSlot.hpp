#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"

class Config;

struct TimeSlotStruct {
    struct Wsensor {
        private:
            unsigned int _num[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot wireless sensor number
            unsigned int _type[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot wireless sensor type 
            friend class Config;

        public:
            const unsigned int num(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; return _num[slot][displayNum]; }
            const unsigned int type(unsigned int slot, unsigned int displayNum) { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; return _type[slot][displayNum]; }
    };


    private:
        unsigned int _period[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot durations
        unsigned int _sensor[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot data sources: 0-Time, 1-Date, 2-BME280, 3-BMP180, 4-SHT21, 5-DHT22, 6-DS18B20, 7-ESP32, 8-Thingspeak, 9-Weather forecast, 10-Wireless sensor, 11-BME680 
        unsigned int _data[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot sensors types
        unsigned int _thing[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot thingspeak field number
        char _color[TIMESLOTS][DISPLAYS][8] = { // Segment display timeslot colors 
            {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"},
            {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}
        };
        friend class Config;

    public:
        Wsensor wsensor;
        const unsigned int period(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; if(_period[slot][displayNum] > 99) return 0; return _period[slot][displayNum]; }
        const unsigned int sensor(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; if(_sensor[slot][displayNum] > 11) return 0; return _sensor[slot][displayNum]; }
        const unsigned int data(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; if(_data[slot][displayNum] > 4) return 0; return _data[slot][displayNum]; }
        const unsigned int thing(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; if(_thing[slot][displayNum] > 7) return 0; return _thing[slot][displayNum]; }
        const char* color(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return (char*) ""; if(displayNum >= DISPLAYS) return (char*) ""; return _color[slot][displayNum]; }
        void setColor(char color[6], unsigned int slotNum, unsigned int displayNum) {
            if(slotNum >= TIMESLOTS) return;
            if(displayNum >= DISPLAYS) return;
            _color[slotNum][displayNum][0] = '#';
            for(unsigned int i=1; i<7; i++) _color[slotNum][displayNum][i] = color[i - 1];
            _color[slotNum][displayNum][7] = '\0';
        }
};