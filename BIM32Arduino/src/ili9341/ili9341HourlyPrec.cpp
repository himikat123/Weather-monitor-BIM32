#include "./ili9341.hpp"

#include "../config.hpp"
#include "./pictures/symbols/drop.hpp"

void ILI9341::_hourlyPrec(uint8_t num, uint16_t y) {
    uint16_t x = num * 32 + 30;
    _showImg(x + 2, y, symb_drop, sizeof(symb_drop));
    float prec = _hrPrec[num];
    char pr[8];
    if(config.weather.provider() == 0) sprintf(pr, "%f%s", prec ? prec : 0.0, lang.mm());
    if(config.weather.provider() == 2) sprintf(pr, "%d%%", (int)round(prec));
    _printText(x + 11, y + 2, 21, 12, String(pr), FONT_TINY, CENTER, TEXT_COLOR);
}