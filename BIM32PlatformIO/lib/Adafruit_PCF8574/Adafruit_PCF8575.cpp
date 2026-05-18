/*!
 *  @file Adafruit_PCF8575.cpp
 *
 *  @brief Adafruit PCF8575 I2C Potentiometer
 *
 *  @details
 *
 * 	I2C Driver for the NXP PCF8575 I2C GPIO expander
 *
 * 	This is a library for the Adafruit PCF8575 breakout:
 * 	http://www.adafruit.com/products
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *  Dependencies
 *
 *  This library depends on the Adafruit BusIO library
 *
 *  @author Author
 *
 *  ladyada for Adafruit Industries
 *
 * 	License
 *
 * 	BSD (see license.txt)
 */

#include "Adafruit_PCF8575.h"

/*!
 *    @brief  Instantiates a new PCF8575 class
 */
Adafruit_PCF8575::Adafruit_PCF8575(void) {}

/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  i2c_address
 *            The I2C address to be used.
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_PCF8575::begin(uint8_t i2c_address, TwoWire *wire) {
  delete (i2c_dev);

  i2c_dev = new Adafruit_I2CDevice(i2c_address, wire);

  if (!i2c_dev->begin()) {
    return false;
  }

  return true;
}

/*!
 *    @brief  Write one 'word' of data directly to the GPIO control register
 *    @param  d The data to write
 *    @return True if we were able to write the data successfully over I2C
 */
bool Adafruit_PCF8575::digitalWriteWord(uint16_t d) {
  _writebuf = d;
  return i2c_dev->write((uint8_t *)&_writebuf, 2);
}

/*!
 *    @brief  Read one 'word' of data directly from the GPIO control register
 *    @return The byte of data read from the device
 */
uint16_t Adafruit_PCF8575::digitalReadWord(void) {
  i2c_dev->read((uint8_t *)&_readbuf, 2);
  return _readbuf;
}

/*!
 *    @brief  Set one GPIO expander pin to 'high' (weak pullup) or 'low'
 * (grounded)
 *    @param  pinnum The GPIO pin number, from 0 to 7 inclusive
 *    @param  val The boolean value to write: true means activate the pullup
 *    and false means turn on the sinking transistor.
 *    @return True if we were able to write the data successfully over I2C
 */
bool Adafruit_PCF8575::digitalWrite(uint8_t pinnum, bool val) {
  if (val) {
    _writebuf |= 1UL << pinnum;
  } else {
    _writebuf &= ~(1UL << pinnum);
  }
  return i2c_dev->write((uint8_t *)&_writebuf, 2);
}

/*!
 *    @brief  Set one GPIO expander pin to 'output' (grounded) or 'input' (weak
 * pullup)
 *    @param  pinnum The GPIO pin number, from 0 to 15 inclusive
 *    @param  val The value to write: INPUT or INPUT_PULLUP means activate the
 * pullup and OUTPUT means turn on the sinking transistor, as this is an open
 * drain device
 *    @return True if we were able to write the data successfully over I2C
 */
bool Adafruit_PCF8575::pinMode(uint8_t pinnum, uint8_t val) {
  if ((val == INPUT) || (val == INPUT_PULLUP)) {
    _writebuf |= 1UL << pinnum;
  } else {
    _writebuf &= ~(1UL << pinnum);
  }
  return i2c_dev->write((uint8_t *)&_writebuf, 2);
}

/*!
 *    @brief  Get a GPIO expander pin value
 *    @param  pinnum The GPIO pin number, from 0 to 7 inclusive
 *    @return True if the pin logic is NOT ground, false if the pin logic is
 * ground
 */
bool Adafruit_PCF8575::digitalRead(uint8_t pinnum) {
  i2c_dev->read((uint8_t *)&_readbuf, 2);
  return (_readbuf >> pinnum) & 0x1;
}
