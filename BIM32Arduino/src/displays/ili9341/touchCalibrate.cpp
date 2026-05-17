#include "./ili9341.hpp"

#include "../../globals.hpp"
#include "../../languages/languages.hpp"

int ILI9341::_avg(int a, int b) {
    return (a & b) + ((a ^ b) >> 1);
}

void ILI9341::_touch_calibrate() {
    tft.fillScreen(TFT_BLACK);
    _printText(0, 110, 319, 22, lang.touchCalibrate(), FONT1, CENTER, TEXT_COLOR);

    #if defined(BIM32_CYD)
        TS_Point p = _calibrationPoint(0);
        _calData[0] = p.x; 
        _calData[1] = p.y;
        p = _calibrationPoint(1);
        _calData[0] = _avg(_calData[0], p.x); 
        _calData[3] = p.y;
        p = _calibrationPoint(2);
        _calData[2] = p.x; 
        _calData[3] = _avg(_calData[3], p.y);
        p = _calibrationPoint(3);
        _calData[2] = _avg(_calData[2], p.x);
        _calData[1] = _avg(_calData[1], p.y);
        tft.fillRect(0, 0, 320, 22, 0);
    #else
        tft.calibrateTouch(_calData, TFT_MAGENTA, TFT_BLACK, 15);
    #endif

        String json = "{\"calData\":[";
        json += String(_calData[0]) + ",";
        json += String(_calData[1]) + ",";
        json += String(_calData[2]) + ",";
        json += String(_calData[3]) + ",";
        json += String(_calData[4]);
        json += "]}";
        File file = LittleFS.open("/touch.json", FILE_WRITE);
        file.print(json);
        file.close();

        _printText(0, 110, 319, 22, lang.calibrationDone(), FONT1, CENTER, TEXT_COLOR);

    vTaskDelay(2000);
    tft.fillScreen(TFT_BLACK);
    _forced = true;
    showHomeScreen();
}

#if defined(BIM32_CYD)
    TS_Point ILI9341::_calibrationPoint(uint8_t cornerNr) {
        uint16_t corner[4][8] = {
            {0, 0, 0, 0, 0, 0, 19, 19},
            {0, 239, 0, 219, 0, 239, 20, 219},
            {299, 239, 319, 219, 299, 219, 319, 239},
            {299, 0, 319, 0, 300, 19, 319, 0},
        };
        tft.fillRect(0, 0, 320, 22, 0);
        tft.fillRect(0, 218, 320, 22, 0);
        tft.drawFastHLine(corner[cornerNr][0], corner[cornerNr][1], 20, TEXT_COLOR);
        tft.drawFastVLine(corner[cornerNr][2], corner[cornerNr][3], 20, TEXT_COLOR);
        tft.drawLine(corner[cornerNr][4], corner[cornerNr][5], corner[cornerNr][6], corner[cornerNr][7], TEXT_COLOR);
        while(!touchscreen.touched()) taskYIELD();
        while(touchscreen.touched()) taskYIELD();
        vTaskDelay(500);
        return touchscreen.getPoint();
    }
#endif