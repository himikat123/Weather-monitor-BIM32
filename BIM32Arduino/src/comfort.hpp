class Comfort {
    public:
        void calculate();
        void soundNotify();
        void devicesControl();

    private:
        float _temp = 40400.0;
        float _hum = 40400.0;
        float _iaq = 40400.0;
        float _co2 = 40400.0;
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

/**
 * Calculate comfort level 
 */
void Comfort::calculate() {
    _temp = agregateComfortData.temp();
    _hum = agregateComfortData.hum();
    _iaq = agregateComfortData.iaq();
    _co2 = agregateComfortData.co2();

    if(validate.temp(_temp)) {
        if(_temp < (config.comfort.temp.min() - (config.comfort.temp.minHysteresis() / 2))) _heater = true;
        if(_temp > (config.comfort.temp.min() + (config.comfort.temp.minHysteresis() / 2))) _heater = false;

        if(_temp > (config.comfort.temp.max() + (config.comfort.temp.maxHysteresis() / 2))) _cooler = true;
        if(_temp < (config.comfort.temp.max() - (config.comfort.temp.maxHysteresis() / 2))) _cooler = false;

        if(!_heater and !_cooler) _tempLevel = TEMP_COMFORTABLE;
        if(_heater and !_cooler) _tempLevel = TEMP_TOO_COLD;
        if(!_heater and _cooler) _tempLevel = TEMP_TOO_HOT;
    }
    else {
        _heater = false;
        _cooler = false;
        _tempLevel = TEMP_UNDEFINED;
    }

    if(validate.hum(_hum)) {
        if(_hum < (config.comfort.hum.min() - config.comfort.hum.minHysteresis())) _humidifier = true;
        if(_hum > (config.comfort.hum.min() + config.comfort.hum.minHysteresis())) _humidifier = false;

        if(_hum > (config.comfort.hum.max() + config.comfort.hum.maxHysteresis())) _dehumidifier = true;
        if(_hum < (config.comfort.hum.max() - config.comfort.hum.maxHysteresis())) _dehumidifier = false;

        if(!_humidifier and !_dehumidifier) _humLevel = HUM_COMFORTABLE;
        if(_humidifier and !_dehumidifier) _humLevel = HUM_TOO_DRY;
        if(!_humidifier and _dehumidifier) _humLevel = HUM_TOO_HUMID;
    }
    else {
        _humidifier = false;
        _dehumidifier = false;
        _humLevel = HUM_UNDEFINED;
    }

    if(validate.iaq(_iaq)) {
        if(_iaq < 90.0) _iaqLevel = AIR_CLEAN;
        if(_iaq >= 90.0 and _iaq <= 100.0 and _iaqLevel == AIR_HEAVILY_POLLUTED) _iaqLevel = AIR_CLEAN;
        if(_iaq > 100.0 and _iaq < 190.0) _iaqLevel = AIR_POLLUTED;
        if(_iaq >= 190.0 and _iaq <= 200.0 and _iaqLevel == AIR_CLEAN) _iaqLevel = AIR_POLLUTED;
        if(_iaq > 200.0) _iaqLevel = AIR_HEAVILY_POLLUTED;
    }
    else _iaqLevel = AIR_UNDEFINED;

    if(validate.co2(_co2)) {
        if(_co2 < 700.0) _co2Level = AIR_CLEAN;
        if(_co2 >= 700.0 and _co2 <= 800.0 and _co2Level == AIR_HEAVILY_POLLUTED) _co2Level = AIR_CLEAN;
        if(_co2 > 800.0 and _co2 < 1300.0) _co2Level = AIR_POLLUTED;
        if(_co2 >= 1300.0 and _co2 <= 1400.0 and _co2Level == AIR_CLEAN) _co2Level = AIR_POLLUTED;
        if(_co2 > 1400.0) _co2Level = AIR_HEAVILY_POLLUTED;
    }
    else _co2Level = AIR_UNDEFINED;

    if(_iaqLevel > AIR_CLEAN or _co2Level > AIR_CLEAN) _purifier = true;
    else _purifier = false;

    state.comfort = COMFORTABLE;
    if(_tempLevel == TEMP_UNDEFINED && _humLevel == HUM_UNDEFINED) state.comfort = UNDEFINED;
    if(_tempLevel == TEMP_TOO_HOT && _humLevel < HUM_TOO_HUMID) state.comfort = HOT;
    if(_tempLevel == TEMP_TOO_COLD && _humLevel < HUM_TOO_HUMID) state.comfort = COLD;
    if(_tempLevel < TEMP_TOO_HOT && _humLevel == HUM_TOO_HUMID) state.comfort = HUMID;
    if(_tempLevel < TEMP_TOO_HOT && _humLevel == HUM_TOO_DRY) state.comfort = DRY;
    if(_tempLevel == TEMP_TOO_HOT && _humLevel == HUM_TOO_HUMID) state.comfort = HOT_HUMID;
    if(_tempLevel == TEMP_TOO_HOT && _humLevel == HUM_TOO_DRY) state.comfort = HOT_DRY;
    if(_tempLevel == TEMP_TOO_COLD && _humLevel == HUM_TOO_HUMID) state.comfort = COLD_HUMID;
    if(_tempLevel == TEMP_TOO_COLD && _humLevel == HUM_TOO_DRY) state.comfort = COLD_DRY;
    state.iaq_level = _iaqLevel;
    state.co2_level = _co2Level;
}

void Comfort::soundNotify() {
    if(config.comfort.temp.sound()) sound.tempNotify(_tempLevel);
    if(config.comfort.hum.sound()) sound.humNotify(_humLevel);
    if(config.comfort.iaq.sound()) sound.airNotify(_iaqLevel);
    else if(config.comfort.co2.sound()) sound.airNotify(_co2Level);
}

void Comfort::devicesControl() {
    sensors.comfortDevices(_heater, _cooler, _humidifier, _dehumidifier, _purifier);
}