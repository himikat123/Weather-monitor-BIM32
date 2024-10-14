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

    if(config.display_type(DISPLAY_1) == LCD) {
        /* Initialize display */
        if(config.display_model(DISPLAY_1) == D_NX4832K035 or config.display_model(DISPLAY_1) == D_NX4832T035) nextion.init();
        if(config.display_model(DISPLAY_1) == D_ILI9341) ili9341.showHomeScreen();
    }

    /* Initialize WS2812b display 1 */
    if(config.display_type(DISPLAY_1) == NEOPIXEL_DISPLAY1) {
        ws2812b_1.init(DISPLAY_1, WS2812_1_DAT_PIN);
    }

    while(1) {
        if(config.display_type(DISPLAY_1)) {
            /* Display 1 toogle if display button was pressed */
            if(global.display_but_pressed[DISPLAY_1]) {
                global.display_but_pressed[DISPLAY_1] = false;
                global.disp_autoOff[DISPLAY_1] = millis();

                if(config.display_type(DISPLAY_1) == LCD) {
                    if(config.display_model(DISPLAY_1) == D_NX4832K035 or config.display_model(DISPLAY_1) == D_NX4832T035) nextion.displayToggle();
                    if(config.display_model(DISPLAY_1) == D_ILI9341) ili9341.displayToggle();
                }

                if(config.display_type(DISPLAY_1) == NEOPIXEL_DISPLAY1) {
                    ws2812b_1.displayToggle();
                }

                vTaskDelay(500);
            }

            /* Once in 0.5 second */
            if(millis() - millis_05s >= 500) {
                millis_05s = millis();

                if(config.display_type(DISPLAY_1) == LCD) {
                    /* LCD Display update */
                    if(config.display_model(DISPLAY_1) == D_NX4832K035 or config.display_model(DISPLAY_1) == D_NX4832T035) nextion.refresh();
                    if(config.display_model(DISPLAY_1) == D_ILI9341) ili9341.refresh();
                }

                /* WS2812b display slow down points blinking frequency if the device isn't connected to the network */
                ws2812b_1.setDotFreq(global.net_connected ? 500 : 1000);

                /* WS2812b brightness change and display update */
                if(config.display_type(DISPLAY_1) == NEOPIXEL_DISPLAY1) {
                    ws2812b_1.brightness(get_brightness(DISPLAY_1), global.reduc[DISPLAY_1]);
                }

                /* LCD/TFT display brightness change */
                nextion.brightness(get_brightness(DISPLAY_1));
                ili9341.brightness(get_brightness(DISPLAY_1));

                /* Check if need and it's time to turn off the display */
                if(isTimeoutOffTime(DISPLAY_1)) {
                    if(nextion.isDisplayOn()) nextion.displayOff();
                    if(ili9341.isDisplayOn()) ili9341.displayOff();
                    if(ws2812b_1.isDisplayOn()) ws2812b_1.displayOff();
                }

                uint8_t itsNightOffTime = isNightOffTime(DISPLAY_1) ? 1 : 0;
                if(global.disp_night_state[DISPLAY_1] != itsNightOffTime) {
                    global.disp_night_state[DISPLAY_1] = itsNightOffTime;
                    if(itsNightOffTime) {
                        if(nextion.isDisplayOn()) nextion.displayOff();
                        if(ili9341.isDisplayOn()) ili9341.displayOff();
                        if(ws2812b_1.isDisplayOn()) ws2812b_1.displayOff();
                    }
                    else {
                        if(!nextion.isDisplayOn()) nextion.displayOn(false);
                        if(!ili9341.isDisplayOn()) ili9341.displayOn();
                        if(!ws2812b_1.isDisplayOn()) ws2812b_1.displayOn();
                    }
                }
            }

            /* WS2812b display 1 update */
            if(config.display_type(DISPLAY_1) == NEOPIXEL_DISPLAY1) {
                ws2812b_1.refresh();
            }

            if(config.display_type(DISPLAY_1) == LCD) {
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
    unsigned int disp_millis = 0;

    /* Initialize WS2812b display 2 */
    if(config.display_type(DISPLAY_2) == NEOPIXEL_DISPLAY2) {
        ws2812b_2.init(DISPLAY_2, WS2812_2_DAT_PIN);
    }

    while(1) {
        if(config.display_type(DISPLAY_2)) {

            /* Display 2 toogle if display button was pressed */
            if(global.display_but_pressed[DISPLAY_2]) {
                global.display_but_pressed[DISPLAY_2] = false;
                global.disp_autoOff[DISPLAY_2] = millis();
                if(config.display_type(DISPLAY_2) == NEOPIXEL_DISPLAY2) {
                    ws2812b_2.displayToggle();
                }
            }

            /* WS2812b display 2 update */
            if(config.display_type(DISPLAY_2) == NEOPIXEL_DISPLAY2) {
                ws2812b_2.refresh();
            }

            /* Brightness change once in 1 second */
            if((millis() - bright_update) > 1000) {
                bright_update = millis();
                ws2812b_2.brightness(get_brightness(DISPLAY_2), global.reduc[DISPLAY_2]);

                /* WS2812b display slow down points blinking frequency if the device isn't connected to the network */
                ws2812b_2.setDotFreq(global.net_connected ? 500 : 1000);

                /* Check if need and it's time to turn off the display */
                if(isTimeoutOffTime(DISPLAY_2)) { 
                    if(ws2812b_2.isDisplayOn()) ws2812b_2.displayOff();
                }

                uint8_t itsNightOffTime = isNightOffTime(DISPLAY_2) ? 1 : 0;
                if(global.disp_night_state[DISPLAY_2] != itsNightOffTime) {
                    global.disp_night_state[DISPLAY_2] = itsNightOffTime;
                    if(itsNightOffTime) {
                        if(ws2812b_2.isDisplayOn()) ws2812b_2.displayOff();
                    }
                    else {
                        if(!ws2812b_2.isDisplayOn()) ws2812b_2.displayOn();
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