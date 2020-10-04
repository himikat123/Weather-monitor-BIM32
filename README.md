# Weather-monitor-BIM32
## Weather monitor based on ESP32

<center>
  <img src="/img/20200928_221642.jpg">
</center> 

A short list of weather monitor features:<br><br>

Connecting to your home network WiFi 2.4 GHz<br>
Display of the current weather and 3-day forecast<br>
Display 5-day hourly weather forecast<br>
Show weather history (hourly) 1 day ago<br>
Building graphs of temperature, humidity and probability of precipitation<br>
Home temperature and humidity display<br>
Moon phase display<br>
Clock (small/large/with seconds/arrow) with NTP synchronization<br>
Calendar with the ability to scroll to plus/minus infinity<br>
Sending and/or receiving data using the MQTT protocol<br>
Sending and/or receiving data from/to thingspeak.com<br>
Sending data to narodmon.ru<br>
Wireless temperature/pressure/humidity/ambient light sensor<br>
Support for wired temperature/pressure/humidity/ambient light sensors<br>
Auto adjustment of screen backlight brightness (by light sensor, by time or by sunrise and sunset)<br>

## Schematic diagram of a weather monitor<br>

I used ready-made modules, so the device schematic is very simple. WiFi module <b>Nodemcu 32S</b> 30pin is applied.
This is the heart and soul of the device, all the basic logic of the weather monitor is performed by this module. This module is an ESP32 SoC chip with all the necessary wiring, flash memory, USB-> UART converter and a power supply voltage regulator.<br>

<center>
  <img src="/img/">
</center><br><br>

Модуль дисплея Nextion enhanced 3.5 дюймов. Представляет собой дисплей HMI TFT, со встроенным графическим процессором, 
с 16 Мб флэш-памяти, 1024 байт EEPROM и ОЗУ 3584 байт. Дисплей берет на себя задачи прорисовки графики, и обработки 
тапов по тачскрину, а так же сомостоятельно обеспечивает ход часов, календаря, анимации и прокрутки текста, что позволяет 
существенно разгрузить основной (ESP32) процессор. Связь дисплея с основным процессором осуществляется посредством интерфейса UART.<br>

display<br><br>

Модуль радиоканала HC-12 предназначен здесь для связи с беспроводным датчиком. Рабочая частота – 433,4 – 473,0 МГц. 
Дальность передачи информации – до 1 км на открытом пространстве. Количество каналов передачи данных – 100. 
В данный модуль встроен микроконтроллер, который самостоятельно решает все вопросы приема или передачи данных. 
Связь этого модуля с основным процессором осуществляется посредством интерфейса UART. <br>
Если применение беспроводного датчика не планируется, то можно этот модуль не устанавливать.<br>

HC-12<br><br>

Монитор погоды умеет считывать датчики температуры, давления, влажности и освещенности.<br>
Поддерживаются следующие датчики:<br>
BME280<br>
BMP180<br>
DHT22<br>
SHT21<br>
BH1750<br>
MAX44009<br>
Можно подключить один, ниодного, несколько или все сразу из этого списка.<br><br>

Печатную плату заказывал в Китае, так как возиться с лазерным утюгом не было никакого желания. Платы у меня плохо получаются.<br>

Плата монитора погоды<br><br>

Посмотрите видеообзор монитора погоды, это поможет не только ознакомиться с ним, но так же внесет ясность в методику настройки параметров прибора.<br>

Видеообзор монитора погоды

Для прошивки монитора погоды понадобится micro-sd карта, micro-USB кабель и компьютер.<br>
Чтоб прошить дисплей, нужно скопировать файл прошивки дисплея (файл с расширением .tft) на micro-sd карту 
(карта должна быть отформатирована в файловой системе FAT32). Затем вставить флешку в слот micro-sd дисплея 
и подать ему питание. Дисплей сам прошьется с флешки, процесс прошивки можно будет наблюдать на экране.<br>

Чтоб прошить ESP32 скачайте программу Flash Download Tools и настройте все как на скриншоте. То есть, в верхней 
строке укажите файл BIM32_ESP32.bin, а в строке ниже - BIM32_ESP32_SPIFFS.bin. COM-порт укажите тот, на 
котором реально сидит ваша ESP32. <br>

download tool<br><br>

Ну и конечно, на последок, парочка фотографий устройства в деле.<br>

1<br>
2<br>
3<br>
4<br>
5<br>
6<br><br>
