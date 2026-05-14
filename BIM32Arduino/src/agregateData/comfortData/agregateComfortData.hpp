#pragma once

class AgregateComfortData {
    private:
        static constexpr int WEATHER_TEMP = 1;
        static constexpr int WSENSOR_TEMP = 2;
        static constexpr int THING_TEMP   = 3;
        static constexpr int BME280_TEMP  = 4;
        static constexpr int BMP180_TEMP  = 5;
        static constexpr int SHT21_TEMP   = 6;
        static constexpr int DHT22_TEMP   = 7;
        static constexpr int DS18B20_TEMP = 8;
        static constexpr int BME680_TEMP  = 9;

        static constexpr int WEATHER_HUM  = 1;
        static constexpr int WSENSOR_HUM  = 2;
        static constexpr int THING_HUM    = 3;
        static constexpr int BME280_HUM   = 4;
        static constexpr int SHT21_HUM    = 5;
        static constexpr int DHT22_HUM    = 6;
        static constexpr int BME680_HUM   = 7;

        static constexpr int BME680_IAQ   = 1;
        static constexpr int WSENSOR_CO2  = 1;

    public:
        float temp();
        float hum();
        float iaq();
        float co2();
};