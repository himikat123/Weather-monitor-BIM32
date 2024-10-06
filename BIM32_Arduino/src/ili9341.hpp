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

    #define PAGE_MAIN         0
    #define PAGE_BIG_CLOCK    1
    #define PAGE_SMALL_CLOCK  2
    #define PAGE_NETWORK      3
    #define PAGE_CALENDAR     4
    #define PAGE_HOURLY       5
    #define PAGE_HISTORY_OUT  6
    #define PAGE_HISTORY_IN   7
    #define PAGE_ALARM        8

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
        bool _bigClockSkeleton = false;
        bool _smallClockSkeleton = false;
        bool _calendarSkeleton = false;
        int _calendarShiftSeconds = 0;
        int8_t _calendarShiftDirection = 0;


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
        void _leftButton();
        void _rightButton();
        void _dateWeekday();
        void _bigClockPage();
        void _smallClockPage();
        bool _isLeapYear(unsigned int year);
        uint8_t _numberOfDaysInMonth(uint8_t month, uint16_t year);
        void _calendarPage();
        void _touch_calibrate();
};

/**
 * Display and fonts initialisation
 */
void ILI9341::init(void) {
    tft.begin();
    tft.setRotation(3);
    tft.setSwapBytes(true);

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
        if(_page == PAGE_BIG_CLOCK) _bigClockPage();
        if(_page == PAGE_SMALL_CLOCK) _smallClockPage();
        if(_page == PAGE_CALENDAR) _calendarPage();
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
        if(font == FONT1) tft.loadFont(Ubuntu_14);
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
    tft.fillSmoothCircle(70, 24, 3, points ? CLOCK_COLOR : BG_COLOR, BG_COLOR);
    tft.fillSmoothCircle(70, 52, 3, points ? CLOCK_COLOR : BG_COLOR, BG_COLOR);
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
void ILI9341::_leftButton() {
    _showImg(0, 106, symb_left, sizeof(symb_left));
}

/**
 * Right button
 */
void ILI9341::_rightButton() {
    _showImg(295, 106, symb_right, sizeof(symb_right));
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
        _leftButton();
        _rightButton();
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
                if(_page == PAGE_MAIN) page = PAGE_NETWORK;
                else {
                    page = PAGE_MAIN;
                    _calendarShiftSeconds = 0;
                }
            }

            // Main page
            if(_page == PAGE_MAIN) {
                // Big clock
                if(_touchX < 140 && _touchY < 80) {
                    page = PAGE_BIG_CLOCK;
                    _bigClockSkeleton = false;
                }
                // Calendar
                if(_touchX > 145 && _touchX < 180 && _touchY < 33) {
                    page = PAGE_CALENDAR;
                    _calendarSkeleton = false;
                }
                //
                if(_touchX > 145 && _touchY > 33 && _touchY < 80) page = PAGE_HISTORY_IN;
                if(_touchX < 284 && _touchY > 81 && _touchY < 160) page = PAGE_HISTORY_OUT;
                if(_touchX > 284 && _touchY > 130 && _touchY < 162) page = PAGE_ALARM;
                if(_touchX < 106 && _touchY > 162) Serial.println("Forecast1 clicked");
                if(_touchX > 106 && _touchX < 208 && _touchY > 162) Serial.println("Forecast2 clicked");
                if(_touchX > 208 && _touchY > 162) Serial.println("Forecast3 clicked");
                if(_touchY > 162) page = PAGE_HOURLY;
            }

            // Clock page
            if(_page == PAGE_BIG_CLOCK || _page == PAGE_SMALL_CLOCK) {
                // Big/Small clock switch 
                if(_touchY > 55 && _touchY < 185) {
                    if(millis() - _pageSwitchedTime > 1000) {
                        page = _page == PAGE_BIG_CLOCK ? PAGE_SMALL_CLOCK : PAGE_BIG_CLOCK;
                        _bigClockSkeleton = false;
                        _smallClockSkeleton = false;
                    }
                }
                // Switch to calendar
                if((_touchX > 40 && _touchX < 250 && _touchY < 36) || _touchY > 188) {
                    page = PAGE_CALENDAR;
                    _calendarSkeleton = false;
                }
            }

            // Calendar page
            if(_page == PAGE_CALENDAR) {
                /* left button */
                if(_touchX < 32 && _touchY > 100 && _touchY < 136) _calendarShiftDirection = -1;
                /* right button */
                if(_touchX > 286 && _touchY > 100 && _touchY < 136) _calendarShiftDirection = 1;
            }

            if(_page != page) {
                _forced = true;
                if(page != 100) {
                    _page = page;
                    _pageSwitchedTime = millis();
                }
                if(page == PAGE_MAIN) showHomeScreen();
            }
            Serial.print("page: "); 
            switch (_page) {
                case PAGE_MAIN: Serial.println("MAIN"); break;
                case PAGE_BIG_CLOCK: Serial.println("BIG CLOCK"); break;
                case PAGE_SMALL_CLOCK: Serial.println("SMALL CLOCK"); break;
                case PAGE_NETWORK: Serial.println("NETWORK"); break;
                case PAGE_CALENDAR: Serial.println("CALENDAR"); break;
                case PAGE_HOURLY: Serial.println("HOURLY"); break;
                case PAGE_HISTORY_OUT: Serial.println("HISTORY OUT"); break;
                case PAGE_HISTORY_IN: Serial.println("HISTORY IN"); break;
                case PAGE_ALARM: Serial.println("ALARM"); break;
                default: ; break;
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