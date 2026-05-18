#pragma once
#include <Arduino.h>

/* English, Deutsch, Русский, Polski, Українська, Български, Español */
static const char* weatherDescriptionTxt[16][7] PROGMEM = {
    {"Clear sky", "Klarer Himmel", "Ясно", "Czyste Niebo", "Ясно", "Ясно", "Despejado"},
    {"Mainly clear", "Überwiegend klar", "Преимущественно ясно", "Głównie jasne", "Переважно ясно", "Предимно ясно", "Mayormente despejado"},
    {"Partly cloudy", "Teilweise bewölkt", "Переменная облачность", "Częściowe zachmurzenie", "Мінлива хмарність", "Предимно облачно", "Parcialmente nublado"},
    {"Overcast", "Bedeckt", "Пасмурно", "Pochmurny", "Хмарно", "Облачно", "Nublado"},
    {"Fog", "Nebel", "Туман", "Mgła", "Туман", "Мъгла", "Neblina"},
    {"Depositing rime fog", "Ablagerung von Raureifnebel", "Туман с инеем", "Deponująca się mgła szronowa", "Туман з інеєм", "Мъгла със скреж", "Escarcha"},
    {"Drizzle", "Nieselregen", "Морось", "Mżawka", "Мряка", "Дъжд", "Llovizna"},
    {"Freezing drizzle", "Gefrierender Nieselregen", "Изморозь", "Marznąca mżawka", "Ізморозь", "Скреж", "Llovizna helada"},
    {"Rain", "Regen", "Дождь", "Deszcz", "Дощ", "Дъжд", "Lluvia"},
    {"Freezing rain", "Gefrierender Regen", "Ледяной дождь", "Marznący deszcz", "Крижаний дощ", "Леден дъжд", "Lluvia helada"},
    {"Snowfall", "Schneefall", "Снегопад", "Opady śniegu", "Снігопад", "Снеговалеж", "Nevada"},
    {"Snow grains", "Schneekörner", "Снежные зерна", "Ziarna śniegu", "Снігові зерна", "Скреж", "Copos de nieve"},
    {"Rain showers", "Regenschauer", "Ливень", "Przelotne opady deszczu", "Злива", "Ръмеж", "Chubascos"},
    {"Snow showers", "Schneeschauer", "Сильный снегопад", "Opady śniegu", "Сильний снігопад", "Обилен снеговалеж", "Tormenta de nieve"},
    {"Thunderstorm", "Gewitter", "Гроза", "Burza z piorunami", "Гроза", "Буря", "Tormenta eléctrica"},
    {"Thunderstorm with hail", "Gewitter mit Hagel", "Гроза с градом", "Burza z gradem", "Гроза з градом", "Буря с градушка", "Tormenta eléctrica con granizo"}
};