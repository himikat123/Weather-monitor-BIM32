#include "agregateLcdData.hpp"

#include "../../globals.hpp"
#include "../../config/config.hpp"
#include "../../sensors/sensors.hpp"
#include "../../weather/weather.hpp"
#include "../../cloud/thingspeak/thingspeak.hpp"
#include "../../wsensor/wirelessSensor.hpp"

float AgregateLcdData::humOut() {
    float hum = UNDEFINED_FLOAT;
    switch(config.display.source.humOut.sens()) {
        case WEATHER_HUM_OUT: { /* humudity from weather forecast */
            hum = weather.get_currentHum();
        }; break;
        case WSENSOR_HUM_OUT: { /* humidity from wireless sensor */
            if(wsensor.dataRelevance(config.display.source.humOut.wsensNum()))
                hum = wsensor.get_humidity(config.display.source.humOut.wsensNum());
        }; break;
        case THING_HUM_OUT: { /* humidity from thingspeak */
            if(thingspeak.dataRelevance()) 
                hum = thingspeak.get_field(config.display.source.humOut.thing());
        }; break;
        case BME280_HUM_OUT: { /* humidity from BME280 */
            hum = sensors.get_bme280_hum();
        }; break;
        case SHT21_HUM_OUT: { /* humidity from SHT21 */
            hum = sensors.get_sht21_hum();
        }; break;
        case DHT22_HUM_OUT: { /* humidity from DHT22 */
            hum = sensors.get_dht22_hum();
        }; break;
        case BME680_HUM_OUT: { /* humidity from BME680 */
            hum = sensors.get_bme680_hum();
        }; break;
        default: ; break;
    }
    return hum;
}