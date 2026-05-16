#include <Adafruit_PCF8574.h> // v1.1.1 https://github.com/adafruit/Adafruit_PCF8574
#include "./sensors.hpp"

/**
 * Initialize PCF8574 port-expander
 */
void Sensors::_PCF8574Init(void) {
    if(pcf8574.begin(0x20, &Wire)) {
        _pcf8574_det = true;
        for(uint8_t i=0; i<8; i++) pcf8574.pinMode(i, OUTPUT);
        for(uint8_t i=0; i<8; i++) pcf8574.digitalWrite(i, LOW);
    }
}

/**
 * Send data to PCF8574
 */
void Sensors::comfortDevices(bool heater, bool cooler, bool humidifier, bool dehumidifier, bool purifier) {
    if(_pcf8574_det) {
        pcf8574.digitalWrite(0, heater);
        pcf8574.digitalWrite(1, cooler);
        pcf8574.digitalWrite(2, humidifier);
        pcf8574.digitalWrite(3, dehumidifier);
        pcf8574.digitalWrite(4, purifier);
    }
}