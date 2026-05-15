#include "./ili9341.hpp"

#include "./pictures/iconsBig/b01d.hpp"
#include "./pictures/iconsBig/b01n.hpp"
#include "./pictures/iconsBig/b02d.hpp"
#include "./pictures/iconsBig/b02n.hpp"
#include "./pictures/iconsBig/b04.hpp"
#include "./pictures/iconsBig/b09.hpp"
#include "./pictures/iconsBig/b10.hpp"
#include "./pictures/iconsBig/b11d.hpp"
#include "./pictures/iconsBig/b11n.hpp"
#include "./pictures/iconsBig/b13.hpp"
#include "./pictures/iconsBig/b50.hpp"
#include "./pictures/iconsBig/bLoading.hpp"

/**
 * Display current weather icon
 */
void ILI9341::_showWeatherIcon() {
    if(_prevCurrIcon != _currIcon || _prevIsDay != _isDay || _forced) {
        switch(_currIcon) {
            case 1: _showImg(0, 104, _isDay ? icon_big_01_d : icon_big_01_n, _isDay ? sizeof(icon_big_01_d) : sizeof(icon_big_01_n)); break;
            case 2: _showImg(0, 104, _isDay ? icon_big_02_d : icon_big_02_n, _isDay ? sizeof(icon_big_02_d) : sizeof(icon_big_02_n)); break;
            case 3: _showImg(0, 104, icon_big_04, sizeof(icon_big_04)); break;
            case 4: _showImg(0, 104, icon_big_09, sizeof(icon_big_09)); break;
            case 5: _showImg(0, 104, icon_big_10, sizeof(icon_big_10)); break;
            case 6: _showImg(0, 104, _isDay ? icon_big_11_d : icon_big_11_n, _isDay ? sizeof(icon_big_11_d) : sizeof(icon_big_11_n)); break;
            case 7: _showImg(0, 104, icon_big_13, sizeof(icon_big_13)); break;
            case 8: _showImg(0, 104, icon_big_50, sizeof(icon_big_50)); break;
            default: _showImg(0, 104, icon_big_loading, sizeof(icon_big_loading)); break;
        }
        _prevCurrIcon = _currIcon;
        _prevIsDay = _isDay;
    }
}