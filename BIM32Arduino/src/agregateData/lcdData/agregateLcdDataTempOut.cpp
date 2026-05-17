#include "agregateLcdData.hpp"

#include "../../globals.hpp"
#include "../../config/config.hpp"
#include "../../sensors/sensors.hpp"
#include "../../weather/weather.hpp"
#include "../../cloud/thingspeak/thingspeak.hpp"
#include "../../wsensor/wirelessSensor.hpp"

float AgregateLcdData::tempOut() {
    float temp = UNDEFINED_FLOAT;
    switch(config.display.source.tempOut.sens()) {
        case WEATHER_TEMP_OUT: { /* temperature from weather forecast */
            temp = weather.get_currentTemp();
        }; break;
        case WSENSOR_TEMP_OUT: { /* temperature from wireless sensor */
            if(wsensor.dataRelevance(config.display.source.tempOut.wsensNum())) 
                temp = wsensor.get_temperature(
                    config.display.source.tempOut.wsensNum(), 
                    config.display.source.tempOut.temp()
                );
        }; break;
        case THING_TEMP_OUT: { /* temperature from thingspeak */
            if(thingspeak.dataRelevance())
                temp = thingspeak.get_field(config.display.source.tempOut.thing());
        }; break;
        case BME280_TEMP_OUT: { /* temperature from BME280 */
            temp = sensors.get_bme280_temp();
        }; break;
        case BMP180_TEMP_OUT: { /* temperature from BMP180 */
            temp = sensors.get_bmp180_temp();
        }; break;
        case SHT21_TEMP_OUT: { /* temperature from SHT21 */
            temp = sensors.get_sht21_temp();
        }; break;
        case DHT22_TEMP_OUT: { /* temperature from DHT22 */
            temp = sensors.get_dht22_temp();
        }; break;
        case DS18B20_TEMP_OUT: { /* temperature from DS18B20 */
            temp = sensors.get_ds18b20_temp();
        }; break;
        case BME680_TEMP_OUT: { /* temperature from BME680 */
            temp = sensors.get_bme680_temp();
        }; break;
        default: ; break;
    }
    return temp;
}