#define HOUR      0
#define MINUTE    1
#define DISPLAY_1 0
#define DISPLAY_2 1

unsigned int get_brightness(unsigned int display_num);
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
            if(config.display.model(DISPLAY_1) == D_ILI9341) ili9341.showHomeScreen();
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
            pcf8575_1.init(DISPLAY_1, PCF8575_1_SCL_PIN, PCF8575_1_SDA_PIN, NUMITRON_1_PWM_PIN, WS2812_1_DAT_PIN);
    #endif

    while(1) {
        #if defined(BIM32_CYD)
            if(millis() - millis_05s >= 500) {
                millis_05s = millis();
                ili9341.refresh();
                ili9341.brightness(get_brightness(DISPLAY_1));

                if(isTimeoutOffTime(DISPLAY_1)) {
                    if(ili9341.isDisplayOn()) ili9341.displayOff();
                }

                uint8_t itsOffTime = isNightOffTime(DISPLAY_1) ? 1 : 0;
                if(state.disp_night_state[DISPLAY_1] != itsOffTime || state.display_state[DISPLAY_1]) {
                    state.disp_night_state[DISPLAY_1] = itsOffTime;
                    if(state.display_state[DISPLAY_1]) {
                        itsOffTime = state.display_state[DISPLAY_1] - 1;
                        state.display_state[DISPLAY_1] = 0;
                    }
                    if(itsOffTime) if(ili9341.isDisplayOn()) ili9341.displayOff();
                    else if(!ili9341.isDisplayOn()) ili9341.displayOn();
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
                        pcf8575_1.displayToggle();
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
                    pcf8575_1.setDotFreq(dotFreq);

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
                        pcf8575_1.brightness(get_brightness(DISPLAY_1), state.reduc[DISPLAY_1]);
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
                                if(nextion.isDisplayOn()) nextion.displayOff();
                            }
                            if(config.display.model(DISPLAY_1) == D_ILI9341) {
                                if(ili9341.isDisplayOn()) ili9341.displayOff();
                            }
                        }
                        if(config.display.type(DISPLAY_1) == PIXEL_LEDS_DISPLAY) {
                            if(ws2812b_1.isDisplayOn()) ws2812b_1.displayOff();
                        }
                        if(config.display.type(DISPLAY_1) == SEGMENT_DISPLAY) {
                            if(config.display.model(DISPLAY_1) <= D_TM1637) {
                                if(tm1637_1.isDisplayOn()) tm1637_1.displayOff();
                            }
                            if(config.display.model(DISPLAY_1) >= D_MAX7219) {
                                if(max7219_1.isDisplayOn()) max7219_1.displayOff();
                            }
                        }
                        if(config.display.type(DISPLAY_1) == NUMITRON_DISPLAY) {
                            if(pcf8575_1.isDisplayOn()) pcf8575_1.displayOff();
                        }
                    }

                    uint8_t itsOffTime = isNightOffTime(DISPLAY_1) ? 1 : 0;
                    if((state.disp_night_state[DISPLAY_1] != itsOffTime) || (state.display_state[DISPLAY_1] > 0)) {
                        state.disp_night_state[DISPLAY_1] = itsOffTime;
                        if(state.display_state[DISPLAY_1] > 0) {
                            itsOffTime = state.display_state[DISPLAY_1] - 1;
                            state.display_state[DISPLAY_1] = 0;
                        }
                        if(itsOffTime) {
                            if(config.display.type(DISPLAY_1) == LCD_DISPLAY) {
                                if(
                                    config.display.model(DISPLAY_1) == D_NX4832K035 or 
                                    config.display.model(DISPLAY_1) == D_NX4832T035 or
                                    config.display.model(DISPLAY_1) == D_NX4827K043
                                ) {
                                    if(nextion.isDisplayOn()) nextion.displayOff();
                                }
                                if(config.display.model(DISPLAY_1) == D_ILI9341) {
                                    if(ili9341.isDisplayOn()) ili9341.displayOff();
                                }
                            }
                            if(config.display.type(DISPLAY_1) == PIXEL_LEDS_DISPLAY) {
                                if(ws2812b_1.isDisplayOn()) ws2812b_1.displayOff();
                            }
                            if(config.display.type(DISPLAY_1) == SEGMENT_DISPLAY) {
                                if(config.display.model(DISPLAY_1) <= D_TM1637) {
                                    if(tm1637_1.isDisplayOn()) tm1637_1.displayOff();
                                }
                                if(config.display.model(DISPLAY_1) >= D_MAX7219) {
                                    if(max7219_1.isDisplayOn()) max7219_1.displayOff();
                                }
                            }
                            if(config.display.type(DISPLAY_1) == NUMITRON_DISPLAY) {
                                if(pcf8575_1.isDisplayOn()) pcf8575_1.displayOff();
                            }
                        }
                        else {
                            if(config.display.type(DISPLAY_1) == LCD_DISPLAY) {
                                if(
                                    config.display.model(DISPLAY_1) == D_NX4832K035 or 
                                    config.display.model(DISPLAY_1) == D_NX4832T035 or
                                    config.display.model(DISPLAY_1) == D_NX4827K043
                                ) {
                                    if(!nextion.isDisplayOn()) nextion.displayOn(false);
                                }
                                if(config.display.model(DISPLAY_1) == D_ILI9341) {
                                    if(!ili9341.isDisplayOn()) ili9341.displayOn();
                                }
                            }
                            if(config.display.type(DISPLAY_1) == PIXEL_LEDS_DISPLAY) {
                                if(!ws2812b_1.isDisplayOn()) ws2812b_1.displayOn();
                            }
                            if(config.display.type(DISPLAY_1) == SEGMENT_DISPLAY) {
                                if(config.display.model(DISPLAY_1) <= D_TM1637) {
                                    if(!tm1637_1.isDisplayOn()) tm1637_1.displayOn();
                                }
                                if(config.display.model(DISPLAY_1) >= D_MAX7219) {
                                    if(!max7219_1.isDisplayOn()) max7219_1.displayOn();
                                }
                            }
                            if(config.display.type(DISPLAY_1) == NUMITRON_DISPLAY) {
                                if(!pcf8575_1.isDisplayOn()) pcf8575_1.displayOn();
                            }
                        }
                    }
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
                    pcf8575_1.refresh();
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
        pcf8575_2.init(DISPLAY_2, PCF8575_2_SCL_PIN, PCF8575_2_SDA_PIN, NUMITRON_2_PWM_PIN, WS2812_2_DAT_PIN);
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
                    pcf8575_2.displayToggle();
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
                pcf8575_2.refresh();
            }

            /* Brightness change once in 1 second */
            if((millis() - bright_update) > 1000) {
                bright_update = millis();

                /* 7 segment display slow down points blinking frequency if the device isn't connected to the network */
                uint16_t dotFreq = state.net_connected ? 500 : 1000;
                ws2812b_2.setDotFreq(dotFreq);
                tm1637_2.setDotFreq(dotFreq);
                max7219_2.setDotFreq(dotFreq);
                pcf8575_2.setDotFreq(dotFreq);

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

                /* PCF8575 brightness change */
                if(config.display.type(DISPLAY_2) == NUMITRON_DISPLAY) {
                    pcf8575_2.brightness(get_brightness(DISPLAY_2), state.reduc[DISPLAY_2]);
                }

                /* Check if need and it's time to turn off the display */
                if(isTimeoutOffTime(DISPLAY_2)) {
                    if(config.display.type(DISPLAY_2) == PIXEL_LEDS_DISPLAY) { 
                        if(ws2812b_2.isDisplayOn()) ws2812b_2.displayOff();
                    }
                    if(config.display.type(DISPLAY_2) == SEGMENT_DISPLAY) {
                        if(config.display.model(DISPLAY_2) <= D_TM1637) {
                            if(tm1637_2.isDisplayOn()) tm1637_2.displayOff();
                        }
                        if(config.display.model(DISPLAY_2) >= D_MAX7219) {
                            if(max7219_2.isDisplayOn()) max7219_2.displayOff();
                        }
                    }
                    if(config.display.type(DISPLAY_2) == NUMITRON_DISPLAY) {
                        if(pcf8575_2.isDisplayOn()) pcf8575_2.displayOff();
                    }
                }

                uint8_t itsOffTime = isNightOffTime(DISPLAY_2) ? 1 : 0;
                if((state.disp_night_state[DISPLAY_2] != itsOffTime) || (state.display_state[DISPLAY_2] > 0)) {
                    state.disp_night_state[DISPLAY_2] = itsOffTime;
                    if(state.display_state[DISPLAY_2] > 0) {
                        itsOffTime = state.display_state[DISPLAY_2] - 1;
                        state.display_state[DISPLAY_2] = 0;
                    }
                    if(itsOffTime) {
                        if(config.display.type(DISPLAY_2) == PIXEL_LEDS_DISPLAY) {
                            if(ws2812b_2.isDisplayOn()) ws2812b_2.displayOff();
                        }
                        if(config.display.type(DISPLAY_2) == SEGMENT_DISPLAY) {
                            if(config.display.model(DISPLAY_2) <= D_TM1637) {
                                if(tm1637_2.isDisplayOn()) tm1637_2.displayOff();
                            }
                            if(config.display.model(DISPLAY_2) >= D_MAX7219) {
                                if(max7219_2.isDisplayOn()) max7219_2.displayOff();
                            }
                        }
                        if(config.display.type(DISPLAY_2) == NUMITRON_DISPLAY) {
                            if(pcf8575_2.isDisplayOn()) pcf8575_2.displayOff();
                        }
                    }
                    else {
                        if(config.display.type(DISPLAY_2) == PIXEL_LEDS_DISPLAY) {
                            if(!ws2812b_2.isDisplayOn()) ws2812b_2.displayOn();
                        }
                        if(config.display.type(DISPLAY_2) == SEGMENT_DISPLAY) {
                            if(config.display.model(DISPLAY_2) <= D_TM1637) {
                                if(!tm1637_2.isDisplayOn()) tm1637_2.displayOn();
                            }
                            if(config.display.model(DISPLAY_2) >= D_MAX7219) {
                                if(!max7219_2.isDisplayOn()) max7219_2.displayOn();
                            }
                        }
                        if(config.display.type(DISPLAY_2) == NUMITRON_DISPLAY) {
                            if(!pcf8575_2.isDisplayOn()) pcf8575_2.displayOn();
                        }
                    }
                }
            }
        }

        vTaskDelay(10);
    }
}

/**
 * Display brightness calculation
 */
unsigned int get_brightness(unsigned int display_num) {
    switch(config.display.brightness.method(display_num)) {

        /* By sunrise and sunset */
        case 0:
            return weather.get_isDay() 
                ? config.display.brightness.day(display_num) 
                : config.display.brightness.night(display_num);

        /* By ambeint light sensor */
        case 1: {
            float bright = 0.0;
            if(config.display.lightSensor.type(display_num) == 0) {
                bright = validate.volt(sensors.get_analog_voltage()) 
                    ? sensors.get_analog_voltage() * 30 
                    : 20;
            }
            if(config.display.lightSensor.type(display_num) == 1) {
                bright = validate.light(sensors.get_max44009_light()) 
                    ? sensors.get_max44009_light() 
                    : 20;
            }
            if(config.display.lightSensor.type(display_num) == 2) {
                bright = validate.light(sensors.get_bh1750_light()) 
                    ? sensors.get_bh1750_light() 
                    : 20;
            }
            bright *= (float)config.display.lightSensor.sensitivity(display_num) / 20.0;
            if(bright < 1.0) bright = 1.0;
            if(bright > 100.0) bright = 100.0;
            return round(bright);
        };

        /* By user defined time */
        case 2: {
            unsigned int morning = config.display.dayTime(display_num, HOUR) * 60 + config.display.dayTime(display_num, MINUTE);
            unsigned int evening = config.display.nightTime(display_num, HOUR) * 60 + config.display.nightTime(display_num, MINUTE);
            unsigned int cur_time = int(hour()) * 60 + minute();
            if(morning < evening) {
                if(morning <= cur_time && cur_time < evening) return config.display.brightness.day(display_num);
            }
            else {
                if(cur_time >= morning || cur_time < evening) return config.display.brightness.day(display_num);
            }
            return config.display.brightness.night(display_num);
        };

        /* Constant brightness */
        case 3: return config.display.brightness.day(display_num);

        default: return 50; break;
    }
}

/**
 * Check if need be and it's time to turn off display
 */
bool isTimeoutOffTime(unsigned int dispNum) {
    if(config.display.autoOff(dispNum) > 0 and ((millis() - state.disp_autoOff[dispNum]) > (config.display.autoOff(dispNum) * 60000))) {
        state.reduc[dispNum] = true;
        if((millis() - state.disp_autoOff[dispNum]) > ((config.display.autoOff(dispNum) * 60000) + 5000)) return true;
    }
    else state.reduc[dispNum] = false;
    return false;
}

/**
 * Check if need be and it's time to turn off display
 */
bool isNightOffTime(unsigned int dispNum) {
    if(config.display.nightOff.need(dispNum)) {
        uint16_t fromM = config.display.nightOff.from(dispNum, false) * 60 + config.display.nightOff.from(dispNum, true);
        uint16_t toM = config.display.nightOff.to(dispNum, false) * 60 + config.display.nightOff.to(dispNum, true);
        uint16_t nowM = hour() * 60 + minute();

        if(fromM < toM) return (fromM <= nowM && nowM < toM);
        else return (nowM >= fromM || nowM < toM);
    }
    return false;
}