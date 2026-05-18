#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/

#include "./sound.hpp"
#include "../globals.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/** 
 * Check if it's time to ring the alarm
 */
void Sound::alarm() {
    if(_mp3_found) {
        for(int i=0; i<ALARMS; i++) {
            if(config.alarm.state(i) and config.alarm.weekday(i, weekday() == 1 ? 6 : weekday() - 2)) {
                if(config.alarm.time(i, 0) == hour() and config.alarm.time(i, 1) == minute() and _alarm_rang != minute()) { 
                    play(2, config.alarm.melodie(i));
                    _alarm_rang = minute();
                }
            }
        }
        if(minute() == 0 and second() >= 0 and second() <= 5) _alarm_rang = 60;

        // Stop alarm ringing if the alarm button was pressed
        if(state.alarm_but_pressed) {
            stopPlaying();
            state.alarm_but_pressed = false;
        }
    }
}