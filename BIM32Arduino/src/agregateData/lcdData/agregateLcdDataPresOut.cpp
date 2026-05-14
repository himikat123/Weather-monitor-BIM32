#include "agregateLcdData.hpp"

#include "../../globals.hpp"
#include "../../config.hpp"
#include "../../sensors.hpp"
#include "../../weather.hpp"
#include "../../thingspeak.hpp"
#include "../../wirelessSensor.hpp"

float AgregateLcdData::presOut() {
    float pres = UNDEFINED_FLOAT;
    switch(config.display.source.presOut.sens()) {
        case WEATHER_PRES_OUT: { // pressure outside from weather forecast
            pres = weather.get_currentPres();
        }; break;
        case WSENSOR_PRES_OUT: { // presure outside from wireless sensor
            if(wsensor.dataRelevance(config.display.source.presOut.wsensNum()))
                pres = wsensor.get_pressure(config.display.source.presOut.wsensNum());
        }; break;
        case THING_PRES_OUT: { // presure outside from thingspeak
            if(thingspeak.dataRelevance()) 
                pres = thingspeak.get_field(config.display.source.presOut.thing());
        }; break;
        case BME280_PRES_OUT: { // pressure outside from BME280
            pres = sensors.get_bme280_pres();
        }; break;
        case BMP180_PRES_OUT: { // pressure outside from BMP180
            pres = sensors.get_bmp180_pres();
        }; break;
        case BME680_PRES_OUT: { // pressure outside from BME680
            pres = sensors.get_bme680_pres();
        }; break;
    }
    return pres;
}