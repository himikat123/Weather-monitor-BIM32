#include "./ili9341.hpp"

#include "../../config/config.hpp"
#include "../../validation/validate.hpp"
#include "../../languages/languages.hpp"

void ILI9341::_hourlyTemp(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 16, BG_COLOR);
    char buf[10];
    if(validate.temp(_hrTemp[num])) sprintf(buf, "%.1f°", _hrTemp[num]);
    else sprintf(buf, "--°");
    _printText(num * 32 + 30, y, 32, 12, String(buf), FONT_SMALL, CENTER, TEMPERATURE_COLOR);
}

void ILI9341::_hourlyHum(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    String h = validate.hum(_hrHum[num]) ? String((int)round(_hrHum[num])) : "--";
    _printText(num * 32 + 30, y, 32, 12, h + "%", FONT_TINY, CENTER, HUMIDITY_COLOR);
}

void ILI9341::_hourlyPres(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    String p = validate.pres(_hrPres[num]) ? String((int)round(_hrPres[num])) : "--";
    _printText(num * 32 + 30, y, 32, 12, p + (config.units_pres() == HPA ? String(lang.hpa()[0]) : lang.mm()), FONT_TINY, CENTER, PRESSURE_COLOR);
}