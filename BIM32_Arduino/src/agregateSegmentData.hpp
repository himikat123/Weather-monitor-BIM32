#define CLOCK 0
#define DATE 1
#define TEMP 2
#define HUM 3
#define PRES 4
#define CO2 5
#define IAQ 6

class AgregateSegmentData {
    private:
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

    public:
        float slotData(uint8_t sensor, uint8_t type, uint8_t timeSlot, uint8_t dispNum, uint8_t* dType);
};

float AgregateSegmentData::slotData(uint8_t sensor, uint8_t type, uint8_t timeSlot, uint8_t dispNum, uint8_t* dType) {
    float data = UNDEFINED_FLOAT;

    switch(sensor) {
        case CLOCK_SOURCE: { // Clock
            *dType = CLOCK;
        }; break;
        case DATE_SOURCE: { // Date
            *dType = DATE;
        }; break;
        case BME280_SOURCE: { // BME280
            if(type == 0) {
                data = sensors.get_bme280_temp();
                *dType = TEMP;
            }
            if(type == 1) {
                data = sensors.get_bme280_hum();
                *dType = HUM;
            }
            if(type == 2) {
                data = sensors.get_bme280_pres();
                *dType = PRES;
            }
        }; break;
        case BMP180_SOURCE: {// BMP180
            if(type == 0) {
                data = sensors.get_bmp180_temp();
                *dType = TEMP;
            }
            if(type == 1) {
                data = sensors.get_bmp180_pres();
                *dType = PRES;
            }
        }; break;
        case SHT21_SOURCE: {// SHT21
            if(type == 0) {
                data = sensors.get_sht21_temp();
                *dType = TEMP;
            }
            if(type == 1) {
                data = sensors.get_sht21_hum();
                *dType = HUM;
            }
        }; break;
        case DHT22_SOURCE: { // DHT22
            if(type == 0) {
                data = sensors.get_dht22_temp();
                *dType = TEMP;
            }
            if(type == 1) {
                data = sensors.get_dht22_hum();
                *dType = HUM;
            }
        }; break;
        case DS18B20_SOURCE: { // DS18B20
            data = sensors.get_ds18b20_temp();
            *dType = TEMP;
        }; break;
        case ESP32_SOURCE: { // ESP32
            data = sensors.get_esp32_temp();
            *dType = TEMP;
        }; break;
        case THING_SOURCE: { // Thingspeak
            data = thingspeak.dataRelevance()
                ? thingspeak.get_field(config.display.timeSlot.thing(timeSlot, dispNum))
                : 40400.0;
            if(type == 0) *dType = TEMP;
            if(type == 1) *dType = HUM;
            if(type == 2) *dType = PRES;
        }; break;
        case WEATHER_SOURCE: { // Weather
            bool dataRelevant = (now() - weather.get_currentUpdated() < 1200);
            if(type == 0) {
                data = dataRelevant ? weather.get_currentTemp() : 40400.0;
                *dType = TEMP;
            }
            if(type == 1) {
                data = dataRelevant ? weather.get_currentHum() : 40400.0;
                *dType = HUM;
            }
            if(type == 2) {
                data = dataRelevant ? weather.get_currentPres() : 40400.0;
                *dType = PRES;
            }
        }; break;
        case WSENSOR_SOURCE: { // Wireless sensor
            unsigned int wsensNum = config.display.timeSlot.wsensor.num(timeSlot, dispNum);
            unsigned int wsensType = config.display.timeSlot.wsensor.type(timeSlot, dispNum);
            if(wsensType <= 4) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_temperature(wsensNum, wsensType)
                    : 40400.0;
                *dType = TEMP;
            }
            if(wsensType == 5) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_humidity(wsensNum)
                    : 40400.0;
                *dType = HUM;
            }
            if(wsensType == 6) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_pressure(wsensNum)
                    : 40400.0;
                *dType = PRES;
            }
            if(wsensType == 7) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_co2(wsensNum)
                    : 40400.0;
                *dType = CO2;
            }
        }; break;
        case BME680_SOURCE: { // BME680
            if(type == 0) {
                data = sensors.get_bme680_temp();
                *dType = TEMP;
            }
            if(type == 1) {
                data = sensors.get_bme680_hum();
                *dType = HUM;
            }
            if(type == 2) {
                data = sensors.get_bme680_pres();
                *dType = PRES;
            }
            if(type == 3) {
                data = sensors.get_bme680_iaq();
                *dType = IAQ;
            }
        }; break;
        default: ; break;
    }

    return data;
}