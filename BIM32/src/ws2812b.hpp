#include <LiteLED.h> // v1.2.0 https://github.com/Xylopyrographer/LiteLED/tree/main

LiteLED strip_1(LED_STRIP_WS2812, 0, RMT_CHANNEL_0);
LiteLED strip_2(LED_STRIP_WS2812, 0, RMT_CHANNEL_1);

/* maximum possible number of LEDs in the display 
 * 6 digits of 7 segments of 3 LEDs + 4 LEDs for hour dots + 1 "sacrificial" LED 
 */
#define PIXELS 131


class WS2812b {
  public:
    void init(uint8_t dispNum, uint8_t pin);
    void setDotFreq(unsigned int freq);
    void displayToggle();
    void displayOn();
    void displayOff();
    bool isDisplayOn();
    void refresh();
    void brightness(unsigned int intensity, bool reduc);

  private:
    uint8_t _dispNum = 0;                                   // display number
    uint8_t _pixelCount = 1;                                // number of pixels in display
    int _disp_seg_img[2][6] = {                             // display digits
      {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE}, 
      {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE}
    };   
    bool _clock_pixels[PIXELS];                             // display pixels
    bool _clock_pixels_prev[PIXELS];                        // display pixels previous slot
    unsigned int _mills = 0;                                // delay variable
    unsigned int _timeSlot = 0;                             // display slot number
    unsigned int _timeSlotPrev = 0;                         // previous slot number 
    bool _dots = false;                                     // time points
    bool _clockdots = false;                                // need for time points
    unsigned int _dotmills = 0;                             // delay variable for time points
    unsigned int _dotfreq = 500;                            // time points period
    bool _power = true;                                     // display on/off flag
    unsigned int _brightness = 40;                          // display brightness
    uint8_t _red_prev = 0, _green_prev = 0, _blue_prev = 0; // previous slot color
    unsigned int _connect = 0;                              // wifi connect animation

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
    void _sendToDisplay(uint8_t red[PIXELS], uint8_t green[PIXELS], uint8_t blue[PIXELS], bool clock_pixels[PIXELS]);
    void _to7segmentCode(unsigned int ledsSeg, unsigned int ledsDig, int dig0, int dig1, int dig2, int dig3);
};

/**
 * Display initialize
 * @param dispNum display number
 * @param pin display pin number
 */
void WS2812b::init(uint8_t dispNum, uint8_t pin) {
  _dispNum = dispNum;
 
  switch(config.display_model(dispNum)) {
    case 0: _pixelCount = 4 * 7 * 1 + 2 + 1; break; // 4 digits, 7 segments, 1 led per segment, 2 clock points, 1 sacrificial led
    case 1: _pixelCount = 4 * 7 * 2 + 2 + 1; break;
    case 2: _pixelCount = 4 * 7 * 3 + 2 + 1; break;
    case 3: _pixelCount = 6 * 7 * 1 + 4 + 1; break;
    case 4: _pixelCount = 6 * 7 * 2 + 4 + 1; break;
    case 5: _pixelCount = 6 * 7 * 3 + 4 + 1; break;
    default: _pixelCount = 1; break;
  } 
  
  if(_dispNum == 0) {
    strip_1.begin(pin, _pixelCount);
    strip_1.brightness(255);
    strip_1.clear();
  }
  if(_dispNum == 1) {
    strip_2.begin(pin, _pixelCount);
    strip_2.brightness(255);
    strip_2.clear();
  }
  
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
 * @param intensity brightness
 * @param reduc brightness is reduced or not
 */
void WS2812b::brightness(unsigned int intensity, bool reduc) {
  _brightness = reduc ? round(intensity / 2) : intensity;
}

/**
 * Displays the next display slot
 */
void WS2812b::refresh() {
  _connect = 10; // Disable animation of connecting to the network
  
  /* Slot switch */
  if(millis() - _mills >= config.display_timeSlot_period(_timeSlot, _dispNum) * 1000 or config.display_timeSlot_period(_timeSlot, _dispNum) == 0) {
    _timeSlot++;
    for(unsigned int i=_timeSlot; i<8; i++) {
      if(config.display_timeSlot_period(_timeSlot, _dispNum) == 0) _timeSlot++;
      else break;
    }
    if(_timeSlot > 7) _timeSlot = 0;
    _mills = millis();
  }

  /* Displaying the next display slot */
  if(millis() - _dotmills >= _dotfreq) {
    _dotmills = millis();
    
    if(config.display_timeSlot_period(_timeSlot, _dispNum) > 0) {
      _clockdots = false;

      switch(config.display_timeSlot_sensor(_timeSlot, _dispNum)) {
        case 0: // Clock 
          _clockdots = true;
          _clock();
          break;
        case 1: // Date
          _date();
          break;
        case 2: // BME280
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 0) _temp(sensors.get_bme280_temp(config.bme280_temp_corr()));
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 1) _hum(sensors.get_bme280_hum(config.bme280_hum_corr()));
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 2) _pres(sensors.get_bme280_pres(config.bme280_pres_corr()));
          break;
        case 3: // BMP180
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 0) _temp(sensors.get_bmp180_temp(config.bmp180_temp_corr()));
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 1) _pres(sensors.get_bmp180_pres(config.bmp180_pres_corr()));
          break;
        case 4: // SHT21
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 0) _temp(sensors.get_sht21_temp(config.sht21_temp_corr()));
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 1) _hum(sensors.get_sht21_hum(config.sht21_hum_corr()));
          break;
        case 5: // DHT22
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 0) _temp(sensors.get_dht22_temp(config.dht22_temp_corr()));
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 1) _hum(sensors.get_dht22_hum(config.dht22_hum_corr()));
          break;
        case 6: // DS18B20
          _temp(sensors.get_ds18b20_temp(config.ds18b20_temp_corr()));
          break;
        case 7: // ESP32
          _temp(sensors.get_esp32_temp(config.esp32_temp_corr()));
          break;
        case 8: // Thingspeak
          if((now() - thingspeak.get_updated()) < (config.thingspeakReceive_expire() * 60)) {
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 0) _temp(thingspeak.get_field(config.display_timeSlot_thing(_timeSlot, _dispNum)));
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 1) _hum(thingspeak.get_field(config.display_timeSlot_thing(_timeSlot, _dispNum)));
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 2) _pres(thingspeak.get_field(config.display_timeSlot_thing(_timeSlot, _dispNum)));
          }
          else {
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 0) _temp(40400.0);
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 1) _hum(40400.0);
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 2) _pres(40400.0);
          }
          break;
        case 9: // Weather
          if(now() - weather.get_currentUpdated() < 1200) {
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 0) _temp(weather.get_currentTemp());
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 1) _hum(weather.get_currentHum());
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 2) _pres(weather.get_currentPres());
          }
          else {
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 0) _temp(40400.0);
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 1) _hum(40400.0);
            if(config.display_timeSlot_data(_timeSlot, _dispNum) == 2) _pres(40400.0);
          }
          break;
        case 10: { // Wireless sensor
          unsigned int wsensNum = config.display_timeSlot_wsensor_num(_timeSlot, _dispNum);
          unsigned int wsensType = config.display_timeSlot_wsensor_type(_timeSlot, _dispNum);
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
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 0) _temp(sensors.get_bme680_temp(config.bme680_temp_corr()));
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 1) _hum(sensors.get_bme680_hum(config.bme680_hum_corr()));
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 2) _pres(sensors.get_bme680_pres(config.bme680_pres_corr()));
          if(config.display_timeSlot_data(_timeSlot, _dispNum) == 3) _iaq(sensors.get_bme680_iaq(config.bme680_iaq_corr()));
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
 * @param t temperature
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
 * @param h humidity
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
 * @param p pressure
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
 * @param i Index Air Quality
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
 * @param c CO2 level
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
  String color = config.display_timeSlot_color(_timeSlot, _dispNum);
  unsigned int number = strtol(&color[1], NULL, 16);
  uint8_t red = number >> 16;
  uint8_t green = number >> 8 & 0xFF;
  uint8_t blue = number & 0xFF;
  unsigned int maxBright = config.display_brightness_max(_dispNum);
  unsigned int minBright = config.display_brightness_min(_dispNum);
  
  /* Max brightness limit */
  do {
    uint8_t r = float(red) / 100.0 * float(_brightness);
    uint8_t g = float(green) / 100.0 * float(_brightness);
    uint8_t b = float(blue) / 100.0 * float(_brightness);
    if((r > maxBright or g > maxBright or b > maxBright) and _brightness) _brightness--;
    else { red = r; green = g; blue = b;
    }
  }
  while(red > maxBright or green > maxBright or blue > maxBright);
  
  /* Min brightness limit */
  do {
    uint8_t r = float(red) / 100.0 * float(_brightness);
    uint8_t g = float(green) / 100.0 * float(_brightness);
    uint8_t b = float(blue) / 100.0 * float(_brightness);
    if(r < minBright and g < minBright and b < minBright) _brightness++;
    else { red = r; green = g; blue = b; }
  }
  while(red < minBright and green < minBright and blue < minBright);

  /* Translate to 7 segment code */
  unsigned int ledsSeg = config.display_model(_dispNum) + 1;
  unsigned int ledsDig = ledsSeg * 7;
  _to7segmentCode(ledsSeg, ledsDig, _disp_seg_img[0][0], _disp_seg_img[0][1], _disp_seg_img[0][2], _disp_seg_img[0][3]);
    
  /* Turn on/off the clock points */
  if(config.display_animation_points(_dispNum) == 0) _clock_pixels[ledsDig * 2] = _clock_pixels[ledsDig * 2 + 1] = _dots ? _clockdots ? 1 : 0 : 0;
  if(config.display_animation_points(_dispNum) == 1) {
    _clock_pixels[ledsDig * 2] = _dots ? _clockdots ? 1 : 0 : 0;
    _clock_pixels[ledsDig * 2 + 1] = !_dots ? _clockdots ? 1 : 0 : 0;
  }
  if(config.display_animation_points(_dispNum) == 2) _clock_pixels[ledsDig * 2] = _clock_pixels[ledsDig * 2 + 1] = _clockdots ? 1 : 0;
  if(config.display_animation_points(_dispNum) == 3) _clock_pixels[ledsDig * 2] = _clock_pixels[ledsDig * 2 + 1] = 0;

  uint8_t reds[PIXELS];
  uint8_t greens[PIXELS];
  uint8_t blues[PIXELS];
  
  /**
   * With animation
   */
  if((config.display_animation_type(_dispNum) > 0) && (config.display_animation_type(_dispNum) <= 7) && (_timeSlot != _timeSlotPrev)) {
    int shift[7][7][4] = {
      {{0, -1, -2, -3}, {4, 0, -1, -2}, {3, 4, 0, -1}, {2, 3, 4, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{-2, -3, -4, 0}, {-3, -4, 0, 1}, {-4, 0, 1, 2}, {0, 1, 2, 3}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, -1, -2, -3}, {0, 0, -1, -2}, {0, 0, 0, -1}, {0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 2}, {0, 1, 2, 3}},
      {{-2, -3, -4, 0}, {-3, -4, 0, 0}, {-4, 0, 0, 0}, {0, 0, 0, 0}, {4, 0, 0, 0}, {3, 4, 0, 0}, {2, 3, 4, 0}},
      {{-2, 0, 0, -3},  {0, 0, 0, 0},   {2, 0, 0, 3},  {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{-1, -2, -3, 4}, {-1, -2, 3, 4}, {-1, 2, 3, 4}, {1, 2, 3, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{1, -2, -3, -4}, {1, 2, -3, -4}, {1, 2, 3, -4}, {1, 2, 3, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
    };
    unsigned int frames[7] = {4, 4, 7, 7, 3, 4, 4};
    unsigned int anim_type = config.display_animation_type(_dispNum) - 1;
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
      
      vTaskDelay(round((1.0 / float(config.display_animation_speed(_dispNum))) * 1000));
    }
    _to7segmentCode(ledsSeg, ledsDig, _disp_seg_img[0][0], _disp_seg_img[0][1], _disp_seg_img[0][2], _disp_seg_img[0][3]);
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
  if(config.display_animation_type(_dispNum) && (_timeSlot != _timeSlotPrev)) {
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
 * @param red   array with red pixels
 * @param green array with green pixels
 * @param blue  array with blue pixels
 * @param clock_pixels array with states of pixels (on/off)
 */
void WS2812b::_sendToDisplay(uint8_t red[PIXELS], uint8_t green[PIXELS], uint8_t blue[PIXELS], bool clock_pixels[PIXELS]) {
  for(unsigned int i=0; i<_pixelCount; i++) {
    if(i == 0) {
      if(_dispNum == 0) strip_1.setPixel(i, 0);
      if(_dispNum == 1) strip_2.setPixel(i, 0);
    }
    else {
      if(clock_pixels[i - 1] and _power) {
        rgb_t pixel = { .r = red[i], .g = green[i], .b = blue[i] };
        if(_dispNum == 0) strip_1.setPixel(i, pixel);
        if(_dispNum == 1) strip_2.setPixel(i, pixel);
      }
      else {
        if(_dispNum == 0) strip_1.setPixel(i, 0);
        if(_dispNum == 1) strip_2.setPixel(i, 0);
      }
    }
  }
  if(_dispNum == 0) strip_1.show();
  if(_dispNum == 1) strip_2.show();
}

/**
 * Translate to 7 segment code
 * @param ledsSeg number of LEDs in segment
 * @param ledsDig number of LEDs in digit
 * @param dig0_dig3 digits left to tight
 */
void WS2812b::_to7segmentCode(unsigned int ledsSeg, unsigned int ledsDig, int dig0, int dig1, int dig2, int dig3) {
  for(unsigned int i=0; i<ledsDig; i++) {
    _clock_pixels[ledsDig * 0 + i] = bitRead(font_ws2812[dig0], i / ledsSeg) ? 1 : 0;
    _clock_pixels[ledsDig * 1 + i] = bitRead(font_ws2812[dig1], i / ledsSeg) ? 1 : 0;
    _clock_pixels[ledsDig * 2 + i + 2] = bitRead(font_ws2812[dig2], i / ledsSeg) ? 1 : 0;
    _clock_pixels[ledsDig * 3 + i + 2] = bitRead(font_ws2812[dig3], i / ledsSeg) ? 1 : 0;
  }
}