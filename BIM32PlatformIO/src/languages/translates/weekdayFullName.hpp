#pragma once
#include <Arduino.h>

/* English, Deutsch, Русский, Polski, Українська, Български, Español */
static const char* weekdayFullNameTxt[7][7] PROGMEM = {
    {"Sunday", "Sonntag", "Воскресенье", "Niedziela", "Неділя", "Неделя", "Domingo"},
    {"Monday", "Montag", "Понедельник", "Poniedziałek", "Понеділок", "Понеделник", "Lunes"},
    {"Tuesday", "Dienstag", "Вторник", "Wtorek", "Вівторок", "Вторник", "Martes"},
    {"Wednesday", "Mittwoch", "Среда", "Środa", "Середа", "Сряда", "Miércoles"},
    {"Thursday", "Donnerstag", "Четверг", "Czwartek", "Четвер", "Четвъртък", "Jueves"},
    {"Friday", "Freitag", "Пятница", "Piątek", "П'ятниця", "Петък", "Viernes"},
    {"Saturday", "Samstag", "Суббота", "Sobota", "Субота", "Събота", "Sábado"}
};