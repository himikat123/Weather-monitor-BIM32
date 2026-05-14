#pragma once
#include <Arduino.h>

class AgregateLcdData {
    public:
        static constexpr int WEATHER_TEMP_IN       = 1;
        static constexpr int WSENSOR_TEMP_IN       = 2;
        static constexpr int THING_TEMP_IN         = 3;
        static constexpr int SEQUENCE_TEMP_IN      = 4;
        static constexpr int BME280_TEMP_IN        = 5;
        static constexpr int BMP180_TEMP_IN        = 6;
        static constexpr int SHT21_TEMP_IN         = 7;
        static constexpr int DHT22_TEMP_IN         = 8;
        static constexpr int DS18B20_TEMP_IN       = 9;
        static constexpr int BME680_TEMP_IN        = 10;

        static constexpr int WEATHER_TEMP_OUT      = 1;
        static constexpr int WSENSOR_TEMP_OUT      = 2;
        static constexpr int THING_TEMP_OUT        = 3;
        static constexpr int BME280_TEMP_OUT       = 4;
        static constexpr int BMP180_TEMP_OUT       = 5;
        static constexpr int SHT21_TEMP_OUT        = 6;
        static constexpr int DHT22_TEMP_OUT        = 7;
        static constexpr int DS18B20_TEMP_OUT      = 8;
        static constexpr int BME680_TEMP_OUT       = 9;

        static constexpr int WEATHER_TEMP_SEQUENCE = 1;
        static constexpr int WSENSOR_TEMP_SEQUENCE = 2;
        static constexpr int THING_TEMP_SEQUENCE   = 3;
        static constexpr int BME280_TEMP_SEQUENCE  = 4;
        static constexpr int BMP180_TEMP_SEQUENCE  = 5;
        static constexpr int SHT21_TEMP_SEQUENCE   = 6;
        static constexpr int DHT22_TEMP_SEQUENCE   = 7;
        static constexpr int DS18B20_TEMP_SEQUENCE = 8;
        static constexpr int BME680_TEMP_SEQUENCE  = 9;

        static constexpr int WEATHER_HUM_IN        = 1;
        static constexpr int WSENSOR_HUM_IN        = 2;
        static constexpr int THING_HUM_IN          = 3;
        static constexpr int SEQUENCE_HUM_IN       = 4;
        static constexpr int BME280_HUM_IN         = 5;
        static constexpr int SHT21_HUM_IN          = 6;
        static constexpr int DHT22_HUM_IN          = 7;
        static constexpr int BME680_HUM_IN         = 8;

        static constexpr int WEATHER_HUM_SEQUENCE  = 1;
        static constexpr int WSENSOR_HUM_SEQUENCE  = 2;
        static constexpr int THING_HUM_SEQUENCE    = 3;
        static constexpr int BME280_HUM_SEQUENCE   = 4;
        static constexpr int SHT21_HUM_SEQUENCE    = 5;
        static constexpr int DHT22_HUM_SEQUENCE    = 6;
        static constexpr int BME680_HUM_SEQUENCE   = 7;
 
        static constexpr int WEATHER_HUM_OUT       = 1;
        static constexpr int WSENSOR_HUM_OUT       = 2;
        static constexpr int THING_HUM_OUT         = 3;
        static constexpr int BME280_HUM_OUT        = 4;
        static constexpr int SHT21_HUM_OUT         = 5;
        static constexpr int DHT22_HUM_OUT         = 6;
        static constexpr int BME680_HUM_OUT        = 7;
 
        static constexpr int WEATHER_PRES_OUT      = 1;
        static constexpr int WSENSOR_PRES_OUT      = 2;
        static constexpr int THING_PRES_OUT        = 3;
        static constexpr int BME280_PRES_OUT       = 4;
        static constexpr int BMP180_PRES_OUT       = 5;
        static constexpr int BME680_PRES_OUT       = 6;
 
        static constexpr int WEATHER_WIND_SPEED    = 1;
        static constexpr int WSENSOR_WIND_SPEED    = 2;
        static constexpr int THING_WIND_SPEED      = 3;
 
        static constexpr int WEATHER_WIND_DIR      = 1;
        static constexpr int WSENSOR_WIND_DIR      = 2;
        static constexpr int THING_WIND_DIR        = 3;
 
        static constexpr int WSENSOR_VOLT          = 1;
        static constexpr int THING_VOLT            = 2;
        static constexpr int DATE                  = 3;
        static constexpr int BME680_IAQ            = 4;
        static constexpr int BME680_ABS_HUM        = 5;
        static constexpr int BME680_DEW_POINT      = 6;
        static constexpr int BME280_ABS_HUM        = 7;
        static constexpr int BME280_DEW_POINT      = 8;
        static constexpr int DHT22_ABS_HUM         = 9;
        static constexpr int DHT22_DEW_POINT       = 10;
        static constexpr int SHT21_ABS_HUM         = 11;
        static constexpr int SHT21_DEW_POINT       = 12;
        static constexpr int WEATHER_ABS_HUM       = 13;
        static constexpr int WEATHER_DEW_POINT     = 14;
 
        static constexpr int WSENSOR_BAT_VOLT      = 0;
        static constexpr int WSENSOR_BAT_PERCENT   = 1;
        static constexpr int WSENSOR_PZEM_VOLT     = 2;
        static constexpr int WSENSOR_S8_CO2        = 3;
 
        static constexpr int THING_BAT_VOLT        = 0;
        static constexpr int THING_BAT_PERCENT     = 1;
 
        static constexpr int WSENSOR_S8_COLOR      = 1;
        static constexpr int BME680_IAQ_COLOR      = 4;
 
        static constexpr int WSENSOR_BAT_SYMB      = 1;
        static constexpr int THING_BAT_SYMB        = 2;

        float tempIn(float* tempSequence);
        float tempOut();
        float humIn(float* humSequence);
        float humOut();
        float presOut();
        float windSpeed();
        int windDir();
        void sequenceNames(String* nameSequence);
        String voltage();
        uint8_t voltageColor();
        int batteryLevel();
        String comfort();
        uint8_t windDirection(int deg);

    private:
        void _tempSequence(float* tempSequence);
        void _humSequence(float* humSequence);
        String _voltageWsensor();
        String _voltageThingspeak();
        String _iaq();
        String _absoluteHum(float temp, float hum);
        String _dewPoint(float temp, float hum);
        String _localDate();
};

extern AgregateLcdData agregateLcdData;