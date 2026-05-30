#pragma once
#include <Arduino.h>

class Config;

struct SensorsStruct {
    struct Temp {
        private: 
            float _tempCorr = 0.0; friend class Config;

        public: 
            const float tempCorr() const { return _tempCorr; }
    };

    struct Light {
        private: 
            float _lightCorr = 0.0; friend class Config;
        
        public: 
            const float lightCorr() const { return _lightCorr; }
    };

    struct Voltage {
        private: 
            float _voltageCorr = 0.0; friend class Config;
    
        public: 
            const float voltageCorr() const { return _voltageCorr; }
    };

    struct TempHum : public Temp {
        private: 
            float _humCorr = 0.0; friend class Config;
        
        public: 
            const float humCorr() const { return _humCorr; }
    };

    struct TempPres : public Temp {
        private: 
            float _presCorr = 0.0; friend class Config;
        
        public: 
            const float presCorr() const { return _presCorr; }
    };

    struct TempHumPres : public TempHum {
        private: 
            float _presCorr = 0.0; friend class Config;
        
        public: 
            const float presCorr() const { return _presCorr; }
    };

    struct TempHumPresIaq : public TempHumPres {
        private: 
            float _iaqCorr = 0.0; friend class Config;
        
        public: 
            const float iaqCorr() const { return _iaqCorr; }
    };

    public:
        TempHumPres bme280;
        TempPres bmp180;
        TempHum sht21;
        TempHum dht22;
        Temp ds18b20;
        Temp esp32;
        Light max44009;
        Light bh1750;
        Voltage analog;
        TempHumPresIaq bme680;
};