#pragma once
#include <Arduino.h>

#include "../../globals.hpp"

class Config;

struct SoundStruct {
    private:
        unsigned int _vol = 15; // Sound volume
        unsigned int _eq = 0; // Equalizer: 0-Normal, 1-Pop, 2-Rock, 3-Jazz, 4-Classic, 5-Bass
        unsigned int _hourly = 2; // Hourly signal: 0-Always ON, 1-Always OFF, 2-On from dawn to dusk, 3-Enabled by time
        char _hourFrom[6] = "07:00"; // The hour from which the hourly signal is turned on
        char _hourTo[6] = "22:00"; // The hour from which the hourly signal is turned off
        friend class Config;

    public:
        const unsigned int vol() const { return _vol; }
        const unsigned int eq() const { return _eq; }
        const unsigned int hourly() const { return _hourly; }
        const unsigned int hourFrom(bool level) const { return getTime(level, _hourFrom); }
        const unsigned int hourTo(bool level) { return getTime(level, _hourTo); }
        void setVol(unsigned int vol) { _vol = vol; }
        void setEq(unsigned int eq) { _eq = eq; }
};