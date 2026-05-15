#include "./ili9341.hpp"

/**
 * Display daily forecast temperatures
 */
void ILI9341::_showForecastTemps() {
    for(uint8_t i=0; i<3; i++) {
        if(_prevDTemps[i] != _dTemps[i] || _forced) {
            _showTemperature(_dTemps[i], i * 106 + 49, 183, FONT2, TEMPERATURE_COLOR);
            _prevDTemps[i] = _dTemps[i];
        }

        if(_prevNTemps[i] != _nTemps[i] || _forced) {
            _showTemperature(_nTemps[i], i * 106 + 49, 203, FONT2, TEMP_MIN_COLOR);
            _prevNTemps[i] = _nTemps[i];
        }
    }
}