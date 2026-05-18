#include <Arduino.h>
#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include "./config.hpp"

// #include "structures/comfort.hpp"
// #include "structures/network.hpp"
// #include "structures/accessPoint.hpp"
// #include "structures/weather.hpp"
// #include "structures/clock.hpp"
// #include "structures/display/display.hpp"
// #include "structures/sound.hpp"
// #include "structures/sensors.hpp"
// #include "structures/wsensor.hpp"
// #include "structures/cloud/cloud.hpp"
// #include "structures/alarm.hpp"
// #include "structures/account.hpp"

void readConfig(void) {
    /* Read config file */
    Serial.println(SEPARATOR);
    Serial.print("Read config file... ");
    File file = LittleFS.open("/config.json");
    if(file) {
        while(file.available()) {
            //String json = file.readString();
            JsonDocument conf;

            DeserializationError error = deserializeJson(conf, /*json*/ file);
            if(!error) {
                //WiFi network
                for(unsigned int i=0; i<NETWORKS; i++) {
                    _copy(conf["network"]["ssid"][i], network._ssid[i]);
                    _copy(conf["network"]["pass"][i], network._pass[i]);
                }
                _copy(conf["network"]["type"], network._type);
                _copy(conf["network"]["ip"], network._ip);
                _copy(conf["network"]["mask"], network._mask);
                _copy(conf["network"]["gw"], network._gw);
                _copy(conf["network"]["dns1"], network._dns1);
                _copy(conf["network"]["dns2"], network._dns2);

                // Access point
                _copy(conf["accessPoint"]["ssid"], accessPoint._ssid);
                _copy(conf["accessPoint"]["pass"], accessPoint._pass);
                _copy(conf["accessPoint"]["chnl"], accessPoint._chnl);
                _copy(conf["accessPoint"]["ip"], accessPoint._ip);
                _copy(conf["accessPoint"]["mask"], accessPoint._mask);

                // Weather
                for(unsigned int i=0; i<PROVIDERS; i++) _copy(conf["weather"]["appid"][i], weather._appid[i]);
                _copy(conf["weather"]["city"], weather._city);
                _copy(conf["weather"]["cityid"], weather._cityid);
                _copy(conf["weather"]["lon"], weather._lon);
                _copy(conf["weather"]["lat"], weather._lat);
                _copy(conf["weather"]["provider"], weather._provider);
                _copy(conf["weather"]["citysearch"], weather._citysearch);
                _copy(conf["weather"]["corr"]["t"], weather._tempCorr);
                _copy(conf["weather"]["corr"]["h"], weather._humCorr);
                _copy(conf["weather"]["corr"]["p"], weather._presCorr);

                // Localization 
                _copy(conf["lang"], _lang);
                _copy(conf["units"]["pres"], _units_pres);

                // Clock
                _copy(conf["clock"]["format"], clock._format);
                _copy(conf["clock"]["ntp"], clock._ntp);
                _copy(conf["clock"]["utc"], clock._utc);
                _copy(conf["clock"]["dlst"], clock._dlst);
                _copy(conf["clock"]["ntp_period"], clock._ntpPeriod);

                // Display
                for(unsigned int i=0; i<DISPLAYS; i++) {
                    _copy(conf["display"]["type"][i], display._type[i]);
                    _copy(conf["display"]["model"][i], display._model[i]);
                    for(unsigned int o=0; o<8; o++) {
                        _copy(conf["display"]["order"][i][o], display._order[i][o]);
                    }
                    _copy(conf["display"]["dayTime"][i], display._dayTime[i]);
                    _copy(conf["display"]["nightTime"][i], display._nightTime[i]);
                    _copy(conf["display"]["autoOff"][i], display._autoOff[i]);
                    _copy(conf["display"]["nightOff"]["need"][i], display.nightOff._need[i]);
                    _copy(conf["display"]["nightOff"]["from"][i], display.nightOff._from[i]);
                    _copy(conf["display"]["nightOff"]["to"][i], display.nightOff._to[i]);
                    _copy(conf["display"]["brightMethod"][i], display.brightness._method[i]);
                    _copy(conf["display"]["brightness"]["day"][i], display.brightness._day[i]);
                    _copy(conf["display"]["brightness"]["night"][i], display.brightness._night[i]);
                    _copy(conf["display"]["brightness"]["min"][i], display.brightness._min[i]);
                    _copy(conf["display"]["brightness"]["max"][i], display.brightness._max[i]);
                    _copy(conf["display"]["lightSensor"][i], display.lightSensor._type[i]);
                    _copy(conf["display"]["lightSensor_sensitivity"][i], display.lightSensor._sensitivity[i]);
                    _copy(conf["display"]["animation"]["type"][i], display.animation._type[i]);
                    _copy(conf["display"]["animation"]["speed"][i], display.animation._speed[i]);
                    _copy(conf["display"]["animation"]["points"][i], display.animation._points[i]);
                    for(unsigned int t=0; t<TIMESLOTS; t++) {
                        _copy(conf["display"]["timeSlot"]["period"][t][i], display.timeSlot._period[t][i]);
                        _copy(conf["display"]["timeSlot"]["sensor"][t][i], display.timeSlot._sensor[t][i]);
                        _copy(conf["display"]["timeSlot"]["data"][t][i], display.timeSlot._data[t][i]);
                        _copy(conf["display"]["timeSlot"]["thing"][t][i], display.timeSlot._thing[t][i]);
                        _copy(conf["display"]["timeSlot"]["color"][t][i], display.timeSlot._color[t][i]);
                        _copy(conf["display"]["timeSlot"]["wsensor"]["num"][t][i], display.timeSlot.wsensor._num[t][i]);
                        _copy(conf["display"]["timeSlot"]["wsensor"]["type"][t][i], display.timeSlot.wsensor._type[t][i]); 
                    }
                }
                _copy(conf["display"]["source"]["tempOut"]["sens"], display.source.tempOut._sens);
                _copy(conf["display"]["source"]["tempOut"]["wsensNum"], display.source.tempOut._wsensNum);
                _copy(conf["display"]["source"]["tempOut"]["temp"], display.source.tempOut._temp);
                _copy(conf["display"]["source"]["tempOut"]["thing"], display.source.tempOut._thing);
                _copy(conf["display"]["source"]["humOut"]["sens"], display.source.humOut._sens);
                _copy(conf["display"]["source"]["humOut"]["wsensNum"], display.source.humOut._wsensNum);
                _copy(conf["display"]["source"]["humOut"]["thing"], display.source.humOut._thing);
                _copy(conf["display"]["source"]["presOut"]["sens"], display.source.presOut._sens);
                _copy(conf["display"]["source"]["presOut"]["wsensNum"], display.source.presOut._wsensNum);
                _copy(conf["display"]["source"]["presOut"]["thing"], display.source.presOut._thing);
                _copy(conf["display"]["source"]["tempIn"]["sens"], display.source.tempIn._sens);
                _copy(conf["display"]["source"]["tempIn"]["wsensNum"], display.source.tempIn._wsensNum);
                _copy(conf["display"]["source"]["tempIn"]["temp"], display.source.tempIn._temp);
                _copy(conf["display"]["source"]["tempIn"]["thing"], display.source.tempIn._thing);
                _copy(conf["display"]["source"]["humIn"]["sens"], display.source.humIn._sens);
                _copy(conf["display"]["source"]["humIn"]["wsensNum"], display.source.humIn._wsensNum);
                _copy(conf["display"]["source"]["humIn"]["thing"], display.source.humIn._thing);
                _copy(conf["display"]["source"]["volt"]["sens"], display.source.volt._sens);
                _copy(conf["display"]["source"]["volt"]["wsensNum"], display.source.volt._wsensNum);
                _copy(conf["display"]["source"]["volt"]["volt"], display.source.volt._volt);
                _copy(conf["display"]["source"]["volt"]["thing"], display.source.volt._thing);
                _copy(conf["display"]["source"]["volt"]["thingType"], display.source.volt._thingType);
                _copy(conf["display"]["source"]["bat"]["sens"], display.source.bat._sens);
                _copy(conf["display"]["source"]["bat"]["wsensNum"], display.source.bat._wsensNum);
                _copy(conf["display"]["source"]["bat"]["thing"], display.source.bat._thing);
                _copy(conf["display"]["source"]["descr"], display.source._descr);
                _copy(conf["display"]["source"]["wind"]["speed"]["sens"], display.source.wind.speed._sens);
                _copy(conf["display"]["source"]["wind"]["speed"]["wsensNum"], display.source.wind.speed._wsensNum);
                _copy(conf["display"]["source"]["wind"]["speed"]["thing"], display.source.wind.speed._thing);
                _copy(conf["display"]["source"]["wind"]["dir"]["sens"], display.source.wind.dir._sens);
                _copy(conf["display"]["source"]["wind"]["dir"]["wsensNum"], display.source.wind.dir._wsensNum);
                _copy(conf["display"]["source"]["wind"]["dir"]["thing"], display.source.wind.dir._thing);
                _copy(conf["display"]["source"]["sequence"]["dur"], display.source.sequence._dur);
                for(unsigned int i=0; i<SEQUENCES; i++) {
                    _copy(conf["display"]["source"]["sequence"]["name"][i], display.source.sequence._name[i]);
                    _copy(conf["display"]["source"]["sequence"]["temp"][i], display.source.sequence._temp[i]);
                    _copy(conf["display"]["source"]["sequence"]["thngtemp"][i], display.source.sequence._thngtemp[i]);
                    _copy(conf["display"]["source"]["sequence"]["hum"][i], display.source.sequence._hum[i]);
                    _copy(conf["display"]["source"]["sequence"]["thnghum"][i], display.source.sequence._thnghum[i]);
                    _copy(conf["display"]["source"]["sequence"]["wsenshum"][i], display.source.sequence._wsenshum[i]);
                    for(unsigned int k=0; k<WSENSORS; k++) {
                        _copy(conf["display"]["source"]["sequence"]["wsenstemp"][i][k], display.source.sequence._wsenstemp[i][k]);
                    }
                }

                // Sound
                _copy(conf["sound"]["vol"], sound._vol);
                _copy(conf["sound"]["eq"], sound._eq);
                _copy(conf["sound"]["hourly"], sound._hourly);
                _copy(conf["sound"]["hour"]["from"], sound._hourFrom);
                _copy(conf["sound"]["hour"]["to"], sound._hourTo);

                // Sensors
                _copy(conf["sensors"]["bme280"]["t"], sensors.bme280._tempCorr);
                _copy(conf["sensors"]["bme280"]["h"], sensors.bme280._humCorr);
                _copy(conf["sensors"]["bme280"]["p"], sensors.bme280._presCorr);
                _copy(conf["sensors"]["bmp180"]["t"], sensors.bmp180._tempCorr);
                _copy(conf["sensors"]["bmp180"]["p"], sensors.bmp180._presCorr);
                _copy(conf["sensors"]["sht21"]["t"], sensors.sht21._tempCorr);
                _copy(conf["sensors"]["sht21"]["h"], sensors.sht21._humCorr);
                _copy(conf["sensors"]["dht22"]["t"], sensors.dht22._tempCorr);
                _copy(conf["sensors"]["dht22"]["h"], sensors.dht22._humCorr);
                _copy(conf["sensors"]["ds18b20"]["t"], sensors.ds18b20._tempCorr);
                _copy(conf["sensors"]["esp32"]["t"], sensors.esp32._tempCorr);
                _copy(conf["sensors"]["max44009"]["l"], sensors.max44009._lightCorr);
                _copy(conf["sensors"]["bh1750"]["l"], sensors.bh1750._lightCorr);
                _copy(conf["sensors"]["analog"]["v"], sensors.analog._voltageCorr);
                _copy(conf["sensors"]["bme680"]["t"], sensors.bme680._tempCorr);
                _copy(conf["sensors"]["bme680"]["h"], sensors.bme680._humCorr);
                _copy(conf["sensors"]["bme680"]["p"], sensors.bme680._presCorr);
                _copy(conf["sensors"]["bme680"]["i"], sensors.bme680._iaqCorr);

                // Wireless sensors
                for(unsigned int i=0; i<WSENSORS; i++) {
                    for(unsigned int k=0; k<WSENSOR_TEMPS; k++) {
                        _copy(conf["wsensor"]["temp"][i][k], wsensor._tempCorr[i][k]);
                    }
                    _copy(conf["wsensor"]["hum"][i], wsensor._humCorr[i]);
                    _copy(conf["wsensor"]["pres"][i], wsensor._presCorr[i]);
                    _copy(conf["wsensor"]["wind"]["speed"], wsensor._windSpeedCorr[i]);
                    _copy(conf["wsensor"]["wind"]["dir"], wsensor._windDirCorr[i]);
                    _copy(conf["wsensor"]["light"][i], wsensor._lightCorr[i]);
                    _copy(conf["wsensor"]["volt"][i], wsensor._voltCorr[i]);
                    _copy(conf["wsensor"]["curr"][i], wsensor._currCorr[i]);
                    _copy(conf["wsensor"]["pow"][i], wsensor._powCorr[i]);
                    _copy(conf["wsensor"]["enrg"][i], wsensor._enrgCorr[i]);
                    _copy(conf["wsensor"]["freq"][i], wsensor._freqCorr[i]);
                    _copy(conf["wsensor"]["co2"][i], wsensor._co2Corr[i]);
                    _copy(conf["wsensor"]["bat"]["k"][i], wsensor._batK[i]);
                    _copy(conf["wsensor"]["bat"]["type"][i], wsensor._batType[i]);
                    _copy(conf["wsensor"]["expire"][i], wsensor._expire[i]); 
                }
                _copy(conf["wsensor"]["channel"], wsensor._channel);

                // Weather history repository
                _copy(conf["history"]["period"], cloud.history._period);
                _copy(conf["history"]["channelID"], cloud.history._channelID);
                _copy(conf["history"]["wrkey"], cloud.history._wrkey);
                _copy(conf["history"]["rdkey"], cloud.history._rdkey);
                for(unsigned int i=0; i<7; i++) {
                    _copy(conf["history"]["fields"][i], cloud.history._fields[i]);
                    _copy(conf["history"]["wSensors"][i], cloud.history._wsensors[i]);
                    _copy(conf["history"]["wTypes"][i], cloud.history._wtypes[i]);
                    _copy(conf["history"]["tFields"][i], cloud.history._types[i]);
                }

                // Thingspeak send
                _copy(conf["thingspeakSend"]["period"], cloud.thingspeakSend._period);
                _copy(conf["thingspeakSend"]["channelID"], cloud.thingspeakSend._channelID);
                _copy(conf["thingspeakSend"]["wrkey"], cloud.thingspeakSend._wrkey);
                _copy(conf["thingspeakSend"]["rdkey"], cloud.thingspeakSend._rdkey);
                for(unsigned int i=0; i<THNG_FIELDS; i++) {
                    _copy(conf["thingspeakSend"]["fields"][i], cloud.thingspeakSend._fields[i]);
                    _copy(conf["thingspeakSend"]["types"][i], cloud.thingspeakSend._types[i]);
                    _copy(conf["thingspeakSend"]["wsensors"][i], cloud.thingspeakSend._wsensors[i]);
                    _copy(conf["thingspeakSend"]["wtypes"][i], cloud.thingspeakSend._wtypes[i]);
                }

                // Thingspeak receive
                _copy(conf["thingspeakReceive"]["period"], cloud.thingspeakReceive._period);
                _copy(conf["thingspeakReceive"]["channelID"], cloud.thingspeakReceive._channelID);
                _copy(conf["thingspeakReceive"]["rdkey"], cloud.thingspeakReceive._rdkey);
                _copy(conf["thingspeakReceive"]["expire"], cloud.thingspeakReceive._expire);

                // Narodmon send
                _copy(conf["narodmonSend"]["period"], cloud.narodmonSend._period);
                _copy(conf["narodmonSend"]["lon"], cloud.narodmonSend._lon);
                _copy(conf["narodmonSend"]["lat"], cloud.narodmonSend._lat);
                _copy(conf["narodmonSend"]["name"], cloud.narodmonSend._name);
                for(unsigned int i=0; i<NAROD_FIELDS; i++) {
                    _copy(conf["narodmonSend"]["sensors"][i], cloud.narodmonSend._sensors[i]);
                    _copy(conf["narodmonSend"]["metrics"][i], cloud.narodmonSend._metrics[i]);
                    _copy(conf["narodmonSend"]["types"][i], cloud.narodmonSend._types[i]);
                    _copy(conf["narodmonSend"]["wsensors"][i], cloud.narodmonSend._wsensors[i]);
                    _copy(conf["narodmonSend"]["wtypes"][i], cloud.narodmonSend._wtypes[i]);
                }

                // MQTT send
                _copy(conf["mqttSend"]["period"], cloud.mqttSend._period);
                _copy(conf["mqttSend"]["broker"], cloud.mqttSend._broker);
                _copy(conf["mqttSend"]["port"], cloud.mqttSend._port);
                _copy(conf["mqttSend"]["user"], cloud.mqttSend._user);
                _copy(conf["mqttSend"]["pass"], cloud.mqttSend._pass);
                for(unsigned int i=0; i<MQTT_TOPICS; i++) {
                    _copy(conf["mqttSend"]["sensors"][i], cloud.mqttSend._sensors[i]);
                    _copy(conf["mqttSend"]["types"][i], cloud.mqttSend._types[i]);
                    _copy(conf["mqttSend"]["wsensors"][i], cloud.mqttSend._wsensors[i]);
                    _copy(conf["mqttSend"]["wtypes"][i], cloud.mqttSend._wtypes[i]);
                    _copy(conf["mqttSend"]["topics"][i], cloud.mqttSend._topics[i]);
                }

                // Comfort
                _copy(conf["comfort"]["temp"]["source"], comfort.temp._source);
                _copy(conf["comfort"]["temp"]["wsensNum"], comfort.temp._wsensNum);
                _copy(conf["comfort"]["temp"]["sens"], comfort.temp._sens);
                _copy(conf["comfort"]["temp"]["thing"], comfort.temp._thing);
                _copy(conf["comfort"]["temp"]["min"][0], comfort.temp._min);
                _copy(conf["comfort"]["temp"]["max"][0], comfort.temp._max);
                _copy(conf["comfort"]["temp"]["min"][1], comfort.temp._minHysteresis);
                _copy(conf["comfort"]["temp"]["max"][1], comfort.temp._maxHysteresis);
                _copy(conf["comfort"]["temp"]["sound"], comfort.temp._sound);
                _copy(conf["comfort"]["hum"]["source"], comfort.hum._source);
                _copy(conf["comfort"]["hum"]["wsensNum"], comfort.hum._wsensNum);
                _copy(conf["comfort"]["hum"]["thing"], comfort.hum._thing);
                _copy(conf["comfort"]["hum"]["min"][0], comfort.hum._min);
                _copy(conf["comfort"]["hum"]["max"][0], comfort.hum._max);
                _copy(conf["comfort"]["hum"]["min"][1], comfort.hum._minHysteresis);
                _copy(conf["comfort"]["hum"]["max"][1], comfort.hum._maxHysteresis);
                _copy(conf["comfort"]["hum"]["sound"], comfort.hum._sound);
                _copy(conf["comfort"]["iaq"]["source"], comfort.iaq._source);
                _copy(conf["comfort"]["iaq"]["sound"], comfort.iaq._sound);
                _copy(conf["comfort"]["co2"]["source"], comfort.co2._source);
                _copy(conf["comfort"]["co2"]["wsensNum"], comfort.co2._wsensNum);
                _copy(conf["comfort"]["co2"]["sound"], comfort.co2._sound);

                // Account
                _copy(conf["account"]["name"], account._name);
                _copy(conf["account"]["required"], account._required);

                Serial.println("done");
            }
            else Serial.println(" Configuration file corrupted");
        }
    }
    else Serial.println(" No configuration file found");

    /* Read alarm file */
    Serial.print("Read alarm file... ");
    file = LittleFS.open("/alarm.json");
    if(file) {
        while(file.available()) {
            String json = file.readString();
            JsonDocument alarms;

            DeserializationError error = deserializeJson(alarms, json);
            if(!error) {
                // Alarm
                for(unsigned int i=0; i<ALARMS; i++) {
                    _copy(alarms["alarm"]["states"][i], alarm._states[i]);
                    _copy(alarms["alarm"]["melodies"][i], alarm._melodies[i]);
                    for(unsigned int k=0; k<2; k++) _copy(alarms["alarm"]["time"][i][k], alarm._time[i][k]);
                    for(unsigned int k=0; k<7; k++) _copy(alarms["alarm"]["weekdays"][i][k], alarm._weekdays[i][k]);
                }

                Serial.println("done");
            }
            else Serial.println(" Alarm file corrupted");
        }
    }
    else Serial.println(" No alarm file found");

    /* Read user file */
    Serial.print("Read user file... ");
    file = LittleFS.open("/user.us");
    if(file) {
        while(file.available()) {
            String json = file.readString();
            JsonDocument conf;
            DeserializationError error = deserializeJson(conf, json);
            if(!error) {
                _copy(conf["pass"], account._pass);
                Serial.println("done");
            }
            else Serial.println(" User file corrupted");
        }
    }
    else Serial.println(" No user file found");

    /* Read touch calibration file */
    Serial.print("Read touch calibration file... ");
    file = LittleFS.open("/touch.json");
    if(file) {
        while(file.available()) {
            String json = file.readString();
            JsonDocument conf;
            DeserializationError error = deserializeJson(conf, json);
            if(!error) {
                for(uint8_t i=0; i<5; i++) {
                    _copy(conf["calData"][i], _calData[i]);
                }
                Serial.println("done");
            }
            else Serial.println(" Touch calibration file corrupted");
        }
    }
    else Serial.println(" No touch calibration file found");
}