#include "./ili9341.hpp"

#include "./fonts/ubuntu21.hpp"

/**
 * Display weather description
 */
void ILI9341::_showDescription() {
    if(_prevDescription != _description || _forced) {
        tft.loadFont(Ubuntu_21);
        uint16_t w = tft.textWidth(_description);
        tft.unloadFont();
        _prevFont = 5;
        _printText(0, 84, 319, 20, _description, w > 319 ? FONT1 : FONT2, CENTER, TEXT_COLOR);
        _prevDescription = _description;
    }
}