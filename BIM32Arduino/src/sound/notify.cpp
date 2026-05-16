#include "./sound.hpp"
#include "../globals.hpp"

void Sound::tempNotify(int tempLevel) {
    if(_isAllowed()) {
        if(_prevTempLevel != tempLevel) {
            _prevTempLevel = tempLevel;
            switch(tempLevel) {
                case TEMP_COMFORTABLE: play(1, 25); break;
                case TEMP_TOO_COLD: play(1, 26); break;
                case TEMP_TOO_HOT: play(1, 27); break;
                default: ; break;
            }
        }
    }
}

void Sound::humNotify(int humLevel) {
    if(_isAllowed()) {
        if(_prevHumLevel != humLevel) {
            _prevHumLevel = humLevel;
            switch(humLevel) {
                case HUM_COMFORTABLE: play(1, 28); break;
                case HUM_TOO_DRY: play(1, 29); break;
                case HUM_TOO_HUMID: play(1, 30); break;
                default: ; break;
            }
        }
    }
}

void Sound::airNotify(unsigned int airLevel) {
    if(_isAllowed()) {
        if(_prevAirLevel != airLevel) {
            _prevAirLevel = airLevel;
            switch(airLevel) {
                case AIR_CLEAN: play(1, 31); break;
                case AIR_POLLUTED: play(1, 32); break;
                case AIR_HEAVILY_POLLUTED: play(1, 33); break;
                default: ; break;
            }
        }
    }
}