#include "./ili9341.hpp"

#include "./pictures/iconsSmall/s01.hpp"
#include "./pictures/iconsSmall/s02.hpp"
#include "./pictures/iconsSmall/s04.hpp"
#include "./pictures/iconsSmall/s09.hpp"
#include "./pictures/iconsSmall/s10.hpp"
#include "./pictures/iconsSmall/s11.hpp"
#include "./pictures/iconsSmall/s13.hpp"
#include "./pictures/iconsSmall/s50.hpp"
#include "./pictures/iconsSmall/sLoading.hpp"

/**
 * Display daily forecast icons
 */
void ILI9341::_showForecastIcons() {
    for(uint8_t i=0; i<3; i++) {
        if(_prevIcons[i] != _icons[i] || _forced) {
            switch(_icons[i]) {
                case 1: _showImg(i * 106 + 7, 183, icon_small_01, sizeof(icon_small_01)); break;
                case 2: _showImg(i * 106 + 7, 183, icon_small_02, sizeof(icon_small_02)); break;
                case 3: _showImg(i * 106 + 7, 183, icon_small_04, sizeof(icon_small_04)); break;
                case 4: _showImg(i * 106 + 7, 183, icon_small_09, sizeof(icon_small_09)); break;
                case 5: _showImg(i * 106 + 7, 183, icon_small_10, sizeof(icon_small_10)); break;
                case 6: _showImg(i * 106 + 7, 183, icon_small_11, sizeof(icon_small_11)); break;
                case 7: _showImg(i * 106 + 7, 183, icon_small_13, sizeof(icon_small_13)); break;
                case 8: _showImg(i * 106 + 7, 183, icon_small_50, sizeof(icon_small_50)); break;
                default: _showImg(i * 106 + 7, 183, icon_small_loading, sizeof(icon_small_loading)); break;
            }
            _prevIcons[i] = _icons[i];
        }
    }
    _forced = false;
}