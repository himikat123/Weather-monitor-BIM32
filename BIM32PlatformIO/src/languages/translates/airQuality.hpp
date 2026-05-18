#pragma once
#include <Arduino.h>

/* English, Deutsch, Русский, Polski, Українська, Български, Español */
static const char* airQualityTxt[3][7] PROGMEM = {
    {"Clean Air", "Saubere Luft", "Воздух чистый", "Powietrze czyste", "Повітря чисте", "Въздухът е чист", "Aire limpio"},
    {"Air is polluted", "Luft ist verschmutzt", "Воздух загрязнен", "Powietrze zanieczyszczone", "Повітря забруднене", "Въздухът е замърсен", "Aire contaminado"},
    {"Air is heavily polluted", "Luft ist stark verschmutzt", "Воздух сильно загрязнен", "Powietrze silnie zanieczyszczone", "Повітря сильно забруднене", "Въздухът е силно замърсен", "Aire muy contaminado"}
};