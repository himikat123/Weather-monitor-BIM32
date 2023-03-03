unsigned int get_brightness(unsigned int display_num);
bool isOffTime(unsigned int dispNum, bool buttonWasPressed);

void TaskDisplay(void *pvParameters) {
  (void) pvParameters;

  unsigned int bright_update = 0;
  unsigned int disp_millis[2] = {0, 0};
  bool buttonWasPressed[2] = {false, false};

  #define HOUR 0
  #define MINUTE 1
  #define NEXTION 0
  #define WS2812B 1
  
  
  nextion.init(); // Initialize Nextion display
    
  while(1) {
    /* Display 1 toogle if display 1 button was pressed */
    if(global.display1_but_pressed) {
      global.display1_but_pressed = false;
      global.disp_autoOff[NEXTION] = millis();
      nextion.displayToggle();
      buttonWasPressed[0] = !buttonWasPressed[0];
    }

    /* Display 2 toogle if display 2 button was pressed */
    if(global.display2_but_pressed) {
      global.display2_but_pressed = false;
      global.disp_autoOff[WS2812B] = millis();
      ws2812b.displayToggle();
      buttonWasPressed[1] = !buttonWasPressed[1];
    }
      
    /* Nextion display update once in 5 seconds */
    if(millis() - disp_millis[NEXTION] > 5000) {
      disp_millis[NEXTION] = millis();
      nextion.tick();
    }
    
    /* WS2812b brightness change and display update (once in 0.5 second) */
    ws2812b.setIntensity(get_brightness(WS2812B), global.reduc[WS2812B]);
    if(config.display_type(WS2812B)) ws2812b.tick();

    /* Brightness change once in 1 second */
    if((millis() - bright_update) > 1000) {
      bright_update = millis();

      /* Nextion display brightness change */
      nextion.setIntensity(get_brightness(NEXTION), global.reduc[NEXTION]);

      /* WS2812b display slow down points blinking frequency if the device isn't connected to the network */
      if(config.display_type(WS2812B)) ws2812b.setDotFreq(global.net_connected ? 500 : 1000);

      /* Check if need and it's time to turn off the display Nextion */
      if(isOffTime(NEXTION, buttonWasPressed[NEXTION])) { 
        if(nextion.isDisplayOn()) nextion.displayOff(); 
      }
      else if(!buttonWasPressed[NEXTION] && config.display_nightOff(NEXTION)) nextion.displayOn(false);

      /* Check if need and it's time to turn off the display WS2812b */
      if(isOffTime(WS2812B, buttonWasPressed[WS2812B])) { 
        if(ws2812b.isDisplayOn()) ws2812b.displayOff(); 
      }
      else if(!buttonWasPressed[WS2812B] && config.display_nightOff(WS2812B)) ws2812b.displayOn();
    }

    /* Receive data from Nextion */
    nextion.dataReceive();

    /* Hourly signal */
    sound.hourlySignal();

    /* Alarm */
    sound.alarm();

    /* Automatic daily restart of the device at the specified time.
     * Uncomment this line and specify the time if you need autorestart. */
    // if(hour() == 12 and minute() == 0 and second() == 0) ESP.restart();
    
    vTaskDelay(500);

    /* Stop all OS tasks while updating the firmware */
    if(global.stopOS) {
      vTaskDelete(task_sensors_handle);
      vTaskDelete(task_display_handle);
    }
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
