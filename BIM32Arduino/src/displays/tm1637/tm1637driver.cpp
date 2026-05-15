#include <Arduino.h>
#include "./tm1637driver.hpp"

/**
 * TM163 initialize
 * @param clk display CLK pin number
 * @param dat display DAT pin number
 */
void TM1637_Driver::init(int8_t clk, int8_t dat) {
    _clk = clk;
    _dat = dat;
}

/**
 * Send a byte to display
 */
void TM1637_Driver::_writeByte(byte data) {
    if(_clk > 0 && _dat > 0) {
        for(uint8_t i=0; i<8; i++) {
            digitalWrite(_clk, LOW);
            delayMicroseconds(50);
            if(data & 0x01) digitalWrite(_dat, HIGH);
            else digitalWrite(_dat, LOW);
            data >>= 1;
            delayMicroseconds(50);
            digitalWrite(_clk, HIGH);
            delayMicroseconds(50);
        }
        digitalWrite(_clk, LOW);
        delayMicroseconds(50);
        digitalWrite(_dat, HIGH);
        delayMicroseconds(50);
        digitalWrite(_clk, HIGH);
        pinMode(_dat, INPUT);
        delayMicroseconds(50);
        uint8_t ack = digitalRead(_dat);
        if(ack == 0) digitalWrite(_dat, LOW);
        delayMicroseconds(50);
        pinMode(_dat, OUTPUT);
        delayMicroseconds(50);
    }
}

/**
 * Sends start command to display
 */
void TM1637_Driver::start() {
    if(_clk > 0 && _dat > 0) {
        digitalWrite(_clk, HIGH);
        delayMicroseconds(50);
        digitalWrite(_dat, HIGH);
        delayMicroseconds(50);
        digitalWrite(_dat, LOW);
        delayMicroseconds(50);
        digitalWrite(_clk, LOW);
        delayMicroseconds(50);
    }
}

/**
 * Sends stop command to display
 */
void TM1637_Driver::stop(void) {
    if(_clk > 0 && _dat > 0) {
        digitalWrite(_clk, LOW);
        delayMicroseconds(50);
        digitalWrite(_dat, LOW);
        delayMicroseconds(50);
        digitalWrite(_clk, HIGH);
        delayMicroseconds(50);
        digitalWrite(_dat, HIGH);
        delayMicroseconds(50);
    }
}