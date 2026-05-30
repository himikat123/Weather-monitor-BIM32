#pragma once
#include <Arduino.h>

#include "./animation.hpp"
#include "./nightOff.hpp"
#include "./brightness.hpp"
#include "./lightSensor.hpp"
#include "./source.hpp"
#include "./timeSlot.hpp"
#include "../../../globals.hpp"

class Config;

struct DisplayStruct {
    private:
        unsigned int _type[DISPLAYS] = { 0 }; // Display type
        unsigned int _model[DISPLAYS] = { 0 }; // Display model
        unsigned int _cntLeds[DISPLAYS] = { 0 }; // LEDs per segment
        unsigned int _sLed[DISPLAYS] = { 0 }; // Use a sacrificial LED
        unsigned int _order[DISPLAYS][8] = { {1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 4, 5, 6, 7, 8} }; // Digits order
        char _dayTime[DISPLAYS][6] = {"07:00", "07:00"}; // Time to switch to day mode
        char _nightTime[DISPLAYS][6] = {"21:00", "21:00"}; // Time to switch to night mode
        unsigned int _autoOff[DISPLAYS] = { 0 }; // Display auto-off time 0...1440
        friend class Config;

    public:
        AnimationStruct animation;
        NightOffStruct nightOff;
        BrightnessStruct brightness;
        LightSensorStruct lightSensor;
        SourceStruct source;
        TimeSlotStruct timeSlot;
        const unsigned int type(unsigned int num) const { if(num >= DISPLAYS) return 0; return _type[num]; }
        const unsigned int model(unsigned int num) const { if(num >= DISPLAYS) return 0; return _model[num]; }
        const unsigned int cntLeds(unsigned int num) const { if(num >= DISPLAYS) return 0; return _cntLeds[num]; }
        const unsigned int sLed(unsigned int num) const { if(num >= DISPLAYS) return 0; return _sLed[num]; }
        const unsigned int order(unsigned int num, unsigned int dig) const { if(num >= DISPLAYS || dig > 7) return 0; return _order[num][dig]; }
        const unsigned int dayTime(unsigned int num, bool level) const { if(num >= DISPLAYS) return 0; return getTime(level, _dayTime[num]); }
        const unsigned int nightTime(unsigned int num, bool level) const { if(num >= DISPLAYS) return 0; return getTime(level, _nightTime[num]); }
        const unsigned int autoOff(unsigned int num) const { if(num >= DISPLAYS) return 0; if(_autoOff[num] > 1440) return 0; return _autoOff[num]; }
};