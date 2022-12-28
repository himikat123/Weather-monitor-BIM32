unsigned int get_brightness(unsigned int display_num);
bool isOffTime(unsigned int dispNum);

void TaskDisplay(void *pvParameters) {
  (void) pvParameters;

  unsigned int bright_update = 0;
  unsigned int disp_millis[2] = {0, 0};

  #define HOUR 0
  #define MINUTE 1
  #define NEXTION 0
  #define WS2812B 1
  
  
  nextion.init(); // Initialize Nextion display
    
  while(1) {  
    /* Nextion display update once in 5 seconds */
    if(millis() - disp_millis[NEXTION] > 5000) {
      disp_millis[NEXTION] = millis();
      nextion.tick();
    }
    /* WS2812b display update once in 0.5 second */
    if(config.display_type(WS2812B)) ws2812b.tick();

    /* Brightness change once in 1 second */
    if((millis() - bright_update) > 1000) {
      bright_update = millis();

      /* Nextion display brightness change */
      nextion.setIntensity(get_brightness(NEXTION), global.reduc[NEXTION]);

      /* WS2812b display */
      if(config.display_type(WS2812B)) {

        /* Slow down points blinking frequency if the device isn't connected to the network */
        ws2812b.setDotFreq(global.net_connected ? 500 : 1000);

        /* WS2812b display brightness change */
        ws2812b.setIntensity(get_brightness(WS2812B), global.reduc[WS2812B]);
      }

      /* Check if need and it's time to turn off the display Nextion */
      if(isOffTime(NEXTION)) { 
        if(nextion.isDisplayOn()) nextion.displayOff(); 
      }

      /* Check if need and it's time to turn off the display WS2812b */
      if(isOffTime(WS2812B)) { 
        if(ws2812b.isDisplayOn()) ws2812b.displayOff(); 
      }
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
 * Check if need and it's time to turn off display
 */
bool isOffTime(unsigned int dispNum) {
  if(config.display_autoOff(dispNum) > 0 and ((millis() - global.disp_autoOff[dispNum]) > (config.display_autoOff(dispNum) * 60000))) {
    global.reduc[dispNum] = true;
    if((millis() - global.disp_autoOff[dispNum]) > ((config.display_autoOff(dispNum) * 60000) + 5000)) return true;
  }
  else global.reduc[dispNum] = false;
  return false;
}
