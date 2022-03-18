# Weather monitor BIM32
## Weather monitor based on ESP32

![Weather monitor based on ESP32](/img/screen.png) 

### A short list of weather monitor features:

* Connecting to your home network WiFi 2.4 GHz
* Display of the current weather and 3-day forecast
* Display 5-day hourly weather forecast
* Show weather history (hourly) 1 day ago
* Building graphs of temperature, humidity and probability of precipitation
* Home temperature and humidity display
* Clock (small/large/with seconds/arrow) with NTP synchronization
* Calendar with the ability to scroll to plus/minus infinity
* Sending and/or receiving data using the MQTT protocol
* Sending and/or receiving data from/to thingspeak.com
* Sending data to narodmon.ru
* Wireless temperature/pressure/humidity/ambient light sensor
* Support for wired temperature/pressure/humidity/ambient light sensors
* Auto adjustment of screen backlight brightness (by light sensor, by time or by sunrise and sunset)

## Schematic diagram of a weather monitor<br>
<p align="center">
  <img src="/img/BIM32schematic.png" alt="weather monitor BIM32 shematic
</p><br><br>

I used ready-made modules, so the device schematic is very simple. WiFi module <b>Nodemcu 32S</b> 30pin is applied.
This is the heart and soul of the device, all the basic logic of the weather monitor is performed by this module. This module is an ESP32 SoC chip with all the necessary wiring, flash memory, USB-> UART converter and a power supply voltage regulator.<br>

<p align="center">
  <img src="/img/NodeMCU-32S-Lua-WiFi-IoT-Entwicklung-Board-Serielle-WiFi-Modul-ESP32-38PIN-30PIN-ESP32-ESP32S-Entwicklung.jpg_960x960.jpg">
</p><br><br>

As a display, I used the <b>NX4832K035</b> Nextion enhanced 3.5 inch module. It is an HMI TFT display, with an integrated graphics processor,
with 16 MB flash memory, 1024 bytes EEPROM and 3584 bytes RAM. The display takes over the tasks of drawing graphics, and processing
taps on the touchscreen, as well as independently provides the clock, calendar, animation and text scrolling, which allows
significantly unload the main (<b>ESP32</b>) processor. The display communicates with the main processor via the <b>UART interface.</b>

<p align="center">
  <img src="/img/NX4832K035-1.jpg">
</p><br><br>

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

I ordered a PCB in China, because there was no desire to torment with a homemade one.<br>

<p align="center">
  <img src="/img/BIM32_t.png">
</p><br><br>

Watch a video overview of the weather monitor. The review is made in Russian, but you can turn on subtitles.<br>

<p align="center">
  <a href="https://www.youtube.com/watch?v=fijWaK1R-Vs">
    <img src="/img/yt.png">
  </a>
</p><br><br>

To flash the weather monitor you need a micro-sd card, micro-USB cable and a computer.<br>
To flash the <b>display</b>, you need to copy the display firmware file (file with the <b>.tft</b> extension) to the micro-sd card
(the card must be formatted with the <b>FAT32</b> file system). Then insert the micro-sd card into the micro-sd slot of the display
and turn him on. The display itself will be flashed from the micro-sd card, the flashing process can be watched on the screen.<br>

To flash the <b>ESP32</b>, download the <b>Flash Download Tools</b> and configure everything as in the screenshot. <b>COM port</b> specify the one on
which your ESP32 actually sits.<br>

<p align="center">
  <img src="/img/ESP_DOWNLOAD_TOOL.png">
</p><br><br>

And of course, lastly, a couple of photos of the device in action.<br>

<p align="center">
  <img src="/img/20200918_165534.jpg" alt="Weather monitor">
</p><br>
<p align="center">
  <img src="/img/20200918_165717.jpg" alt="Weather monitor">
</p><br>
<p align="center">
  <img src="/img/20200918_165741.jpg" alt="Weather monitor">
</p><br>
<p align="center">
  <img src="/img/20200918_170005.jpg" alt="Weather monitor">
</p><br>
