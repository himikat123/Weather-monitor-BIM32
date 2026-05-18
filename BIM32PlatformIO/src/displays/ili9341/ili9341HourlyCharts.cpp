#include "./ili9341.hpp"

void ILI9341::_displayLcdHourlyCharts(uint8_t type) {
    uint8_t step = 32, grid = 31;

    tft.fillRect(42, 0, grid * 8, 72, BG_COLOR);
    for(uint8_t i=0; i<10; i++) tft.drawFastHLine(42, i * 8, grid * 8 - 8, GRID_COLOR);
    for(uint8_t i=0; i<grid; i++) tft.drawFastVLine(i * 8 + 42, 0, 72, GRID_COLOR);

    for(uint8_t cht=0; cht<4; cht++) {
        float chartMin = 10000.0, chartMax = -10000.0, ch[8];

        for(uint8_t i=0; i<8; i++) {
            if(cht == 0) ch[i] = _hrTemp[i];
            if(cht == 1) ch[i] = _hrPres[i];
            if(cht == 2) ch[i] = _hrPrec[i];
            if(cht == 3) ch[i] = _hrHum[i];
        }
        for(uint8_t i=0; i<8; i++) {
            if(ch[i] < chartMin) chartMin = ch[i];
            if(ch[i] > chartMax) chartMax = ch[i];
        }

        float m = chartMax - chartMin;
        float k = m ? (64 / m) : 0;
        for(uint8_t i=0; i<8; i++) {
            ch[i] -= chartMin;
            if(k > 0) ch[i] *= k;
            else ch[i] /= 2;
            if(ch[i] > 72) ch[i] = 72;
        }

        for(uint8_t i=0; i<7; i++) {
            if(cht == 0) { // temperature
                tft.drawLine(i * step + 50, 67 - ch[i], i * step + step + 50, 67 - ch[i + 1], TEMPERATURE_COLOR);
            }
            if(cht == 1 && (type == HOURLY || type == HISTORY_OUT)) { // pressure
                tft.drawLine(i * step + 50, 69 - ch[i], i * step + step + 50, 69 - ch[i + 1], PRESSURE_COLOR);
            }
            if(cht == 2 && type == HOURLY) { // precipitation
                tft.drawLine(i * step + 50, 71 - ch[i], i * step + step + 50, 71 - ch[i + 1], HUMIDITY_COLOR);
            }
            if(cht == 3 && (type == HISTORY_OUT || type == HISTORY_IN)) { // humidity
                tft.drawLine(i * step + 50, 71 - ch[i], i * step + step + 50, 71 - ch[i + 1], HUMIDITY_COLOR);
            }
        }
    }    
}