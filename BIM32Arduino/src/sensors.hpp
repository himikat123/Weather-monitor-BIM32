#pragma once

#include <Wire.h>
#include <Adafruit_BME280.h> // v2.2.4 https://github.com/adafruit/Adafruit_BME280_Library
#include <Adafruit_BMP085.h> // v1.2.4 https://github.com/adafruit/Adafruit-BMP085-Library
#include "SHT21.h" // https://github.com/markbeee/SHT21
#include "DHTesp.h" // v1.19.0 http://desire.giesecke.tk/index.php/2018/01/30/esp32-dht11/
#include <MAX44009.h> // v1.2.3 https://github.com/dantudose/MAX44009
#include <BH1750.h> // v1.3.0 https://github.com/claws/BH1750
#include <OneWire.h> // v2.3.7 https://github.com/PaulStoffregen/OneWire
#include <DallasTemperature.h> // v3.9.0 https://github.com/milesburton/Arduino-Temperature-Control-Library
#include "bsec.h" // v1.8.1492 https://www.bosch-sensortec.com/software-tools/software/bsec/
#include <Adafruit_PCF8574.h> // v1.1.1 https://github.com/adafruit/Adafruit_PCF8574
#include "DS3232.h" // v0.4.1 https://github.com/RobTillaart/DS3232

#include "pinout.hpp"
#include "state.hpp"

#define DS18B20_RESOLUTION    12 /* DS18B20 resolution 9,10,11 or 12 bits */

class Sensors {
    protected:
        OneWire             oneWire;
        DallasTemperature   term;
        DeviceAddress       thermometer;
        SHT21               sht21;
        DHTesp              dht;
        MAX44009            max_light;
        BH1750              lightMeter;
        Adafruit_BMP085     bmp;
        Adafruit_BME280     bme;
        Adafruit_Sensor     *bme_temp;
        Adafruit_Sensor     *bme_pressure;
        Adafruit_Sensor     *bme_humidity;
        Adafruit_PCF8574    pcf8574;
        Bsec                iaqSensor;
        DS3231              rtc;
    public:
        Sensors() : 
            oneWire(ONE_WIRE_BUS_PIN), 
            term(&oneWire), 
            lightMeter(0x23), 
            bme_temp(bme.getTemperatureSensor()), 
            bme_pressure(bme.getPressureSensor()), 
            bme_humidity(bme.getHumiditySensor()) 
        { }

        void init(void);
        void read(void);
        void BME680Read(void);
        float get_esp32_temp();
        float get_bme280_temp();
        float get_bme280_hum();
        float get_bme280_pres();
        float get_bmp180_temp();
        float get_bmp180_pres();
        float get_sht21_temp();
        float get_sht21_hum();
        float get_dht22_temp();
        float get_dht22_hum();
        float get_ds18b20_temp();
        float get_max44009_light();
        float get_bh1750_light();
        float get_analog_voltage();
        float get_bme680_temp();
        float get_bme680_hum();
        float get_bme680_pres();
        float get_bme680_iaq();
        unsigned int get_bme680_iaq_accuracy();
        void get_ds3231_timeDate();
        void set_ds3231_timeDate();
        void comfortDevices(bool heater, bool cooler, bool humidifier, bool dehumidifier, bool purifier);
        float absoluteHum(float temp, float hum);
        float dewPoint(float temp, float hum);
        float mmHg(float pres);

    private:
        bool _bme280_det = false;
        bool _bmp180_det = false;
        bool _sht21_det = false;
        bool _dht22_det = false;
        bool _ds18b20_det = false;
        bool _max44009_det = false;
        bool _bh1750_det = false;
        bool _bme680_det = false;
        bool _pcf8574_det = false;
        bool _ds3231_det = false;
        uint8_t _bme680_bsecState[BSEC_MAX_STATE_BLOB_SIZE] = {0};
        uint16_t _bme680_stateUpdateCounter = 0;
        unsigned int _bme680_stateCounter = 0;
        unsigned int _bme680_stateTimestamp = 0;

        void _BME280Init(void);
        void _BMP180Init(void);
        void _SHT21Init(void);
        void _DHT22Init(void);
        void _DS18B20Init(void);
        void _MAX44009Init(void);
        void _BH1750Init(void);
        void _BME680Init(void);
        void _PCF8574Init(void);
        bool _BME680_validateIaqSensorStatus(void);
        void _BME680_loadState(void);
        void _BME680_updateState(void);
        void _DS3231Init(void);

        void _BME280Read(void);
        void _BMP180Read(void);
        void _SHT21Read(void);
        void _DHT22Read(void);
        void _DS18B20Read(void);
        void _MAX44009Read(void);
        void _BH1750Read(void);
        void _AnalogRead(void);
        void _ESP32Read(void);
};

extern Sensors sensors;