#include "./ili9341.hpp"

#include "./pictures/symbols/home.hpp"
#include "./pictures/symbols/hum.hpp"
#include "./pictures/symbols/tempPlus.hpp"
#include "./pictures/symbols/pres.hpp"
#include "./pictures/symbols/wind.hpp"

void ILI9341::_drawSkeleton() {
    tft.drawFastHLine(0, 80, 319, FRAME_COLOR);
    tft.drawFastVLine(143, 2, 75, FRAME_COLOR);
    tft.drawSmoothRoundRect(0, 165, 10, 10, 106, 74, FRAME_COLOR, BG_COLOR);
    tft.drawSmoothRoundRect(106, 165, 10, 10, 106, 74, FRAME_COLOR, BG_COLOR);
    tft.drawSmoothRoundRect(212, 165, 10, 10, 107, 74, FRAME_COLOR, BG_COLOR);
    _showImg(145, 48, symb_home, sizeof(symb_home));
    _showImg(243, 48, symb_hum, sizeof(symb_hum));
    _showImg(62, 104, symb_temp_plus, sizeof(symb_temp_plus));
    _showImg(143, 109, symb_hum, sizeof(symb_hum));
    _showImg(222, 109, symb_pres, sizeof(symb_pres));
    _showImg(61, 146, symb_wind, sizeof(symb_wind));
    _showTime();
}