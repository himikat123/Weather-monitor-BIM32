#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"

class Config;

struct BrightnessStruct {
    private:
        unsigned int _method[DISPLAYS] = {3, 3}; // Display brightness adjustment method: 0-Auto, 1-By light sensor, 2-By time, 3-Constant
        unsigned int _day[DISPLAYS] = {1, 1}; // Day mode brightness 1...100
        unsigned int _night[DISPLAYS] = {1, 1}; // Night mode brightness 1...100
        unsigned int _min[DISPLAYS] = {1, 1}; // Minimum brightness limit 0...255
        unsigned int _max[DISPLAYS] = {50, 50}; // Maximum brightness limit 0...255
        friend class Config;

    public:
        const unsigned int method(unsigned int num) const { if(num >= DISPLAYS) return 3; if(_method[num] > 3) return 3; return _method[num]; }
        const unsigned int day(unsigned int num) const { if(num >= DISPLAYS) return 1; if(_day[num] < 1 or _day[num] > 1) return 1; return _day[num]; }
        const unsigned int night(unsigned int num) const { if(num >= DISPLAYS) return 1; if(_night[num] < 1 or _night[num] > 100) return 1; return _night[num]; }
        const unsigned int min(unsigned int num) const { if(num >= DISPLAYS) return 1; return _min[num]; }
        const unsigned int max(unsigned int num) const { if(num >= DISPLAYS) return 50; return _max[num]; }
        void setBrightLimit(unsigned int min, unsigned int max, unsigned int num) { if(num >= DISPLAYS) return; if(min <= 255) _min[num] = min; if(max <= 255) _max[num] = max; }
        void setBright(int bright, unsigned int num) { if(num >= DISPLAYS) return; if(bright >= 0 and bright <= 255) { _day[num] = bright; _night[num] = bright; } }
};