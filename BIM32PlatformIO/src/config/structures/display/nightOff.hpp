#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"

class Config;

struct NightOffStruct {
    private:
        bool _need[DISPLAYS] = {false, false}; // Turn off display at night
        char _from[DISPLAYS][6] = {"23:00", "23:00"}; // The hour from which the display is turned off
        char _to[DISPLAYS][6] = {"07:00", "07:00"}; // The hour from which the display is turned on
        friend class Config;

    public:
        const bool need(unsigned int num) const { if(num >= DISPLAYS) return false; return _need[num]; }
        const unsigned int from(unsigned int num, bool level) const { if(num >= DISPLAYS) return 0; return getTime(level, _from[num]); }
        const unsigned int to(unsigned int num, bool level) const { if(num >= DISPLAYS) return 0; return getTime(level, _to[num]); }
};