#include "./ili9341.hpp"

#include "../weather.hpp"
#include "./pictures/iconsTiny/icon_tiny_01.hpp"
#include "./pictures/iconsTiny/icon_tiny_02.hpp"
#include "./pictures/iconsTiny/icon_tiny_04.hpp"
#include "./pictures/iconsTiny/icon_tiny_09.hpp"
#include "./pictures/iconsTiny/icon_tiny_10.hpp"
#include "./pictures/iconsTiny/icon_tiny_11.hpp"
#include "./pictures/iconsTiny/icon_tiny_13.hpp"
#include "./pictures/iconsTiny/icon_tiny_50.hpp"
#include "./pictures/iconsTiny/icon_tiny_loading.hpp"

void ILI9341::_hourlyIcon(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 32, BG_COLOR);
    uint16_t x = num * 32 + 30;
    switch(weather.get_hourlyIcon(num + _hourlyShift)) {
        case 1: _showImg(x, y, icon_tiny_01, sizeof(icon_tiny_01)); break;
        case 2: _showImg(x, y, icon_tiny_02, sizeof(icon_tiny_02)); break;
        case 3: _showImg(x, y, icon_tiny_02, sizeof(icon_tiny_02)); break;
        case 4: _showImg(x, y, icon_tiny_04, sizeof(icon_tiny_04)); break;
        case 9: _showImg(x, y, icon_tiny_09, sizeof(icon_tiny_09)); break;
        case 10: _showImg(x, y, icon_tiny_10, sizeof(icon_tiny_10)); break;
        case 11: _showImg(x, y, icon_tiny_11, sizeof(icon_tiny_11)); break;
        case 13: _showImg(x, y, icon_tiny_13, sizeof(icon_tiny_13)); break;
        case 50: _showImg(x, y, icon_tiny_50, sizeof(icon_tiny_50)); break;
        default: _showImg(x, y, icon_tiny_loading, sizeof(icon_tiny_loading)); break;
    }
}