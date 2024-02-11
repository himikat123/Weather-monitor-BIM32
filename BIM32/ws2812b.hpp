#include <NeoPixelBus.h> // v2.7.8 https://github.com/Makuna/NeoPixelBus

#define PIXELS 87

// WS2812b display up to 87 LEDs, pin WS2812_DAT_PIN
NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0800KbpsMethod> strip(PIXELS, WS2812_DAT_PIN);

class WS2812b {
  public:
    void init();
    void setDotFreq(unsigned int freq);
    void displayToggle();
    void displayOn();
    void displayOff();
    bool isDisplayOn();
    void tick();
    void setIntensity(unsigned int intensity, bool reduc);
    void connecting(void);

  private:
    int _disp_seg_img[2][5] = {{SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE}, {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE}}; // display digits
    bool _clock_pixels[PIXELS]; // display pixels
    bool _clock_pixels_prev[PIXELS]; // display pixels previous slot
    unsigned int _mills = 0; // delay variable
    unsigned int _timeSlot = 0; // display slot number
    unsigned int _timeSlotPrev = 0; // previous slot number 
    bool _dots = false; // time points
    bool _clockdots = false; // need for time points
    unsigned int _dotmills = 0; // delay variable for time points
    unsigned int _dotfreq = 500; // time points period
    bool _power = true; // display on/off flag
    unsigned int _brightness = 40; // display brightness
    unsigned int _red_prev = 0, _green_prev = 0, _blue_prev = 0;  // previous slot color
    unsigned int _connect = 0; // wifi connect animation

    void _clock();
    void _date();
    void _temp(float t);
    void _hum(float h);
    void _pres(float p);
    void _iaq(float i);
    void _co2(float co2);
    void _wind(float sp, int wd);
    void _des(String str);
    void _print();
    void _sendToDisplay(unsigned int red[PIXELS], unsigned int green[PIXELS], unsigned int blue[PIXELS], bool clock_pixels[PIXELS]);
    void _to7segmentCode(unsigned int ledsSeg, unsigned int ledsDig, int dig0, int dig1, int dig2, int dig3);
};

/**
 * Display initialize
 */
void WS2812b::init() {
  strip.Begin();
  strip.Show();
  for(unsigned int i=0; i<PIXELS; i++) {
    _clock_pixels[i] = 0; 
  }
}

/**
 * Sets time points frequency (period)
 * @param frequency (period)
 */
void WS2812b::setDotFreq(unsigned int freq) {
  _dotfreq = freq;
}

/**
 * Toggles display (on/off)
 */
void WS2812b::displayToggle() {
  _power = !_power;
}

/**
 * Turns on the display
 */
void WS2812b::displayOn() {
  _power = true;
}

/*
 * Turns off the display
 */
void WS2812b::displayOff() {
  _power = false;
}

/**
 * Returns true if display is on
 */
bool WS2812b::isDisplayOn() {
  return _power;
}

/**
 * Sets display brightness
 * @param brightness intensity
 */
void WS2812b::setIntensity(unsigned int intensity, bool reduc) {
  _brightness = reduc ? round(intensity / 2) : intensity;
}

/**
 * Displays the next display slot
 */
void WS2812b::tick() {
  _connect = 10; // Disable animation of connecting to the network
  
  /* Slot switch */
  if(millis() - _mills >= config.display_timeSlot_period(_timeSlot, 1) * 1000 or config.display_timeSlot_period(_timeSlot, 1) == 0) {
    _timeSlot++;
    for(unsigned int i=_timeSlot; i<8; i++) {
      if(config.display_timeSlot_period(_timeSlot, 1) == 0) _timeSlot++;
      else break;
    }
    if(_timeSlot > 7) _timeSlot = 0;
    _mills = millis();
  }

  /* Displaying the next display slot */
  if(millis() - _dotmills >= _dotfreq) {
    _dotmills = millis();
    
    if(config.display_timeSlot_period(_timeSlot, 1) > 0) {
      _clockdots = false;

      switch(config.display_timeSlot_sensor(_timeSlot, 1)) {
        case 0: // Clock 
          _clockdots = true;
          _clock();
          break;
        case 1: // Date
          _date();
          break;
        case 2: // BME280
          if(config.display_timeSlot_data(_timeSlot, 1) == 0) _temp(sensors.get_bme280_temp(config.bme280_temp_corr()));
          if(config.display_timeSlot_data(_timeSlot, 1) == 1) _hum(sensors.get_bme280_hum(config.bme280_hum_corr()));
          if(config.display_timeSlot_data(_timeSlot, 1) == 2) _pres(sensors.get_bme280_pres(config.bme280_pres_corr()));
          break;
        case 3: // BMP180
          if(config.display_timeSlot_data(_timeSlot, 1) == 0) _temp(sensors.get_bmp180_temp(config.bmp180_temp_corr()));
          if(config.display_timeSlot_data(_timeSlot, 1) == 1) _pres(sensors.get_bmp180_pres(config.bmp180_pres_corr()));
          break;
        case 4: // SHT21
          if(config.display_timeSlot_data(_timeSlot, 1) == 0) _temp(sensors.get_sht21_temp(config.sht21_temp_corr()));
          if(config.display_timeSlot_data(_timeSlot, 1) == 1) _hum(sensors.get_sht21_hum(config.sht21_hum_corr()));
          break;
        case 5: // DHT22
          if(config.display_timeSlot_data(_timeSlot, 1) == 0) _temp(sensors.get_dht22_temp(config.dht22_temp_corr()));
          if(config.display_timeSlot_data(_timeSlot, 1) == 1) _hum(sensors.get_dht22_hum(config.dht22_hum_corr()));
          break;
        case 6: // DS18B20
          _temp(sensors.get_ds18b20_temp(config.ds18b20_temp_corr()));
          break;
        case 7: // ESP32
          _temp(sensors.get_esp32_temp(config.esp32_temp_corr()));
          break;
        case 8: // Thingspeak
          if((now() - thingspeak.get_updated()) < (config.thingspeakReceive_expire() * 60)) {
            if(config.display_timeSlot_data(_timeSlot, 1) == 0) _temp(thingspeak.get_field(config.display_timeSlot_thing(_timeSlot, 1)));
            if(config.display_timeSlot_data(_timeSlot, 1) == 1) _hum(thingspeak.get_field(config.display_timeSlot_thing(_timeSlot, 1)));
            if(config.display_timeSlot_data(_timeSlot, 1) == 2) _pres(thingspeak.get_field(config.display_timeSlot_thing(_timeSlot, 1)));
          }
          else {
            if(config.display_timeSlot_data(_timeSlot, 1) == 0) _temp(40400.0);
            if(config.display_timeSlot_data(_timeSlot, 1) == 1) _hum(40400.0);
            if(config.display_timeSlot_data(_timeSlot, 1) == 2) _pres(40400.0);
          }
          break;
        case 9: // Weather
          if(now() - weather.get_currentUpdated() < 1200) {
            if(config.display_timeSlot_data(_timeSlot, 1) == 0) _temp(weather.get_currentTemp());
            if(config.display_timeSlot_data(_timeSlot, 1) == 1) _hum(weather.get_currentHum());
            if(config.display_timeSlot_data(_timeSlot, 1) == 2) _pres(weather.get_currentPres());
          }
          else {
            if(config.display_timeSlot_data(_timeSlot, 1) == 0) _temp(40400.0);
            if(config.display_timeSlot_data(_timeSlot, 1) == 1) _hum(40400.0);
            if(config.display_timeSlot_data(_timeSlot, 1) == 2) _pres(40400.0);
          }
          break;
        case 10: { // Wireless sensor
          unsigned int wsensNum = config.display_timeSlot_wsensor_num(_timeSlot, 1);
          unsigned int wsensType = config.display_timeSlot_wsensor_type(_timeSlot, 1);
          if((now() - wsensor.get_updated(wsensNum)) < (config.wsensor_expire(wsensNum) * 60)) {
            if(wsensType >= 0 and wsensType <= 4) _temp(wsensor.get_temperature(wsensNum, wsensType, config.wsensor_temp_corr(wsensNum, wsensType)));
            if(wsensType == 5) _hum(wsensor.get_humidity(wsensNum, config.wsensor_hum_corr(wsensNum)));
            if(wsensType == 6) _pres(wsensor.get_pressure(wsensNum, config.wsensor_pres_corr(wsensNum)));
            if(wsensType == 7) _co2(wsensor.get_co2(wsensNum, config.wsensor_co2_corr(wsensNum)));
          }
          else {
            if(wsensType >= 0 and wsensType <= 4) _temp(40400.0);
            if(wsensType == 5) _hum(40400.0);
            if(wsensType == 6) _pres(40400.0);
            if(wsensType == 7) _co2(40400.0);
          }
        }; break;
        case 11: // BME680
          if(config.display_timeSlot_data(_timeSlot, 1) == 0) _temp(sensors.get_bme680_temp(config.bme680_temp_corr()));
          if(config.display_timeSlot_data(_timeSlot, 1) == 1) _hum(sensors.get_bme680_hum(config.bme680_hum_corr()));
          if(config.display_timeSlot_data(_timeSlot, 1) == 2) _pres(sensors.get_bme680_pres(config.bme680_pres_corr()));
          if(config.display_timeSlot_data(_timeSlot, 1) == 3) _iaq(sensors.get_bme680_iaq(config.bme680_iaq_corr()));
          break;
        default: ; break;
      }
    
      _print();
    }
  }
}

/**
 * Preparing data for displaying the clock
 */
void WS2812b::_clock() {
  unsigned int hr = config.clock_format() ? hour() : hourFormat12();
  _disp_seg_img[0][0] = hr < 10 ? SYMB_SPACE : hr / 10;
  _disp_seg_img[0][1] = hr % 10;
  _disp_seg_img[0][2] = minute() / 10;
  _disp_seg_img[0][3] = minute() % 10;
  _dots = !_dots;
}

/**
 * Preparing data for displaying the date
 */
void WS2812b::_date() {
  _disp_seg_img[0][0] = day() / 10;
  _disp_seg_img[0][1] = day() % 10;
  _disp_seg_img[0][2] = month() / 10;
  _disp_seg_img[0][3] = month() % 10;
}

/**
 * Preparing data for displaying the temperature
 */
void WS2812b::_temp(float t) {
  int tmp = round(t);
  if(!sensors.checkTemp(t)) { // if the data is wrong
    _disp_seg_img[0][0] = SYMB_MINUS;
    _disp_seg_img[0][1] = SYMB_MINUS;
    _disp_seg_img[0][2] = SYMB_DEGREE;
    _disp_seg_img[0][3] = SYMB_C;
  }
  else { // if the data is correct
    unsigned int th = floor(abs(tmp) / 10);
    unsigned int tl = abs(tmp) % 10;
    if(th == 0) th = SYMB_SPACE;
    _disp_seg_img[0][0] = tmp < 0 ? SYMB_MINUS : tmp > 9 ? th : SYMB_SPACE;
    _disp_seg_img[0][1] = tmp < 0 ? tmp < -9 ? th : tl : tl;
    _disp_seg_img[0][2] = tmp < 0 ? tmp < -9 ? tl : SYMB_DEGREE : SYMB_DEGREE;
    _disp_seg_img[0][3] = tmp < 0 ? tmp < -9 ? SYMB_DEGREE : SYMB_C : SYMB_C;
  }
}

/**
 * Preparing data for displaying the humidity
 */
void WS2812b::_hum(float h) {
  int hm = round(h);
  if(!sensors.checkHum(h)) { // if the data is wrong
    _disp_seg_img[0][0] = SYMB_MINUS;
    _disp_seg_img[0][1] = SYMB_MINUS;
    _disp_seg_img[0][2] = SYMB_SPACE;
    _disp_seg_img[0][3] = SYMB_H;
  }
  else { // if the data is correct
    unsigned int hh = floor(hm / 10);
    unsigned int hl = hm % 10;
    if(hh == 0) hh = SYMB_SPACE;
    _disp_seg_img[0][0] = hm > 99 ? 1 : hh;
    _disp_seg_img[0][1] = hm > 99 ? 0 : hl;
    _disp_seg_img[0][2] = hm > 99 ? 0 : SYMB_SPACE;
    _disp_seg_img[0][3] = SYMB_H;
  }
}

/**
 * Preparing data for displaying the pressure
 */
void WS2812b::_pres(float p) {
  int prs = round(p * 0.75);
  if(!sensors.checkPres(p)) { // if the data is wrong
    _disp_seg_img[0][0] = SYMB_MINUS;
    _disp_seg_img[0][1] = SYMB_MINUS;
    _disp_seg_img[0][2] = SYMB_MINUS;
    _disp_seg_img[0][3] = SYMB_P;
  }
  else { // if the data is correct
    _disp_seg_img[0][0] = floor(prs / 100);
    _disp_seg_img[0][1] = floor(prs % 100 / 10);
    _disp_seg_img[0][2] = prs % 10;
    _disp_seg_img[0][3] = SYMB_P;
  }
}

/**
 * Preparing data for displaying the IAQ
 */
void WS2812b::_iaq(float i) {
  int iaq = round(i);
  if(!sensors.checkIaq(i)) { // if the data is wrong
    _disp_seg_img[0][0] = SYMB_A;
    _disp_seg_img[0][1] = SYMB_MINUS;
    _disp_seg_img[0][2] = SYMB_MINUS;
    _disp_seg_img[0][3] = SYMB_MINUS;
  }
  else { // if the data is correct
    _disp_seg_img[0][0] = SYMB_A;
    _disp_seg_img[0][1] = iaq < 100 ? SYMB_SPACE : floor(iaq / 100);
    _disp_seg_img[0][2] = iaq < 10 ? SYMB_SPACE : floor(iaq % 100 / 10);
    _disp_seg_img[0][3] = iaq % 10;
  }
}

/**
 * Preparing data for displaying CO2
 */
void WS2812b::_co2(float c) {
  int co2 = round(c);
  if(!wsensor.checkCo2(c)) { // if the data is wrong
    _disp_seg_img[0][0] = SYMB_MINUS;
    _disp_seg_img[0][1] = SYMB_MINUS;
    _disp_seg_img[0][2] = SYMB_MINUS;
    _disp_seg_img[0][3] = SYMB_MINUS;
  }
  else { // if the data is correct
    _disp_seg_img[0][0] = co2 < 1000 ? SYMB_SPACE : floor(co2 / 1000);
    _disp_seg_img[0][1] = co2 < 100 ? SYMB_SPACE : floor(co2 % 1000 / 100);
    _disp_seg_img[0][2] = co2 < 10 ? SYMB_SPACE : floor(co2 % 100 / 10);
    _disp_seg_img[0][3] = co2 % 10;
  }
}

/**
 * Displays data on the display
 */
void WS2812b::_print() {
  
  /* Color definition */
  String color = config.display_timeSlot_color(_timeSlot, 1);
  unsigned int number = strtol(&color[1], NULL, 16);
  unsigned int red = number >> 16;
  unsigned int green = number >> 8 & 0xFF;
  unsigned int blue = number & 0xFF;
  unsigned int maxBright = config.display_brightness_max(1);
  unsigned int minBright = config.display_brightness_min(1);
  
  /* Max brightness limit */
  do {
    unsigned int r = float(red) / 100.0 * float(_brightness);
    unsigned int g = float(green) / 100.0 * float(_brightness);
    unsigned int b = float(blue) / 100.0 * float(_brightness);
    if((r > maxBright or g > maxBright or b > maxBright) and _brightness) _brightness--;
    else { red = r; green = g; blue = b;
    }
  }
  while(red > maxBright or green > maxBright or blue > maxBright);
  
  /* Min brightness limit */
  do {
    unsigned int r = float(red) / 100.0 * float(_brightness);
    unsigned int g = float(green) / 100.0 * float(_brightness);
    unsigned int b = float(blue) / 100.0 * float(_brightness);
    if(r < minBright and g < minBright and b < minBright) _brightness++;
    else { red = r; green = g; blue = b; }
  }
  while(red < minBright and green < minBright and blue < minBright);

  /* Translate to 7 segment code */
  unsigned int ledsSeg = config.display_type(1);
  unsigned int ledsDig = ledsSeg * 7;
  _to7segmentCode(ledsSeg, ledsDig, _disp_seg_img[0][0], _disp_seg_img[0][1], _disp_seg_img[0][2], _disp_seg_img[0][3]);
    
  /* Turn on/off the clock points */
  if(config.display_animation_points() == 0) _clock_pixels[ledsDig * 2] = _clock_pixels[ledsDig * 2 + 1] = _dots ? _clockdots ? 1 : 0 : 0;
  if(config.display_animation_points() == 1) {
    _clock_pixels[ledsDig * 2] = _dots ? _clockdots ? 1 : 0 : 0;
    _clock_pixels[ledsDig * 2 + 1] = !_dots ? _clockdots ? 1 : 0 : 0;
  }
  if(config.display_animation_points() == 2) _clock_pixels[ledsDig * 2] = _clock_pixels[ledsDig * 2 + 1] = _clockdots ? 1 : 0;
  if(config.display_animation_points() == 3) _clock_pixels[ledsDig * 2] = _clock_pixels[ledsDig * 2 + 1] = 0;

  unsigned int reds[PIXELS];
  unsigned int greens[PIXELS];
  unsigned int blues[PIXELS];
  
  /**
   * Horizontal animation
   */
  if((config.display_animation_type() > 2) && (config.display_animation_type() <= 9) && (_timeSlot != _timeSlotPrev)) {
    int shift[7][7][4] = {
      {{0, -1, -2, -3}, {4, 0, -1, -2}, {3, 4, 0, -1}, {2, 3, 4, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{-2, -3, -4, 0}, {-3, -4, 0, 1}, {-4, 0, 1, 2}, {0, 1, 2, 3}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, -1, -2, -3}, {0, 0, -1, -2}, {0, 0, 0, -1}, {0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 2}, {0, 1, 2, 3}},
      {{-2, -3, -4, 0}, {-3, -4, 0, 0}, {-4, 0, 0, 0}, {0, 0, 0, 0}, {4, 0, 0, 0}, {3, 4, 0, 0}, {2, 3, 4, 0}},
      {{-2, 0, 0, -3}, {0, 0, 0, 0}, {2, 0, 0, 3}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{-1, -2, -3, 4}, {-1, -2, 3, 4}, {-1, 2, 3, 4}, {1, 2, 3, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{1, -2, -3, -4}, {1, 2, -3, -4}, {1, 2, 3, -4}, {1, 2, 3, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
    };
    unsigned int frames[7] = {4, 4, 7, 7, 3, 4, 4};
    unsigned int anim_type = config.display_animation_type() - 3;
    for(unsigned int i=0; i<frames[anim_type]; i++) {
      int s0 = shift[anim_type][i][0] == 0 ? SYMB_SPACE : _disp_seg_img[shift[anim_type][i][0] < 0 ? 1 : 0][abs(shift[anim_type][i][0]) - 1];
      int s1 = shift[anim_type][i][1] == 0 ? SYMB_SPACE : _disp_seg_img[shift[anim_type][i][1] < 0 ? 1 : 0][abs(shift[anim_type][i][1]) - 1];
      int s2 = shift[anim_type][i][2] == 0 ? SYMB_SPACE : _disp_seg_img[shift[anim_type][i][2] < 0 ? 1 : 0][abs(shift[anim_type][i][2]) - 1];
      int s3 = shift[anim_type][i][3] == 0 ? SYMB_SPACE : _disp_seg_img[shift[anim_type][i][3] < 0 ? 1 : 0][abs(shift[anim_type][i][3]) - 1];
      _to7segmentCode(ledsSeg, ledsDig, s0, s1, s2, s3);

      for(unsigned int k=0; k<(ledsDig + 1); k++) {
        reds[k] = shift[anim_type][i][0] < 0 ? _red_prev : red;
        greens[k] = shift[anim_type][i][0] < 0 ? _green_prev : green;
        blues[k] = shift[anim_type][i][0] < 0 ? _blue_prev : blue;
      }
      for(unsigned int k=ledsDig; k<((ledsDig * 2) + 1); k++) {
        reds[k] = shift[anim_type][i][1] < 0 ? _red_prev : red;
        greens[k] = shift[anim_type][i][1] < 0 ? _green_prev : green;
        blues[k] = shift[anim_type][i][1] < 0 ? _blue_prev : blue;
      }
      for(unsigned int k=((ledsDig * 2) + 2); k<((ledsDig * 3) + 3); k++) {
        reds[k] = shift[anim_type][i][2] < 0 ? _red_prev : red;
        greens[k] = shift[anim_type][i][2] < 0 ? _green_prev : green;
        blues[k] = shift[anim_type][i][2] < 0 ? _blue_prev : blue;
      }
      for(unsigned int k=((ledsDig * 3) + 2); k<PIXELS; k++) {
        reds[k] = shift[anim_type][i][3] < 0 ? _red_prev : red;
        greens[k] = shift[anim_type][i][3] < 0 ? _green_prev : green;
        blues[k] = shift[anim_type][i][3] < 0 ? _blue_prev : blue;
      }
      
      _clock_pixels[ledsDig * 2] = _clock_pixels[ledsDig * 2 + 1] = 0;
      
      _sendToDisplay(reds, greens, blues, _clock_pixels);
      
      vTaskDelay(round((1.0 / float(config.display_animation_speed())) * 1000));
    }
    _to7segmentCode(ledsSeg, ledsDig, _disp_seg_img[0][0], _disp_seg_img[0][1], _disp_seg_img[0][2], _disp_seg_img[0][3]);
    _sendToDisplay(reds, greens, blues, _clock_pixels);
  }

  /**
   * Vertical animation 
   */
  if(((config.display_animation_type() == 1) || (config.display_animation_type() == 2)) && (_timeSlot != _timeSlotPrev)) {
    bool pixels[PIXELS];
    int shift[2][3][2][PIXELS] = {
      {
        {
          {999,-4,999,2,6,0,1, 999,-11,999,9,13,7,8, 14,999, 999,-20,999,18,22,16,17, 999,-27,999,25,29,23,24, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
          {-5,-6,-3,999,1,999,-4, -12,-13,-10,999,8,999,-11, 999,-15, -21,-22,-19,999,17,999,-20, -28,-29,-26,999,24,999,-27, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        },
        {
          {999,999,-4,-4,999,999,2,2,6,6,0,0,1,1, 999,999,-11,-11,999,999,9,9,13,13,7,7,8,8, 14,999, 999,999,-20,-20,999,999,18,18,22,22,16,16,17,17, 999,999,-27,-27,999,999,25,25,29,29,23,23,24,24, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
          {-5,-5,-6,-6,-3,-3,999,999,1,1,999,999,-4,-4, -12,-12,-13,-13,-10,-10,999,999,8,8,999,999,-11,-11, 999,-15, -21,-21,-22,-22,-19,-19,999,999,17,17,999,999,-20,-20, -28,-28,-29,-29,-26,-26,999,999,24,24,999,999,-27,-27, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        },
        {
          {999,999,999,-4,-4,-4,999,999,999,2,2,2,6,6,6,0,0,0,1,1,1, 999,999,999,-11,-11,-11,999,999,999,9,9,9,13,13,13,7,7,7,8,8,8, 14,999, 999,999,999,-20,-20,-20,999,999,999,18,18,18,22,22,22,16,16,16,17,17,17, 999,999,999,-27,-27,-27,999,999,999,25,25,25,29,29,29,23,23,23,24,24,24, 0},
          {-5,-5,-5,-6,-6,-6,-3,-3,-3,999,999,999,1,1,1,999,999,999,-4,-4,-4, -12,-12,-12,-13,-13,-13,-10,-10,-10,999,999,999,8,8,8,999,999,999,-11,-11,-11, 999,-15, -21,-21,-21,-22,-22,-22,-19,-19,-19,999,999,999,17,17,17,999,999,999,-20,-20,-20, -28,-28,-28,-29,-29,-29,-26,-26,-26,999,999,999,24,24,24,999,999,999,-27,-27,-27, 0}
        }
      },
      {
        {
          {5,6,3,999,-1,999,4, 12,13,10,999,-8,999,11, 999,14, 21,22,19,999,-17,999,20, 28,29,26,999,-24,999,27, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
          {999,4,999,-2,-6,-999,-1, 999,11,999,-9,-13,-7,-8, -15,999, 999,20,999,-18,-22,-16,-17, 999,27,999,-25,-29,-23,-24, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        },
        {
          {5,5,6,6,3,3,999,999,-1,-1,999,999,4,4, 12,12,13,13,10,10,999,999,-8,-8,999,999,11,11, 999,14, 21,21,22,22,19,19,999,999,-17,17,999,999,20,20, 28,28,29,29,26,26,999,999,-24,-24,999,999,27,27, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
          {999,999,4,4,999,999,-2,-2,-6,-6,-999,-999,-1,-1, 999,999,11,11,999,999,-9,-9,-13,-13,-7,-7,-8,-8, -15,999, 999,999,20,20,999,999,-18,-18,-22,-22,-16,-16,-17,-17, 999,999,27,27,999,999,-25,-25,-29,-29,-23,-23,-24,-24, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        },
        {
          {5,5,5,6,6,6,3,3,3,999,999,999,-1,-1,-1,999,999,999,4,4,4, 12,12,12,13,13,13,10,10,10,999,999,999,-8,-8,-8,999,999,999,11,11,11, 999,14, 21,21,21,22,22,22,19,19,19,999,999,999,-17,17,-17,999,999,999,20,20,20, 28,28,28,29,29,29,26,26,26,999,999,999,-24,-24,-24,999,999,999,27,27,27},
          {999,999,999,4,4,4,999,999,999,-2,-2,-2,-6,-6,-6,-999,-999,-999,-1,-1,-1, 999,999,999,11,11,11,999,999,999,-9,-9,-9,-13,-13,-13,-7,-7,-7,-8,-8,-8, -15,999, 999,999,999,20,20,20,999,999,999,-18,-18,-18,-22,-22,-22,-16,-16,-16,-17,-17,-17, 999,999,999,27,27,27,999,999,999,-25,-25,-25,-29,-29,-29,-23,-23,-23,-24,-24,-24}
        }
      }
    };
    
    for(unsigned int frame=0; frame<2; frame++) {
      for(unsigned int i=0; i<PIXELS; i++) {
        if(shift[config.display_animation_type() - 1][ledsSeg - 1][frame][i] < 0) {
          if(shift[config.display_animation_type() - 1][ledsSeg - 1][frame][i] == -999) pixels[i] = _clock_pixels[0];
          else pixels[i] = _clock_pixels[abs(shift[config.display_animation_type() - 1][ledsSeg - 1][frame][i])];
          reds[i] = red;
          greens[i] = green;
          blues[i] = blue;
        }
        else {
          if(shift[config.display_animation_type() - 1][ledsSeg - 1][frame][i] == 999) pixels[i] = false;
          else pixels[i] = _clock_pixels_prev[shift[config.display_animation_type() - 1][ledsSeg - 1][frame][i]];      
          reds[i] = _red_prev;
          greens[i] = _green_prev;
          blues[i] = _blue_prev;
        }
      }
      _sendToDisplay(reds, greens, blues, pixels);
      vTaskDelay(round((1.0 / float(config.display_animation_speed())) * 1000));
    }
    
    for(unsigned int i=0; i<PIXELS; i++) {
      reds[i] = red;
      greens[i] = green;
      blues[i] = blue;
    }
    _sendToDisplay(reds, greens, blues, _clock_pixels);  
  }

  /* No animation needed */
  else {
    for(unsigned int i=0; i<PIXELS; i++) {
      reds[i] = red;
      greens[i] = green;
      blues[i] = blue;
    }
    _sendToDisplay(reds, greens, blues, _clock_pixels);
  }

  /* Save display data to previous display data */
  if(config.display_animation_type() && (_timeSlot != _timeSlotPrev)) {
    for(unsigned int i=0; i<PIXELS; i++) _clock_pixels_prev[i] = _clock_pixels[i];
    for(unsigned int i=0; i<4; i++) _disp_seg_img[1][i] = _disp_seg_img[0][i];
    _timeSlotPrev = _timeSlot;
    _red_prev = red;
    _green_prev = green;
    _blue_prev = blue;
  }
}

/**
 * Send data to display
 */
void WS2812b::_sendToDisplay(unsigned int red[PIXELS], unsigned int green[PIXELS], unsigned int blue[PIXELS], bool clock_pixels[PIXELS]) {
  RgbColor ledOff(0);
  for(unsigned int i=0; i<PIXELS; i++) {
    RgbColor ledOn(red[i], green[i], blue[i]);
    if(i == 0) strip.SetPixelColor(i, ledOff);
    else {
      if(clock_pixels[i - 1] and _power) strip.SetPixelColor(i, ledOn);
      else strip.SetPixelColor(i, ledOff);
    }
  }
  strip.Show();
}

/**
 * Translate to 7 segment code 
 */
void WS2812b::_to7segmentCode(unsigned int ledsSeg, unsigned int ledsDig, int dig0, int dig1, int dig2, int dig3) {
  for(unsigned int i=0; i<ledsDig; i++) {
    _clock_pixels[ledsDig * 0 + i] = bitRead(font_ws2812[dig0], i / ledsSeg) ? 1 : 0;
    _clock_pixels[ledsDig * 1 + i] = bitRead(font_ws2812[dig1], i / ledsSeg) ? 1 : 0;
    _clock_pixels[ledsDig * 2 + i + 2] = bitRead(font_ws2812[dig2], i / ledsSeg) ? 1 : 0;
    _clock_pixels[ledsDig * 3 + i + 2] = bitRead(font_ws2812[dig3], i / ledsSeg) ? 1 : 0;
  }
}

/**
 * Connecting to the network animation
 */
void WS2812b::connecting() {
  if(_connect < 3) {
    _disp_seg_img[0][0] = SYMB_ANT1;
    _disp_seg_img[0][1] = _connect > 0 ? SYMB_ANT2 : SYMB_SPACE;
    _disp_seg_img[0][2] = _connect > 1 ? SYMB_ANT3 : SYMB_SPACE;
    _disp_seg_img[0][3] = SYMB_SPACE;
    _print();
    if(_connect < 2) _connect++;
    else _connect = 0;
  }
}
