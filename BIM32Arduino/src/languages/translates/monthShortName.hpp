#pragma once
#include <Arduino.h>

/* English, Deutsch, Русский, Polski, Українська, Български, Español */
static const char* monthShortNameTxt[12][7] PROGMEM = {
    {"Jan", "Jan", "янв", "sty", "січ", "яну", "ene"},
    {"Feb", "Feb", "фев", "lut", "лют", "фев", "feb"},
    {"Mar", "Mär", "мар", "mar", "бер", "мар", "mar"},
    {"Apr", "Apr", "апр", "kwi", "кві", "апр", "abr"},
    {"May", "Mai", "мая", "maj", "тра", "май", "may"},
    {"Jun", "Jun", "июн", "cze", "чер", "юни", "jun"},
    {"Jul", "Jul", "июл", "lip", "лип", "юли", "jul"},
    {"Aug", "Aug", "авг", "sie", "сер", "авг", "ago"},
    {"Sep", "Sep", "сен", "wrz", "вер", "сеп", "sep"},
    {"Oct", "Okt", "окт", "paź", "жов", "окт", "oct"},
    {"Nov", "Nov", "ноя", "lis", "лис", "ное", "nov"},
    {"Dec", "Dez", "дек", "gru", "гру", "дек", "dic"}
};
