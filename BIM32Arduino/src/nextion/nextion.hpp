#pragma once

#include <Arduino.h>
#include "../globals.hpp"
#include "../lcdDisplay.hpp"
#include <EasyNextionLibrary.h> // v1.0.6 https://github.com/Seithan/EasyNextionLibrary

#include "../config.hpp"
#include "../globals.hpp"


class Nextion : LcdDisplay {
    #define OPENWEATHERMAP 0
    #define WEATHERBIT 1
    #define OPEN_METEO 2

    protected:
        EasyNex _nextion;

    public:
        Nextion() : _nextion(Serial1) {}
        void init();
        void showLogo();
        void refresh();
        void displayToggle();
        void displayOn(bool doinit = true);
        void displayOff();
        bool isDisplayOn();
        void brightness(unsigned int bright);
        void setDisplayRTC();
        void dataReceive();

    private:
        int _customData = -1; // flag of key symbols of receive from the display
        String _receivedData = "";
        uint16_t _air_color[4] = { 2016, 65520, 64512, 63488 };

        void _NX4832K035_setRTC();
        void _NX4832T035_timeDate();
        void _networkPage();
        void _showVoltage();
        void _showBattery();
        void _showAntenna();
        void _showTempIn();
        void _showTempOut();
        void _showHumIn();
        void _showHumOut();
        void _showPres();
        void _showSequence();
        void _showComfortLevel();
        void _showWeatherDescription();
        void _showCurrentIcon();
        void _showThermometer();
        void _showWindSpeed();
        void _showWindDirection();
        void _showUpdated();
        void _showWeatherForecast();
        void _hourlyData();
        void _daily2hourly();
        void _historyOut();
        void _historyIn();
        void _alarms();
};

extern Nextion nextion;