#pragma once
#include <Arduino.h>

/* English, Deutsch, Русский, Polski, Українська, Български, Español */
static const char* monthDayTxt[12][7] PROGMEM = {
    {"January", "Januar", "января", "stycznia", "січня", "януари", "enero"},
    {"February", "Februar", "февраля", "lutego", "лютого", "февруари", "febrero"},
    {"March", "März", "марта", "marca", "березня", "март", "marzo"},
    {"April", "April", "апреля", "kwietnia", "квітня", "март", "abril"},
    {"May", "Mai", "мая", "maja", "травня", "май", "mayo"},
    {"June", "Juni", "июня", "czerwca", "червня", "юни", "junio"},
    {"July", "Juli", "июля", "lipca", "липня", "юли", "julio"},
    {"August", "August", "августа", "sierpnia", "серпня", "август", "agosto"},
    {"September", "September", "сентября", "września", "вересня", "септември", "septiembre"},
    {"October", "Oktober", "октября", "października", "жовтня", "октомври", "octubre"},
    {"November", "November", "ноября", "listopada", "листопада", "ноември", "noviembre"},
    {"December", "Dezember", "декабря", "grudnia", "грудня", "декември", "diciembre"}
};