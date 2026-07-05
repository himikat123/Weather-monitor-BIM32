#include <Arduino.h>

#include "./sound.hpp"
#include "../globals.hpp"
#include "../pinout.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../weather/weather.hpp"

/**
 * DF player initialization
 */
void Sound::init(void) {
    Serial.println("Initialize DF Player...");
    _initRMT();
    _reset(); 
}

/**
 * Change volume
 */
void Sound::volume(unsigned int vol) {
    if(state.mp3_found && vol <= 30) _sendCommand(0x06, 0x00, vol);
}

/**
 * Change equalizer
 */
void Sound::equalizer(unsigned int eq) {
    if(state.mp3_found && eq <= 5) _sendCommand(0x07, 0x00, eq);
}

/**
 * Play a track
 * @param folder number
 * @param track number
 */
void Sound::play(unsigned int folder, unsigned int track) {
    if(!state.mp3_found) return;

    equalizer(config.sound.eq());
    vTaskDelay(100);
    volume(config.sound.vol());
    vTaskDelay(100);

    if(folder==2 and track<=20) {
        folder = 1;
        track++;
    }
    else if(folder==1 and track<=33) {
        if(String(config.lang()) == "de") folder = 3;
        else if(String(config.lang()) == "ru") folder = 4;
        else if(String(config.lang()) == "pl") folder = 5;
        else if(String(config.lang()) == "ua") folder = 6;
        else if(String(config.lang()) == "bg") folder = 7;
        else if(String(config.lang()) == "es") folder = 8;
        else folder = 2;
        if(track == 0) track = 24;
    }

    for(uint8_t i=0; i<5; i++) {
        _sendCommand(0x0F, folder, track);
        time_t mils = millis();
        while(1) {
            if(!digitalRead(MP3_BUSY_PIN)) {
                i = 5;
                break;
            }
            if(millis() - mils > 10000) {
                _reset();
                vTaskDelay(1000);
                break;
            }
        }
    }
}

/**
 * Stop playing
 */
void Sound::stopPlaying(void) {
    if(state.mp3_found) _sendCommand(0x16, 0x00, 0x00);
}

/**
 * Check if sound is allowed
 */
bool Sound::_isAllowed() {
    if(state.mp3_found and digitalRead(MP3_BUSY_PIN)) {
        switch(config.sound.hourly()) {
            case 0: return true;
            case 2: if(weather.get_isDay()) return true;
            case 3: return _hourlyCheck();
            default: return false;
        }
    }
    return false;
}