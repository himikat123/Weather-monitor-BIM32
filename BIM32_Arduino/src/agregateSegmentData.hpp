#define CLOCK 0
#define DATE 1
#define TEMP 2
#define HUM 3
#define PRES 4
#define CO2 5
#define IAQ 6

class AgregateSegmentData {
    public:
        float slotData(uint8_t sensor, uint8_t type, uint8_t timeSlot, uint8_t dispNum, uint8_t* dType, bool* clockDots);
};

float AgregateSegmentData::slotData(uint8_t sensor, uint8_t type, uint8_t timeSlot, uint8_t dispNum, uint8_t* dType, bool* clockDots) {
    float data = -40400.0;
    *clockDots = false;

    switch(sensor) {
        case 0: { // Clock 
            *clockDots = true;
            *dType = CLOCK;
        }; break;
        case 1: { // Date
            *dType = DATE;
        }; break;
        case 2: { // BME280
            if(type == 0) {
                data = sensors.get_bme280_temp(CORRECTED);
                *dType = TEMP;
            }
            if(type == 1) {
                data = sensors.get_bme280_hum(CORRECTED);
                *dType = HUM;
            }
            if(type == 2) {
                data = sensors.get_bme280_pres(CORRECTED);
                *dType = PRES;
            }
        }; break;
        case 3: {// BMP180
            if(type == 0) {
                data = sensors.get_bmp180_temp(CORRECTED);
                *dType = TEMP;
            }
            if(type == 1) {
                data = sensors.get_bmp180_pres(CORRECTED);
                *dType = PRES;
            }
        }; break;
        case 4: {// SHT21
            if(type == 0) {
                data = sensors.get_sht21_temp(CORRECTED);
                *dType = TEMP;
            }
            if(type == 1) {
                data = sensors.get_sht21_hum(CORRECTED);
                *dType = HUM;
            }
        }; break;
        case 5: { // DHT22
            if(type == 0) {
                data = sensors.get_dht22_temp(CORRECTED);
                *dType = TEMP;
            }
            if(type == 1) {
                data = sensors.get_dht22_hum(CORRECTED);
                *dType = HUM;
            }
        }; break;
        case 6: { // DS18B20
            data = sensors.get_ds18b20_temp(CORRECTED);
            *dType = TEMP;
        }; break;
        case 7: { // ESP32
            data = sensors.get_esp32_temp(CORRECTED);
            *dType = TEMP;
        }; break;
        case 8: { // Thingspeak
            data = thingspeak.dataRelevance()
                ? thingspeak.get_field(config.display_timeSlot_thing(timeSlot, dispNum))
                : 40400.0;
            if(type == 0) *dType = TEMP;
            if(type == 1) *dType = HUM;
            if(type == 2) *dType = PRES;
        }; break;
        case 9: { // Weather
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
        case 10: { // Wireless sensor
            unsigned int wsensNum = config.display_timeSlot_wsensor_num(timeSlot, dispNum);
            unsigned int wsensType = config.display_timeSlot_wsensor_type(timeSlot, dispNum);
            if(wsensType <= 4) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_temperature(wsensNum, wsensType, CORRECTED)
                    : 40400.0;
                *dType = TEMP;
            }
            if(wsensType == 5) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_humidity(wsensNum, CORRECTED)
                    : 40400.0;
                *dType = HUM;
            }
            if(wsensType == 6) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_pressure(wsensNum, CORRECTED)
                    : 40400.0;
                *dType = PRES;
            }
            if(wsensType == 7) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_co2(wsensNum, CORRECTED)
                    : 40400.0;
                *dType = CO2;
            }
        }; break;
        case 11: { // BME680
            if(type == 0) {
                data = sensors.get_bme680_temp(CORRECTED);
                *dType = TEMP;
            }
            if(type == 1) {
                data = sensors.get_bme680_hum(CORRECTED);
                *dType = HUM;
            }
            if(type == 2) {
                data = sensors.get_bme680_pres(CORRECTED);
                *dType = PRES;
            }
            if(type == 3) {
                data = sensors.get_bme680_iaq(CORRECTED);
                *dType = IAQ;
            }
        }; break;
        default: ; break;
    }

    return data;
}