#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"

class Config;

struct Animation {
    private:
        unsigned int _type[DISPLAYS] = { 0 }; // Display animation number 0...9
        unsigned int _speed[DISPLAYS] = { 10, 10 }; // Display animation speed 1...30
        unsigned int _points[DISPLAYS] = { 0 }; // Display animation clock points 0...4
        friend class Config;

    public:
        const unsigned int type(unsigned int num) const { if(num >= DISPLAYS) return 0; return _type[num]; }
        const unsigned int speed(unsigned int num) const { if(num >= DISPLAYS) return 0; return _speed[num]; }
        const unsigned int points(unsigned int num) const { if(num >= DISPLAYS) return 0; return _points[num]; }
        void setType(unsigned int type, unsigned int displayNum) { if(type <= 9 and displayNum < DISPLAYS) _type[displayNum] = type; }
        void setSpeed(unsigned int speed, unsigned int displayNum) { if(speed >= 1 and speed <= 30 and displayNum < DISPLAYS) _speed[displayNum] = speed; }
        void setPoints(unsigned int points, unsigned int displayNum) { if(points <= 7 and displayNum < DISPLAYS) _points[displayNum] = points; }
    };