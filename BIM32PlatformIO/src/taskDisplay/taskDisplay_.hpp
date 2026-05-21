#pragma once

#include "./globals.hpp"
#include "./pinout.hpp"
#include "./config/config.hpp"
#include "./state/state.hpp"
#include "./sensors/sensors.hpp"
#include "./weather/weather.hpp"
#include "./displays/nextion/nextion.hpp"
#include "./displays/ili9341/ili9341.hpp"
#include "./displays/ws2812/ws2812display.hpp"
#include "./displays/max7219/max7219segmentDisplay.hpp"
#include "./displays/tm1637/tm1637display.hpp"
#include "./displays/numitron/numitronSegmentDisplay.hpp"

ILI9341 ili9341;
WS2812b ws2812b_1;
WS2812b ws2812b_2;
MAX7219_S max7219_1;
MAX7219_S max7219_2;
TM1637 tm1637_1;
TM1637 tm1637_2;
Numitron numitron_1;
Numitron numitron_2;
Weather weather;

void TaskDisplay1(void *pvParameters);
void TaskDisplay2(void *pvParameters);
unsigned int get_brightness(unsigned int dispNum);
bool isTimeoutOffTime(unsigned int dispNum);
bool isNightOffTime(unsigned int dispNum);

void TaskDisplay1(void *pvParameters) {
    (void) pvParameters;
    unsigned int millis_05s = 0;

    #if defined(BIM32_CYD)
        ili9341.showHomeScreen();
    #else
        if(config.display.type(DISPLAY_1) == LCD_DISPLAY) {
            /* Initialize LCD display */
            if(
                config.display.model(DISPLAY_1) == D_NX4832K035 or 
                config.display.model(DISPLAY_1) == D_NX4832T035 or
                config.display.model(DISPLAY_1) == D_NX4827K043
            ) nextion.init();
            if(config.display.model(DISPLAY_1) == D_ILI9341) {
                ili9341.showHomeScreen();
            }
        }

        /* Initialize WS2812b display 1 */
        if(config.display.type(DISPLAY_1) == PIXEL_LEDS_DISPLAY) {
            ws2812b_1.init(DISPLAY_1, WS2812_1_DAT_PIN);
        }

        /* Initialize 7 segment display */
        if(config.display.type(DISPLAY_1) == SEGMENT_DISPLAY) {
            /* Initialize TM1637 display 1 */
            if(config.display.model(DISPLAY_1) <= D_TM1637) 
                tm1637_1.init(DISPLAY_1, TM1637_1_CLK_PIN, TM1637_1_DAT_PIN);
            /* Initialize MAX1637 display 1 */
            if(config.display.model(DISPLAY_1) >= D_MAX7219) 
                max7219_1.init(DISPLAY_1, MAX7219_1_CLK_PIN, MAX7219_1_DAT_PIN, MAX7219_1_LOAD_PIN);
        }

        /* Initialize numitron display */
        if(config.display.type(DISPLAY_1) == NUMITRON_DISPLAY)
            numitron_1.init(DISPLAY_1, NUMITRON_1_SCL_PIN, NUMITRON_1_SDA_PIN, NUMITRON_1_PWM_PIN, WS2812_1_DAT_PIN);
    #endif

    while(1) {
        #if defined(BIM32_CYD)
            if(millis() - millis_05s >= 500) {
                millis_05s = millis();
                ili9341.refresh();
                ili9341.brightness(get_brightness(DISPLAY_1));

                if(isTimeoutOffTime(DISPLAY_1)) {
                    if(ili9341.isdisplayOn()) ili9341.displayOff();
                }

                uint8_t itsOffTime = isNightOffTime(DISPLAY_1) ? 1 : 0;
                if(state.disp_night_state[DISPLAY_1] != itsOffTime || state.display_state[DISPLAY_1]) {
                    state.disp_night_state[DISPLAY_1] = itsOffTime;
                    if(state.display_state[DISPLAY_1]) {
                        itsOffTime = state.display_state[DISPLAY_1] - 1;
                        state.display_state[DISPLAY_1] = 0;
                    }
                    if(itsOffTime) if(ili9341.isdisplayOn()) ili9341.displayOff();
                    else if(!ili9341.isdisplayOn()) ili9341.displayOn(bool doinit);
                }
            }

            ili9341.getTouch();
        #else 
            if(config.display.type(DISPLAY_1)) {
                /* Display 1 toogle if display button was pressed */
                if(state.display_btn_pressed[DISPLAY_1]) {
                    state.display_btn_pressed[DISPLAY_1] = false;
                    state.disp_autoOff[DISPLAY_1] = millis();

                    if(config.display.type(DISPLAY_1) == LCD_DISPLAY) {
                        if(
                            config.display.model(DISPLAY_1) == D_NX4832K035 or 
                            config.display.model(DISPLAY_1) == D_NX4832T035 or
                            config.display.model(DISPLAY_1) == D_NX4827K043
                        ) {
                            nextion.displayToggle();
                        }
                        if(config.display.model(DISPLAY_1) == D_ILI9341) {
                            ili9341.displayToggle();
                        }
                    }

                    if(config.display.type(DISPLAY_1) == PIXEL_LEDS_DISPLAY) {
                        ws2812b_1.displayToggle();
                    }

                    if(config.display.type(DISPLAY_1) == SEGMENT_DISPLAY) {
                        if(config.display.model(DISPLAY_1) <= D_TM1637) tm1637_1.displayToggle();
                        if(config.display.model(DISPLAY_1) >= D_MAX7219) max7219_1.displayToggle();
                    }

                    if(config.display.type(DISPLAY_1) == NUMITRON_DISPLAY) {
                        numitron_1.displayToggle();
                    }

                    vTaskDelay(500);
                }

                /* Once in 0.5 second */
                if(millis() - millis_05s >= 500) {
                    millis_05s = millis();

                    if(config.display.type(DISPLAY_1) == LCD_DISPLAY) {
                        /* LCD Display update */
                        if(
                            config.display.model(DISPLAY_1) == D_NX4832K035 or 
                            config.display.model(DISPLAY_1) == D_NX4832T035 or
                            config.display.model(DISPLAY_1) == D_NX4827K043
                        ) {
                            nextion.refresh();
                        }
                        if(config.display.model(DISPLAY_1) == D_ILI9341) {
                            ili9341.refresh();
                        }
                    }

                    /* 7 segment display slow down points blinking frequency if the device isn't connected to the network */
                    uint16_t dotFreq = state.net_connected ? 500 : 1000;
                    ws2812b_1.setDotFreq(dotFreq);
                    tm1637_1.setDotFreq(dotFreq);
                    max7219_1.setDotFreq(dotFreq);
                    numitron_1.setDotFreq(dotFreq);

                    /* WS2812b brightness change */
                    if(config.display.type(DISPLAY_1) == PIXEL_LEDS_DISPLAY) {
                        ws2812b_1.brightness(get_brightness(DISPLAY_1), state.reduc[DISPLAY_1]);
                    }

                    if(config.display.type(DISPLAY_1) == SEGMENT_DISPLAY) {
                        /* tm1637 brightness change */
                        if(config.display.model(DISPLAY_1) <= D_TM1637) {
                            tm1637_1.brightness(get_brightness(DISPLAY_1), state.reduc[DISPLAY_1]);
                        }
                        /* max7219 brightness change */
                        if(config.display.model(DISPLAY_1) >= D_MAX7219) {
                            max7219_1.brightness(get_brightness(DISPLAY_1), state.reduc[DISPLAY_1]);
                        }
                    }

                    if(config.display.type(DISPLAY_1) == NUMITRON_DISPLAY) {
                        numitron_1.brightness(get_brightness(DISPLAY_1), state.reduc[DISPLAY_1]);
                    }

                    /* LCD/TFT display brightness change */
                    unsigned int br = get_brightness(DISPLAY_1);
                    nextion.brightness(br);
                    ili9341.brightness(br);

                    /* Check if need and it's time to turn off the display */
                    if(isTimeoutOffTime(DISPLAY_1)) {
                        if(config.display.type(DISPLAY_1) == LCD_DISPLAY) {
                            if(
                                config.display.model(DISPLAY_1) == D_NX4832K035 or 
                                config.display.model(DISPLAY_1) == D_NX4832T035 or
                                config.display.model(DISPLAY_1) == D_NX4827K043
                            ) {
                                if(nextion.isdisplayOn()) nextion.displayOff();
                            }
                            if(config.display.model(DISPLAY_1) == D_ILI9341) {
                                if(ili9341.isdisplayOn()) ili9341.displayOff();
                            }
                        }
                        if(config.display.type(DISPLAY_1) == PIXEL_LEDS_DISPLAY) {
                            if(ws2812b_1.isdisplayOn()) ws2812b_1.displayOff();
                        }
                        if(config.display.type(DISPLAY_1) == SEGMENT_DISPLAY) {
                            if(config.display.model(DISPLAY_1) <= D_TM1637) {
                                if(tm1637_1.isdisplayOn()) tm1637_1.displayOff();
                            }
                            if(config.display.model(DISPLAY_1) >= D_MAX7219) {
                                if(max7219_1.isdisplayOn()) max7219_1.displayOff();
                            }
                        }
                        if(config.display.type(DISPLAY_1) == NUMITRON_DISPLAY) {
                            if(numitron_1.isdisplayOn()) numitron_1.displayOff();
                        }
                    }

                    _itsOffTime(); // new
                }

                /* WS2812b display 1 update */
                if(config.display.type(DISPLAY_1) == PIXEL_LEDS_DISPLAY) {
                    ws2812b_1.refresh();
                }

                if(config.display.type(DISPLAY_1) == SEGMENT_DISPLAY) {
                    /* TM1637 display 1 update */
                    if(config.display.model(DISPLAY_1) <= D_TM1637) tm1637_1.refresh();
                    /* MAX7219 display 1 update */
                    if(config.display.model(DISPLAY_1) >= D_MAX7219) max7219_1.refresh();
                }

                /* Numitron display 1 update */
                if(config.display.type(DISPLAY_1) == NUMITRON_DISPLAY) {
                    numitron_1.refresh();
                }

                if(config.display.type(DISPLAY_1) == LCD_DISPLAY) {
                    /* Receive data from Nextion */
                    if(
                        config.display.model(DISPLAY_1) == D_NX4832K035 or 
                        config.display.model(DISPLAY_1) == D_NX4832T035 or
                        config.display.model(DISPLAY_1) == D_NX4827K043
                    ) {
                        nextion.dataReceive();
                    }

                    if(config.display.model(DISPLAY_1) == D_ILI9341) {
                        ili9341.getTouch();
                    }
                }
            }
        #endif

        vTaskDelay(10);
    }
}

void TaskDisplay2(void *pvParameters) {
    (void) pvParameters;
    unsigned int bright_update = 0;

    /* Initialize WS2812b display 2 */
    if(config.display.type(DISPLAY_2) == PIXEL_LEDS_DISPLAY) {
        ws2812b_2.init(DISPLAY_2, WS2812_2_DAT_PIN);
    }

    if(config.display.type(DISPLAY_2) == SEGMENT_DISPLAY) {
        /* Initialize TM1637 display 2 */
        if(config.display.model(DISPLAY_2) <= D_TM1637) 
            tm1637_2.init(DISPLAY_2, TM1637_2_CLK_PIN, TM1637_2_DAT_PIN);
        /* Initialize MAX7219 display 2 */
        if(config.display.model(DISPLAY_2) >= D_MAX7219) 
            max7219_2.init(DISPLAY_2, MAX7219_2_CLK_PIN, MAX7219_2_DAT_PIN, MAX7219_2_LOAD_PIN);
    }

    /* Initialize Numitron display 2 */
    if(config.display.type(DISPLAY_2) == NUMITRON_DISPLAY) {
        numitron_2.init(DISPLAY_2, NUMITRON_2_SCL_PIN, NUMITRON_2_SDA_PIN, NUMITRON_2_PWM_PIN, WS2812_2_DAT_PIN);
    }

    while(1) {
        if(config.display.type(DISPLAY_2)) {

            /* Display 2 toogle if display button was pressed */
            if(state.display_btn_pressed[DISPLAY_2]) {
                state.display_btn_pressed[DISPLAY_2] = false;
                state.disp_autoOff[DISPLAY_2] = millis();
                if(config.display.type(DISPLAY_2) == PIXEL_LEDS_DISPLAY) {
                    ws2812b_2.displayToggle();
                }
                if(config.display.type(DISPLAY_2) == SEGMENT_DISPLAY) {
                    if(config.display.model(DISPLAY_2) <= D_TM1637) tm1637_2.displayToggle();
                    if(config.display.model(DISPLAY_2) >= D_MAX7219) max7219_2.displayToggle();
                }
                if(config.display.type(DISPLAY_2) == NUMITRON_DISPLAY) {
                    numitron_2.displayToggle();
                }
            }

            if(config.display.type(DISPLAY_2) == PIXEL_LEDS_DISPLAY) {
                ws2812b_2.refresh();
            }

            if(config.display.type(DISPLAY_2) == SEGMENT_DISPLAY) {
                if(config.display.model(DISPLAY_2) <= D_TM1637) tm1637_2.refresh();
                if(config.display.model(DISPLAY_2) >= D_MAX7219) max7219_2.refresh();
            }

            if(config.display.type(DISPLAY_2) == NUMITRON_DISPLAY) {
                numitron_2.refresh();
            }

            /* Brightness change once in 1 second */
            if((millis() - bright_update) > 1000) {
                bright_update = millis();

                /* 7 segment display slow down points blinking frequency if the device isn't connected to the network */
                uint16_t dotFreq = state.net_connected ? 500 : 1000;
                ws2812b_2.setDotFreq(dotFreq);
                tm1637_2.setDotFreq(dotFreq);
                max7219_2.setDotFreq(dotFreq);
                numitron_2.setDotFreq(dotFreq);

                /* WS2812b brightness change */
                if(config.display.type(DISPLAY_2) == PIXEL_LEDS_DISPLAY) {
                    ws2812b_2.brightness(get_brightness(DISPLAY_2), state.reduc[DISPLAY_2]);
                }

                if(config.display.type(DISPLAY_2) == SEGMENT_DISPLAY) {
                    /* tm1637 brightness change */
                    if(config.display.model(DISPLAY_2) <= D_TM1637) {
                        tm1637_2.brightness(get_brightness(DISPLAY_2), state.reduc[DISPLAY_2]);
                    }
                    /* max7219 brightness change */
                    if(config.display.model(DISPLAY_2) >= D_MAX7219) {
                        max7219_2.brightness(get_brightness(DISPLAY_2), state.reduc[DISPLAY_2]);
                    }
                }

                /* Numitron brightness change */
                if(config.display.type(DISPLAY_2) == NUMITRON_DISPLAY) {
                    numitron_2.brightness(get_brightness(DISPLAY_2), state.reduc[DISPLAY_2]);
                }

                /* Check if need and it's time to turn off the display */
                if(isTimeoutOffTime(DISPLAY_2)) {
                    if(config.display.type(DISPLAY_2) == PIXEL_LEDS_DISPLAY) { 
                        if(ws2812b_2.isdisplayOn()) ws2812b_2.displayOff();
                    }
                    if(config.display.type(DISPLAY_2) == SEGMENT_DISPLAY) {
                        if(config.display.model(DISPLAY_2) <= D_TM1637) {
                            if(tm1637_2.isdisplayOn()) tm1637_2.displayOff();
                        }
                        if(config.display.model(DISPLAY_2) >= D_MAX7219) {
                            if(max7219_2.isdisplayOn()) max7219_2.displayOff();
                        }
                    }
                    if(config.display.type(DISPLAY_2) == NUMITRON_DISPLAY) {
                        if(numitron_2.isdisplayOn()) numitron_2.displayOff();
                    }
                }

                _itsOffTime(); // new
            }
        }

        vTaskDelay(10);
    }
}

