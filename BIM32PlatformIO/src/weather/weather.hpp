#pragma once

#include <Arduino.h>

class Weather {
    public:
        void update();
        float get_currentTemp();
        float get_currentHum();
        float get_currentPres();
        float get_currentWindSpeed();
        int get_currentWindDir();
        unsigned int get_currentIcon();
        bool get_isDay();
        String get_description();
        time_t get_currentUpdated();

        float get_dailyDayTemp(unsigned int num);
        float get_dailyNightTemp(unsigned int num);
        float get_dailyWindSpeed(unsigned int num);
        unsigned int get_dailyIcon(unsigned int num);

        unsigned int get_hourlyDate(unsigned int num);
        unsigned int get_hourlyIcon(unsigned int num);
        float get_hourlyTemp(unsigned int num);
        float get_hourlyPres(unsigned int num);
        float get_hourlyWindSpeed(unsigned int num);
        int get_hourlyWindDir(unsigned int num);
        float get_hourlyPrec(unsigned int num); 

    private:
        float _lon = 0.0;
        float _lat = 0.0;

        unsigned int _convertIcon(int code);
        const char* _openMeteoCode2Description(uint8_t code);
        unsigned int _openMeteoIcon(int code);
        unsigned int _weatherbitIcon(int code);
        void _updateWeatherbitDaily(void);
        void _updateOpenweathermapHourly(void);
        void _updateOpenMeteoDaily(void);
        void _updateOpenMeteoHourly(void);
        void _calculateDaily(void);
};

extern Weather weather;