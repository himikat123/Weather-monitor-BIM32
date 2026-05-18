#pragma once

#include "../globals.hpp"
#include "../agregateData/comfortData/agregateComfortData.hpp"

class Comfort {
    public:
        void calculate();
        void soundNotify();
        void devicesControl();

    private:
        AgregateComfortData agregateComfortData;
        static constexpr float AIR_CLEAN_LEVEL_DN = 90.0;
        static constexpr float AIR_CLEAN_LEVEL_UP = 100.0;
        static constexpr float AIR_POLLUTED_LEVEL_DN = 190.0;
        static constexpr float AIR_POLLUTED_LEVEL_UP = 200.0;
        static constexpr float CO2_CLEAN_LEVEL_DN = 700.0;
        static constexpr float CO2_CLEAN_LEVEL_UP = 800.0;
        static constexpr float CO2_POLLUTED_LEVEL_DN = 1300.0;
        static constexpr float CO2_POLLUTED_LEVEL_UP = 1400.0;

        float _temp = UNDEFINED_FLOAT;
        float _hum = UNDEFINED_FLOAT;
        float _iaq = UNDEFINED_FLOAT;
        float _co2 = UNDEFINED_FLOAT;
        int _tempLevel = TEMP_COMFORTABLE;
        int _humLevel = HUM_COMFORTABLE;
        int _iaqLevel = AIR_CLEAN;
        int _co2Level = AIR_CLEAN;
        bool _heater = false;
        bool _cooler = false;
        bool _humidifier = false;
        bool _dehumidifier = false;
        bool _purifier = false;
};