#pragma once

class AgregateSegmentData {
    public:
        static constexpr int CLOCK_SOURCE   = 0;
        static constexpr int DATE_SOURCE    = 1;
        static constexpr int BME280_SOURCE  = 2;
        static constexpr int BMP180_SOURCE  = 3;
        static constexpr int SHT21_SOURCE   = 4;
        static constexpr int DHT22_SOURCE   = 5;
        static constexpr int DS18B20_SOURCE = 6;
        static constexpr int ESP32_SOURCE   = 7;
        static constexpr int THING_SOURCE   = 8;
        static constexpr int WEATHER_SOURCE = 9;
        static constexpr int WSENSOR_SOURCE = 10;
        static constexpr int BME680_SOURCE  = 11;

        static constexpr int CLOCK_TYPE = 0;
        static constexpr int DATE_TYPE = 1;
        static constexpr int TEMP_TYPE = 2;
        static constexpr int HUM_TYPE = 3;
        static constexpr int PRES_TYPE = 4;
        static constexpr int CO2_TYPE = 5;
        static constexpr int IAQ_TYPE = 6;

        float slotData(uint8_t sensor, uint8_t type, uint8_t timeSlot, uint8_t dispNum, uint8_t* dType);
};