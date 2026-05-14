#include "./ili9341.hpp"

#include "../languages.hpp"
#include "../thingspeak.hpp"

void ILI9341::_historyInPage() {
    if(!_historyInSkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _displayLcdHistoryTitle(lang.historyIn());
        _historyInSkeleton = true;
        _forced = true;
    }

    if(_prevHistoryInChecksum != _historyInChecksum || _prevHistoryInShift != _historyInShift || _forced) {
        for(uint8_t i=0; i<8; i++) {
            _hrTemp[i] = thingspeak.get_historyField(3, i + _historyInShift);
            _hourlyTemp(i, 86);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrDate[i] = thingspeak.get_historyUpdated(i + _historyInShift);
            _hourlyWeekday(i, 116);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrHum[i] = thingspeak.get_historyField(4, i + _historyInShift);
            _hourlyHum(i, 102);
        }
        for(uint8_t i=0; i<8; i++) {
            _hourlyDate(i, 136);
            _hourlyTime(i, 150);
        }
        _displayLcdHourlyCharts(HISTORY_IN);
        _rightButton(_historyInShift < 16);
        _leftButton(_historyInShift > 0);
        _prevHistoryInChecksum = _historyInChecksum;
        _prevHistoryInShift = _historyInShift;
    }

    _forced = false;
}

void ILI9341::_historyOutPage() {
    if(!_historyOutSkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _displayLcdHistoryTitle(lang.historyOut());
        _historyOutSkeleton = true;
        _forced = true;
    }

    if(_prevHistoryOutChecksum != _historyOutChecksum || _prevHistoryOutShift != _historyOutShift || _forced) {
        for(uint8_t i=0; i<8; i++) {
            _hrTemp[i] = thingspeak.get_historyField(0, i + _historyOutShift);
            _hourlyTemp(i, 86);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrDate[i] = thingspeak.get_historyUpdated(i + _historyOutShift);
            _hourlyWeekday(i, 130);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrHum[i] = thingspeak.get_historyField(1, i + _historyOutShift);
            _hourlyHum(i, 102);
        }
        for(uint8_t i=0; i<8; i++) {
            _hrPres[i] = thingspeak.get_historyField(2, i + _historyOutShift);
            _hourlyPres(i, 116);
        }
        for(uint8_t i=0; i<8; i++) {
            _hourlyDate(i, 150);
            _hourlyTime(i, 164);
        }
        _displayLcdHourlyCharts(HISTORY_OUT);
        _rightButton(_historyOutShift < 16);
        _leftButton(_historyOutShift > 0);
        _prevHistoryOutChecksum = _historyOutChecksum;
        _prevHistoryOutShift = _historyOutShift;
    }

    _forced = false;
}

void ILI9341::_displayLcdHistoryTitle(String title) {
    tft.drawFastHLine(38, 190, 247, FRAME1_COLOR);
    tft.drawFastVLine(38, 190, 26, FRAME1_COLOR);
    tft.drawFastHLine(39, 215, 246, TEXT_COLOR);
    tft.drawFastVLine(285, 190, 26, TEXT_COLOR);
    _printText(39, 196, 246, 16, title, FONT1, CENTER, TEXT_COLOR);
}