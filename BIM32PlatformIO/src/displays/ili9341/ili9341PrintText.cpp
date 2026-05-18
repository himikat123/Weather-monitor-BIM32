#include <Arduino.h>
#include "./ili9341.hpp"

#include "./fonts/ubuntu9.hpp"
#include "./fonts/ubuntu11.hpp"
#include "./fonts/ubuntu14.hpp"
#include "./fonts/ubuntu18.hpp"
#include "./fonts/ubuntu21.hpp"
#include "./fonts/ubuntu29.hpp"
#include "./fonts/segment96.hpp"
#include "./fonts/segment140.hpp"

void ILI9341::_printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color) {
    _printText(x, y, width, height, text, font, align, color, BG_COLOR, false);
}

void ILI9341::_printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color, uint16_t bgColor) {
    _printText(x, y, width,  height, text, font, align, color, bgColor, false);
}

void ILI9341::_printText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text, uint8_t font, uint8_t align, uint16_t color, uint16_t bgColor, bool valign) {
    if(_prevFont != font) {
        if(font == FONT_TINY) tft.loadFont(Ubuntu_9);
        else if(font == FONT_SMALL) tft.loadFont(Ubuntu_11);
        else if(font == FONT1) tft.loadFont(Ubuntu_14);
        else if(font == FONT2) tft.loadFont(Ubuntu_21);
        else if(font == FONT3) tft.loadFont(Ubuntu_29);
        else if(font == FONTPR) tft.loadFont(Ubuntu_18);
        else if(font == FONT_SEGMENTS_SML) tft.loadFont(segment_96);
        else if(font == FONT_SEGMENTS_BIG) tft.loadFont(segment_140);
        _prevFont = font;
    }

    tft.fillRect(x, y, width, height, bgColor);
    tft.setTextColor(color, bgColor);

    String croppedText = "";
    for(size_t i = 0; i < text.length(); ) {
        uint8_t c = text[i];
        uint8_t charLength = 1;
        if((c & 0x80) == 0x00) charLength = 1;
        else if((c & 0xE0) == 0xC0) charLength = 2;
        //else if((c & 0xF0) == 0xE0) charLength = 3;
        //else if((c & 0xF8) == 0xF0) charLength = 4;
        String currentChar = text.substring(i, i + charLength);
        String testText = croppedText + currentChar;
        uint16_t textWidth = tft.textWidth(testText);
        if(textWidth > width) break;
        croppedText = testText;
        i += charLength;
    }

    if(align == CENTER || align == RIGHT) {
        uint16_t w = tft.textWidth(croppedText);
        if(align == RIGHT) x += width - w - 4;
        else x += (width / 2) - (w / 2);
    }

    uint16_t h = tft.fontHeight();
    tft.setCursor(x, valign ? (y + height / 2 - h / 2) : y);
    tft.print(croppedText);
}