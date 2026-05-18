#include "agregateLcdData.hpp"

#include "../../globals.hpp"
#include "../../config/config.hpp"
#include "../../sensors/sensors.hpp"
#include "../../weather/weather.hpp"
#include "../../cloud/thingspeak/thingspeak.hpp"
#include "../../wsensor/wirelessSensor.hpp"

float AgregateLcdData::humIn(float* humSequence) {
    float hum = UNDEFINED_FLOAT;
    switch(config.display.source.humIn.sens()) {
        case WEATHER_HUM_IN: { /* humudity from weather forecast */
            hum = weather.get_currentHum();
        }; break;
        case WSENSOR_HUM_IN: { /* humidity from wireless sensor */
            if(wsensor.dataRelevance(config.display.source.humIn.wsensNum()))
                hum = wsensor.get_humidity(config.display.source.humIn.wsensNum());
        }; break;
        case THING_HUM_IN: { /* humidity from thingspeak */
            if(thingspeak.dataRelevance()) 
                hum = thingspeak.get_field(config.display.source.humIn.thing());
        }; break;
        case SEQUENCE_HUM_IN: { /* humidity from sequence */
            _humSequence(humSequence);
        }; break;
        case BME280_HUM_IN: { /* humidity from BME280 */
            hum = sensors.get_bme280_hum();
        }; break;
        case SHT21_HUM_IN: { /* humidity from SHT21 */
            hum = sensors.get_sht21_hum();
        }; break;
        case DHT22_HUM_IN: { /* humidity from DHT22 */
            hum = sensors.get_dht22_hum();
        }; break;
        case BME680_HUM_IN: { /* humidity from BME680 */
            hum = sensors.get_bme680_hum();
        }; break;
        default: ; break;
    }
    return hum;
}