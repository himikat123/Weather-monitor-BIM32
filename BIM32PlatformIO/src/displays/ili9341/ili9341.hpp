#pragma once

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#include "../lcdDisplay.hpp"

class ILI9341 : LcdDisplay {
    ILI9341() {}

    static constexpr int LEFT              = 0;
    static constexpr int CENTER            = 1;
    static constexpr int RIGHT             = 2;

    static constexpr int FONT1             = 0;
    static constexpr int FONT2             = 1;
    static constexpr int FONT3             = 2;
    static constexpr int FONT_SEGMENTS_BIG = 3;
    static constexpr int FONT_SEGMENTS_SML = 4;
    static constexpr int FONT_SMALL        = 5;
    static constexpr int FONT_TINY         = 6;
    static constexpr int FONTPR            = 7;

    static constexpr int BG_COLOR          = 0x0000; // black
    static constexpr int FRAME_COLOR       = 0x001F; // blue
    static constexpr int TEXT_COLOR        = 0xFFFF; // white
    static constexpr int TEMPERATURE_COLOR = 0xFFE0; // yellow
    static constexpr int TEMP_MIN_COLOR    = 0xFBA0; // orange
    static constexpr int HUMIDITY_COLOR    = 0x759F; // cyan
    static constexpr int PRESSURE_COLOR    = 0xFC1F; // magenta
    static constexpr int CLOCK_COLOR       = 0x07E0; // green
    static constexpr int BATTERY_COLOR     = 0x07E0; // green
    static constexpr int BATTERY_LOW_COLOR = 0xF800; // red
    static constexpr int ANTENNA_COLOR     = 0x03EF; // dark cyan
    static constexpr int WEEKDAY_COLOR     = 0xFFE0; // yellow
    static constexpr int WEEKEND_COLOR     = 0xF800; // red
    static constexpr int MONTH_COLOR       = 0xF400; // orange
    static constexpr int TODAY_COLOR       = 0x001E; // blue
    static constexpr int TODAY_BG_COLOR    = 0x07DE; // light blue
    static constexpr int FRAME1_COLOR      = 0x73AE; // light gray
    static constexpr int GROUND_COLOR      = 0x31A6; // dark gray
    static constexpr int GRID_COLOR        = 0x4a69; // gray

    static constexpr int PAGE_MAIN         = 0;
    static constexpr int PAGE_BIG_CLOCK    = 1;
    static constexpr int PAGE_SMALL_CLOCK  = 2;
    static constexpr int PAGE_NETWORK      = 3;
    static constexpr int PAGE_CALENDAR     = 4;
    static constexpr int PAGE_HOURLY       = 5;
    static constexpr int PAGE_HISTORY_OUT  = 6;
    static constexpr int PAGE_HISTORY_IN   = 7;
    static constexpr int PAGE_ALARM        = 8;

    static constexpr int HOURLY            = 0;
    static constexpr int HISTORY_OUT       = 1;
    static constexpr int HISTORY_IN        = 2;

    protected:
        TFT_eSPI tft{}; 

#if defined(BIM32_CYD)
        SPIClass touchscreenSPI{VSPI};
        XPT2046_Touchscreen touchscreen{XPT2046_CS, XPT2046_IRQ};
#endif

    public:
        static ILI9341& getInstance() {
            static ILI9341 instance;
            return instance;
        }

        ILI9341(const ILI9341&) = delete;
        void operator=(const ILI9341&) = delete;

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
        bool _cmfType = false;
        time_t _prevCmfTime = 0;
        uint16_t _calData[5];

        void _sequenceSlotSkip();
        void _sequenceSlotNext();
        void _showImg(uint16_t x, uint16_t y, const byte img[], uint16_t size);
        const byte* _number_picture(uint8_t num);
        uint16_t _number_picture_size(uint8_t num);
        void _printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color);
        void _printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color, uint16_t bgColor);
        void _printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color, uint16_t bgColor, bool valign);
        void _showTemperature(float temp, uint16_t x, uint16_t y, uint8_t font, uint16_t color);
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
        TS_Point _calibrationPoint(uint8_t cornerNr);
        int _avg(int a, int b);
        void _touch_calibrate();
};

inline ILI9341& ili9341 = ILI9341::getInstance();