#include "agregateLcdData.hpp"

#include "../../config/config.hpp"
#include "../../sensors/sensors.hpp"
#include "../../weather/weather.hpp"

String AgregateLcdData::voltage() {
    String value = "";
    switch(config.display.source.volt.sens()) {
        case WSENSOR_VOLT: value = _voltageWsensor(); break; /* from wireless sensor */
        case THING_VOLT: value = _voltageThingspeak(); break; /* from thingspeak */
        case DATE: value = _localDate(); break; /* date */
        case BME680_IAQ: value = _iaq(); break; // iaq from BME680
        case BME680_ABS_HUM: value = _absoluteHum(sensors.get_bme680_temp(), sensors.get_bme680_hum()); break; // absolute humidity from BME680
        case BME680_DEW_POINT: value = _dewPoint(sensors.get_bme680_temp(), sensors.get_bme680_hum()); break; // dew point from BME680
        case BME280_ABS_HUM: value = _absoluteHum(sensors.get_bme280_temp(), sensors.get_bme280_hum()); break; // absolute humidity from BME280
        case BME280_DEW_POINT: value = _dewPoint(sensors.get_bme280_temp(), sensors.get_bme280_hum()); break; // dew point from BME280
        case DHT22_ABS_HUM: value = _absoluteHum(sensors.get_dht22_temp(), sensors.get_dht22_hum()); break; // absolute humidity from DHT22
        case DHT22_DEW_POINT: value = _dewPoint(sensors.get_dht22_temp(), sensors.get_dht22_hum()); break; // dew point from DHT22
        case SHT21_ABS_HUM: value = _absoluteHum(sensors.get_sht21_temp(), sensors.get_sht21_hum()); break; // absolute humidity from SHT21
        case SHT21_DEW_POINT: value = _dewPoint(sensors.get_sht21_temp(), sensors.get_sht21_hum()); break; // dew point from SHT21
        case WEATHER_ABS_HUM: value = _absoluteHum(weather.get_currentTemp(), weather.get_currentHum()); break; // absolute humidity from weather forecast
        case WEATHER_DEW_POINT: value = _dewPoint(weather.get_currentTemp(), weather.get_currentHum()); break; // dew point from weather forecast
        default: ; break;
    }
    return value;
}