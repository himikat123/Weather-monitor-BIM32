#pragma once

#include <Arduino.h>
#include <EasyNextionLibrary.h> // v1.0.6 https://github.com/Seithan/EasyNextionLibrary
#include "../lcdDisplay.hpp"

class Nextion : public LcdDisplay {
    protected:
        EasyNex _nextion;

    public:
        static Nextion& getInstance() {
            static Nextion instance;
            return instance;
        }

        Nextion(const Nextion&) = delete;
        void operator=(const Nextion&) = delete;

        void init();
        void showLogo();
        void refresh();
        void displayToggle();
        void displayOn(bool doinit);
        virtual void displayOn() override;
        virtual void displayOff() override;
        virtual bool isDisplayOn() const override;
        void brightness(unsigned int bright);
        void setDisplayRTC();
        void dataReceive();

    private:
        Nextion() : _nextion(Serial1) {}

        int _customData = -1; // flag of key symbols of receive from the display
        String _receivedData = "";
        uint16_t _air_color[4] = { 2016, 65520, 64512, 63488 };

        void _setRTC();
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

inline Nextion& nextion = Nextion::getInstance();