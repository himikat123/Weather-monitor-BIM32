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

    if(config.display_type(DISPLAY_1) == LCD_DISPLAY) {
        /* Initialize LCD display */
        if(config.display_model(DISPLAY_1) == D_NX4832K035 or config.display_model(DISPLAY_1) == D_NX4832T035) nextion.init();
        if(config.display_model(DISPLAY_1) == D_ILI9341) ili9341.showHomeScreen();
    }

    /* Initialize WS2812b display 1 */
    if(config.display_type(DISPLAY_1) == NEOPIXEL_DISPLAY) {
        ws2812b_1.init(DISPLAY_1, WS2812_1_DAT_PIN);
    }

    /* Initialize 7 segment display */
    if(config.display_type(DISPLAY_1) == SEGMENT_DISPLAY) {
        /* Initialize TM1637 display 1 */
        if(config.display_model(DISPLAY_1) <= 1) tm1637_1.init(DISPLAY_1, TM1637_1_CLK_PIN, TM1637_1_DAT_PIN);
        /* Initialize MAX1637 display 1 */
        if(config.display_model(DISPLAY_1) >= 2) max7219_1.init(DISPLAY_1, MAX7219_1_CLK_PIN, MAX7219_1_DAT_PIN, MAX7219_1_LOAD_PIN);
    }

    /* Initialize numitron display */
    if(config.display_type(DISPLAY_1) == NUMITRON_DISPLAY) {
        pcf8575_1.init(DISPLAY_1, PCF8575_1_SCL_PIN, PCF8575_1_SDA_PIN, NUMITRON_1_PWM_PIN, WS2812_1_DAT_PIN);
    }

    while(1) {
        if(config.display_type(DISPLAY_1)) {
            /* Display 1 toogle if display button was pressed */
            if(global.display_but_pressed[DISPLAY_1]) {
                global.display_but_pressed[DISPLAY_1] = false;
                global.disp_autoOff[DISPLAY_1] = millis();

                if(config.display_type(DISPLAY_1) == LCD_DISPLAY) {
                    if(config.display_model(DISPLAY_1) == D_NX4832K035 or config.display_model(DISPLAY_1) == D_NX4832T035) nextion.displayToggle();
                    if(config.display_model(DISPLAY_1) == D_ILI9341) ili9341.displayToggle();
                }

                if(config.display_type(DISPLAY_1) == NEOPIXEL_DISPLAY) {
                    ws2812b_1.displayToggle();
                }

                if(config.display_type(DISPLAY_1) == SEGMENT_DISPLAY) {
                    if(config.display_model(DISPLAY_1) <= 1) tm1637_1.displayToggle();
                    if(config.display_model(DISPLAY_1) >= 2) max7219_1.displayToggle();
                }

                if(config.display_type(DISPLAY_1) == NUMITRON_DISPLAY) {
                    pcf8575_1.displayToggle();
                }

                vTaskDelay(500);
            }

            /* Once in 0.5 second */
            if(millis() - millis_05s >= 500) {
                millis_05s = millis();

                if(config.display_type(DISPLAY_1) == LCD_DISPLAY) {
                    /* LCD Display update */
                    if(config.display_model(DISPLAY_1) == D_NX4832K035 or config.display_model(DISPLAY_1) == D_NX4832T035) nextion.refresh();
                    if(config.display_model(DISPLAY_1) == D_ILI9341) ili9341.refresh();
                }

                /* 7 segment display slow down points blinking frequency if the device isn't connected to the network */
                uint16_t dotFreq = global.net_connected ? 500 : 1000;
                ws2812b_1.setDotFreq(dotFreq);
                tm1637_1.setDotFreq(dotFreq);
                max7219_1.setDotFreq(dotFreq);
                pcf8575_1.setDotFreq(dotFreq);

                /* WS2812b brightness change */
                if(config.display_type(DISPLAY_1) == NEOPIXEL_DISPLAY) {
                    ws2812b_1.brightness(get_brightness(DISPLAY_1), global.reduc[DISPLAY_1]);
                }

                if(config.display_type(DISPLAY_1) == SEGMENT_DISPLAY) {
                    /* tm1637 brightness change */
                    if(config.display_model(DISPLAY_1) <= 1) tm1637_1.brightness(get_brightness(DISPLAY_1), global.reduc[DISPLAY_1]);
                    /* max7219 brightness change */
                    if(config.display_model(DISPLAY_1) >= 2) max7219_1.brightness(get_brightness(DISPLAY_1), global.reduc[DISPLAY_1]);
                }

                if(config.display_type(DISPLAY_1) == NUMITRON_DISPLAY) {
                    pcf8575_1.brightness(get_brightness(DISPLAY_1), global.reduc[DISPLAY_1]);
                }

                /* LCD/TFT display brightness change */
                nextion.brightness(get_brightness(DISPLAY_1));
                ili9341.brightness(get_brightness(DISPLAY_1));

                /* Check if need and it's time to turn off the display */
                if(isTimeoutOffTime(DISPLAY_1)) {
                    if(nextion.isDisplayOn()) nextion.displayOff();
                    if(ili9341.isDisplayOn()) ili9341.displayOff();
                    if(ws2812b_1.isDisplayOn()) ws2812b_1.displayOff();
                    if(tm1637_1.isDisplayOn()) tm1637_1.displayOff();
                    if(max7219_1.isDisplayOn()) max7219_1.displayOff();
                    if(pcf8575_1.isDisplayOn()) pcf8575_1.displayOff();
                }

                uint8_t itsNightOffTime = isNightOffTime(DISPLAY_1) ? 1 : 0;
                if(global.disp_night_state[DISPLAY_1] != itsNightOffTime) {
                    global.disp_night_state[DISPLAY_1] = itsNightOffTime;
                    if(itsNightOffTime) {
                        if(nextion.isDisplayOn()) nextion.displayOff();
                        if(ili9341.isDisplayOn()) ili9341.displayOff();
                        if(ws2812b_1.isDisplayOn()) ws2812b_1.displayOff();
                        if(tm1637_1.isDisplayOn()) tm1637_1.displayOff();
                        if(max7219_1.isDisplayOn()) max7219_1.displayOff();
                        if(pcf8575_1.isDisplayOn()) pcf8575_1.displayOff();
                    }
                    else {
                        if(!nextion.isDisplayOn()) nextion.displayOn(false);
                        if(!ili9341.isDisplayOn()) ili9341.displayOn();
                        if(!ws2812b_1.isDisplayOn()) ws2812b_1.displayOn();
                        if(!tm1637_1.isDisplayOn()) tm1637_1.displayOn();
                        if(!max7219_1.isDisplayOn()) max7219_1.displayOn();
                        if(!pcf8575_1.isDisplayOn()) pcf8575_1.displayOn();
                    }
                }
            }

            /* WS2812b display 1 update */
            if(config.display_type(DISPLAY_1) == NEOPIXEL_DISPLAY) {
                ws2812b_1.refresh();
            }

            if(config.display_type(DISPLAY_1) == SEGMENT_DISPLAY) {
                /* TM1637 display 1 update */
                if(config.display_model(DISPLAY_1) <= 1) tm1637_1.refresh();
                /* MAX7219 display 1 update */
                if(config.display_model(DISPLAY_1) >= 2) max7219_1.refresh();
            }

            /* Numitron display 1 update */
            if(config.display_type(DISPLAY_1) == NUMITRON_DISPLAY) {
                pcf8575_1.refresh();
            }

            if(config.display_type(DISPLAY_1) == LCD_DISPLAY) {
                /* Receive data from Nextion */
                if(config.display_model(DISPLAY_1) == D_NX4832K035 or config.display_model(DISPLAY_1) == D_NX4832T035) {
                    nextion.dataReceive();
                }

                if(config.display_model(DISPLAY_1) == D_ILI9341) {
                    ili9341.getTouch();
                }
            }
        }

        vTaskDelay(10);
    }
}

void TaskDisplay2(void *pvParameters) {
    (void) pvParameters;
    unsigned int bright_update = 0;

    /* Initialize WS2812b display 2 */
    if(config.display_type(DISPLAY_2) == NEOPIXEL_DISPLAY) {
        ws2812b_2.init(DISPLAY_2, WS2812_2_DAT_PIN);
    }

    if(config.display_type(DISPLAY_2) == SEGMENT_DISPLAY) {
        /* Initialize TM1637 display 2 */
        if(config.display_model(DISPLAY_2) <= 1) tm1637_2.init(DISPLAY_2, TM1637_2_CLK_PIN, TM1637_2_DAT_PIN);
        /* Initialize MAX7219 display 2 */
        if(config.display_model(DISPLAY_2) >= 2) max7219_2.init(DISPLAY_2, MAX7219_2_CLK_PIN, MAX7219_2_DAT_PIN, MAX7219_2_LOAD_PIN);
    }

    /* Initialize Numitron display 2 */
    if(config.display_type(DISPLAY_2) == NUMITRON_DISPLAY) {
        pcf8575_2.init(DISPLAY_2, PCF8575_2_SCL_PIN, PCF8575_2_SDA_PIN, NUMITRON_2_PWM_PIN, WS2812_2_DAT_PIN);
    }

    while(1) {
        if(config.display_type(DISPLAY_2)) {

            /* Display 2 toogle if display button was pressed */
            if(global.display_but_pressed[DISPLAY_2]) {
                global.display_but_pressed[DISPLAY_2] = false;
                global.disp_autoOff[DISPLAY_2] = millis();
                if(config.display_type(DISPLAY_2) == NEOPIXEL_DISPLAY) {
                    ws2812b_2.displayToggle();
                }
                if(config.display_type(DISPLAY_2) == SEGMENT_DISPLAY) {
                    if(config.display_model(DISPLAY_2) <= 1) tm1637_2.displayToggle();
                    if(config.display_model(DISPLAY_2) >= 2) max7219_2.displayToggle();
                }
                if(config.display_type(DISPLAY_2) == NUMITRON_DISPLAY) {
                    pcf8575_2.displayToggle();
                }
            }

            if(config.display_type(DISPLAY_2) == NEOPIXEL_DISPLAY) {
                ws2812b_2.refresh();
            }

            if(config.display_type(DISPLAY_2) == SEGMENT_DISPLAY) {
                if(config.display_model(DISPLAY_2) <= 1) tm1637_2.refresh();
                if(config.display_model(DISPLAY_2) >= 2) max7219_2.refresh();
            }

            if(config.display_type(DISPLAY_2) == NUMITRON_DISPLAY) {
                pcf8575_2.refresh();
            }

            /* Brightness change once in 1 second */
            if((millis() - bright_update) > 1000) {
                bright_update = millis();
                ws2812b_2.brightness(get_brightness(DISPLAY_2), global.reduc[DISPLAY_2]);
                tm1637_2.brightness(get_brightness(DISPLAY_2), global.reduc[DISPLAY_2]);
                max7219_2.brightness(get_brightness(DISPLAY_2), global.reduc[DISPLAY_2]);
                pcf8575_2.brightness(get_brightness(DISPLAY_2), global.reduc[DISPLAY_2]);

                /* 7 segment display slow down points blinking frequency if the device isn't connected to the network */
                uint16_t dotFreq = global.net_connected ? 500 : 1000;
                ws2812b_2.setDotFreq(dotFreq);
                tm1637_2.setDotFreq(dotFreq);
                max7219_2.setDotFreq(dotFreq);
                pcf8575_2.setDotFreq(dotFreq);

                /* Check if need and it's time to turn off the display */
                if(isTimeoutOffTime(DISPLAY_2)) { 
                    if(ws2812b_2.isDisplayOn()) ws2812b_2.displayOff();
                    if(tm1637_2.isDisplayOn()) tm1637_2.displayOff();
                    if(max7219_2.isDisplayOn()) max7219_2.displayOff();
                    if(pcf8575_2.isDisplayOn()) pcf8575_2.displayOff();
                }

                uint8_t itsNightOffTime = isNightOffTime(DISPLAY_2) ? 1 : 0;
                if(global.disp_night_state[DISPLAY_2] != itsNightOffTime) {
                    global.disp_night_state[DISPLAY_2] = itsNightOffTime;
                    if(itsNightOffTime) {
                        if(ws2812b_2.isDisplayOn()) ws2812b_2.displayOff();
                        if(tm1637_2.isDisplayOn()) tm1637_2.displayOff();
                        if(max7219_2.isDisplayOn()) max7219_2.displayOff();
                        if(pcf8575_2.isDisplayOn()) pcf8575_2.displayOff();
                    }
                    else {
                        if(!ws2812b_2.isDisplayOn()) ws2812b_2.displayOn();
                        if(!tm1637_2.isDisplayOn()) tm1637_2.displayOn();
                        if(!max7219_2.isDisplayOn()) max7219_2.displayOn();
                        if(!pcf8575_2.isDisplayOn()) pcf8575_2.displayOn();
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
    switch(config.display_brightMethod(display_num)) {

        /* By sunrise and sunset */
        case 0:
            return weather.get_isDay() 
                ? config.display_brightness_day(display_num) 
                : config.display_brightness_night(display_num);

        /* By ambeint light sensor */
        case 1: {
            float bright = 0.0;
            if(config.display_lightSensor(display_num) == 0) {
                bright = validate.volt(sensors.get_analog_voltage(CORRECTED)) 
                    ? sensors.get_analog_voltage(CORRECTED) * 30 
                    : 20;
            }
            if(config.display_lightSensor(display_num) == 1) {
                bright = validate.light(sensors.get_max44009_light(CORRECTED)) 
                    ? sensors.get_max44009_light(CORRECTED) 
                    : 20;
            }
            if(config.display_lightSensor(display_num) == 2) {
                bright = validate.light(sensors.get_bh1750_light(CORRECTED)) 
                    ? sensors.get_bh1750_light(CORRECTED) 
                    : 20;
            }
            bright *= (float)config.display_lightSensor_sensitivity(display_num) / 20.0;
            if(bright < 1.0) bright = 1.0;
            if(bright > 100.0) bright = 100.0;
            return round(bright);
        };

        /* By user defined time */
        case 2: {
            unsigned int morning = config.display_dayTime(display_num, HOUR) * 60 + config.display_dayTime(display_num, MINUTE);
            unsigned int evening = config.display_nightTime(display_num, HOUR) * 60 + config.display_nightTime(display_num, MINUTE);
            unsigned int cur_time = int(hour()) * 60 + minute();
            if(morning < evening) {
                if(morning <= cur_time && cur_time < evening) return config.display_brightness_day(display_num);
            }
            else {
                if(cur_time >= morning || cur_time < evening) return config.display_brightness_day(display_num);
            }
            return config.display_brightness_night(display_num);
        };

        /* Constant brightness */
        case 3: return config.display_brightness_day(display_num);

        default: return 50; break;
    }
}

/**
 * Check if need be and it's time to turn off display
 */
bool isTimeoutOffTime(unsigned int dispNum) {
    if(config.display_autoOff(dispNum) > 0 and ((millis() - global.disp_autoOff[dispNum]) > (config.display_autoOff(dispNum) * 60000))) {
        global.reduc[dispNum] = true;
        if((millis() - global.disp_autoOff[dispNum]) > ((config.display_autoOff(dispNum) * 60000) + 5000)) return true;
    }
    else global.reduc[dispNum] = false;
    return false;
}

/**
 * Check if need be and it's time to turn off display
 */
bool isNightOffTime(unsigned int dispNum) {
    if(config.display_nightOff(dispNum)) {
        uint8_t fromH = config.display_nightOff_from(dispNum);
        uint8_t toH = config.display_nightOff_to(dispNum);
        if(fromH < toH) {
            if(fromH <= hour() && hour() < toH) return true;
        }
        else {
            if(hour() >= fromH || hour() < toH) return true;
        }
    }

    return false;
}