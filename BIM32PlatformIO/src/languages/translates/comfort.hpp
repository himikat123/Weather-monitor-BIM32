#pragma once
#include <Arduino.h>

/* English, Deutsch, Русский, Polski, Українська, Български, Español */
static const char* comfortTxt[9][7] PROGMEM = {
    {"Comfortable", "Gemütlich", "Комфортно", "Przyjemny", "Комфортно", "Комфортно", "Clima agradable"},
    {"Too hot", "Zu heiß", "Жарко", "Gorąco", "Жарко", "Горещо", "Muy caliente"},
    {"Too cold", "Zu kalt", "Холодно", "Zimno", "Холодно", "Студено", "Muy frío"},
    {"Too humid", "Zu feucht", "Слишком влажно", "Zbyt wilgotno", "Занадто волого", "Твърде влажно", "Muy húmedo"},
    {"Too dry", "Zu trocken", "Слишком сухо", "Zbyt sucho", "Занадто сухо", "Твърде сухо", "Muy seco"},
    {"Hot and humid", "Heiß und feucht", "Жарко и влажно", "Gorąco i wilgotno", "Жарко та волого", "Горещо и влажно", "Caliente y húmedo"},
    {"Hot and dry", "Heiß und trocken", "Жарко и сухо", "Gorąco i sucho", "Жарко та сухо", "Горещо и сухо", "Caliente y seco"},
    {"Cold and humid", "Kalt und feucht", "Холодно и влажно", "Zimno i wilgotno", "Холодно та волого", "Студено и влажно", "Frío y húmedo"},
    {"Cold and dry", "Kalt und trocken", "Холодно и сухо", "Zimno i sucho", "Холодно та сухо", "Студено и сухо", "Frío y seco"}
};