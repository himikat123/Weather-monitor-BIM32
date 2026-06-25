#include <Arduino.h>

#include "./wirelessSensor.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../globals.hpp"
 
time_t WirelessSensor::get_updated(unsigned int num) {
    if(num >= WSENSORS) return -1;
    return state.wsensor.time[num];
}

float WirelessSensor::get_temperature(unsigned int num, unsigned int sensor) {
    if(num >= WSENSORS or sensor > 4) return 40400.0;
    return state.wsensor.temp.data[sensor][num] + config.wsensor.tempCorr(num, sensor);
}

float WirelessSensor::get_humidity(unsigned int num) {
    if(num >= WSENSORS) return 40400.0;
    return state.wsensor.hum.data[num] + config.wsensor.humCorr(num);
}

float WirelessSensor::get_pressure(unsigned int num) {
    if(num >= WSENSORS) return 40400.0;
    return state.wsensor.pres.data[num] + config.wsensor.presCorr(num);
}

float WirelessSensor::get_windSpeed(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.wind.speed.data[num] + config.wsensor.windSpeedCorr(num);
}

int WirelessSensor::get_windDir(unsigned int num) {
    if(num >= WSENSORS) return 40400.0;
    unsigned int wdir = state.wsensor.wind.dir.data[num];
    int corr = config.wsensor.windDirCorr(num);
    return ((wdir + corr) % 360 + 360) % 360;
}

float WirelessSensor::get_light(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.light.data[num] + config.wsensor.lightCorr(num);
}

float WirelessSensor::get_voltage(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.voltage.data[num] + config.wsensor.voltCorr(num);
}

float WirelessSensor::get_current(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.current.data[num] + config.wsensor.currCorr(num);
}

float WirelessSensor::get_power(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.power.data[num] + config.wsensor.powCorr(num);
}

float WirelessSensor::get_energy(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.energy.data[num] + config.wsensor.enrgCorr(num);
}

float WirelessSensor::get_frequency(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.freq.data[num] + config.wsensor.freqCorr(num);
}

float WirelessSensor::get_co2(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.co2.data[num] + config.wsensor.co2Corr(num);
}

int WirelessSensor::get_batteryAdc(unsigned int num) {
    if(num >= WSENSORS) return -1;
    return state.wsensor.battery.adc[num];
}

float WirelessSensor::get_batteryVoltage(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.battery.voltage[num];
}

int WirelessSensor::get_batteryLevel(unsigned int num) {
    if(num >= WSENSORS) return -1;
    return state.wsensor.battery.level[num];
}

float WirelessSensor::get_batteryPercentage(unsigned int num) {
    if(num >= WSENSORS) return -1.0;
    return state.wsensor.battery.percentage[num];
}

char* WirelessSensor::get_rawData() {
    return _receivedData;
}