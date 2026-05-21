#include <Arduino.h>
#include <TimeLib.h>

#include "./taskDisplay.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/**
 * Check if need be and it's time to turn off display
 */
bool TaskDisplay::_isNightOffTime() {
    if(config.display.nightOff.need(_dispNum)) {
        uint16_t fromM = config.display.nightOff.from(_dispNum, false) * 60 + config.display.nightOff.from(_dispNum, true);
        uint16_t toM = config.display.nightOff.to(_dispNum, false) * 60 + config.display.nightOff.to(_dispNum, true);
        uint16_t nowM = hour() * 60 + minute();

        if(fromM < toM) return (fromM <= nowM && nowM < toM);
        else return (nowM >= fromM || nowM < toM);
    }
    return false;
}

void TaskDisplay::_itsOffTime() {
    uint8_t itsOffTime = _isNightOffTime() ? 1 : 0;
    if((state.disp_night_state[_dispNum] != itsOffTime) || (state.display_state[_dispNum] > 0)) {
        state.disp_night_state[_dispNum] = itsOffTime;
        if(state.display_state[_dispNum] > 0) {
            itsOffTime = state.display_state[_dispNum] - 1;
            state.display_state[_dispNum] = 0;
        }
        if(itsOffTime) {
            if(config.display.type(_dispNum) == LCD_DISPLAY) {
                if(config.display.model(_dispNum) <= D_NX4827K043) {
                    if(nextion.isDisplayOn()) nextion.displayOff();
                }
                if(config.display.model(_dispNum) == D_ILI9341) {
                    if(ili9341.isDisplayOn()) ili9341.displayOff();
                }
            }

            if(config.display.type(_dispNum) == PIXEL_LEDS_DISPLAY) {
                if(_ws2812b->isDisplayOn()) _ws2812b->displayOff();
            }

            if(config.display.type(_dispNum) == SEGMENT_DISPLAY) {
                if(config.display.model(_dispNum) <= D_TM1637) {
                    if(_tm1637->isDisplayOn()) _tm1637->displayOff();
                }
                if(config.display.model(_dispNum) >= D_MAX7219) {
                    if(_max7219s->isDisplayOn()) _max7219s->displayOff();
                }
            }

            if(config.display.type(_dispNum) == NUMITRON_DISPLAY) {
                if(_numitron->isDisplayOn()) _numitron->displayOff();
            }
        }
        else {
            if(config.display.type(_dispNum) == LCD_DISPLAY) {
                if(config.display.model(_dispNum) <= D_NX4827K043) {
                    if(!nextion.isDisplayOn()) nextion.displayOn(false);
                }
                if(config.display.model(_dispNum) == D_ILI9341) {
                    if(!ili9341.isDisplayOn()) ili9341.displayOn();
                }
            }

            if(config.display.type(_dispNum) == PIXEL_LEDS_DISPLAY) {
                if(!_ws2812b->isDisplayOn()) _ws2812b->displayOn();
            }

            if(config.display.type(_dispNum) == SEGMENT_DISPLAY) {
                if(config.display.model(_dispNum) <= D_TM1637) {
                    if(!_tm1637->isDisplayOn()) _tm1637->displayOn();
                }
                if(config.display.model(_dispNum) >= D_MAX7219) {
                    if(!_max7219s->isDisplayOn()) _max7219s->displayOn();
                }
            }

            if(config.display.type(_dispNum) == NUMITRON_DISPLAY) {
                if(!_numitron->isDisplayOn()) _numitron->displayOn();
            }
        }
    }
}