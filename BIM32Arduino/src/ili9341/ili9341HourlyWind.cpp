#include "./ili9341.hpp"

#include "../weather.hpp"
#include "../validate.hpp"
#include "../languages.hpp"
#include "./pictures/symbols/wind_north_tiny.hpp"
#include "./pictures/symbols/wind_north_east_tiny.hpp"
#include "./pictures/symbols/wind_east_tiny.hpp"
#include "./pictures/symbols/wind_south_east_tiny.hpp"
#include "./pictures/symbols/wind_south_tiny.hpp"
#include "./pictures/symbols/wind_south_west_tiny.hpp"
#include "./pictures/symbols/wind_west_tiny.hpp"
#include "./pictures/symbols/wind_north_west_tiny.hpp"

void ILI9341::_hourlyWindSpeed(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    float wind = weather.get_hourlyWindSpeed(num + _hourlyShift);
    String ws = validate.windSpeed(wind) ? String((int)round(wind)) : "--";
    _printText(num * 32 + 30, y, 32, 12, ws + lang.ms(), FONT_TINY, CENTER, TEXT_COLOR);
}

void ILI9341::_hourlyWindDir(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    uint16_t x = num * 32 + 30;
    int dir = weather.get_hourlyWindDir(num + _hourlyShift);
    if(dir >= 0 && dir <= 360) {
        if((dir >= 338 && dir <= 360) || (dir >= 0 && dir < 22)) _showImg(x + 10, y, wind_north_tiny, sizeof(wind_north_tiny));
        else if(dir >= 22 && dir < 67) _showImg(x + 10, y, wind_north_east_tiny, sizeof(wind_north_east_tiny));
        else if(dir >= 67 && dir < 112) _showImg(x + 10, y, wind_east_tiny, sizeof(wind_east_tiny));
        else if(dir >= 112 && dir < 157) _showImg(x + 10, y, wind_south_east_tiny, sizeof(wind_south_east_tiny));
        else if(dir >= 157 && dir < 202) _showImg(x + 10, y, wind_south_tiny, sizeof(wind_south_tiny));
        else if(dir >= 202 && dir < 247) _showImg(x + 10, y, wind_south_west_tiny, sizeof(wind_south_west_tiny));
        else if(dir >= 247 && dir < 292) _showImg(x + 10, y, wind_west_tiny, sizeof(wind_west_tiny));
        else if(dir >= 292 && dir < 338) _showImg(x + 10, y, wind_north_west_tiny, sizeof(wind_north_west_tiny));
    }
    else tft.fillRect(x + 12, y, 12, 12, BG_COLOR);
}