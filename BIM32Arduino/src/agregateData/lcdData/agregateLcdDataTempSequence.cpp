#include "agregateLcdData.hpp"

#include "../../globals.hpp"
#include "../../config.hpp"
#include "../../sensors.hpp"
#include "../../weather.hpp"
#include "../../thingspeak.hpp"
#include "../../wirelessSensor.hpp"

void AgregateLcdData::_tempSequence(float* tempSequence) {
    for(uint8_t i=0; i<4; i++) {
        tempSequence[i] = UNDEFINED_FLOAT;
        switch(config.display.source.sequence.temp(i)) {
            case WEATHER_TEMP_SEQUENCE: { /* Forecast */
                tempSequence[i] = weather.get_currentTemp();
            }; break;
            case WSENSOR_TEMP_SEQUENCE: { /* wireless sensor */
                if(wsensor.dataRelevance(config.display.source.sequence.wsenstemp(i, 0)))
                    tempSequence[i] = wsensor.get_temperature(
                        config.display.source.sequence.wsenstemp(i, 0),
                        config.display.source.sequence.wsenstemp(i, 1)
                    );
            }; break;
            case THING_TEMP_SEQUENCE: { /* thingspeak */
                if(thingspeak.dataRelevance())
                    tempSequence[i] = thingspeak.get_field(config.display.source.sequence.thngtemp(i));
            }; break;
            case BME280_TEMP_SEQUENCE: { /* BME280 */
                tempSequence[i] = sensors.get_bme280_temp();
            }; break;
            case BMP180_TEMP_SEQUENCE: { /* BMP180 */
                tempSequence[i] = sensors.get_bmp180_temp();
            }; break;
            case SHT21_TEMP_SEQUENCE: { /* SHT21 */
                tempSequence[i] = sensors.get_sht21_temp();
            }; break;
            case DHT22_TEMP_SEQUENCE: { /* DHT22 */
                tempSequence[i] = sensors.get_dht22_temp();
            }; break;
            case DS18B20_TEMP_SEQUENCE: { /* DS18B20 */
                tempSequence[i] = sensors.get_ds18b20_temp();
            }; break;
            case BME680_TEMP_SEQUENCE: { /* BME680 */
                tempSequence[i] = sensors.get_bme680_temp();
            }; break;
            default: ; break;
        }
    }
}