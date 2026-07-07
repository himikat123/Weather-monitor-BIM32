#include "bsec.h" // v1.8.1492 https://www.bosch-sensortec.com/software-tools/software/bsec/
#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include "FS.h"
#include <LittleFS.h>
#include <TimeLib.h>

#include "./sensors.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/**
 * Initialize BME680 sensor
 */
void Sensors::_BME680Init(void) {
    const uint8_t bsec_config_iaq[] = {
        #include "config/generic_33v_3s_4d/bsec_iaq.txt"
    };
  
    iaqSensor.begin(BME68X_I2C_ADDR_HIGH, Wire);
    _bme680_det = _BME680_validateIaqSensorStatus();
    if(_bme680_det) {
        iaqSensor.setConfig(bsec_config_iaq);
        _BME680_validateIaqSensorStatus();
        _BME680_loadState();

        bsec_virtual_sensor_t sensorList[4] = {
            BSEC_OUTPUT_IAQ,
            BSEC_OUTPUT_RAW_PRESSURE,
            BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
            BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY
        };
        iaqSensor.updateSubscription(sensorList, 4, BSEC_SAMPLE_RATE_LP);
        _BME680_validateIaqSensorStatus();
    }
}

/*
 * Check BME680 Status
 * @return true if there are no problems
 */
bool Sensors::_BME680_validateIaqSensorStatus(void) {
    if(iaqSensor.bsecStatus != BSEC_OK) return false;
    if(iaqSensor.bme68xStatus != BME68X_OK) return false;
    return true;
}

/**
 * Load BME680 state from EEPROM
 */
void Sensors::_BME680_loadState(void) {
    Serial.println(SEPARATOR);
    Serial.print("Read BME680 state file... ");

    File file = LittleFS.open("/bme680.json");
    if(file) {
        while(file.available()) {
            String json = file.readString();

            JsonDocument state;
            DeserializationError errorState = deserializeJson(state, json);

            if(!errorState) {
                _bme680_stateTimestamp = state["timestamp"];
                _bme680_stateCounter = state["counter"];
                for(uint8_t i=0; i<BSEC_MAX_STATE_BLOB_SIZE; i++) {
                    _bme680_bsecState[i] = state["data"][i];
                }

                if(_bme680_stateCounter > 0) {
                    iaqSensor.setState(_bme680_bsecState);
                    _BME680_validateIaqSensorStatus();

                    Serial.println("done");
                }
                else Serial.println("state file is empty");
            }
            else {
                Serial.println(" BME680 state file corrupted");
                break;
            }
        }
    }
    else Serial.println(" No BME680 file found");
}

/**
 * Update BME680 status file
 */
void Sensors::_BME680_updateState(void) {
    #define STATE_SAVE_PERIOD  UINT32_C(6 * 60 * 60)           /* 6 hours - 4 times a day */

    if((now() - _bme680_stateTimestamp >= STATE_SAVE_PERIOD) and (iaqSensor.iaqAccuracy >= 3)) {
        Serial.println(SEPARATOR);
        Serial.print("Update BME680 state file... ");

        if(LittleFS.exists("/bme680.json")) {
            iaqSensor.getState(_bme680_bsecState);
            _BME680_validateIaqSensorStatus();

            _bme680_stateTimestamp = now();
            _bme680_stateCounter++;

            char datetime[20];
            sprintf(datetime, "%02d.%02d.%d %02d:%02d:%02d", day(), month(), year(), hour(), minute(), second());

            JsonDocument json;
            json["timestamp"] = _bme680_stateTimestamp;
            json["datetime"] = datetime;
            json["counter"] = _bme680_stateCounter;
            for(uint8_t i=0; i<BSEC_MAX_STATE_BLOB_SIZE; i++) {
                json["data"][i] = _bme680_bsecState[i];
            }

            String data = "";
            serializeJsonPretty(json, data);
            //Serial.println(data);

            File file = LittleFS.open("/bme680.json", "w");
            if(file) {
                file.print(data);
                file.close();
                data = String();
                Serial.println("done");
            }
            else Serial.println("Failed to save BME680 state file");
        }
        else Serial.println("File /bme680.json does not extist");
    }
}

/**
 * Read data from BME680 sensor
 */
void Sensors::BME680Read(void) {
    if(_bme680_det) {
        if(iaqSensor.run()) {
            state.bme680.temp = iaqSensor.temperature;
            state.bme680.hum = iaqSensor.humidity;
            state.bme680.pres = iaqSensor.pressure / 100.0F;
            state.bme680.iaq = iaqSensor.iaq;
            state.bme680.iaqAccr = iaqSensor.iaqAccuracy;
            state.bme680.updated = true;
            _BME680_updateState();
        }
        else _BME680_validateIaqSensorStatus();
    }
    else {
        state.bme680.temp = 40400.0;
        state.bme680.hum = 40400.0;
        state.bme680.pres = 40400.0;
        state.bme680.iaq = 40400.0;
    }
}

float Sensors::get_bme680_temp() {
    return state.bme680.temp + config.sensors.bme680.tempCorr();
}

float Sensors::get_bme680_hum() {
    return state.bme680.hum + config.sensors.bme680.humCorr();
}

float Sensors::get_bme680_pres() {
    float pres = state.bme680.pres;
    return (config.units_pres() ? pres : mmHg(pres)) + config.sensors.bme680.presCorr();
}

float Sensors::get_bme680_iaq() {
    return state.bme680.iaq + config.sensors.bme680.iaqCorr();
}

unsigned int Sensors::get_bme680_iaq_accuracy() {
    return state.bme680.iaqAccr;
}