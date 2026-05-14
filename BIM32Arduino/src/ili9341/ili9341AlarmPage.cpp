#include "./ili9341.hpp"

#include "../config.hpp"
#include "../languages.hpp"

void ILI9341::_alarmPage() {
    if(!_alarmSkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        for(uint8_t i=0; i<4; i++) tft.drawFastHLine(0, i * 79 + 1, 288, TEXT_COLOR);
        for(uint8_t i=0; i<5; i++) tft.drawFastVLine(i * 72, 1, 238, TEXT_COLOR);
        _alarmSkeleton = true;
        _forced = true;
    }

    if(_prevAlarmChecksum != _alarmChecksum || _forced) {
        uint8_t alarmNr = 0;
        for(uint8_t v=0; v<3; v++) {
            for(uint8_t h=0; h<4; h++) {
                uint16_t x = h * 72, y = v * 79 + 1;
                // time
                char buf[6];
                sprintf(buf, "%d:%02d", config.alarm.time(alarmNr, 0), config.alarm.time(alarmNr, 1));
                _printText(x + 1, y + 27, 46, 16, String(buf), FONT1, CENTER, TEXT_COLOR);
                // checkbox
                uint16_t xc = x + 48;
                tft.drawFastHLine(xc, y + 24, 18, GRID_COLOR);
                tft.drawFastVLine(xc, y + 24, 18, GRID_COLOR);
                tft.drawFastHLine(xc, y + 42, 18, FRAME1_COLOR);
                tft.drawFastVLine(xc + 18, y + 24, 19, FRAME1_COLOR);
                if(config.alarm.state(alarmNr)) tft.fillRect(xc + 2, y + 26, 15, 15, FRAME_COLOR);
                else tft.fillRect(xc + 2, y + 26, 15, 15, BG_COLOR);
                alarmNr++;
            }
        }
        alarmNr = 0;
        for(uint8_t v=0; v<3; v++) {
            for(uint8_t h=0; h<4; h++) {
                uint16_t x = h * 72, y = v * 79 + 1;
                // alarm number
                char alarmNum[32];
                sprintf(alarmNum, "%s %d", lang.alarm(), alarmNr + 1);
                _printText(x + 1, y + 5, 70, 11, String(alarmNum), FONT_TINY, CENTER, TEXT_COLOR);
                // weekdays
                _printText(x + 1, y + 48, 19, 11, lang.weekdayShortName(2), FONT_TINY, RIGHT, TEXT_COLOR);
                _printText(x + 16, y + 48, 40, 11, ". . . . . . . . .", FONT_TINY, CENTER, TEXT_COLOR);
                _printText(x + 57, y + 48, 15, 11, lang.weekdayShortName(1), FONT_TINY, LEFT, WEEKEND_COLOR);
                for(uint8_t i=0; i<7; i++) {
                    uint16_t xw = x + i * 9 + 5;
                    tft.drawFastHLine(xw, y + 62, 7, GRID_COLOR);
                    tft.drawFastVLine(xw, y + 62, 7, GRID_COLOR);
                    tft.drawFastHLine(xw, y + 69, 7, FRAME1_COLOR);
                    tft.drawFastVLine(xw + 7, y + 62, 8, FRAME1_COLOR);
                    if(config.alarm.weekday(alarmNr, i)) tft.fillRect(xw + 1, y + 63, 6, 6, i < 6 ? FRAME_COLOR : WEEKEND_COLOR);
                    else tft.fillRect(xw + 1, y + 63, 6, 6, BG_COLOR);
                }
                alarmNr++;
            }
        }
        _prevAlarmChecksum = _alarmChecksum;
        _forced = false;
    }
}