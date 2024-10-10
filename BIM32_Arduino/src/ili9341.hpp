#include <TFT_eSPI.h> // v2.5.34 https://github.com/Bodmer/TFT_eSPI
TFT_eSPI tft = TFT_eSPI();
#include <JPEGDecoder.h> // v2.0.0 https://github.com/Bodmer/JPEGDecoder

#define minimum(a,b)     (((a) < (b)) ? (a) : (b))


class ILI9341 : LcdDisplay {
    #define LEFT              0
    #define CENTER            1
    #define RIGHT             2

    #define FONT1             0
    #define FONT2             1
    #define FONT3             2
    #define FONT_SEGMENTS_BIG 3
    #define FONT_SEGMENTS_SML 4
    #define FONT_SMALL        5
    #define FONT_TINY         6

    #define BG_COLOR          0x0000 // black
    #define FRAME_COLOR       0x001F // blue
    #define TEXT_COLOR        0xFFFF // white
    #define TEMPERATURE_COLOR 0xFFE0 // yellow
    #define TEMP_MIN_COLOR    0xFBA0 // orange
    #define HUMIDITY_COLOR    0x07FF // cyan
    #define PRESSURE_COLOR    0xF81F // magenta
    #define CLOCK_COLOR       0x07E0 // green
    #define BATTERY_COLOR     0x07E0 // green
    #define BATTERY_LOW_COLOR 0xF800 // red
    #define ANTENNA_COLOR     0x03EF // dark cyan
    #define WEEKDAY_COLOR     0xFFE0 // yellow
    #define WEEKEND_COLOR     0xF800 // red
    #define MONTH_COLOR       0xF400 // orange
    #define TODAY_COLOR       0x001E // blue
    #define TODAY_BG_COLOR    0x07DE // light blue
    #define FRAME1_COLOR      0x73AE // light gray
    #define GROUND_COLOR      0x31A6 // dark gray
    #define GRID_COLOR        0x4a69 // gray

    #define PAGE_MAIN         0
    #define PAGE_BIG_CLOCK    1
    #define PAGE_SMALL_CLOCK  2
    #define PAGE_NETWORK      3
    #define PAGE_CALENDAR     4
    #define PAGE_HOURLY       5
    #define PAGE_HISTORY_OUT  6
    #define PAGE_HISTORY_IN   7
    #define PAGE_ALARM        8

    #define HOURLY      0
    #define HISTORY_OUT 1
    #define HISTORY_IN  2

    public:
        void init();
        void showLogo();
        void showHomeScreen();
        void refresh();
        void brightness(unsigned int bright);
        void displayToggle();
        void displayOn();
        void displayOff();
        bool isDisplayOn();
        void getTouch();

    private:
        uint8_t _prevFont = 5;
        unsigned int _prevBright = 40400;
        uint16_t _air_color[4] = { BATTERY_COLOR, 0xFFE0, 0xFD20, 0xF800 };
        uint32_t _sequenceMillis = 0;
        uint8_t _sequenceSlot = 0;
        uint16_t _touchX = 0; 
        uint16_t _touchY = 0;
        uint8_t _page = PAGE_MAIN;
        time_t _pageSwitchedTime = 0;
        bool _networkSkeleton = false;
        bool _bigClockSkeleton = false;
        bool _smallClockSkeleton = false;
        bool _calendarSkeleton = false;
        bool _hourlySkeleton = false;
        bool _historyInSkeleton = false;
        bool _historyOutSkeleton = false;
        bool _alarmSkeleton = false;
        int _calendarShiftSeconds = 0;
        int8_t _calendarShiftDirection = 0;
        float _hrTemp[8]; 
        float _hrHum[8]; 
        float _hrPres[8];
        float _hrPrec[8];
        time_t _hrDate[8];


        void _sequenceSlotSkip();
        void _sequenceSlotNext();
        void _showImg(uint16_t x, uint16_t y, const byte img[], uint16_t size);
        const byte* _number_picture(uint8_t num);
        uint16_t _number_picture_size(uint8_t num);
        void _printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color);
        void _printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color, uint16_t bgColor);
        void _printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color, uint16_t bgColor, bool valign);
        void _showTemperature(int temp, uint16_t x, uint16_t y, uint8_t font, uint16_t color);
        void _showHumidity(int hum, uint16_t x, uint16_t y);
        void _showTime();
        void _clockPoints();
        void _showWeekday();
        void _showWeekdays();
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
        void _showAlarmIcon();
        void _showForecastIcons();
        void _showForecastTemps();
        void _showForecastWinds();
        void _closeButton();
        void _leftButton(bool show);
        void _rightButton(bool show);
        void _dateWeekday();
        void _networkPage();
        void _bigClockPage();
        void _smallClockPage();
        bool _isLeapYear(unsigned int year);
        uint8_t _numberOfDaysInMonth(uint8_t month, uint16_t year);
        void _calendarPage();
        void _historyInPage();
        void _historyOutPage();
        void _alarmPage();
        void _hourlyPage();
        void _hourlyTemp(uint8_t num, uint16_t y);
        void _hourlyHum(uint8_t num, uint16_t y);
        void _hourlyPres(uint8_t num, uint16_t y);
        void _hourlyIcon(uint8_t num, uint16_t y);
        void _hourlyWeekday(uint8_t num, uint16_t y);
        void _hourlyDate(uint8_t num, uint16_t y);
        void _hourlyTime(uint8_t num, uint16_t y);
        void _hourlyWindSpeed(uint8_t num, uint16_t y);
        void _hourlyWindDir(uint8_t num, uint16_t y);
        void _hourlyPrec(uint8_t num, uint16_t y);
        void _displayLcdHourlyCharts(uint8_t type);
        void _displayLcdHistoryTitle(String title);
        void _touch_calibrate();
};

/**
 * Display and fonts initialisation
 */
void ILI9341::init(void) {
    tft.begin();
    tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.setTextWrap(false, false);

    pinMode(TFT_BACKLIGHT, OUTPUT);
    brightness(1023);

    uint16_t calData[5];
    bool calDataValid = false;
    for(uint8_t i=0; i<5; i++) {
        calData[i] = config.calData(i);
        if(calData[i]) calDataValid = true;
    }
    if(calDataValid) tft.setTouch(calData);
}

void ILI9341::showLogo() {
    _showImg(0, 0, logo, sizeof(logo));
    vTaskDelay(2000);
}

void ILI9341::showHomeScreen() {
    tft.fillScreen(BG_COLOR);
    _drawSkeleton();
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

        if(_page == PAGE_MAIN) {
            _sequenceSlotSkip();
            _showTemperatureInside();
            _showTemperatureOutside();
            _showHumidityInside();
            _showHumidityOutside();
            _showPressure();
            _showWeekday();
            _showForecastTemps();
            _showDescription();
            _showWeekdays();
            _showForecastWinds();
            _showVoltageOrPercentage();
            _showComfort();
            _showWindSpeed();
            _showUpdTime();
            _showTime();
            _clockPoints();
            _showBatteryLevel();
            _showAntenna();
            _showWeatherIcon();
            _showWindDirection();
            _showAlarmIcon();
            _showForecastIcons();
            _sequenceSlotNext();
        }
        if(_page == PAGE_NETWORK) _networkPage();
        if(_page == PAGE_BIG_CLOCK) _bigClockPage();
        if(_page == PAGE_SMALL_CLOCK) _smallClockPage();
        if(_page == PAGE_CALENDAR) _calendarPage();
        if(_page == PAGE_HOURLY) _hourlyPage();
        if(_page == PAGE_HISTORY_IN) _historyInPage();
        if(_page == PAGE_HISTORY_OUT) _historyOutPage();
        if(_page == PAGE_ALARM) _alarmPage();
    }
}

void ILI9341::brightness(unsigned int bright) {
    if(_power) {
        uint8_t brgt = global.reduc[0] ? round(bright / 2) : bright;
        float r = 100 * log10(2) / log10(255);
        uint16_t br = round(pow(2, (brgt / r)));
        if(br < 1023) analogWrite(TFT_BACKLIGHT, br);
        _prevBright = bright;
    }
    else analogWrite(TFT_BACKLIGHT, 0);
}

void ILI9341::_showImg(uint16_t x, uint16_t y, const byte img[], uint16_t size) {
    JpegDec.decodeArray(img, size);
    uint16_t *pImg;
    uint16_t mcu_w = JpegDec.MCUWidth;
    uint16_t mcu_h = JpegDec.MCUHeight;
    uint32_t max_x = JpegDec.width;
    uint32_t max_y = JpegDec.height;
    uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
    uint32_t min_h = minimum(mcu_h, max_y % mcu_h);
    uint32_t win_w = mcu_w;
    uint32_t win_h = mcu_h;
    max_x += x;
    max_y += y;

    while(JpegDec.readSwappedBytes()) {
        pImg = JpegDec.pImage;
        int mcu_x = JpegDec.MCUx * mcu_w + x;
        int mcu_y = JpegDec.MCUy * mcu_h + y;
        if(mcu_x + mcu_w <= max_x) win_w = mcu_w;
        else win_w = min_w;
        if(mcu_y + mcu_h <= max_y) win_h = mcu_h;
        else win_h = min_h;
        if(win_w != mcu_w) {
            uint16_t *cImg;
            int p = 0;
            cImg = pImg + win_w;
            for(int h=1; h<win_h; h++) {
                p += mcu_w;
                for(int w=0; w<win_w; w++) {
                    *cImg = *(pImg + w + p);
                    cImg++;
                }
            }
        }

        if((mcu_x + win_w) <= tft.width() && (mcu_y + win_h) <= tft.height()) {
            tft.pushRect(mcu_x, mcu_y, win_w, win_h, pImg);
        }
        else if((mcu_y + win_h) >= tft.height()) JpegDec.abort();
    }
}

void ILI9341::_printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color) {
    _printText(x, y, width, height, text, font, align, color, BG_COLOR, false);
}

void ILI9341::_printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color, uint16_t bgColor) {
    _printText(x, y, width,  height, text, font, align, color, bgColor, false);
}

void ILI9341::_printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color, uint16_t bgColor, bool valign) {
    if(_prevFont != font) {
        if(font == FONT_TINY) tft.loadFont(Ubuntu_9);
        else if(font == FONT_SMALL) tft.loadFont(Ubuntu_11);
        else if(font == FONT1) tft.loadFont(Ubuntu_14);
        else if(font == FONT2) tft.loadFont(Ubuntu_21);
        else if(font == FONT3) tft.loadFont(Ubuntu_29);
        else if(font == FONT_SEGMENTS_SML) tft.loadFont(segment_96);
        else if(font == FONT_SEGMENTS_BIG) tft.loadFont(segment_140);
        _prevFont = font;
    }
    tft.fillRect(x, y, width, height, bgColor);
    tft.setTextColor(color, bgColor);
    if(align == CENTER or align == RIGHT) {
        uint16_t w = tft.textWidth(text);
        if(align == RIGHT) x += width - w - 4;
        else x += (width / 2) - (w / 2);
    }
    uint16_t h = tft.fontHeight();
    tft.setCursor(x, valign ? (y + height / 2 - h / 2) : y);
    tft.print(text);
}

/**
 * Display temperature
 */
void ILI9341::_showTemperature(int temp, uint16_t x, uint16_t y, uint8_t font, uint16_t color) {
    String buf = validate.temp(temp) ? (String(temp) + "°C") : "--";
    _printText(x, y, font == FONT3 ? 70 : 56, font == FONT3 ? 26 : 20, buf, font, CENTER, color);
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
    _showImg(145, 48, symb_home, sizeof(symb_home));
    _showImg(243, 48, symb_hum, sizeof(symb_hum));
    _showImg(62, 104, symb_temp_plus, sizeof(symb_temp_plus));
    _showImg(143, 109, symb_hum, sizeof(symb_hum));
    _showImg(222, 109, symb_pres, sizeof(symb_pres));
    _showImg(61, 146, symb_wind, sizeof(symb_wind));
    _showTime();
}

const byte* ILI9341::_number_picture(uint8_t num) {
    switch (num) {
        case 1: return number_1;
        case 2: return number_2;
        case 3: return number_3;
        case 4: return number_4;
        case 5: return number_5;
        case 6: return number_6;
        case 7: return number_7;
        case 8: return number_8;
        case 9: return number_9;
        default: return number_0;
    }
}

uint16_t ILI9341::_number_picture_size(uint8_t num) {
    switch (num) {
        case 1: return sizeof(number_1);
        case 2: return sizeof(number_2);
        case 3: return sizeof(number_3);
        case 4: return sizeof(number_4);
        case 5: return sizeof(number_5);
        case 6: return sizeof(number_6);
        case 7: return sizeof(number_7);
        case 8: return sizeof(number_8);
        case 9: return sizeof(number_9);
        default: return sizeof(number_0);
    }
}

void ILI9341::_showTime() {
    if(_prevTHour != _tHour || _forced) {
        if(_tHour < 10) tft.fillRect(0, 0, 32, 78, BG_COLOR);
        else _showImg(0, 0, _number_picture(_tHour / 10), _number_picture_size(_tHour / 10));
        _showImg(33, 0, _number_picture(_tHour % 10), _number_picture_size(_tHour % 10));
        _prevTHour = _tHour;
    }
    if(_prevTMinute != _tMinute || _forced) {
        _showImg(77, 0, _number_picture(_tMinute / 10), _number_picture_size(_tMinute / 10));
        _showImg(109, 0, _number_picture(_tMinute % 10), _number_picture_size(_tMinute % 10));
        _prevTMinute = _tMinute;
    }
}

void ILI9341::_showWeekday() {
    if(_prevTWeekday != _tWeekday || _forced) {
        _printText(146, 6, 40, 20, lang.weekdayShortName(_tWeekday), FONT2, LEFT, CLOCK_COLOR);
    }
}

void ILI9341::_showWeekdays() {
    if(_prevTWeekday != _tWeekday || _forced) {
        unsigned int wd = _tWeekday;
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
void ILI9341::_clockPoints() {
    boolean points = millis() % 1000 > 500;
    tft.fillSmoothCircle(70, 24, 2, points ? HUMIDITY_COLOR : BG_COLOR, BG_COLOR);
    tft.fillSmoothCircle(70, 52, 2, points ? HUMIDITY_COLOR : BG_COLOR, BG_COLOR);
}

/**
 * Display antenna symbol
 */
void ILI9341::_showAntenna() {
    if(_prevRssi != _rssi || _prevIsApMode != _isApMode || _forced) {
        if(_isApMode) _showImg(292, 1, ant_acpoint, sizeof(ant_acpoint));
        else {
            if(_rssi > -51) _showImg(292, 1, ant4, sizeof(ant4));
            if(_rssi < -50 && _rssi > -76) _showImg(292, 1, ant3, sizeof(ant3));
            if(_rssi <- 75 && _rssi > -96) _showImg(292, 1, ant2, sizeof(ant2));
            if(_rssi < -95) _showImg(292, 1, ant1, sizeof(ant1));
            if(_rssi >= 0) _showImg(292, 1, ant0, sizeof(ant0));
        }
        _prevRssi = _rssi;
        _prevIsApMode = _isApMode;
    }
}

void ILI9341::_sequenceSlotSkip() {
    for(uint8_t i=0; i<4; i++) {
        if(config.display_source_sequence_name(_sequenceSlot) == "") {
            if(_sequenceSlot < 3) _sequenceSlot++;
            else _sequenceSlot = 0;
        }
        else i = 4;
    }
}

void ILI9341::_sequenceSlotNext() {
    if(millis() - _sequenceMillis > config.display_source_sequence_dur() * 1000) {
        _sequenceMillis = millis();
        if(_sequenceSlot < 3) _sequenceSlot++;
        else _sequenceSlot = 0;
    }
}

/**
 * Display temperature inside
 */
void ILI9341::_showTemperatureInside() {
    if(config.display_source_tempIn_sens() == 4) _tempIn = _tempSequence[_sequenceSlot];
    if(_prevTempIn != _tempIn || _forced) {
        _showTemperature(int(round(_tempIn)), 173, 53, FONT3, TEMPERATURE_COLOR);
        _prevTempIn = _tempIn;
    }
}

/**
 * Display temperature outside
 */
void ILI9341::_showTemperatureOutside() {
    if(_prevTempOut != _tempOut || _forced) {
        _showThermometer();
        _showTemperature(int(round(_tempOut)), 71, 113, FONT3, TEMPERATURE_COLOR);
        _prevTempOut = _tempOut;
    }
}

/**
 * Display thermometer icon (red or blue)
 */
void ILI9341::_showThermometer() {
    if(_tempOut < 0.0) _showImg(62, 104, symb_temp_minus, sizeof(symb_temp_minus));
    else _showImg(62, 104, symb_temp_plus, sizeof(symb_temp_plus));
}

/**
 * Display humidity inside
 */
void ILI9341::_showHumidityInside() {
    if(config.display_source_humIn_sens() == 4) _humIn = _humSequence[_sequenceSlot];
    if(_prevHumIn != _humIn || _forced) {
        _showHumidity(int(round(_humIn)), 264, 58);
        _prevHumIn = _humIn;
    }
}

/**
 * Display humidity outside
 */
void ILI9341::_showHumidityOutside() {
    if(_prevHumOut != _humOut || _forced) {
        _showHumidity(int(round(_humOut)), 164, 119);
        _prevHumOut = _humOut;
    }
}

/**
 * Display comfort level
 */
void ILI9341::_showComfort() {
    if(config.display_source_descr() == 2) _comfort = _nameSequence[_sequenceSlot];
    if(_prevComfort != _comfort || _forced) {
        _printText(145, 28, 175, 16, _comfort, FONT1, CENTER, TEXT_COLOR);
        _prevComfort = _comfort;
    }
}

/**
 * Display battery symbol
 */
void ILI9341::_showBatteryLevel() {
    if(_prevBatLevel != _batLevel || _forced) {
        if(validate.batLvl(_batLevel)) {
            switch(_batLevel) {
                case 1: _showImg(258, 2, bat1, sizeof(bat1)); break;
                case 2: _showImg(258, 2, bat2, sizeof(bat2)); break;
                case 3: _showImg(258, 2, bat3, sizeof(bat3)); break;
                case 4: _showImg(258, 2, bat4, sizeof(bat4)); break;
                default: tft.fillRect(258, 2, 32, 21, BG_COLOR); break;
            }
        }
        else tft.fillRect(258, 2, 32, 21, BG_COLOR);
        _prevBatLevel = _batLevel;
    }
}

/**
 * Display voltage, percentage, CO2 or IAQ
 */
void ILI9341::_showVoltageOrPercentage() {
    if(_prevVolt != _volt || _prevVoltColor != _voltColor || _forced) {
        _printText(198, 10, 58, 16, _volt, FONT1, RIGHT, _air_color[_voltColor]);
        _prevVolt = _volt;
        _prevVoltColor = _voltColor;
    }
}

/**
 * Display current weather icon
 */
void ILI9341::_showWeatherIcon() {
    if(_prevCurrIcon != _currIcon || _prevIsDay != _isDay || _forced) {
        switch(_currIcon) {
            case 1: _showImg(0, 104, _isDay ? icon_big_01_d : icon_big_01_n, _isDay ? sizeof(icon_big_01_d) : sizeof(icon_big_01_n)); break;
            case 2: _showImg(0, 104, _isDay ? icon_big_02_d : icon_big_02_n, _isDay ? sizeof(icon_big_02_d) : sizeof(icon_big_02_n)); break;
            case 3: _showImg(0, 104, icon_big_04, sizeof(icon_big_04)); break;
            case 4: _showImg(0, 104, icon_big_09, sizeof(icon_big_09)); break;
            case 5: _showImg(0, 104, icon_big_10, sizeof(icon_big_10)); break;
            case 6: _showImg(0, 104, _isDay ? icon_big_11_d : icon_big_11_n, _isDay ? sizeof(icon_big_11_d) : sizeof(icon_big_11_n)); break;
            case 7: _showImg(0, 104, icon_big_13, sizeof(icon_big_13)); break;
            case 8: _showImg(0, 104, icon_big_50, sizeof(icon_big_50)); break;
            default: _showImg(0, 104, icon_big_loading, sizeof(icon_big_loading)); break;
        }
        _prevCurrIcon = _currIcon;
        _prevIsDay = _isDay;
    }
}

/**
 * Display weather description
 */
void ILI9341::_showDescription() {
    if(_prevDescription != _description || _forced) {
        tft.loadFont(Ubuntu_21);
        uint16_t w = tft.textWidth(_description);
        tft.unloadFont();
        _prevFont = 5;
        _printText(0, 84, 319, 20, _description, w > 319 ? FONT1 : FONT2, CENTER, TEXT_COLOR);
        _prevDescription = _description;
    }
}

/**
 * Display pressure
 */
void ILI9341::_showPressure() {
    if(_prevPresOut != _presOut || _forced) {
        String buf = validate.pres(_presOut) ? (String(int(round(_presOut * 0.75))) + lang.mm()) : "--";
        _printText(250, 119, 70, 20, buf, FONT2, CENTER, PRESSURE_COLOR);
        _prevPresOut = _presOut;
    }
}

/**
 * Display wind speed
 */
void ILI9341::_showWindSpeed() {
    if(_prevWindSpd != _windSpd || _forced) {
        String wnd = validate.wind(_windSpd) ? String(int(round(_windSpd))) + lang.ms() : "--";
        _printText(93, 146, 40, 16, wnd, FONT1, CENTER, TEXT_COLOR);
        _prevWindSpd = _windSpd;
    }
}

/**
 * Display wind direction
 */
void ILI9341::_showWindDirection() {
    if(_prevWindDir != _windDir || _forced) {
        switch(_windDir) {
            case 1: _showImg(133, 143, wind_north_east, sizeof(wind_north_east)); break;
            case 2: _showImg(133, 143, wind_east, sizeof(wind_east)); break;
            case 3: _showImg(133, 143, wind_south_east, sizeof(wind_south_east)); break;
            case 4: _showImg(133, 143, wind_south, sizeof(wind_south)); break;
            case 5: _showImg(133, 143, wind_south_west, sizeof(wind_south_west)); break;
            case 6: _showImg(133, 143, wind_west, sizeof(wind_west)); break;
            case 7: _showImg(133, 143, wind_north_west, sizeof(wind_north_west)); break;
            default: _showImg(133, 143, wind_north, sizeof(wind_north)); break;
        }
        _prevWindDir = _windDir;
    }
}

/**
 * Display the time and date of the last weather update
 */
void ILI9341::_showUpdTime() {
    if(_prevWeatherUpdated != _weatherUpdated || _forced) {
        time_t t = _weatherUpdated;
        char buf[32] = "";
        sprintf(buf, "%02d.%02d.%d %02d:%02d", day(t), month(t), year(t), hour(t), minute(t));
        _printText(176, 148, 117, 16, t > 0 ? buf : " ", FONT1, LEFT, TEXT_COLOR);
        if(t > 0) {
            tft.drawCircle(167, 153, 5, TEXT_COLOR);
            tft.drawFastHLine(166, 148, 4, BG_COLOR);
            tft.drawFastHLine(162, 148, 3, TEXT_COLOR);
            tft.drawFastVLine(165, 149, 3, TEXT_COLOR);
        }
        _prevWeatherUpdated = _weatherUpdated;
    }
}

/**
 * Display Alarm icon
 */
void ILI9341::_showAlarmIcon() {
    uint8_t alarmOn = 0;
    for(uint8_t i=0; i<12; i++) {
        alarmOn |= config.alarm_state(i);
    }
    if(_prevAlarmOn != alarmOn || _forced) {
        if(alarmOn) _showImg(294, 140, symb_alarm, sizeof(symb_alarm));
        else _showImg(294, 140, symb_alarm_off, sizeof(symb_alarm_off));
        _prevAlarmOn = alarmOn;
    }
}

/**
 * Display daily forecast icons
 */
void ILI9341::_showForecastIcons() {
    for(uint8_t i=0; i<3; i++) {
        if(_prevIcons[i] != _icons[i] || _forced) {
            switch(_icons[i]) {
                case 1: _showImg(i * 106 + 7, 183, icon_small_01, sizeof(icon_small_01)); break;
                case 2: _showImg(i * 106 + 7, 183, icon_small_02, sizeof(icon_small_02)); break;
                case 3: _showImg(i * 106 + 7, 183, icon_small_04, sizeof(icon_small_04)); break;
                case 4: _showImg(i * 106 + 7, 183, icon_small_09, sizeof(icon_small_09)); break;
                case 5: _showImg(i * 106 + 7, 183, icon_small_10, sizeof(icon_small_10)); break;
                case 6: _showImg(i * 106 + 7, 183, icon_small_11, sizeof(icon_small_11)); break;
                case 7: _showImg(i * 106 + 7, 183, icon_small_13, sizeof(icon_small_13)); break;
                case 8: _showImg(i * 106 + 7, 183, icon_small_50, sizeof(icon_small_50)); break;
                default: _showImg(i * 106 + 7, 183, icon_small_loading, sizeof(icon_small_loading)); break;
            }
            _prevIcons[i] = _icons[i];
        }
    }
    _forced = false;
}

/**
 * Display daily forecast temperatures
 */
void ILI9341::_showForecastTemps() {
    for(uint8_t i=0; i<3; i++) {
        if(_prevDTemps[i] != _dTemps[i] || _forced) {
            _showTemperature(int(round(_dTemps[i])), i * 106 + 49, 183, FONT2, TEMPERATURE_COLOR);
            _prevDTemps[i] = _dTemps[i];
        }

        if(_prevNTemps[i] != _nTemps[i] || _forced) {
            _showTemperature(int(round(_nTemps[i])), i * 106 + 49, 203, FONT2, TEMP_MIN_COLOR);
            _prevNTemps[i] = _nTemps[i];
        }
    }
}

/**
 * Display daily forecast winds
 */
void ILI9341::_showForecastWinds() {
    for(uint8_t i=0; i<3; i++) {
        if(_prevWinds[i] != _winds[i] || _forced) {
            String wnd = validate.wind(_winds[i]) ? String(int(round(_winds[i]))) + lang.ms() : "--";
            _printText(i * 106 + 31, 224, 44, 15, wnd, FONT1, CENTER, TEXT_COLOR);
            _prevWinds[i] = _winds[i];
        }
    }
}

/**
 * Close button
 */
void ILI9341::_closeButton() {
    _showImg(289, 0, symb_close, sizeof(symb_close));
}

/**
 * Left button
 */
void ILI9341::_leftButton(bool show) {
    if(show) _showImg(0, 106, symb_left, sizeof(symb_left));
    else tft.fillRect(0, 106, 24, 24, BG_COLOR);
}

/**
 * Right button
 */
void ILI9341::_rightButton(bool show) {
    if(show) _showImg(295, 106, symb_right, sizeof(symb_right));
    else tft.fillRect(295, 106, 24, 24, BG_COLOR);
}

void ILI9341::_dateWeekday() {
    if(_prevTWeekday != weekday() || _forced) {
        tft.fillRect(29, 0, 261, 36, BG_COLOR);
        _printText(30, 8, 259, 30, lang.weekdayFullName(weekday()), FONT3, CENTER, TEMPERATURE_COLOR);
        _prevTWeekday = weekday();
    }
    if(_prevTDay != day() || _prevTMonth != month() || _prevTYear != year() || _forced) {
        String buf = "";
        if(config.lang() == "en") buf = lang.monthDay(month()) + ", " + String(day()) + " " +  String(year());
        else if(config.lang() == "de") buf = String(day()) + ". " + lang.monthDay(month()) + " " + String(year());
        else buf = String(day()) + " " + lang.monthDay(month()) + " " + String(year());
        tft.fillRect(0, 198, 319, 40, BG_COLOR);
        _printText(0, 200, 319, 30, buf, FONT3, CENTER, TEMP_MIN_COLOR);
        _prevTDay = day();
        _prevTMonth = month();
        _prevTYear = year();
    }
}

/**
 * Display Netoerk page
 */
void ILI9341::_networkPage() {
    const uint8_t x = 160, y = 2, sl = 6, w = 150;
    const uint8_t sr = x + sl;

    if(!_networkSkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _networkSkeleton = true;
        _forced = true;
        tft.fillRect(48, 10, 226, 39, FRAME1_COLOR);
        tft.fillRect(50, 12, 222, 35, GROUND_COLOR);
        _printText(50, 17, 222, 28, "WiFi", FONT3, CENTER, TEXT_COLOR, GROUND_COLOR);

        for(uint8_t i=0; i<2; i++) {
            for(uint8_t k=0; k<6; k++) {
                tft.fillRect(i * x + 4, k * 30 + 60, 154, 24, FRAME1_COLOR);
                tft.fillRect(i * x + sl, k * 30 + 62, w, 20, GROUND_COLOR);
            }
        }
        _printText(sl, 64 + y, w, 12, lang.network(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
        _printText(sl, 94 + y, w, 12, lang.signalLevel(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
        _printText(sl, 124 + y, w, 12, lang.ipAddr(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
        _printText(sl, 154 + y, w, 12, lang.macAddr(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
        _printText(sl, 185 + y, w, 12, lang.esp32Temp(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
        _printText(sl, 214 + y, w, 12, lang.firmware(), FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    }

    String ssid = global.apMode ? config.accessPoint_ssid() : WiFi.SSID();;
    String rssi = global.apMode ? "100%" : String(_rssi) + "dBm";
    String ip = global.apMode ? config.accessPoint_ip() : WiFi.localIP().toString();;
    String mac = global.apMode ? WiFi.softAPmacAddress() : WiFi.macAddress();
    float esp32Temp = sensors.get_esp32_temp(RAW);
    String fw = FW;

    if(_prevNetSsid != ssid || _forced) 
        _printText(sr, 64 + y, w, 12, ssid, FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    if(_prevNetRssi != rssi || _forced) 
        _printText(sr, 94 + y, w, 12, rssi, FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    if(_prevNetIp != ip || _forced) 
        _printText(sr, 124 + y, w, 12, ip, FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    if(_prevNetMac != mac || _forced) 
        _printText(sr, 154 + y, w, 12, mac, FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    if(_prevNetTemp != esp32Temp || _forced) 
        _printText(sr, 184 + y, w, 12, String((int)round(esp32Temp)) + "°C", FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);
    if(_prevNetFw != fw || _forced) 
        _printText(sr, 214 + y, w, 12, fw, FONT1, CENTER, TEXT_COLOR, GROUND_COLOR);

    _prevNetSsid = ssid;
    _prevNetRssi = rssi;
    _prevNetIp = ip;
    _prevNetMac = mac;
    _prevNetTemp = esp32Temp;
    _prevNetFw = fw;
    _forced = false;
}

/**
 * Display Big Clock page
 */
void ILI9341::_bigClockPage() {
    if(!_bigClockSkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _bigClockSkeleton = true;
        _forced = true;
    }
    if(_prevTHour != hour() || _forced) {
        tft.fillRect(0, 66, 160, 124, BG_COLOR);
        _printText(0, 68, 155, 120, String(hour()), FONT_SEGMENTS_BIG, RIGHT, CLOCK_COLOR);
        _prevTHour = hour();
    }

    boolean points = millis() % 1000 > 500;
    tft.fillSmoothCircle(164, 98, points ? 6 : 7, points ? CLOCK_COLOR : BG_COLOR, BG_COLOR);
    tft.fillSmoothCircle(162, 144, points ? 6 : 7, points ? CLOCK_COLOR : BG_COLOR, BG_COLOR);

    if(_prevTMinute != minute() || _forced) {
        char buf[3];
        sprintf(buf, "%02d", minute());
        tft.fillRect(172, 66, 143, 122, BG_COLOR);
        _printText(174, 68, 141, 120, String(buf), FONT_SEGMENTS_BIG, LEFT, CLOCK_COLOR);
        _prevTMinute = minute();
    }
    _dateWeekday();
    _forced = false;
}

/**
 * Display Small Clock page
 */
void ILI9341::_smallClockPage() {
    if(!_smallClockSkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _smallClockSkeleton = true;
        _forced = true;
    }
    if(_prevTHour != hour() || _forced) {
        tft.fillRect(1, 80, 106, 2, BG_COLOR);
        _printText(1, 82, 106, 96, String(hour()), FONT_SEGMENTS_SML, RIGHT, CLOCK_COLOR);
        _prevTHour = hour();
    }
    if(_prevTMinute != minute() || _forced) {
        char buf[3];
        sprintf(buf, "%02d", minute());
        tft.fillRect(108, 80, 106, 2, BG_COLOR);
        _printText(108, 82, 106, 96, String(buf), FONT_SEGMENTS_SML, CENTER, CLOCK_COLOR);
        _prevTMinute = minute();
    }
    if(_prevTSecond != second() || _forced) {
        char buf[3];
        sprintf(buf, "%02d", second());
        tft.fillRect(219, 80, 106, 2, BG_COLOR);
        _printText(219, 82, 106, 96, String(buf), FONT_SEGMENTS_SML, LEFT, CLOCK_COLOR);
        tft.fillSmoothCircle(112, 100, 3, CLOCK_COLOR, BG_COLOR);
        tft.fillSmoothCircle(110, 134, 3, CLOCK_COLOR, BG_COLOR);
        tft.fillSmoothCircle(215, 100, 3, CLOCK_COLOR, BG_COLOR);
        tft.fillSmoothCircle(213, 134, 3, CLOCK_COLOR, BG_COLOR);
        _prevTSecond = second();
    }
    _dateWeekday();
    _forced = false;
}

bool ILI9341::_isLeapYear(unsigned int year) {
	return ((!(year % 4) && (year % 100)) || !(year % 400));
}

uint8_t ILI9341::_numberOfDaysInMonth(uint8_t month, uint16_t year) {
    uint8_t daysInMonth = 0;
    const uint8_t months[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	daysInMonth = month == 2 && _isLeapYear(year) ? 29 : months[month];
    return daysInMonth;
}

void ILI9341::_calendarPage() {
    if(!_calendarSkeleton) {
        tft.fillScreen(TFT_BLACK);
        for(uint8_t i=1; i<=7; i++) {
            String wd = i == 7 ? lang.weekdayShortName(1) : lang.weekdayShortName(i + 1);
            _printText(i * 36, 40, 36, 26, wd, FONT2, CENTER, i < 7 ? WEEKDAY_COLOR : WEEKEND_COLOR);
        }
        _closeButton();
        _leftButton(true);
        _rightButton(true);
        _calendarSkeleton = true;
        _prevCalendarShiftSeconds = -1;
    }

    time_t shiftSeconds = now() + _calendarShiftSeconds;
    if(_calendarShiftDirection > 0) {
        uint8_t daysInMonth = _numberOfDaysInMonth(month(shiftSeconds), year(shiftSeconds)); 
        time_t secondsUntilNextMonth = (daysInMonth - day(shiftSeconds)) * SECS_PER_DAY + SECS_PER_DAY;
        _calendarShiftSeconds += secondsUntilNextMonth;
    }
    if(_calendarShiftDirection < 0) { 
        time_t secondsUntilPrevMonth =  day(shiftSeconds) * SECS_PER_DAY + SECS_PER_DAY;
        _calendarShiftSeconds -= secondsUntilPrevMonth;
    }
    shiftSeconds = now() + _calendarShiftSeconds;

    int8_t firstWeekday = 0;
    for(uint8_t i=0; i<31; i++) {
        if(day(shiftSeconds - (i * SECS_PER_DAY)) == 1) {
            firstWeekday = weekday(shiftSeconds - (i * SECS_PER_DAY)) - 1;
            break;
        }
    }
    firstWeekday -= 1;
    if(firstWeekday < 0) firstWeekday = 6;
    uint8_t daysInMonth = _numberOfDaysInMonth(month(shiftSeconds), year(shiftSeconds));

    if(_prevCalendarShiftSeconds != _calendarShiftSeconds || _prevTDay != day()) {
        String mon = lang.monthFullName(month(shiftSeconds)) + " " + String(year(shiftSeconds));
        _printText(30, 8, 260, 20, mon, FONT2, CENTER, MONTH_COLOR, BG_COLOR);
        uint8_t cday = 1;
        bool clndRun = false;
        for(uint8_t w=0; w<6; w++) {
            for(uint8_t d=0; d<7; d++) {
                bool today = day() == cday && month() == month(shiftSeconds) && year() == year(shiftSeconds);
                uint16_t tdColor = today ? TODAY_COLOR : (d == 6 ? WEEKEND_COLOR : TEXT_COLOR);
                uint16_t bgColor = today ? TODAY_BG_COLOR : BG_COLOR;
                if(firstWeekday == d) clndRun = true;
                if(cday > daysInMonth) clndRun = false;
                if(clndRun) {
                    _printText(d * 36 + 36, w * 28 + 64, 32, 28, String(cday), FONT2, CENTER, tdColor, bgColor, true);
                    cday++;
                }
                else _printText(d * 36 + 36, w * 28 + 64, 32, 28, "  ", FONT2, CENTER, TEXT_COLOR);
            }
        }
    }
    _prevCalendarShiftSeconds = _calendarShiftSeconds;
    _prevTDay = day();
    _calendarShiftDirection = 0;
}

void ILI9341::_hourlyPage() {
    if(!_hourlySkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _hourlySkeleton = true;
        _forced = true;
    }

    if(_prevHourlyChecksum != _hourlyChecksum || _prevHourlyShift != _hourlyShift || _forced) {
        for(uint8_t i=0; i<8; i++) {
            _hrTemp[i] = weather.get_hourlyTemp(i + _hourlyShift);
            _hourlyTemp(i, 86);
        }
        for(uint8_t i=0; i<8; i++) _hourlyIcon(i, 116);
        for(uint8_t i=0; i<8; i++) {
            _hrDate[i] = weather.get_hourlyDate(i + _hourlyShift);
            _hourlyWeekday(i, 156);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrPres[i] = weather.get_hourlyPres(i + _hourlyShift);
            _hourlyPres(i, 102);
        }
        for(uint8_t i=0; i<8; i++) {
            _hourlyDate(i, 176);
            _hourlyTime(i, 190);
        }
        for(uint8_t i=0; i<8; i++) _hourlyWindSpeed(i, 204);
        for(uint8_t i=0; i<8; i++) _hourlyWindDir(i, 216);
        for(uint8_t i=0; i<8; i++) _hourlyPrec(i, 230);
        _displayLcdHourlyCharts(HOURLY);
        _rightButton(_hourlyShift < 32);
        _leftButton(_hourlyShift > 0);
        _prevHourlyChecksum = _hourlyChecksum;
        _prevHourlyShift = _hourlyShift;
    }

    _forced = false;
}

void ILI9341::_hourlyTemp(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 16, BG_COLOR);
    char buf[10];
    if(validate.temp(_hrTemp[num])) sprintf(buf, "%.1f°", _hrTemp[num]);
    else sprintf(buf, "--°");
    _printText(num * 32 + 30, y, 32, 12, String(buf), FONT_SMALL, CENTER, TEMPERATURE_COLOR);
}

void ILI9341::_hourlyHum(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    String h = validate.hum(_hrHum[num]) ? String((int)round(_hrHum[num])) : "--";
    _printText(num * 32 + 30, y, 32, 12, h + "%", FONT_TINY, CENTER, HUMIDITY_COLOR);
}

void ILI9341::_hourlyPres(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    String p = validate.pres(_hrPres[num]) ? String((int)round(_hrPres[num] * 0.75)) : "--";
    _printText(num * 32 + 30, y, 32, 12, p + lang.mm(), FONT_TINY, CENTER, PRESSURE_COLOR);
}

void ILI9341::_hourlyIcon(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 32, BG_COLOR);
    uint16_t x = num * 32 + 30;
    switch(weather.get_hourlyIcon(num + _hourlyShift)) {
        case 1: _showImg(x, y, icon_tiny_01, sizeof(icon_tiny_01)); break;
        case 2: _showImg(x, y, icon_tiny_02, sizeof(icon_tiny_02)); break;
        case 3: _showImg(x, y, icon_tiny_02, sizeof(icon_tiny_02)); break;
        case 4: _showImg(x, y, icon_tiny_04, sizeof(icon_tiny_04)); break;
        case 9: _showImg(x, y, icon_tiny_09, sizeof(icon_tiny_09)); break;
        case 10: _showImg(x, y, icon_tiny_10, sizeof(icon_tiny_10)); break;
        case 11: _showImg(x, y, icon_tiny_11, sizeof(icon_tiny_11)); break;
        case 13: _showImg(x, y, icon_tiny_13, sizeof(icon_tiny_13)); break;
        case 50: _showImg(x, y, icon_tiny_50, sizeof(icon_tiny_50)); break;
        default: _showImg(x, y, icon_tiny_loading, sizeof(icon_tiny_loading)); break;
    }
}

void ILI9341::_hourlyWeekday(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 20, BG_COLOR);
    String wd = lang.weekdayShortName(weekday(_hrDate[num]));
    _printText(num * 32 + 30, y, 32, 18, wd, FONT1, CENTER, TEXT_COLOR);
}

void ILI9341::_hourlyDate(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    uint8_t dt = day(_hrDate[num]);
    String mo = lang.monthShortName(month(_hrDate[num]));
    _printText(num * 32 + 30, y, 32, 12, String(dt) + mo, FONT_TINY, CENTER, TEXT_COLOR);
}

void ILI9341::_hourlyTime(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    char buf[10];
    sprintf(buf, "%d:%02d", hour(_hrDate[num]), minute(_hrDate[num]));
    _printText(num * 32 + 30, y, 32, 12, String(buf), FONT_TINY, CENTER, TEXT_COLOR);
}

void ILI9341::_hourlyWindSpeed(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    float wind = weather.get_hourlyWindSpeed(num + _hourlyShift);
    String ws = validate.wind(wind) ? String((int)round(wind)) : "--";
    _printText(num * 32 + 30, y, 32, 12, ws + lang.ms(), FONT_TINY, CENTER, TEXT_COLOR);
}

void ILI9341::_hourlyWindDir(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    uint16_t x = num * 32 + 30;
    int dir = weather.get_hourlyWindDir(num + _hourlyShift);
    if(dir >= 0 && dir <= 360) {
        if((dir >= 338 && dir <= 360) || (dir >= 0 && dir < 22)) _showImg(x + 10, y, wind_north_tiny, sizeof(wind_north_tiny));
        else if(dir >= 22 && dir < 67) _showImg(x + 10, y, wind_north_east_tiny, sizeof(wind_north_east_tiny));
        else if(dir >= 67 && dir < 112) _showImg(x + 10, y, wind_east_tiny, sizeof(wind_east_tiny));
        else if(dir >= 112 && dir < 157) _showImg(x + 10, y, wind_south_east_tiny, sizeof(wind_south_east_tiny));
        else if(dir >= 157 && dir < 202) _showImg(x + 10, y, wind_south_tiny, sizeof(wind_south_tiny));
        else if(dir >= 202 && dir < 247) _showImg(x + 10, y, wind_south_west_tiny, sizeof(wind_south_west_tiny));
        else if(dir >= 247 && dir < 292) _showImg(x + 10, y, wind_west_tiny, sizeof(wind_west_tiny));
        else if(dir >= 292 && dir < 338) _showImg(x + 10, y, wind_north_west_tiny, sizeof(wind_north_west_tiny));
    }
    else tft.fillRect(x + 12, y, 12, 12, BG_COLOR);
}

void ILI9341::_hourlyPrec(uint8_t num, uint16_t y) {
    uint16_t x = num * 32 + 30;
    _showImg(x + 2, y, symb_drop, sizeof(symb_drop));
    float prec = _hrPrec[num];
    String pr = "--";
    if(config.weather_provider() == 0) pr = prec ? String(prec) : "0" + lang.mm();
    if(config.weather_provider() == 2) pr = String((int)round(prec)) + '%';
    _printText(x + 11, y + 2, 21, 12, pr, FONT_TINY, CENTER, TEXT_COLOR);
}

void ILI9341::_displayLcdHourlyCharts(uint8_t type) {
    uint8_t step = 32, grid = 31;

    tft.fillRect(42, 0, grid * 8, 72, BG_COLOR);
    for(uint8_t i=0; i<10; i++) tft.drawFastHLine(42, i * 8, grid * 8 - 8, GRID_COLOR);
    for(uint8_t i=0; i<grid; i++) tft.drawFastVLine(i * 8 + 42, 0, 72, GRID_COLOR);

    for(uint8_t cht=0; cht<4; cht++) {
        float chartMin = 10000.0, chartMax = -10000.0, ch[8];

        for(uint8_t i=0; i<8; i++) {
            if(cht == 0) ch[i] = _hrTemp[i];
            if(cht == 1) ch[i] = _hrPres[i];
            if(cht == 2) ch[i] = _hrPrec[i];
            if(cht == 3) ch[i] = _hrHum[i];
        }
        for(uint8_t i=0; i<8; i++) {
            if(ch[i] < chartMin) chartMin = ch[i];
            if(ch[i] > chartMax) chartMax = ch[i];
        }

        float m = chartMax - chartMin;
        float k = m ? (64 / m) : 0;
        for(uint8_t i=0; i<8; i++) {
            ch[i] -= chartMin;
            if(k > 0) ch[i] *= k;
            else ch[i] /= 2;
            if(ch[i] > 72) ch[i] = 72;
        }

        for(uint8_t i=0; i<7; i++) {
            if(cht == 0) { // temperature
                tft.drawLine(i * step + 50, 67 - ch[i], i * step + step + 50, 67 - ch[i + 1], TEMPERATURE_COLOR);
            }
            if(cht == 1 && (type == HOURLY || type == HISTORY_OUT)) { // pressure
                tft.drawLine(i * step + 50, 69 - ch[i], i * step + step + 50, 69 - ch[i + 1], PRESSURE_COLOR);
            }
            if(cht == 2 && type == HOURLY) { // precipitation
                tft.drawLine(i * step + 50, 71 - ch[i], i * step + step + 50, 71 - ch[i + 1], HUMIDITY_COLOR);
            }
            if(cht == 3 && (type == HISTORY_OUT || type == HISTORY_IN)) { // humidity
                tft.drawLine(i * step + 50, 71 - ch[i], i * step + step + 50, 71 - ch[i + 1], HUMIDITY_COLOR);
            }
        }
    }    
}

void ILI9341::_displayLcdHistoryTitle(String title) {
    tft.drawFastHLine(38, 190, 247, FRAME1_COLOR);
    tft.drawFastVLine(38, 190, 26, FRAME1_COLOR);
    tft.drawFastHLine(39, 215, 246, TEXT_COLOR);
    tft.drawFastVLine(285, 190, 26, TEXT_COLOR);
    _printText(39, 196, 246, 16, title, FONT1, CENTER, TEXT_COLOR);
}

void ILI9341::_historyInPage() {
    if(!_historyInSkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _displayLcdHistoryTitle(lang.historyIn());
        _historyInSkeleton = true;
        _forced = true;
    }

    if(_prevHistoryInChecksum != _historyInChecksum || _prevHistoryInShift != _historyInShift || _forced) {
        for(uint8_t i=0; i<8; i++) {
            _hrTemp[i] = thingspeak.get_historyField(3, i + _historyInShift);
            _hourlyTemp(i, 86);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrDate[i] = thingspeak.get_historyUpdated(i + _historyInShift);
            _hourlyWeekday(i, 116);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrHum[i] = thingspeak.get_historyField(1, i + _historyInShift);
            _hourlyHum(i, 102);
        }
        for(uint8_t i=0; i<8; i++) {
            _hourlyDate(i, 136);
            _hourlyTime(i, 150);
        }
        _displayLcdHourlyCharts(HISTORY_IN);
        _rightButton(_historyInShift < 16);
        _leftButton(_historyInShift > 0);
        _prevHistoryInChecksum = _historyInChecksum;
        _prevHistoryInShift = _historyInShift;
    }

    _forced = false;
}

void ILI9341::_historyOutPage() {
    if(!_historyOutSkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _displayLcdHistoryTitle(lang.historyOut());
        _historyOutSkeleton = true;
        _forced = true;
    }

    if(_prevHistoryOutChecksum != _historyOutChecksum || _prevHistoryOutShift != _historyOutShift || _forced) {
        for(uint8_t i=0; i<8; i++) {
            _hrTemp[i] = thingspeak.get_historyField(0, i + _historyOutShift);
            _hourlyTemp(i, 86);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrDate[i] = thingspeak.get_historyUpdated(i + _historyInShift);
            _hourlyWeekday(i, 130);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrHum[i] = thingspeak.get_historyField(1, i + _historyOutShift);
            _hourlyHum(i, 102);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrPres[i] = thingspeak.get_historyField(2, i + _historyOutShift);
            _hourlyPres(i, 116);
        }
        for(uint8_t i=0; i<8; i++) {
            _hourlyDate(i, 150);
            _hourlyTime(i, 164);
        }
        _displayLcdHourlyCharts(HISTORY_OUT);
        _rightButton(_historyOutShift < 16);
        _leftButton(_historyOutShift > 0);
        _prevHistoryOutChecksum = _historyOutChecksum;
        _prevHistoryOutShift = _historyOutShift;
    }

    _forced = false;
}

void ILI9341::_alarmPage() {

}

void ILI9341::getTouch() {
    bool pressed = tft.getTouch(&_touchX, &_touchY);
    if(pressed) {
        if(digitalRead(SETTINGS_BUTTON_PIN) == 0) _touch_calibrate();
        else {
            Serial.print("x,y = ");
            Serial.print(_touchX);
            Serial.print(",");
            Serial.println(_touchY);
            uint8_t page = 100;

            if(_touchX > 284 && _touchY < 30) {
                // Switch to Network page
                if(_page == PAGE_MAIN) {
                    if(millis() - _pageSwitchedTime > 1000) {
                        page = PAGE_NETWORK;
                        _networkSkeleton = false;
                    }
                }
                // Switch to Main page
                else if(millis() - _pageSwitchedTime > 1000) {
                    page = PAGE_MAIN;
                    _calendarShiftSeconds = 0;
                }
            }

            // On the Main page
            if(_page == PAGE_MAIN) {
                // Switch to Big clock page
                if(_touchX < 140 && _touchY < 80) {
                    page = PAGE_BIG_CLOCK;
                    _bigClockSkeleton = false;
                }

                // Switch to Calendar page
                if(_touchX > 145 && _touchX < 180 && _touchY < 33) {
                    page = PAGE_CALENDAR;
                    _calendarSkeleton = false;
                }

                // Switch to History inside page
                if(_touchX > 145 && _touchY > 33 && _touchY < 80) {
                    page = PAGE_HISTORY_IN;
                    _historyInSkeleton = false;
                }

                // Switch to History outside page
                if(_touchX < 284 && _touchY > 81 && _touchY < 160) {
                    page = PAGE_HISTORY_OUT;
                    _historyOutSkeleton = false;
                }

                // Switch to alarm page
                if(_touchX > 284 && _touchY > 130 && _touchY < 162) {
                    page = PAGE_ALARM;
                    _alarmSkeleton = false;
                }
                
                // Switch to Hourly weather forecast page
                if(_touchY > 162) {
                    uint8_t dayLinks[6];
                    uint8_t dayLink = 0;
                    for(uint8_t i=0; i<40; i++) {
                        if(hour(weather.get_hourlyDate(i)) == 0 && i != 0) dayLinks[dayLink++] = i;
                        if(dayLink > 1) break;
                    }
                    if(_touchX < 106) _hourlyShift = 0;
                    if(_touchX > 106 && _touchX < 208) _hourlyShift = dayLinks[0];
                    if(_touchX > 208) _hourlyShift = dayLinks[1];

                    page = PAGE_HOURLY;
                    _hourlySkeleton = false;
                }
            }

            // On the Clock page
            if(_page == PAGE_BIG_CLOCK || _page == PAGE_SMALL_CLOCK) {
                // Big/Small clock switch 
                if(_touchY > 55 && _touchY < 185) {
                    if(millis() - _pageSwitchedTime > 1000) {
                        page = _page == PAGE_BIG_CLOCK ? PAGE_SMALL_CLOCK : PAGE_BIG_CLOCK;
                        _bigClockSkeleton = false;
                        _smallClockSkeleton = false;
                    }
                }
                // Switch to calendar page
                if((_touchX > 40 && _touchX < 250 && _touchY < 36) || _touchY > 188) {
                    page = PAGE_CALENDAR;
                    _calendarSkeleton = false;
                }
            }

            // Left button
            if(_touchX < 32 && _touchY > 100 && _touchY < 136) {
                // Calendar page
                if(_page == PAGE_CALENDAR) _calendarShiftDirection = -1;
                // Hourly weather forecast page
                if(_page == PAGE_HOURLY) {
                    if(_hourlyShift >= 4) _hourlyShift -= 4; 
                    else _hourlyShift = 0;
                }
                /* History outside page */
                if(_page == PAGE_HISTORY_OUT) {
                    if(_historyOutShift >= 4) _historyOutShift -= 4; 
                    else _historyOutShift = 0;
                }
                /* History inside page */
                if(_page == PAGE_HISTORY_IN) {
                    if(_historyInShift >= 4) _historyInShift -= 4; 
                    else _historyInShift = 0;
                }
            }
            /* Right button */
            if(_touchX > 286 && _touchY > 100 && _touchY < 136) {
                // Calendar page
                if(_page == PAGE_CALENDAR) _calendarShiftDirection = 1;
                /* Hourly weather forecast page */
                if(_page == PAGE_HOURLY) {
                    if(_hourlyShift <= 28) _hourlyShift += 4;
                    else _hourlyShift = 32;
                }
                /* History outside page */
                if(_page == PAGE_HISTORY_OUT) {
                    if(_historyOutShift <= 12) _historyOutShift += 4;
                    else _historyOutShift = 16;
                }
                /* History inside page */
                if(_page == PAGE_HISTORY_IN) {
                    if(_historyInShift <= 12) _historyInShift += 4;
                    else _historyInShift = 16;
                }
            }

            if(_page != page) {
                _forced = true;
                if(page != 100) {
                    _page = page;
                    _pageSwitchedTime = millis();
                }
                if(page == PAGE_MAIN) showHomeScreen();
            }
        }
    }
}

void ILI9341::_touch_calibrate() {
    uint16_t calData[5];

    tft.fillScreen(TFT_BLACK);
    _printText(0, 110, 319, 22, lang.touchCalibrate(), FONT1, CENTER, TEXT_COLOR);
    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    String json = "{\"calData\":[";
    json += String(calData[0]) + ",";
    json += String(calData[1]) + ",";
    json += String(calData[2]) + ",";
    json += String(calData[3]) + ",";
    json += String(calData[4]);
    json += "]}";
    File file = LittleFS.open("/touch.json", FILE_WRITE);
    file.print(json);
    file.close();

    _printText(0, 110, 319, 22, lang.calibrationDone(), FONT1, CENTER, TEXT_COLOR);
    vTaskDelay(2000);
    tft.fillScreen(TFT_BLACK);
    _forced = true;
    showHomeScreen();
}