#pragma once
#include <Arduino.h>

/* English, Deutsch, Русский, Polski, Українська, Български, Español */
static const char* alarmTxt[7] PROGMEM = {"Alarm", "Wecker", "Будильник", "Budzik", "Будильник", "Будилник", "Alarma"};
static const char* networkTxt[7] PROGMEM = {"Network", "Netzwerk", "Сеть", "Sieć", "Мережа", "Мрежа", "Red"};
static const char* signalLevelTxt[7] PROGMEM = {"Signal level", "Signalpegel", "Уровень сигнала", "Siła sygnału", "Рівень сигналу", "Ниво на сигнала", "Nivel de señal"};
static const char* ipAddrTxt[7] PROGMEM = {"IP address", "IP Adresse", "IP адрес", "Adres IP", "IP адреса", "IP адрес", "Dirección IP"};
static const char* macAddrTxt[7] PROGMEM = {"MAC address", "MAC Adresse", "MAC адрес", "MAC adres", "MAC адреса", "MAC адрес", "Dirección MAC"};
static const char* esp32TempTxt[7] PROGMEM = {"ESP32 Temperature", "ESP32 Temperatur", "Температура ESP32", "Temperatura ESP32", "Температура ESP32", "Температура ESP32", "Temperatura ESP32"};
static const char* firmwareTxt[7] PROGMEM = {"Firmware", "Firmware", "Прошивка", "Oprogramowanie", "Прошивка", "Фърмуер", "Firmware"};
static const char* mmTxt[7] PROGMEM = {"mm", "mm", "мм", "mm", "мм", "мм", "mm"};
static const char* hpaTxt[7] PROGMEM = {"hPa", "hPa", "гПа", "hPa", "гПа", "гПа", "hPa"};
static const char* msTxt[7] PROGMEM = {"m/s", "m/s", "м/с", "m/s", "м/с", "м/с", "m/s"};
static const char* gpmTxt[7] PROGMEM = {"g/m³", "g/m³", "г/м³", "g/m³", "г/м³", "г/м³", "g/m³"};
static const char* vTxt[7] PROGMEM = {"V", "V", " В", "V", " В", " В", "V"};
static const char* historyInTxt[7] PROGMEM = {"Weather indoor history", "Hauswettergeschichte", "История погоды в доме", "Historia pomiarów w pomieszczeniach", "Історія погоди у будинку", "История на времето в дома", "Historial clima interior"};
static const char* historyOutTxt[7] PROGMEM = {"Weather outdoor history", "Außenwettergeschichte", "История погоды на улице", "Historia pogody na zewnątrz", "Історія погоди на вулиці", "История на времето навън", "Historial clima exterior"};
static const char* touchCalibrateTxt[7] PROGMEM = {"Touch corners as indicated", "Berühren Sie die Ecken wie angegeben", "Прикоснитесь к углам, как указано", "Dotknij narożników zgodnie ze wskazaniem", "Торкніться кутів, як зазначено", "Докоснете ъглите, както е показано", "Toca esquinas según se indica"};
static const char* calibrationDoneTxt[7] PROGMEM = {"Calibration complete", "Die Kalibrierung ist abgeschlossen", "Калибровка завершена", "Kalibracja została zakończona", "Калібрування закінчено", "Калибрирането е завършено", "Calibración terminada"};
static const char* unknownTxt PROGMEM = "--";