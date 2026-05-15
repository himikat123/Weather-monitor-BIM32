#include "./ili9341.hpp"

#include "../validate.hpp"
#include "../languages.hpp"
#include "./pictures/wind/north.hpp"
#include "./pictures/wind/northEast.hpp"
#include "./pictures/wind/east.hpp"
#include "./pictures/wind/southEast.hpp"
#include "./pictures/wind/south.hpp"
#include "./pictures/wind/southWest.hpp"
#include "./pictures/wind/west.hpp"
#include "./pictures/wind/northWest.hpp"

/**
 * Display wind speed
 */
void ILI9341::_showWindSpeed() {
    if(_prevWindSpd != _windSpd || _forced) {
        String wnd = validate.windSpeed(_windSpd) ? String(int(round(_windSpd))) + lang.ms() : "--";
        _printText(93, 146, 40, 16, wnd, FONT1, CENTER, TEXT_COLOR);
        _prevWindSpd = _windSpd;
    }
}

/**
 * Display wind direction
 */
void ILI9341::_showWindDirection() {
    if(_prevWindDir != _windDir || _forced) {
        switch(_windDir) {
            case 1: _showImg(133, 143, wind_north_east, sizeof(wind_north_east)); break;
            case 2: _showImg(133, 143, wind_east, sizeof(wind_east)); break;
            case 3: _showImg(133, 143, wind_south_east, sizeof(wind_south_east)); break;
            case 4: _showImg(133, 143, wind_south, sizeof(wind_south)); break;
            case 5: _showImg(133, 143, wind_south_west, sizeof(wind_south_west)); break;
            case 6: _showImg(133, 143, wind_west, sizeof(wind_west)); break;
            case 7: _showImg(133, 143, wind_north_west, sizeof(wind_north_west)); break;
            default: _showImg(133, 143, wind_north, sizeof(wind_north)); break;
        }
        _prevWindDir = _windDir;
    }
}