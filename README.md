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

## Schematic diagram of a weather monitor
![weather monitor BIM32 shematic](/img/BIM32schematic.png)


I used ready-made modules, so the device schematic is very simple. WiFi module **Nodemcu 32S** 30pin is applied. This is the heart and soul of the device, all the basic logic of the weather monitor is performed by this module. This module is an ESP32 SoC chip with all the necessary wiring, flash memory, USB-> UART converter and a power supply voltage regulator.

![ESP32](/img/NodeMCU-32S-Lua-WiFi-IoT-Entwicklung-Board-Serielle-WiFi-Modul-ESP32-38PIN-30PIN-ESP32-ESP32S-Entwicklung.jpg_960x960.jpg)


As a display, I used the **NX4832K035** Nextion enhanced 3.5 inch module. It is an HMI TFT display, with an integrated graphics processor, with 16 MB flash memory, 1024 bytes EEPROM and 3584 bytes RAM. The display takes over the tasks of drawing graphics, and processing taps on the touchscreen, as well as independently provides the clock, calendar, animation and text scrolling, which allows significantly unload the main (**ESP32**) processor. The display communicates with the main processor via the **UART interface.**

![Nextion Display](/img/NX4832K035-1.jpg)


The **HC-12** radio channel module is designed here for communication with a wireless sensor. Working frequency - 433.4 - 473.0 MHz. Data transmission range - up to 1 km in open space. The number of data transmission channels is 100. This module has a built-in microcontroller that independently solves all issues of receiving or transmitting data. This module communicates with the main processor via the **UART interface.** If the use of a wireless sensor is not planned, then this module can not be installed.

![HC-12](/img/review26-3.jpg)


The weather monitor can read temperature, pressure, humidity and ambient light sensors.
The following sensors are supported:
* **BME280**
* **BMP180**
* **DHT22**
* **SHT21**
* **BH1750**
* **MAX44009**
You can connect one, none, several or all at once from this list.


I ordered a PCB in China, because there was no desire to torment with a homemade one.

![PCB](/img/BIM32_t.png)


Watch a video overview of the weather monitor. The review is made in Russian, but you can turn on subtitles.

[![Video overview](/img/yt.png)](https://www.youtube.com/watch?v=fijWaK1R-Vs)


To flash the weather monitor you need a micro-sd card, micro-USB cable and a computer.
To flash the **display**, you need to copy the display firmware file (file with the **.tft** extension) to the micro-sd card (the card must be formatted with the **FAT32** file system). Then insert the micro-sd card into the micro-sd slot of the display and turn him on. The display itself will be flashed from the micro-sd card, the flashing process can be watched on the screen.

To flash the **ESP32**, download the **Flash Download Tools** and configure everything as in the screenshot. **COM port** specify the one on which your ESP32 actually sits.

![ESP DOWNLOAD TOOL](/img/ESP_DOWNLOAD_TOOL.png)


And of course, lastly, a couple of photos of the device in action.

![Weather monitor](/img/20200918_165534.jpg)

![Weather monitor](/img/20200918_165717.jpg)

![Weather monitor](/img/20200918_165741.jpg)

![Weather monitor](/img/20200918_170005.jpg)