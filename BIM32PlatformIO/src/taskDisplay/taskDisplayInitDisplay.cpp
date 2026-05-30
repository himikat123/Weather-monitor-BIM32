#include <Arduino.h>

#include "../globals.hpp"
#include "../config/config.hpp"
#include "./taskDisplay.hpp"

void TaskDisplay::_initDisplay() {
    int type = config.display.type(_dispNum);
    int model = config.display.model(_dispNum);

    if(type == LCD_DISPLAY) {
        if(model == D_NX4832K035 or model == D_NX4832T035 or model == D_NX4827K043) nextion.init();
        if(model == D_ILI9341) ili9341.showHomeScreen();
    }

    if(type == PIXEL_LEDS_DISPLAY) {
        if(_dispNum == DISPLAY_1) _pixelLed_1.init(DISPLAY_1, WS2812_1_DAT_PIN);
        if(_dispNum == DISPLAY_2) _pixelLed_2.init(DISPLAY_2, WS2812_2_DAT_PIN);
    }

    if(type == SEGMENT_DISPLAY) {
        if(_dispNum == DISPLAY_1) {
            if(model <= D_TM1637) _tm1637_1.init(DISPLAY_1, TM1637_1_CLK_PIN, TM1637_1_DAT_PIN);
            if(model >= D_MAX7219) _max7219s_1.init(DISPLAY_1, MAX7219_1_CLK_PIN, MAX7219_1_DAT_PIN, MAX7219_1_LOAD_PIN);
        }
        if(_dispNum == DISPLAY_2) {
            if(model <= D_TM1637) _tm1637_2.init(DISPLAY_2, TM1637_2_CLK_PIN, TM1637_2_DAT_PIN);
            if(model >= D_MAX7219) _max7219s_2.init(DISPLAY_2, MAX7219_2_CLK_PIN, MAX7219_2_DAT_PIN, MAX7219_2_LOAD_PIN);
        }
    }

    if(type == NUMITRON_DISPLAY) {
        if(_dispNum == DISPLAY_1)
            _numitron_1.init(DISPLAY_1, NUMITRON_1_SCL_PIN, NUMITRON_1_SDA_PIN, NUMITRON_1_PWM_PIN, WS2812_1_DAT_PIN);
        if(_dispNum == DISPLAY_2)
            _numitron_2.init(DISPLAY_2, NUMITRON_2_SCL_PIN, NUMITRON_2_SDA_PIN, NUMITRON_2_PWM_PIN, WS2812_2_DAT_PIN);
    }
}