#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"

class Config;

struct Wsensor {
    private:
        float _tempCorr[WSENSORS][WSENSOR_TEMPS] = { 0 }; // Wireless sensor temperature correction
        float _humCorr[WSENSORS] = { 0 }; // Wireless sensor humidity correction
        float _presCorr[WSENSORS] = { 0 }; // Wireless sensor pressure correction
        float _windSpeedCorr[WSENSORS] = { 0 }; // Wireless sensor wind speed correction
        float _windDirCorr[WSENSORS]= { 0 }; // Wireless sensor wind direction correction
        float _lightCorr[WSENSORS] = { 0 }; // Wireless sensor ambient light correction
        float _voltCorr[WSENSORS] = { 0 }; // Wireless sensor PZEM-004t voltage correction
        float _currCorr[WSENSORS] = { 0 }; // Wireless sensor PZEM-004t current correction
        float _powCorr[WSENSORS] = { 0 }; // Wireless sensor PZEM-004t power correction
        float _enrgCorr[WSENSORS] = { 0 }; // Wireless sensor PZEM-004t energy correction
        float _freqCorr[WSENSORS] = { 0 }; // Wireless sensor PZEM-004t frequency correction
        float _co2Corr[WSENSORS] = { 0 }; // Wireless sensor SenseAir S8 CO2 correction
        float _batK[WSENSORS] = { 125.0, 125.0 }; // Wireless sensor battery voltage ADC division factor
        unsigned int _batType[WSENSORS] = { 0 }; // Wireless sensor battery type: 0 - 3x Batteries (4.5V), 1 - LiIon battery (3.7V) 
        unsigned int _expire[WSENSORS] = { 10, 10 }; // Wireless sensor data expire (minutes) 1...100
        unsigned int _channel = 1; // Wireless sensors channel number 1...100
        friend class Config;

    public:
        const float tempCorr(unsigned int num, unsigned int sens) const { if(num >= WSENSORS) return 0.0; if(sens >= WSENSOR_TEMPS) return 0.0; return _tempCorr[num][sens]; }
        const float humCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _humCorr[num]; }
        const float presCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _presCorr[num]; }
        const float windSpeedCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _windSpeedCorr[num]; }
        const float windDirCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _windDirCorr[num]; }
        const float lightCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _lightCorr[num]; }
        const float voltCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _voltCorr[num]; }
        const float currCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _currCorr[num]; }
        const float powCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _powCorr[num]; }
        const float enrgCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _enrgCorr[num]; }
        const float freqCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _freqCorr[num]; }
        const float co2Corr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _co2Corr[num]; }
        const float batK(unsigned int num) const { if(num >= WSENSORS) return 0.0; if(_batK[num] < 10.0 or _batK[num] > 250.0) return 125.0; return _batK[num]; }
        const unsigned int batType(unsigned int num) const { if(num >= WSENSORS) return 0; if(_batType[num] > 1) return 0; return _batType[num]; }
        const unsigned int channel() const { if(_channel < 1 or _channel > 100) return 1; return _channel; }
        const unsigned int expire(unsigned int num) const { if(num >= WSENSORS) return 0; if(_expire[num] < 1 or _expire[num] > 100) return 10; return _expire[num]; }
};