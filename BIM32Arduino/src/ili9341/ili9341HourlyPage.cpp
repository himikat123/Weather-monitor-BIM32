#include "./ili9341.hpp"

#include "../weather.hpp"

void ILI9341::_hourlyPage() {
    if(!_hourlySkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _hourlySkeleton = true;
        _forced = true;
    }

    if(_prevHourlyChecksum != _hourlyChecksum || _prevHourlyShift != _hourlyShift || _forced) {
        for(uint8_t i=0; i<8; i++) {
            _hrTemp[i] = weather.get_hourlyTemp(i + _hourlyShift);
            _hourlyTemp(i, 86);
        }
        for(uint8_t i=0; i<8; i++) _hourlyIcon(i, 116);
        for(uint8_t i=0; i<8; i++) {
            _hrDate[i] = weather.get_hourlyDate(i + _hourlyShift);
            _hourlyWeekday(i, 156);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrPres[i] = weather.get_hourlyPres(i + _hourlyShift);
            _hourlyPres(i, 102);
        }
        for(uint8_t i=0; i<8; i++) {
            _hourlyDate(i, 176);
            _hourlyTime(i, 190);
        }
        for(uint8_t i=0; i<8; i++) _hourlyWindSpeed(i, 204);
        for(uint8_t i=0; i<8; i++) _hourlyWindDir(i, 216);
        for(uint8_t i=0; i<8; i++) _hourlyPrec(i, 230);
        _displayLcdHourlyCharts(HOURLY);
        _rightButton(_hourlyShift < 32);
        _leftButton(_hourlyShift > 0);
        _prevHourlyChecksum = _hourlyChecksum;
        _prevHourlyShift = _hourlyShift;
    }

    _forced = false;
}