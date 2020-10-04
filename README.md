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
<center>
  <img src="/img/BIM32schematic.png">
</center><br><br>

I used ready-made modules, so the device schematic is very simple. WiFi module <b>Nodemcu 32S</b> 30pin is applied.
This is the heart and soul of the device, all the basic logic of the weather monitor is performed by this module. This module is an ESP32 SoC chip with all the necessary wiring, flash memory, USB-> UART converter and a power supply voltage regulator.<br>

<center>
  <img src="/img/NodeMCU-32S-Lua-WiFi-IoT-Entwicklung-Board-Serielle-WiFi-Modul-ESP32-38PIN-30PIN-ESP32-ESP32S-Entwicklung.jpg_960x960.jpg">
</center><br><br>

As a display, I used the <b>NX4832K035</b> Nextion enhanced 3.5 inch module. It is an HMI TFT display, with an integrated graphics processor,
with 16 MB flash memory, 1024 bytes EEPROM and 3584 bytes RAM. The display takes over the tasks of drawing graphics, and processing
taps on the touchscreen, as well as independently provides the clock, calendar, animation and text scrolling, which allows
significantly unload the main (<b>ESP32</b>) processor. The display communicates with the main processor via the <b>UART interface.</b>

<center>
  <img src="/img/NX4832K035-1.jpg">
<center><br><br>

The <b>HC-12</b> radio channel module is designed here for communication with a wireless sensor. Working frequency - 433.4 - 473.0 MHz.
Data transmission range - up to 1 km in open space. The number of data transmission channels is 100.
This module has a built-in microcontroller that independently solves all issues of receiving or transmitting data.
This module communicates with the main processor via the <b>UART interface.</b><br>
If the use of a wireless sensor is not planned, then this module can not be installed.<br>

<p align="center">
  <img src="/img/review26-3.jpg">
</p><br><br>

The weather monitor can read temperature, pressure, humidity and ambient light sensors.<br>
The following sensors are supported:<br>
<ul>
  <li><b>BME280</b></li>
  <li><b>BMP180</b></li>
  <li><b>DHT22</b></li>
  <li><b>SHT21</b></li>
  <li><b>BH1750</b></li>
  <li><b>MAX44009</b></li>
</ul>
You can connect one, none, several or all at once from this list.<br><br>

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
