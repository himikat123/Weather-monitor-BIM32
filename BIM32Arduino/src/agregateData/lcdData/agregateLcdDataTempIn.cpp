#include "agregateLcdData.hpp"

#include "../../globals.hpp"
#include "../../config.hpp"
#include "../../sensors.hpp"
#include "../../weather.hpp"
#include "../../thingspeak.hpp"
#include "../../wirelessSensor.hpp"

float AgregateLcdData::tempIn(float* tempSequence) {
    float temp = UNDEFINED_FLOAT;
    switch(config.display.source.tempIn.sens()) {
        case WEATHER_TEMP_IN: { /* temperature from weather forecast */
            temp = weather.get_currentTemp();
        }; break;
        case WSENSOR_TEMP_IN: { /* temperature from wireless sensor */
            if(wsensor.dataRelevance(config.display.source.tempIn.wsensNum())) 
                temp = wsensor.get_temperature(
                    config.display.source.tempIn.wsensNum(), 
                    config.display.source.tempIn.temp()
                );
        }; break;
        case THING_TEMP_IN: { /* temperature from thingspeak */
            if(thingspeak.dataRelevance())
                temp = thingspeak.get_field(config.display.source.tempIn.thing());
        }; break;
        case SEQUENCE_TEMP_IN: { /* temperature from sequence */
            _tempSequence(tempSequence);
        }; break;
        case BME280_TEMP_IN: { /* temperature from BME280 */
            temp = sensors.get_bme280_temp();
        }; break;
        case BMP180_TEMP_IN: { /* temperature from BMP180 */
            temp = sensors.get_bmp180_temp();
        }; break;
        case SHT21_TEMP_IN: { /* temperature from SHT21 */
            temp = sensors.get_sht21_temp();
        }; break;
        case DHT22_TEMP_IN: { /* temperature from DHT22 */
            temp = sensors.get_dht22_temp();
        }; break;
        case DS18B20_TEMP_IN: { /* temperature from DS18B20 */
            temp = sensors.get_ds18b20_temp();
        }; break;
        case BME680_TEMP_IN: { /* temperature from BME680 */
            temp = sensors.get_bme680_temp();
        }; break;
        default: ; break;
    }
    return temp;
}