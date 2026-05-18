#include "agregateLcdData.hpp"

#include "../../globals.hpp"
#include "../../config/config.hpp"
#include "../../sensors/sensors.hpp"
#include "../../weather/weather.hpp"
#include "../../cloud/thingspeak/thingspeak.hpp"
#include "../../wsensor/wirelessSensor.hpp"

void AgregateLcdData::_humSequence(float* humSequence) {
    for(uint8_t i=0; i<4; i++) {
        humSequence[i] = UNDEFINED_FLOAT;
        switch(config.display.source.sequence.hum(i)) {
            case WEATHER_HUM_SEQUENCE: { /* Forecast */
                humSequence[i] = weather.get_currentHum();
            }; break;
            case WSENSOR_HUM_SEQUENCE: { /* wireless sensor */
                if(wsensor.dataRelevance(config.display.source.sequence.wsenshum(i)))
                    humSequence[i] = wsensor.get_humidity(config.display.source.sequence.wsenshum(i));
            }; break;
            case THING_HUM_SEQUENCE: { /* thingspeak */
                if(thingspeak.dataRelevance())
                    humSequence[i] = thingspeak.get_field(config.display.source.sequence.thnghum(i));
            }; break;
            case BME280_HUM_SEQUENCE: { /* BME280 */
                humSequence[i] = sensors.get_bme280_hum();
            }; break;
            case SHT21_HUM_SEQUENCE: { /* SHT21 */
                humSequence[i] = sensors.get_sht21_hum();
            }; break;
            case DHT22_HUM_SEQUENCE: { /* DHT22 */
                humSequence[i] = sensors.get_dht22_hum();
            }; break;
            case BME680_HUM_SEQUENCE: { /* BME680 */
                humSequence[i] = sensors.get_bme680_hum();
            }; break;
            default: ; break;
        }
    }
}