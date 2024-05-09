#include <TFT_eSPI.h> // v2.5.34 https://github.com/Bodmer/TFT_eSPI
TFT_eSPI tft = TFT_eSPI();
#include <TJpg_Decoder.h> // v1.0.8 https://github.com/Bodmer/TJpg_Decoder

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  if(y >= tft.height()) return 0;
  tft.pushImage(x, y, w, h, bitmap);
  return 1;
}

class ILI9341 {
  #define LEFT 0
  #define CENTER 1
  #define RIGHT 2

  #define FONT1 0
  #define FONT2 1
  #define FONT3 2
  #define FONT1_FILE "/fonts/Ubuntu-14"
  #define FONT2_FILE "/fonts/Ubuntu-21"
  #define FONT3_FILE "/fonts/Ubuntu-29"

  #define BG_COLOR          0x0000 // black
  #define FRAME_COLOR       0x001F // blue
  #define TEXT_COLOR        0xFFFF // white
  #define TEMPERATURE_COLOR 0xFFE0 // yellow
  #define TEMP_MIN_COLOR    0xFD20 // orange
  #define HUMIDITY_COLOR    0x07FF // cyan
  #define PRESSURE_COLOR    0xF81F // magenta
  #define CLOCK_COLOR       0x07E0 // green
  #define BATTERY_COLOR     0x07E0 // green
  #define BATTERY_LOW_COLOR 0xF800 // red
  #define ANTENNA_COLOR     0x03EF // dark cyan

  public:
    void init(void);
    void refresh(void);
    void clockPoints(void);
    void brightness(unsigned int bright);

  private:
    void _showImg(uint16_t x, uint16_t y, String img);
    void _showTime(uint8_t hr, uint8_t mn);
    void _showWeekday(String wd);
    void _showAntenna(void);
    void _drawSkeleton(void);
    void _showTemperature(int temp, uint16_t x, uint16_t y, uint8_t font, uint16_t color);
    void _showTemperatureInside(int temp);
    void _showTemperatureOutside(int temp);
    void _showHumidity(int hum, uint16_t x, uint16_t y);
    void _showHumidityInside(int hum);
    void _showHumidityOutside(int hum);
    void _showComfort(unsigned int comfort);
    void _printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color);
    void _showBatteryLevel(void);
    void _showVoltageOrPercentage(void);
    void _showWeatherIcon(unsigned int icon, bool isDay);
    void _showDescription(String description);
    void _showPressure(int16_t pres);
    void _showWindSpeed(int8_t windSpeed);
    void _showWindDirection(int windDirection);
    void _showUpdTime(unsigned int dateTime);
    void _showForecast(uint16_t x, uint8_t num, int icon, float tempMax, float tempMin, float wind, String wd);

    int _getTemp(unsigned int sens, unsigned int thing);
    int _getHum(unsigned int sens, unsigned int thing);
    int _getPres(void);

    bool _prevClockPoints = false;
    int _prevTime = -1;
    String _prevWeekday = "";
    unsigned int _prevComfort = 0; 
    int _prevTempIn = -40400;
    int _prevTempOut = -40400;
    int _prevHumIn = -40400;
    int _prevHumOut = -40400;
    int _prevPresOut = -40400;
    int _prevIcon = 404;
    String _prevDescr = "";
    int _prevWindSpeed = -404;
    int _prevWindDirection = -1;
    unsigned int _prevUpdTime = 0;
    String _prevDailyWeekday[3] = {"", "", ""};
    int _prevDailyIcon[3] = {404, 404, 404};
    int _prevTempDailyMax[3] = {-40400, -40400, -40400};
    int _prevTempDailyMin[3] = {-40400, -40400, -40400};
    int _prevWindSpeedDaily[3] = {-1, -1, -1};
    int _prevAnt = 0;
    bool _prevApMode = false;
    bool _prevNetConnected = true;
    int _prevBatLevel = 10;
    float _prevVolt = -40400.00;
    int _prevPercent = -40400;
    float _prevThingBat = 40400.00;
    unsigned int _prevBright = 40400;
};

/**
 * Display and fonts initialisation
 */
void ILI9341::init(void) {
  bool font_missing = false;
  if(LittleFS.exists(String(FONT1_FILE) + ".vlw") == false) font_missing = true;
  if(LittleFS.exists(String(FONT2_FILE) + ".vlw") == false) font_missing = true;
  if(LittleFS.exists(String(FONT3_FILE) + ".vlw") == false) font_missing = true;
  if(font_missing) {
    Serial.println("Font missing in LittleFS, did you upload it?");
    while(1) yield();
  }
  
  tft.begin();
  tft.setRotation(3);

  tft.setSwapBytes(true);
  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(tft_output);
  
  tft.fillScreen(0);
  _drawSkeleton();

  pinMode(TFT_BACKLIGHT, OUTPUT);
  brightness(512);
}

/**
 * Display refresh
 * @param bright - display backlight brightness
 */
void ILI9341::refresh() {
  _showTime(hour(), minute());
  clockPoints();
  int wd = weekday();
  _showWeekday(lang.weekdayShortName(wd));
  _showBatteryLevel();
  _showVoltageOrPercentage();
  _showAntenna();
  
  _showComfort(global.comfort);
  _showTemperatureInside(_getTemp(config.display_source_tempIn_sens(), config.display_source_tempIn_thing()));
  _showHumidityInside(_getHum(config.display_source_humIn_sens(), config.display_source_humIn_thing()));

  _showWeatherIcon(weather.get_currentIcon(), weather.get_isDay());
  _showDescription(weather.get_description());
  _showTemperatureOutside(_getTemp(config.display_source_tempOut_sens(), config.display_source_tempOut_thing()));
  _showHumidityOutside(_getHum(config.display_source_humOut_sens(), config.display_source_humOut_thing()));
  _showPressure(_getPres());
  _showWindSpeed(weather.get_currentWindSpeed());
  _showWindDirection(weather.get_currentWindDir());
  _showUpdTime(weather.get_currentUpdated());
  
  _showForecast(0, 0, weather.get_dailyIcon(0), weather.get_dailyDayTemp(0), weather.get_dailyNightTemp(0), weather.get_dailyWindSpeed(0), lang.weekdayShortName(wd));
  if(++wd > 7) wd = 1;
  _showForecast(106, 1, weather.get_dailyIcon(1), weather.get_dailyDayTemp(1), weather.get_dailyNightTemp(1), weather.get_dailyWindSpeed(1), lang.weekdayShortName(wd));
  if(++wd > 7) wd = 1;
  _showForecast(212, 2, weather.get_dailyIcon(2), weather.get_dailyDayTemp(2), weather.get_dailyNightTemp(2), weather.get_dailyWindSpeed(2), lang.weekdayShortName(wd));
}

/**
 * Change backlight brightness
 */
void ILI9341::brightness(unsigned int bright) {
  if(_prevBright != bright) {
    float r = 100 * log10(2) / log10(255);
    uint16_t br = round(pow(2, (bright / r)));
    if(bright < 1024) analogWrite(TFT_BACKLIGHT, br);
    _prevBright = bright;
  }
}

/**
 * Show a JPG image
 * @param x - x coordinate of the upper left corner of the image
 * @param y - y coordinate of the upper left corner of the image
 * @param img - path in the file system to the image file
 */
void ILI9341::_showImg(uint16_t x, uint16_t y, String img) {
  if(LittleFS.exists(img)) TJpgDec.drawFsJpg(x, y, img, LittleFS);
}

/**
 * 
 */
void ILI9341::_drawSkeleton() {
  tft.drawFastHLine(0, 80, 319, FRAME_COLOR);
  tft.drawFastVLine(143, 2, 75, FRAME_COLOR);
  tft.drawSmoothRoundRect(0, 165, 10, 10, 106, 74, FRAME_COLOR, BG_COLOR);
  tft.drawSmoothRoundRect(106, 165, 10, 10, 106, 74, FRAME_COLOR, BG_COLOR);
  tft.drawSmoothRoundRect(212, 165, 10, 10, 107, 74, FRAME_COLOR, BG_COLOR);
  _showImg(145, 48, "/img/symb/home.jpg");
  _showImg(243, 48, "/img/symb/hum.jpg");
  _showImg(62, 104, "/img/symb/temp+.jpg");
  _showImg(143, 109, "/img/symb/hum.jpg");
  _showImg(222, 109, "/img/symb/pres.jpg");
  _showImg(61, 146, "/img/symb/wind.jpg");
}

void ILI9341::_showTime(uint8_t hr, uint8_t mn) {
  int currTime = hr * 100 + mn;
  if(_prevTime != currTime) {
    char buf[20] = "";
    if(hr < 24) {
      sprintf(buf, "/img/digits/%d.jpg", hr / 10);
      if(hr < 10) tft.fillRect(0, 0, 32, 78, BG_COLOR);
      else _showImg(0, 0, buf);
      sprintf(buf, "/img/digits/%d.jpg", hr % 10);
      _showImg(33, 0, buf);
    }
    if(mn < 60) {
      sprintf(buf, "/img/digits/%d.jpg", mn / 10);
      _showImg(77, 0, buf);
      sprintf(buf, "/img/digits/%d.jpg", mn % 10);
      _showImg(109, 0, buf);
    }
    _prevTime = currTime;
  }
}

void ILI9341::_showWeekday(String wd) {
  if(_prevWeekday != wd) {
    _printText(146, 6, 40, 20, wd, FONT2, LEFT, CLOCK_COLOR);
    _prevWeekday = wd;
  }
}

void ILI9341::clockPoints() {
  if(_prevClockPoints != global.clockPoints) {
    tft.fillCircle(70, 24, 3, global.clockPoints ? CLOCK_COLOR : BG_COLOR);
    tft.fillCircle(70, 52, 3, global.clockPoints ? CLOCK_COLOR : BG_COLOR);
    _prevClockPoints = global.clockPoints;
  }
}

void ILI9341::_showAntenna() {
  if(global.apMode) {
    if(_prevApMode != global.apMode) {
      _showImg(292, 1, "/img/ant/acpoint.jpg");
      _prevApMode = global.apMode;
    }
  }
  else if(!global.net_connected) {
    if(_prevNetConnected != global.net_connected) {
      _showImg(292, 1, "/img/ant/ant_0.jpg");
      _prevNetConnected = global.net_connected;
    }
  }
  else {
    _prevNetConnected = true;
    unsigned int ant = 0;
    int rssi = WiFi.RSSI();
    if(rssi > -51) ant = 4;
    if(rssi < -50 && rssi > -76) ant = 3;
    if(rssi < -75 && rssi > -96) ant = 2;
    if(rssi < -95) ant = 1;
    if(rssi >= 0) ant = 0;
    if(_prevAnt != ant) {
      char buf[20] = "";
      sprintf(buf, "/img/ant/ant_%d.jpg", ant);
      _showImg(292, 1, buf);
      _prevAnt = ant;
    }
  }
}

void ILI9341::_showTemperature(int temp, uint16_t x, uint16_t y, uint8_t font, uint16_t color) {
  String buf = sensors.checkTemp(temp) ? (String(temp) + "°C") : "--";
  _printText(x, y, font == FONT3 ? 70 : 56, font == FONT3 ? 26 : 20, buf, font, font == FONT3 ? CENTER : RIGHT, color);
}

void ILI9341::_showTemperatureInside(int temp) {
  if(_prevTempIn != temp) {
    _showTemperature(temp, 173, 53, FONT3, TEMPERATURE_COLOR);
    _prevTempIn = temp;
  }
}

void ILI9341::_showTemperatureOutside(int temp) {
  if(_prevTempOut != temp) {
    _showTemperature(temp, 71, 113, FONT3, TEMPERATURE_COLOR);
    if(temp < 0) _showImg(62, 104, "/img/symb/temp-.jpg");
    else _showImg(62, 104, "/img/symb/temp+.jpg");
    _prevTempOut = temp;
  }
}

void ILI9341::_showHumidity(int hum, uint16_t x, uint16_t y) {
  String buf = sensors.checkHum(hum) ? (String(hum) + "%") : "--";
  _printText(x, y, 58, 20, buf, FONT2, CENTER, HUMIDITY_COLOR);
}

void ILI9341::_showHumidityInside(int hum) {
  if(_prevHumIn != hum) {
    _showHumidity(hum, 264, 58);
    _prevHumIn = hum;
  }
}

void ILI9341::_showHumidityOutside(int hum) {
  if(_prevHumOut != hum) {
    _showHumidity(hum, 164, 119);
    _prevHumOut = hum;
  }
}

void ILI9341::_showComfort(unsigned int comfort) {
  if(_prevComfort != comfort) {
    if(config.display_source_descr() == 1) {
      _printText(145, 28, 175, 16, lang.comfort(global.comfort), FONT1, CENTER, TEXT_COLOR);
    }
    _prevComfort = comfort;
  }
}

void ILI9341::_printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color) {
  tft.fillRect(x, y, width, height, BG_COLOR);
  if(font == FONT1) tft.loadFont(FONT1_FILE, LittleFS);
  if(font == FONT2) tft.loadFont(FONT2_FILE, LittleFS);
  if(font == FONT3) tft.loadFont(FONT3_FILE, LittleFS);
  tft.setTextColor(color, BG_COLOR);
  uint16_t w = tft.textWidth(text);
  if(align == CENTER) x += (width / 2) - (w / 2);
  if(align == RIGHT) x += width - w - 4;
  tft.setCursor(x, y);
  tft.print(text);
  tft.unloadFont();
}

void ILI9341::_showBatteryLevel() {
  int level = 0;
  if(config.display_source_bat_sens()) {
    if(config.display_source_bat_sens() == 1) {
      if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60) {
        level = thingspeak.get_field(config.display_source_bat_thing());
      }
      else level = 0;
    }
  }
  if(_prevBatLevel != level) {
    if(level >= 1 && level <= 4) {
      char buf[20] = "";
      sprintf(buf, "/img/bat/bat%d.jpg", level);
      _showImg(258, 2, buf);
      _prevBatLevel = level;
    }
    else tft.fillRect(258, 2, 32, 21, BG_COLOR);
  }
}

void ILI9341::_showVoltageOrPercentage() {
  float volt = 40400.0;
  int percent = 40400;
  char buf[8] = "--";

  if(config.display_source_volt_sens() == 1) { // Thingspeak
    if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60) {
      volt = thingspeak.get_field(config.display_source_volt_thing());
      percent = round(volt);
    }

    if(config.display_source_volt_volt() == 0) { // Voltage
      if(_prevVolt != volt) {
        if(sensors.checkBatVolt(volt)) sprintf(buf, "%.2f", volt);
        _printText(198, 10, 58, 16, String(buf) + lang.v(), FONT1, RIGHT, BATTERY_COLOR);
        _prevVolt = volt;
      }
    }

    else if(config.display_source_volt_volt() == 1) { // Percentage
      if(_prevPercent != percent) {
        if(sensors.checkBatPercent(percent)) sprintf(buf, "%d", percent);
        _printText(198, 10, 58, 16, String(buf) + "%", FONT1, RIGHT, BATTERY_COLOR);
        _prevPercent = percent;
      }
    }
  }
}

void ILI9341::_showWeatherIcon(unsigned int icon, bool isDay) {
  if(_prevIcon != (isDay ? (icon * 100) : icon)) {
    char buf[30] = "";
    bool validIcon = false, diffIcon = false;
    if(icon == 3) icon = 2;
    if(icon == 1 || icon == 2 || icon == 11) diffIcon = true;
    if(icon == 4 || icon == 9 || icon == 10 || icon == 13 || icon == 50 || diffIcon) validIcon = true;
    if(validIcon) sprintf(buf, "/img/icons/big/%02d%s.jpg", icon, diffIcon ? isDay ? "_d" : "_n" : "");
    else sprintf(buf, "/img/icons/big/loading.jpg");
    _showImg(0, 104, buf);
    _prevIcon = isDay ? (icon * 100) : icon;
  }
}

void ILI9341::_showDescription(String description) {
  if(_prevDescr != description) {
    tft.loadFont("/fonts/Ubuntu-21", LittleFS);
    uint16_t w = tft.textWidth(description);
    tft.unloadFont();
    _printText(0, 84, 319, 20, description, w > 316 ? FONT1 : FONT2, CENTER, TEXT_COLOR);
    _prevDescr = description;
  }
}

void ILI9341::_showPressure(int16_t pres) {
  if(_prevPresOut != pres) {
    String buf = "--";
    if(sensors.checkPres(pres)) buf = String((int)round(pres * 0.75));
    _printText(250, 119, 70, 20, buf + lang.mm(), FONT2, CENTER, PRESSURE_COLOR);
    _prevPresOut = pres;
  }
}

void ILI9341::_showWindSpeed(int8_t windSpeed) {
  if(_prevWindSpeed != windSpeed) {
    String buf = "--";
    if(weather.checkWind(windSpeed)) buf = String(windSpeed);
    _printText(93, 146, 40, 16, buf + lang.ms(), FONT1, LEFT, TEXT_COLOR);
    _prevWindSpeed = windSpeed;
  }
}

void ILI9341::_showWindDirection(int windDirection) {
  if(_prevWindDirection != windDirection) {
    if((windDirection >= 338 && windDirection < 360) || 
       (windDirection >= 0 && windDirection < 22)
    ) _showImg(133, 143, "/img/wind/n.jpg");
    else if(windDirection >= 22 && windDirection < 67) _showImg(133, 143, "/img/wind/ne.jpg");
    else if(windDirection >= 67 && windDirection < 112) _showImg(133, 143, "/img/wind/e.jpg");
    else if(windDirection >= 112 && windDirection < 157) _showImg(133, 143, "/img/wind/se.jpg");
    else if(windDirection >= 157 && windDirection < 202) _showImg(133, 143, "/img/wind/s.jpg");
    else if(windDirection >= 202 && windDirection < 247) _showImg(133, 143, "/img/wind/sw.jpg");
    else if(windDirection >= 247 && windDirection < 292) _showImg(133, 143, "/img/wind/w.jpg");
    else if(windDirection >= 292 && windDirection < 338) _showImg(133, 143, "/img/wind/nw.jpg");
    else tft.fillRect(133, 143, 16, 16, BG_COLOR);    
    _prevWindDirection = windDirection;
  }
}

void ILI9341::_showUpdTime(unsigned int dateTime) {
  if(_prevUpdTime != dateTime) {
    char buf[20] = "--";
    if(dateTime) sprintf(buf, "%02d.%02d.%d %02d:%02d:%02d", day(dateTime), month(dateTime), year(dateTime), hour(dateTime), minute(dateTime), second(dateTime));
    _printText(186, 148, 133, 16, buf, FONT1, RIGHT, TEXT_COLOR);
    tft.drawCircle(177, 153, 5, TEXT_COLOR);
    tft.drawFastHLine(176, 148, 4, BG_COLOR);
    tft.drawFastHLine(172, 148, 3, TEXT_COLOR);
    tft.drawFastVLine(175, 149, 3, TEXT_COLOR);
    _prevUpdTime = dateTime;
  }
}

void ILI9341::_showForecast(uint16_t x, uint8_t num, int icon, float tempMax, float tempMin, float wind, String wd) {
  if(_prevDailyIcon[num] != icon) {
    char buf[30] = "";
    bool validIcon = false;
    if(icon == 3) icon = 2;
    if(icon == 1 || icon == 2 || icon == 4 || icon == 9 || icon == 10 || icon == 11 || icon == 13 || icon == 50) validIcon = true;
    if(validIcon) sprintf(buf, "/img/icons/small/%02d.jpg", icon);
    else sprintf(buf, "/img/icons/small/loading.jpg");
    _showImg(x + 2, 183, buf);
    _prevDailyIcon[num] = icon;
  }

  if(_prevDailyWeekday[num] != wd) {
    _printText(x + 33, 168, 40, 16, wd, FONT1, CENTER, TEXT_COLOR);
    _prevDailyWeekday[num] = wd;
  }

  int tMax = round(tempMax);
  if(_prevTempDailyMax[num] != tMax) {
    _showTemperature(tMax, x + 49, 183, FONT2, TEMPERATURE_COLOR);
    _prevTempDailyMax[num] = tMax;
  }

  int tMin = round(tempMin);
  if(_prevTempDailyMin[num] != tMin) {
    _showTemperature(tMin, x + 49, 203, FONT2, TEMP_MIN_COLOR);
    _prevTempDailyMin[num] = tMin;
  }

  int wnd = round(wind);
  if(_prevWindSpeedDaily[num] != wnd) {
    String buf = "--";
    if(weather.checkWind(wnd)) buf = String(wnd);
    _printText(x + 31, 224, 44, 15, buf + lang.ms(), FONT1, CENTER, TEXT_COLOR);
    _prevWindSpeedDaily[num] = wnd;
  }
}

/**
 * Get temperature according to settings
 * @param sens temperature source
 * @param thing thinspeak field number
 * @return temperature
 */
int ILI9341::_getTemp(unsigned int sens, unsigned int thing) {
  float temp = 40400.0;
  switch(sens) {
    case 1: temp = weather.get_currentTemp(); break;                            // temperature from weather forecast
    case 2:                                                                     // temperature from thingspeak
      if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60)
        temp = thingspeak.get_field(thing);
      break;
    case 3: temp = sensors.get_bme280_temp(config.bme280_temp_corr()); break;   // temperature from BME280
    case 4: temp = sensors.get_bmp180_temp(config.bmp180_temp_corr()); break;   // temperature from BMP180
    case 5: temp = sensors.get_sht21_temp(config.sht21_temp_corr()); break;     // temperature from SHT21
    case 6: temp = sensors.get_dht22_temp(config.dht22_temp_corr()); break;     // temperature from DHT22
    case 7: temp = sensors.get_ds18b20_temp(config.ds18b20_temp_corr()); break; // temperature from DS18B20
    default: ; break;
  }
  return round(temp);
}

/**
 * Get humidity according to settings
 * @param sens humidity source
 * @param thing thinspeak field number
 * @return humidity
 */
int ILI9341::_getHum(unsigned int sens, unsigned int thing) {
  float hum = 40400.0;
  switch(sens) {
    case 1: hum = weather.get_currentHum(); break;                         // humudity from weather forecast
    case 2:                                                                // humidity from thingspeak
      if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60)
        hum = thingspeak.get_field(thing);
      break;
    case 3: hum = sensors.get_bme280_hum(config.bme280_hum_corr()); break; // humidity from BME280
    case 4: hum = sensors.get_sht21_hum(config.sht21_hum_corr()); break;   // humidity from SHT21
    case 5: hum = sensors.get_dht22_hum(config.dht22_hum_corr()); break;   // humidity from DHT22
    default: ; break;
  }
  return round(hum);
}

/**
 * Get pressure according to settings
 * @return pressure
 */
int ILI9341::_getPres(void) {
  float pres = 40400.0;
  switch(config.display_source_presOut_sens()) {
    case 1: pres = weather.get_currentPres(); break;                          // pressure outside from weather forecast
    case 2:                                                                   // presure outside from thingspeak
      if(now() - thingspeak.get_updated() < config.thingspeakReceive_expire() * 60)
        pres = thingspeak.get_field(config.display_source_presOut_thing());
      break;
    case 3: pres = sensors.get_bme280_pres(config.bme280_pres_corr()); break; // pressure outside from BME280
    case 4: pres = sensors.get_bmp180_pres(config.bmp180_pres_corr()); break; // pressure outside from BMP180
    default: ; break;
  }
  return round(pres);
}
