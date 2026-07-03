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
    Serial.println(SEPARATOR);
    Serial.println("Initialize DF Player...");

    rmt_config_t rmt_tx;
    rmt_tx.rmt_mode = RMT_MODE_TX;
    rmt_tx.channel = RMT_CHANNEL_4;
    rmt_tx.gpio_num = (gpio_num_t)MP3_TX_PIN;
    rmt_tx.mem_block_num = 1;
    rmt_tx.clk_div = 80;
    rmt_tx.tx_config.loop_en = false;
    rmt_tx.tx_config.carrier_en = false;
    rmt_tx.tx_config.idle_output_en = true;
    rmt_tx.tx_config.idle_level = RMT_IDLE_LEVEL_HIGH;
    rmt_config(&rmt_tx);
    rmt_driver_install(rmt_tx.channel, 0, 0);

    if(digitalRead(MP3_BUSY_PIN)) {
        time_t mils = millis();
        _reset();
        while(1) { 
            if(!state.mp3_busy) {
                state.mp3_busy = true;
                _mp3_found = true;
                break;
            }
            if(millis() - mils > 2000) break;
        }
    }
    Serial.printf("DFPlayer %sfound\r\n", _mp3_found ? "" : "NOT ");
}

/**
 * Change volume
 */
void Sound::volume(unsigned int vol) {
    if(vol <= 30) _sendCommand(0x06, 0x00, vol);
}

/**
 * Change equalizer
 */
void Sound::equalizer(unsigned int eq) {
    if(eq <= 5) _sendCommand(0x07, 0x00, eq);
}

/**
 * Play a track
 * @param folder number
 * @param track number
 */
void Sound::play(unsigned int folder, unsigned int track) {
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
    _sendCommand(0x16, 0x00, 0x00);
}

/**
 * Check if sound is allowed
 */
bool Sound::_isAllowed() {
    if(_mp3_found and digitalRead(MP3_BUSY_PIN)) {
        switch(config.sound.hourly()) {
            case 0: return true;
            case 2: if(weather.get_isDay()) return true;
            case 3: return _hourlyCheck();
            default: return false;
        }
    }
    return false;
}