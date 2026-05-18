#pragma once

class AgregateSendData {
    private:
        static constexpr int WEATHER_SOURCE = 1;
            static constexpr int WEATHER_TEMP = 0;
            static constexpr int WEATHER_HUM = 1;
            static constexpr int WEATHER_PRES = 2;
            static constexpr int WEATHER_WIND_SPEED = 3;
            static constexpr int WEATHER_WIND_DIR = 4;
            static constexpr int WEATHER_ABS_HUM = 5;
            static constexpr int WEATHER_DEW_POINT = 6;
        static constexpr int WSENSOR_SOURCE = 2;
            static constexpr int WSENSOR_TEMP = 4;
            static constexpr int WSENSOR_HUM = 5;
            static constexpr int WSENSOR_PRES = 6;
            static constexpr int WSENSOR_WIND_SPEED = 7;
            static constexpr int WSENSOR_WIND_DIR = 8;
            static constexpr int WSENSOR_LIGHT = 9;
            static constexpr int WSENSOR_VOLT = 10;
            static constexpr int WSENSOR_CURR = 11;
            static constexpr int WSENSOR_PWR = 12;
            static constexpr int WSENSOR_ENRG = 13;
            static constexpr int WSENSOR_FREQ = 14;
            static constexpr int WSENSOR_VBAT = 15;
            static constexpr int WSENSOR_BATPRC = 16;
            static constexpr int WSENSOR_BATLVL = 17;
            static constexpr int WSENSOR_CO2 = 18;
            static constexpr int WSENSOR_ABS_HUM = 19;
            static constexpr int WSENSOR_DEW_POINT = 20;
        static constexpr int BME280_SOURCE = 3;
            static constexpr int BME280_TEMP = 0;
            static constexpr int BME280_HUM = 1;
            static constexpr int BME280_PRES = 2;
            static constexpr int BME280_ABS_HUM = 3;
            static constexpr int BME280_DEW_POINT = 4;
        static constexpr int BMP180_SOURCE = 4;
            static constexpr int BMP180_TEMP = 0;
            static constexpr int BMP180_PRES = 1;
        static constexpr int SHT21_SOURCE = 5;
            static constexpr int SHT21_TEMP = 0;
            static constexpr int SHT21_HUM = 1;
            static constexpr int SHT21_ABS_HUM = 2;
            static constexpr int SHT21_DEW_POINT = 3;
        static constexpr int DHT22_SOURCE = 6;
            static constexpr int DHT22_TEMP = 0;
            static constexpr int DHT22_HUM = 1;
            static constexpr int DHT22_ABS_HUM = 2;
            static constexpr int DHT22_DEW_POINT = 3;
        static constexpr int DS18B20_SOURCE = 7;
        static constexpr int MAX44009_SOURCE = 8;
        static constexpr int BH1750_SOURCE = 9;
        static constexpr int ANALOG_SOURCE = 10;
        static constexpr int ESP32_SOURCE = 11;
            static constexpr int ESP32_TEMP = 0;
            static constexpr int ESP32_RUNTIME = 1;
        static constexpr int BME680_SOURCE = 12;
            static constexpr int BME680_TEMP = 0;
            static constexpr int BME680_HUM = 1;
            static constexpr int BME680_PRES = 2;
            static constexpr int BME680_IAQ = 3;
            static constexpr int BME680_ABS_HUM = 4;
            static constexpr int BME680_DEW_POINT = 5;
    public:
        float sendingData(uint8_t field, uint8_t type, uint8_t wsensNum, uint8_t wsensType);
};

extern AgregateSendData agregateSendData;