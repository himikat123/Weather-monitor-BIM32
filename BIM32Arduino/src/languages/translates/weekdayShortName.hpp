#pragma once
#include <Arduino.h>

/* English, Deutsch, Русский, Polski, Українська, Български, Español */
static const char* weekdayShortNameTxt[7][7] PROGMEM = {
    {"Su", "So", "Вс", "Nd", "Нд", "Не", "Do"},
    {"Mo", "Mo", "Пн", "Pn", "Пн", "По", "Lu"},
    {"Tu", "Di", "Вт", "Wt", "Вт", "Вт", "Ma"},
    {"We", "Mi", "Ср", "Śr", "Ср", "Ср", "Mi"},
    {"Th", "Do", "Чт", "Cz", "Чт", "Че", "Ju"},
    {"Fr", "Fr", "Пт", "Pi", "Пт", "Пе", "Vi"},
    {"Sa", "Sa", "Сб", "So", "Сб", "Съ", "Sá"}
};