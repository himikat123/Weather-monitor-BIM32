#include "./ili9341.hpp"

#include "./pictures/symbols/close.hpp"
#include "./pictures/symbols/left.hpp"
#include "./pictures/symbols/right.hpp"

/**
 * Close button
 */
void ILI9341::_closeButton() {
    _showImg(289, 0, symb_close, sizeof(symb_close));
}

/**
 * Left button
 */
void ILI9341::_leftButton(bool show) {
    if(show) _showImg(0, 106, symb_left, sizeof(symb_left));
    else tft.fillRect(0, 106, 24, 24, BG_COLOR);
}

/**
 * Right button
 */
void ILI9341::_rightButton(bool show) {
    if(show) _showImg(295, 106, symb_right, sizeof(symb_right));
    else tft.fillRect(295, 106, 24, 24, BG_COLOR);
}