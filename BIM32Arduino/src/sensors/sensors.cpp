#include <Arduino.h>
#include "./sensors.hpp"

/**
 * Initialize all sensors
 */
void Sensors::init(void) {
    _DS18B20Init();
    _DHT22Init();
    _BME280Init();
    _BMP180Init();
    _SHT21Init();
    _MAX44009Init();
    _BH1750Init();
    _BME680Init();
    _PCF8574Init();
    _DS3231Init();
    Serial.println(SEPARATOR);
    Serial.println("Sensors initialization");
    Serial.printf("%s %s%s\r\n", "DS18B20:  ", _ds18b20_det ? "" : "NOT ", "Detected");
    Serial.printf("%s %s%s\r\n", "BME280:   ", _bme280_det ? "" : "NOT ", "Detected");
    Serial.printf("%s %s%s\r\n", "BMP180:   ", _bmp180_det ? "" : "NOT ", "Detected");
    Serial.printf("%s %s%s\r\n", "SHT21:    ", _sht21_det ? "" : "NOT ", "Detected");
    Serial.printf("%s %s%s\r\n", "DHT22:    ", _dht22_det ? "" : "NOT ", "Detected");
    Serial.printf("%s %s%s\r\n", "MAX44009: ", _max44009_det ? "" : "NOT ", "Detected");
    Serial.printf("%s %s%s\r\n", "BH1750:   ", _bh1750_det ? "" : "NOT ", "Detected");
    Serial.printf("%s %s%s\r\n", "BME680:   ", _bme680_det ? "" : "NOT ", "Detected");
    Serial.printf("%s %s%s\r\n", "PCF8574:  ", _pcf8574_det ? "" : "NOT ", "Detected");
    Serial.printf("%s %s%s\r\n", "DS3231:   ", _ds3231_det ? "" : "NOT ", "Detected");
}

/**
 * Reads all sensors
 */
void Sensors::read(void) {
    _BME280Read();
    _BMP180Read();
    _SHT21Read();
    _DHT22Read();
    _DS18B20Read();
    _MAX44009Read();
    _BH1750Read();
    _ESP32Read();
    _AnalogRead();
}

/*
 * Calculate absolute humidity
 */
float Sensors::absoluteHum(float temp, float hum) {
    float sat = 6.112 * exp((17.67 * temp) / (temp + 243.5));
    float vap = sat * (hum / 100.0);
    float abs = (2.1674 * vap / (273.15 + temp)) * 100;

    return abs;
}

/*
 * Calculate dew point
 */
float Sensors::dewPoint(float temp, float hum) {
    const float a = 17.67, b = 243.5;
  
    float alpha = log(hum / 100.0) + (a * temp) / (b + temp);
    float dp = (b * alpha) / (a - alpha);

    return dp;
}

/*
 * Convert hPa to mmHg
 */
float Sensors::mmHg(float pres) {
    return pres * 0.75;
}