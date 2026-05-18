/*!
 *  @file Adafruit_PCF8574.h
 *
 * 	I2C Driver for PCF8574 I2C GPIO expander
 *
 * 	This is a library for the Adafruit PCF8574 breakout:
 * 	http://www.adafruit.com/products
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *
 *	BSD license (see license.txt)
 */

#ifndef _ADAFRUIT_PCF8574_H
#define _ADAFRUIT_PCF8574_H

#include "Arduino.h"
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>

#define PCF8574_I2CADDR_DEFAULT 0x20 ///< PCF8574 default I2C address

/*!
 *    @brief  Class that stores state and functions for interacting with
 *            the PCF8574 I2C Expander
 */
class Adafruit_PCF8574 {
public:
  Adafruit_PCF8574();
  bool begin(uint8_t i2c_addr = PCF8574_I2CADDR_DEFAULT, TwoWire *wire = &Wire);

  bool digitalWriteByte(uint8_t d);
  bool digitalWrite(uint8_t pinnum, bool val);
  uint8_t digitalReadByte(void);
  bool digitalRead(uint8_t pinnum);
  bool pinMode(uint8_t pinnum, uint8_t val);

private:
  uint8_t _readbuf = 0, _writebuf = 0;

  Adafruit_I2CDevice *i2c_dev;
};

#endif
