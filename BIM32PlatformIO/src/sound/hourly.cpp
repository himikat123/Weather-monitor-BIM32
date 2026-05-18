#include <Arduino.h>
#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/

#include "./sound.hpp"
#include "../globals.hpp"
#include "../config/config.hpp"

bool Sound::_hourlyCheck() {
    uint16_t fromM = config.sound.hourFrom(false) * 60 + config.sound.hourFrom(true);
    uint16_t toM = config.sound.hourTo(false) * 60 + config.sound.hourTo(false);
    uint16_t nowM = hour() * 60 + minute();

    if(fromM < toM) return (fromM <= nowM && nowM < toM);
    else return (nowM >= fromM || nowM < toM);
}

/** 
 * Check if it is time to play hourly signal and playing
 */
void Sound::hourlySignal() {
    if(minute() == 0 and _hourly_rang != hour()) {
        _playHourlySignal();
    }
}

/** 
 * Playing an hourly sound and speaking the time every hour
 */
void Sound::_playHourlySignal() {
    if(_isAllowed()) {
        _hourly_rang = hour();
        play(1, hour());
    }
}