#define HOUR      0
#define MINUTE    1
#define DISPLAY_1 0
#define DISPLAY_2 1

unsigned int get_brightness(unsigned int display_num);
bool isOffTime(unsigned int dispNum, bool buttonWasPressed);


void TaskDisplay1(void *pvParameters) {
  (void) pvParameters;

  const uint8_t LCD          = 1;
  const uint8_t NEOPIXEL     = 2;
  const uint8_t D_NX4832K035 = 0;
  const uint8_t D_NX4832T035 = 1;
  const uint8_t D_ILI9341    = 2;

  unsigned int millis_5s = 0;
  unsigned int millis_05s = 0;
  bool buttonWasPressed = false;

  if(config.display_type(DISPLAY_1) == LCD) {
    /* Initialize Nextion display */
    if(config.display_model(DISPLAY_1) == D_NX4832K035 or config.display_model(DISPLAY_1) == D_NX4832T035) nextion.init();
    
    /* Initialize ILI9341 display */
    if(config.display_model(DISPLAY_1) == D_ILI9341) ili9341.init();
  }

  /* Initialize WS2812b display 1 */
  if(config.display_type(DISPLAY_1) == NEOPIXEL) {
    ws2812b_1.init(DISPLAY_1, WS2812_1_DAT_PIN);
  }

  while(1) {
    if(config.display_type(DISPLAY_1)) {
      
      /* Display 1 toogle if display button was pressed */
      if(global.display_but_pressed[DISPLAY_1]) {
        buttonWasPressed = !buttonWasPressed;
        global.display_but_pressed[DISPLAY_1] = false;
        global.disp_autoOff[DISPLAY_1] = millis();
        
        if(config.display_type(DISPLAY_1) == LCD) {
          if(config.display_model(DISPLAY_1) == D_NX4832K035 or config.display_model(DISPLAY_1) == D_NX4832T035) nextion.displayToggle();
          //if(config.display_model(DISPLAY_1) == D_ILI9341) ili9341.displayToggle();/////////////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }
        
        if(config.display_type(DISPLAY_1) == NEOPIXEL) {
          ws2812b_1.displayToggle();
        }
      }

      /* LCD/TFT display refresh once in 5 seconds */
      if(config.display_type(DISPLAY_1) == LCD) {
        if(millis() - millis_5s >= 5000) {
          millis_5s = millis();
           if(config.display_model(DISPLAY_1) == D_NX4832K035 or config.display_model(DISPLAY_1) == D_NX4832T035) nextion.refresh();
           if(config.display_model(DISPLAY_1) == D_ILI9341) ili9341.refresh();
        }
      }

      /* Once in 0.5 second */
      if(millis() - millis_05s >= 500) {
        millis_05s = millis();

        /* ILI9341 clock points refresh */
        if(config.display_type(DISPLAY_1) == D_ILI9341) {
          ili9341.clockPoints();
        }
        
        /* WS2812b brightness change and display update */
        if(config.display_type(DISPLAY_1) == NEOPIXEL) {
          ws2812b_1.brightness(get_brightness(DISPLAY_1), global.reduc[DISPLAY_1]);
          ws2812b_1.refresh();
        }

        /* LCD/TFT display brightness change */
        nextion.brightness(get_brightness(DISPLAY_1), global.reduc[DISPLAY_1]);
        //ili9341.brightness(get_brightness(DISPLAY_1), global.reduc[DISPLAY_1]); ///////////////////////!!!!!!!!!!!!!!!!!!!!!!!

        /* WS2812b display slow down points blinking frequency if the device isn't connected to the network */
        ws2812b_1.setDotFreq(global.net_connected ? 500 : 1000);

        /* Check if need and it's time to turn off the display */
        if(isOffTime(DISPLAY_1, buttonWasPressed)) { 
          if(nextion.isDisplayOn()) nextion.displayOff();
          //if(ili9341.isDisplayOn()) ili9341.displayOff(); //////////////////!!!!!!!!!!!!!!!!!
          if(ws2812b_1.isDisplayOn()) ws2812b_1.displayOff();
        }
        else if(!buttonWasPressed && config.display_nightOff(DISPLAY_1)) {
          nextion.displayOn(false);
          //ili9341.displayOn(false); //////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          ws2812b_1.displayOn();
        }
      }

      /* Receive data from Nextion */
      nextion.dataReceive();

      /* Automatic daily restart of the device at the specified time.
       * Uncomment this line and specify the time if you need autorestart. */
      // if(hour() == 12 and minute() == 0 and second() == 0) ESP.restart();
    }

    vTaskDelay(1);
  }
}

void TaskDisplay2(void *pvParameters) {
  (void) pvParameters;

  const uint8_t NEOPIXEL = 1;
  
  unsigned int bright_update = 0;
  unsigned int disp_millis = 0;
  bool buttonWasPressed = false;

  /* Initialize WS2812b display 2 */
  if(config.display_type(DISPLAY_2) == NEOPIXEL) {
    ws2812b_2.init(DISPLAY_2, WS2812_2_DAT_PIN);
  }

  while(1) {
    if(config.display_type(DISPLAY_2)) {
      
      /* Display 2 toogle if display button was pressed */
      if(global.display_but_pressed[DISPLAY_2]) {
        global.display_but_pressed[DISPLAY_2] = false;
        global.disp_autoOff[DISPLAY_2] = millis();
        if(config.display_type(DISPLAY_2) == NEOPIXEL) {
          ws2812b_2.displayToggle();
        }
        buttonWasPressed = !buttonWasPressed;
      }

      /* WS2812b brightness change and display update (once in 0.5 second) */
      if(config.display_type(DISPLAY_2) == NEOPIXEL) {
        if(millis() - disp_millis > 500) {
          disp_millis = millis();
          ws2812b_2.brightness(get_brightness(DISPLAY_2), global.reduc[DISPLAY_2]);
          ws2812b_2.refresh();
        }
      }

      /* Brightness change once in 1 second */
      if((millis() - bright_update) > 1000) {
        bright_update = millis();

        /* WS2812b display slow down points blinking frequency if the device isn't connected to the network */
        ws2812b_2.setDotFreq(global.net_connected ? 500 : 1000);

        /* Check if need and it's time to turn off the display */
        if(isOffTime(DISPLAY_2, buttonWasPressed)) { 
          if(ws2812b_2.isDisplayOn()) ws2812b_2.displayOff();
        }
        else if(!buttonWasPressed && config.display_nightOff(DISPLAY_2)) {
          ws2812b_2.displayOn();
        }
      }
    }

    vTaskDelay(1);
  }
}

/**
 * Display brightness calculation
 */
unsigned int get_brightness(unsigned int display_num) {
  switch(config.display_brightMethod(display_num)) {

    /* By sunrise and sunset */
    case 0:
      return(weather.get_isDay() ? config.display_brightness_day(display_num) : config.display_brightness_night(display_num));

    /* By ambeint light sensor */
    case 1: {
      float bright = 0.0;
      if(config.display_lightSensor(display_num) == 0) {
        bright = sensors.checkVolt(sensors.get_analog_voltage(config.analog_voltage_corr())) ? sensors.get_analog_voltage(config.analog_voltage_corr()) * 30 : 20;
      }
      if(config.display_lightSensor(display_num) == 1) {
        bright = sensors.checkLight(sensors.get_max44009_light(config.max44009_light_corr())) ? sensors.get_max44009_light(config.max44009_light_corr()) : 20;
      }
      if(config.display_lightSensor(display_num) == 2) {
        bright = sensors.checkLight(sensors.get_bh1750_light(config.bh1750_light_corr())) ? sensors.get_bh1750_light(config.bh1750_light_corr()) : 20;
      }
      bright *= (float)config.display_lightSensor_sensitivity(display_num) / 20.0;
      if(bright < 1.0) bright = 1.0;
      if(bright > 100.0) bright = 100.0;
      return(round(bright));
    };

    /* By user defined time */
    case 2: {
      unsigned int morning = config.display_dayTime(display_num, HOUR) * 60 + config.display_dayTime(display_num, MINUTE);
      unsigned int evening = config.display_nightTime(display_num, HOUR) * 60 + config.display_nightTime(display_num, MINUTE);
      unsigned int cur_time = int(hour()) * 60 + minute();
      unsigned int bright = (cur_time >= morning and cur_time < evening) ? 
        config.display_brightness_day(display_num) : config.display_brightness_night(display_num);
      return(bright);
    };

    /* Constant brightness */
    case 3:
      return(config.display_brightness_day(display_num));

    default:
      return 50; break;
  }
}

/**
 * Check if need be and it's time to turn off display
 */
bool isOffTime(unsigned int dispNum, bool buttonWasPressed) {
  if(config.display_autoOff(dispNum) > 0 and ((millis() - global.disp_autoOff[dispNum]) > (config.display_autoOff(dispNum) * 60000))) {
    global.reduc[dispNum] = true;
    if((millis() - global.disp_autoOff[dispNum]) > ((config.display_autoOff(dispNum) * 60000) + 5000)) return true;
  }
  else global.reduc[dispNum] = false;

  if(config.display_nightOff(dispNum) && !buttonWasPressed) {
    TimeElements timeElm;
    timeElm.Year = year() - 1970; 
    timeElm.Month = month(); 
    timeElm.Day = day(); 
    timeElm.Hour = config.display_nightOff_from(dispNum); 
    timeElm.Minute = 0; 
    timeElm.Second = 0;
    unsigned int timestampFrom = makeTime(timeElm);
    if(hour() < config.display_nightOff_from(dispNum)) timestampFrom -= 86400;

    timeElm.Hour = config.display_nightOff_to(dispNum);
    unsigned int timestampTo = makeTime(timeElm);

    if(timestampFrom > timestampTo) timestampTo += 86400;
    if(timestampFrom <= now() && now() < timestampTo) return true;
  }

  return false;
}
