class Comfort {
    public:
        void calculate();
        void soundNotify();

    private:
        int _tempLevel = 0;
        int _humLevel = 0;
        unsigned int _iaqLevel = AIR_CLEAN;
        unsigned int _co2Level = AIR_CLEAN;
};

/**
 * Calculate comfort level 
 */
void Comfort::calculate() {
    float temp = agregateData.comfortTemp();
    float hum = agregateData.comfortHum();
    float iaq = agregateData.comfortIaq();
    float co2 = agregateData.comfortCo2();

    if(validate.temp(temp)) {
        _tempLevel = TEMP_COMFORTABLE;
        if(temp > config.comfort_temp_max()) _tempLevel = TEMP_TOO_HOT;
        if(temp < config.comfort_temp_min()) _tempLevel = TEMP_TOO_COLD;
    }
    else _tempLevel = TEMP_UNDEFINED;

    if(validate.hum(hum)) {
        _humLevel = HUM_COMFORTABLE;
        if(hum > config.comfort_hum_max()) _humLevel = HUM_TOO_HUMID;
        if(hum < config.comfort_hum_min()) _humLevel = HUM_TOO_DRY;
    }
    else _humLevel = HUM_UNDEFINED;

    if(validate.iaq(iaq)) {
        _iaqLevel = AIR_CLEAN;
        if(iaq > 100.0) _iaqLevel = AIR_POLLUTED;
        if(iaq > 200.0) _iaqLevel = AIR_HEAVILY_POLLUTED;
    }
    else _iaqLevel = AIR_CLEAN;

    if(validate.co2(co2)) {
        _co2Level = AIR_CLEAN;
        if(co2 > 800.0) _co2Level = AIR_POLLUTED;
        if(co2 > 1400.0) _co2Level = AIR_HEAVILY_POLLUTED;
    }
    else _co2Level = AIR_CLEAN;

    if(_tempLevel == TEMP_UNDEFINED && _humLevel == HUM_UNDEFINED) global.comfort = UNDEFINED;
    if(_tempLevel == TEMP_TOO_HOT && _humLevel < HUM_TOO_HUMID) global.comfort = HOT;
    if(_tempLevel == TEMP_TOO_COLD && _humLevel < HUM_TOO_HUMID) global.comfort = COLD;
    if(_tempLevel < TEMP_TOO_HOT && _humLevel == HUM_TOO_HUMID) global.comfort = HUMID;
    if(_tempLevel < TEMP_TOO_HOT && _humLevel == HUM_TOO_DRY) global.comfort = DRY;
    if(_tempLevel == TEMP_TOO_HOT && _humLevel == HUM_TOO_HUMID) global.comfort = HOT_HUMID;
    if(_tempLevel == TEMP_TOO_HOT && _humLevel == HUM_TOO_DRY) global.comfort = HOT_DRY;
    if(_tempLevel == TEMP_TOO_COLD && _humLevel == HUM_TOO_HUMID) global.comfort = COLD_HUMID;
    if(_tempLevel == TEMP_TOO_COLD && _humLevel == HUM_TOO_DRY) global.comfort = COLD_DRY;
    global.iaq_level = _iaqLevel;
    global.co2_level = _co2Level;
}

void Comfort::soundNotify() {
    if(config.comfort_temp_sound()) sound.tempNotify(_tempLevel);
    if(config.comfort_hum_sound()) sound.humNotify(_humLevel);
    if(config.comfort_iaq_sound()) sound.airNotify(_iaqLevel);
    else if(config.comfort_co2_sound()) sound.airNotify(_co2Level);
}