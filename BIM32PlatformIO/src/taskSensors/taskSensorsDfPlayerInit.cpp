#include <Arduino.h>
#include "./taskSensors.hpp"
#include "../state/state.hpp"
#include "../sound/sound.hpp"
#include "../config/config.hpp"
#include "../pinout.hpp"


void TaskSensors::_DfPlayerInit() {
    if(!state.mp3_found && _sound_init_attempts > 0) {
        if(_sound_init_step == 0) {
            if(_sound_init == 0) {
                _sound_init = millis(); 
                Serial.println("Waiting for DFPlayer hardware to pull up BUSY line...");
            }

            if(digitalRead(MP3_BUSY_PIN) == HIGH) {
                sound.init();
                _sound_init = millis();
                _sound_init_step = 1;    
            }
            else if(millis() - _sound_init > 3000) {
                _sound_init_attempts = 0; 
                state.mp3_found = false;
                _sound_init = 0;

                Serial.println(SEPARATOR);
                Serial.println("DFPlayer NOT found. Disabling sound.");
            }
        }

        else if(_sound_init_step == 1) {
            if(millis() - _sound_init >= 2000) {
                if(digitalRead(MP3_BUSY_PIN) == HIGH) {
                    state.mp3_busy = false;
                    state.mp3_found = true;
                    _sound_init = 0;        
                    _sound_init_step = 2;
                }
                else {
                    _sound_init_step = 0; 
                    _sound_init = 0;
                    _sound_init_attempts--;
                    Serial.println("DFPlayer did not recover after reset, retrying hardware check...");
                }
            }
        }
    }

    if(state.mp3_found && _sound_init_step == 2) {
        sound.equalizer(config.sound.eq());
        vTaskDelay(pdMS_TO_TICKS(100)); 
        sound.volume(config.sound.vol());
        vTaskDelay(pdMS_TO_TICKS(100));

        _sound_init_attempts = -1;
        _sound_init_step = 3;

        Serial.println(SEPARATOR);
        Serial.println("DFPlayer successfully FOUND");
        Serial.println(SEPARATOR);
    }
}