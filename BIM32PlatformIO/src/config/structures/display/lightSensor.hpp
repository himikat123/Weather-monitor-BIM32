#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"

class Config;

struct LightSensorStruct {
    private:
        unsigned int _type[DISPLAYS] = {1, 1}; // Sensor type for brightness adjust: 0-Analog input, 1-MAX44009, 2-BH1750
        unsigned int _sensitivity[DISPLAYS] = {50, 50}; // Ambient light sensor sensibility 1...100
        friend class Config;

    public:
        const unsigned int type(unsigned int num) const { if(num >= DISPLAYS) return 0; if(_type[num] > 2) return 0; return _type[num]; }
        const unsigned int sensitivity(unsigned int num) const { if(num >= DISPLAYS) return 50; if(_sensitivity[num] < 1 or _sensitivity[num] > 100) return 50; return _sensitivity[num]; }
        void setSensitivity(int snstv, unsigned int num) { if(num >= DISPLAYS) return; if(snstv >= 1 and snstv <= 200) _sensitivity[num] = snstv; }
};