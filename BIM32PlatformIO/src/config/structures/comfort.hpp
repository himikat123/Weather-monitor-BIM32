#pragma once
#include <Arduino.h>

class Config;

struct ComfortStruct {
    struct Iaq {
        private:
            unsigned int _source = 0;
            bool _sound = false;
            friend class Config;

        public:
            unsigned int source() const { return _source; }
            bool sound() const { return _sound; }
    };

    struct CO2 : public Iaq {
        private: 
            unsigned int _wsensNum = 0; friend class Config;
        public: 
            unsigned int wsensNum() const { return _wsensNum; }
    };

    struct Hum : public CO2 {
        private:
            unsigned int _thing = 0;
            float _min = 0.0;
            float _max = 0.0;
            float _minHysteresis = 0.0;
            float _maxHysteresis = 0.0;
            friend class Config;

        public:
            unsigned int thing() const { return _thing; }
            float min() const { return _min; }
            float max() const { return _max; }
            float minHysteresis() const { return _minHysteresis; }
            float maxHysteresis() const { return _maxHysteresis; }
    };

    struct Temp : public Hum {
        private: 
            unsigned int _sens = 0; friend class Config;
        public: 
            unsigned int sens() const { return _sens; }
    };

    public:
        Temp temp;
        Hum hum;
        Iaq iaq;
        CO2 co2;
};