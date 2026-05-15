#pragma once
#include <Arduino.h>

/* English, Deutsch, Русский, Polski, Українська, Български, Español */
static const char* monthFullNameTxt[12][7] PROGMEM = {
    {"January", "Januar", "Январь", "Styczeń", "Січень", "Януари", "Enero"},
    {"February", "Februar", "Февраль", "Luty", "Лютий", "Февруари", "Febrero"},
    {"March", "März", "Март", "Marsz", "Березень", "Март", "Marzo"},
    {"April", "April", "Апрель", "Kwiecień", "Квітень", "Април", "Abril"},
    {"May", "Mai", "Май", "Maj", "Травень", "Май", "Mayo"},
    {"June", "Juni", "Июнь", "Czerwiec", "Червень", "Юни", "Junio"},
    {"July", "Juli", "Июль", "Lipiec", "Липень", "Юли", "Julio"},
    {"August", "August", "Август", "Sierpień", "Серпень", "Август", "Agosto"},
    {"September", "September", "Сентябрь", "Wrzesień", "Вересень", "Септември", "Septiembre"},
    {"October", "Oktober", "Октябрь", "Październik", "Жовтень", "Октомври", "Octubre"},
    {"November", "November", "Ноябрь", "Listopad", "Листопад", "Ноември", "Noviembre"},
    {"December", "Dezember", "Декабрь", "Grudzień", "Грудень", "Декември", "Diciembre"}
};