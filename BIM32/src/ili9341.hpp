#include <TFT_eSPI.h> // v2.5.34 https://github.com/Bodmer/TFT_eSPI
TFT_eSPI tft = TFT_eSPI();
#include <TJpg_Decoder.h> // v1.0.8 https://github.com/Bodmer/TJpg_Decoder

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
    if(y >= tft.height()) return 0;
    tft.pushImage(x, y, w, h, bitmap);
    return 1;
}

class ILI9341 : LcdDisplay {
    #define LEFT 0
    #define CENTER 1
    #define RIGHT 2

    #define FONT1 0
    #define FONT2 1
    #define FONT3 2

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
        void init();
        void showLogo();
        void refresh();
        void clockPoints();
        void brightness(unsigned int bright);
        void displayToggle();
        void displayOn();
        void displayOff();
        bool isDisplayOn();

    private:
        unsigned int _prevBright = 40400;
        uint16_t _air_color[4] = { BATTERY_COLOR, 0xFFE0, 0xFD20, 0xF800 };

        void _showImg(uint16_t x, uint16_t y, String img);
        void _printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color);
        void _showTemperature(int temp, uint16_t x, uint16_t y, uint8_t font, uint16_t color);
        void _showHumidity(int hum, uint16_t x, uint16_t y);
        void _showTime();
        void _showWeekday();
        void _showAntenna();
        void _drawSkeleton();
        void _showTemperatureInside();
        void _showTemperatureOutside();
        void _showThermometer();
        void _showHumidityInside();
        void _showHumidityOutside();
        void _showComfort();
        void _showBatteryLevel();
        void _showVoltageOrPercentage();
        void _showWeatherIcon();
        void _showDescription();
        void _showPressure();
        void _showWindSpeed();
        void _showWindDirection();
        void _showUpdTime();
        void _showForecast();
};

/**
 * Display and fonts initialisation
 */
void ILI9341::init(void) {
    tft.begin();
    tft.setRotation(3);

    tft.setSwapBytes(true);
    TJpgDec.setJpgScale(1);
    TJpgDec.setCallback(tft_output);
    
    tft.fillScreen(0);
    _drawSkeleton();

    pinMode(TFT_BACKLIGHT, OUTPUT);
    brightness(1023);
}

void ILI9341::showLogo() {
    _showImg(0, 0, "/img/logo.jpg");
}

/**
 * Toggles display (on/off)
 */
void ILI9341::displayToggle() {
    _power = !_power;
}

/**
 * Turns on the display
 */
void ILI9341::displayOn() {
    _power = true;
}

/*
 * Turns off the display
 */
void ILI9341::displayOff() {
    _power = false;
}

/**
 * Returns true if display is on
 */
bool ILI9341::isDisplayOn() {
    return _power;
}

/**
 * Display refresh
 */
void ILI9341::refresh() {
    if(_power) {
        _getData();

        _showTime();
        clockPoints();
        _showWeekday();
        _showBatteryLevel();
        _showVoltageOrPercentage();
        _showAntenna();
        
        _showComfort();
        _showTemperatureInside();
        _showHumidityInside();

        _showWeatherIcon();
        _showDescription();
        _showTemperatureOutside();
        _showHumidityOutside();
        _showPressure();
        _showWindSpeed();
        _showWindDirection();
        _showUpdTime();
        
        _showForecast();
        _showForecast();
        _showForecast();
    }
}

void ILI9341::brightness(unsigned int bright) {
    if(_power) {
        float r = 100 * log10(2) / log10(255);
        uint16_t br = round(pow(2, (bright / r)));
        if(br < 1023) analogWrite(TFT_BACKLIGHT, br);
        _prevBright = bright;
    }
    else analogWrite(TFT_BACKLIGHT, 0);
}

void ILI9341::_showImg(uint16_t x, uint16_t y, String img) {
    TJpgDec.drawFsJpg(x, y, img, LittleFS);
}

void ILI9341::_printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color) {
    tft.fillRect(x, y, width, height, BG_COLOR);
    if(font == FONT1) tft.loadFont(Ubuntu_14);
    else if(font == FONT2) tft.loadFont(Ubuntu_21);
    else if(font == FONT3) tft.loadFont(Ubuntu_29);
    tft.setTextColor(color, BG_COLOR);
    uint16_t w = tft.textWidth(text);
    if(align == CENTER) x += (width / 2) - (w / 2);
    if(align == RIGHT) x += width - w - 4;
    tft.setCursor(x, y);
    tft.print(text);
    tft.unloadFont();
}

/**
 * Display temperature
 */
void ILI9341::_showTemperature(int temp, uint16_t x, uint16_t y, uint8_t font, uint16_t color) {
    String buf = validate.temp(temp) ? (String(temp) + "°C") : "--";
    _printText(x, y, font == FONT3 ? 70 : 56, font == FONT3 ? 26 : 20, buf, font, font == FONT3 ? CENTER : RIGHT, color);
}

/**
 * Display humidity
 */
void ILI9341::_showHumidity(int hum, uint16_t x, uint16_t y) {
    String buf = validate.hum(hum) ? (String(hum) + "%") : "--";
    _printText(x, y, 58, 20, buf, FONT2, CENTER, HUMIDITY_COLOR);
}

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
    _showImg(33, 0, "/img/digits/0.jpg");
    _showImg(77, 0, "/img/digits/0.jpg");
    _showImg(109, 0, "/img/digits/0.jpg");
}

void ILI9341::_showTime() {
    char buf[20] = "";
    if(_prevTHour != _tHour) {
        sprintf(buf, "/img/digits/%d.jpg", _tHour / 10);
        if(_tHour < 10) tft.fillRect(0, 0, 32, 78, BG_COLOR);
        else _showImg(0, 0, buf);
        sprintf(buf, "/img/digits/%d.jpg", _tHour % 10);
        _showImg(33, 0, buf);
        _prevTHour = _tHour;
    }
    if(_prevTMinute != _tMinute) {
        sprintf(buf, "/img/digits/%d.jpg", _tMinute / 10);
        _showImg(77, 0, buf);
        sprintf(buf, "/img/digits/%d.jpg", _tMinute % 10);
        _showImg(109, 0, buf);
        _prevTMinute = _tMinute;
    }
}

void ILI9341::_showWeekday() {
    if(_prevTWeekday != _tWeekday) {
        unsigned int wd = _tWeekday;
        _printText(146, 6, 40, 20, lang.weekdayShortName(wd), FONT2, LEFT, CLOCK_COLOR);
        _printText(33, 168, 40, 16, lang.weekdayShortName(wd), FONT1, CENTER, TEXT_COLOR);
        if(++wd > 7) wd = 1;
        _printText(139, 168, 40, 16, lang.weekdayShortName(wd), FONT1, CENTER, TEXT_COLOR);
        if(++wd > 7) wd = 1;
        _printText(245, 168, 40, 16, lang.weekdayShortName(wd), FONT1, CENTER, TEXT_COLOR);
        _prevTWeekday = _tWeekday;
    }
}

/**
 * Display clock points
 */
void ILI9341::clockPoints() {
    boolean points = millis() % 1000 > 500;
    tft.fillCircle(70, 24, 3, points ? CLOCK_COLOR : BG_COLOR);
    tft.fillCircle(70, 52, 3, points ? CLOCK_COLOR : BG_COLOR);
}

/**
 * Display antenna symbol
 */
void ILI9341::_showAntenna() {
    if(_prevRssi != _rssi or _prevIsApMode != _isApMode) {
        String ant = "";
        if(_isApMode) ant = "acpoint";
        else {
            if(_rssi > -51) ant = "ant_4";
            if(_rssi < -50 && _rssi > -76) ant = "ant_3";
            if(_rssi <- 75 && _rssi > -96) ant = "ant_2";
            if(_rssi < -95) ant = "ant_1";
            if(_rssi >= 0) ant = "ant_0";
        }
        _showImg(292, 1, "/img/ant/" + ant + ".jpg");
        _prevRssi = _rssi;
        _prevIsApMode = _isApMode;
    }
}

/**
 * Display temperature inside
 */
void ILI9341::_showTemperatureInside() {
    if(_prevTempIn != _tempIn) {
        if(config.display_source_tempIn_sens() != 4) {
            _showTemperature(int(round(_tempIn)), 173, 53, FONT3, TEMPERATURE_COLOR);
        }
        _prevTempIn = _tempIn;
    }
}

/**
 * Display temperature outside
 */
void ILI9341::_showTemperatureOutside() {
    if(_prevTempOut != _tempOut) {
        _showThermometer();
        _showTemperature(int(round(_tempOut)), 71, 113, FONT3, TEMPERATURE_COLOR);
        _prevTempOut = _tempOut;
    }
}

/**
 * Display thermometer icon (red or blue)
 */
void ILI9341::_showThermometer() {
    if(_tempOut < 0.0) _showImg(62, 104, "/img/symb/temp-.jpg");
    else _showImg(62, 104, "/img/symb/temp+.jpg");
}

/**
 * Display humidity inside
 */
void ILI9341::_showHumidityInside() {
    if(_prevHumIn != _humIn) {
        if(config.display_source_humIn_sens() != 4) {
            _showHumidity(int(round(_humIn)), 264, 58);
        }
        _prevHumIn = _humIn;
    }
}

/**
 * Display humidity outside
 */
void ILI9341::_showHumidityOutside() {
    if(_prevHumOut != _humOut) {
        _showHumidity(int(round(_humOut)), 164, 119);
        _prevHumOut = _humOut;
    }
}

/**
 * Display comfort level
 */
void ILI9341::_showComfort() {
    if(_prevComfort != _comfort) {
        if(config.display_source_tempIn_sens() != 4) {
            _printText(145, 28, 175, 16, _comfort, FONT1, CENTER, TEXT_COLOR);
        }
        _prevComfort = _comfort;
    } // TODO добавить отображение последовательности
}

/**
 * Display battery symbol
 */
void ILI9341::_showBatteryLevel() {
    if(_prevBatLevel != _batLevel) {
        if(validate.batLvl(_batLevel)) {
            char buf[20] = "";
            sprintf(buf, "/img/bat/bat%d.jpg", _batLevel);
            _showImg(258, 2, buf);
        }
        else tft.fillRect(258, 2, 32, 21, BG_COLOR);
        _prevBatLevel = _batLevel;
    }
}

/**
 * Display voltage, percentage, CO2 or IAQ
 */
void ILI9341::_showVoltageOrPercentage() {
    if(_prevVolt != _volt or _prevVoltColor != _voltColor) {
        _printText(198, 10, 58, 16, _volt, FONT1, RIGHT, _air_color[_voltColor]);
        _prevVolt = _volt;
        _prevVoltColor = _voltColor;
    }
}

/**
 * Display current weather icon
 */
void ILI9341::_showWeatherIcon() {
    if(_prevCurrIcon != _currIcon or _prevIsDay != _isDay) {
        String path = "/img/icons/big/";
        switch(_currIcon) {
            case 1: path += _isDay ? "01_d" : "01_n"; break;
            case 2: path += _isDay ? "02_d" : "02_n"; break;
            case 3: path += "04"; break;
            case 4: path += "09"; break;
            case 5: path += "10"; break;
            case 6: path += _isDay ? "11_d" : "11_n"; break;
            case 7: path += "13"; break;
            case 8: path += "50"; break;
            default: path += "loading"; break;
        }
        path += ".jpg";
        _showImg(0, 104, path);
        _prevCurrIcon = _currIcon;
        _prevIsDay = _isDay;
    }
}

/**
 * Display weather description
 */
void ILI9341::_showDescription() {
    if(_prevDescription != _description) {
        tft.loadFont("/fonts/Ubuntu-21", LittleFS);
        uint16_t w = tft.textWidth(_description);
        tft.unloadFont();
        _printText(0, 84, 319, 20, _description, w > 316 ? FONT1 : FONT2, CENTER, TEXT_COLOR);
        _prevDescription = _description;
    }
}

/**
 * Display pressure
 */
void ILI9341::_showPressure() {
    if(_prevPresOut != _presOut) {
        String buf = validate.pres(_presOut) ? (String(int(round(_presOut * 0.75))) + lang.mm()) : "--";
        _printText(250, 119, 70, 20, buf, FONT2, CENTER, PRESSURE_COLOR);
        _prevPresOut = _presOut;
    }
}

/**
 * Display wind speed
 */
void ILI9341::_showWindSpeed() {
    if(_prevWindSpd != _windSpd) {
        String wnd = validate.wind(_windSpd) ? String(int(round(_windSpd))) + lang.ms() : "--";
        _printText(93, 146, 40, 16, wnd, FONT1, LEFT, TEXT_COLOR);
        _prevWindSpd = _windSpd;
    }
}

/**
 * Display wind direction
 */
void ILI9341::_showWindDirection() {
    if(_prevWindDir != _windDir) {
        String dir;
        switch(_windDir) {
            case 1: dir = "ne"; break;
            case 2: dir = "e"; break;
            case 3: dir = "se"; break;
            case 4: dir = "s"; break;
            case 5: dir = "sw"; break;
            case 6: dir = "w"; break;
            case 7: dir = "nw"; break;
            default: dir = "n"; break;
        }
        _showImg(133, 143, "/img/wind/" + dir + ".jpg");
        _prevWindDir = _windDir;
    }
}

/**
 * Display the time and date of the last weather update
 */
void ILI9341::_showUpdTime() {
    if(_prevWeatherUpdated != _weatherUpdated) {
        time_t t = _weatherUpdated;
        char buf[32] = "";
        sprintf(buf, "%02d.%02d.%d %d:%02d:%02d", day(t), month(t), year(t), hour(t), minute(t), second(t));
        _printText(186, 148, 133, 16, t > 0 ? buf : "--", FONT1, RIGHT, TEXT_COLOR);
        tft.drawCircle(177, 153, 5, TEXT_COLOR);
        tft.drawFastHLine(176, 148, 4, BG_COLOR);
        tft.drawFastHLine(172, 148, 3, TEXT_COLOR);
        tft.drawFastVLine(175, 149, 3, TEXT_COLOR);
        _prevWeatherUpdated = _weatherUpdated;
    }
}

/**
 * Display daily weather forecast
 */
void ILI9341::_showForecast() {
    for(uint8_t i=0; i<3; i++) {
        if(_prevIcons[i] != _icons[i]) {
            String path = "/img/icons/small/";
            switch(_icons[i]) {
                case 1: path += "01"; break;
                case 2: path += "02"; break;
                case 3: path += "04"; break;
                case 4: path += "09"; break;
                case 5: path += "10"; break;
                case 6: path += "11"; break;
                case 7: path += "13"; break;
                case 8: path += "50"; break;
                default: path += "loading"; break;
            }
            path += ".jpg";
            _showImg(i * 106 + 2, 183, path);
            _prevIcons[i] = _icons[i];
        }

        if(_prevDTemps[i] != _dTemps[i]) {
            _showTemperature(int(round(_dTemps[i])), i * 106 + 49, 183, FONT2, TEMPERATURE_COLOR);
            _prevDTemps[i] = _dTemps[i];
        }

        if(_prevNTemps[i] != _nTemps[i]) {
            _showTemperature(int(round(_nTemps[i])), i * 106 + 49, 203, FONT2, TEMP_MIN_COLOR);
            _prevNTemps[i] = _nTemps[i];
        }

        if(_prevWinds[i] != _winds[i]) {
            String wnd = validate.wind(_winds[i]) ? String(int(round(_winds[i]))) + lang.ms() : "--";
            _printText(i * 106 + 31, 224, 44, 15, wnd, FONT1, CENTER, TEXT_COLOR);
            _prevWinds[i] = _winds[i];
        }
    }
}