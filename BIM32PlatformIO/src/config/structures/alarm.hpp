#pragma once
#include <Arduino.h>
#include "../../globals.hpp"

class Config;

struct AlarmStruct {
    private:
        unsigned int _time[ALARMS][2] = { // Alarm time [hour, minute]
            {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}
        };
        unsigned int _weekdays[ALARMS][7] = { 0 }; // Alarm days of the week [mo, tu, we, th, fr, sa, su]
        unsigned int _states[ALARMS] = { 0 }; // Alarm state (0-Off, 1-On)
        unsigned int _melodies[ALARMS] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }; // Alarm selected melody (1...20)
        friend class Config;

    public:
        const unsigned int time(unsigned int num, unsigned int level) const {return (num > ALARMS or level > 1) ? 7 : _time[num][level]; }  
        const unsigned int weekday(unsigned int num, unsigned int day) const { return (num > ALARMS or day > 6) ? 0 : _weekdays[num][day]; }
        const unsigned int state(unsigned int num) const { return (num > ALARMS) ? 0 : _states[num]; }
        const unsigned int melodie(unsigned int num) const { return (num > ALARMS) ? 0 : _melodies[num]; }
        void setState(uint8_t num, uint8_t state) { if(num > ALARMS) return; _states[num] = state ? 1 : 0; }
        void setTime(uint8_t num, uint8_t level, uint8_t val) { if(num > ALARMS or level > 1 or val > (level == 0 ? 23 : 59)) return; _time[num][level] = val; }
        void setWeekday(uint8_t num, uint8_t day, uint8_t val) { if(num > ALARMS or day > 6) return; _weekdays[num][day] = val ? 1 : 0; };
};