#pragma once
#include <Arduino.h>

const uint8_t font_numitron[2][18] PROGMEM = {
    { // !e !d !g !f !a !c !b !h
        0x21, // 0
        0xF9, // 1
        0x15, // 2
        0x91, // 3
        0xC9, // 4
        0x83, // 5
        0x03, // 6
        0xF1, // 7
        0x01, // 8
        0x81, // 9
        0xC5, // °
        0x41, // A
        0x27, // C
        0x45, // P
        0x49, // H
        0xFF, // space
        0xDF, // -
        0x47  // F
    },
    {
        0x20, // 0.
        0xF8, // 1.
        0x14, // 2.
        0x90, // 3.
        0xC8, // 4.
        0x82, // 5.
        0x02, // 6.
        0xF0, // 7.
        0x00, // 8.
        0x80, // 9.
        0xC4, // °.
        0x40, // A.
        0x26, // C.
        0x44, // P.
        0x48, // H.
        0xFE, // space + .
        0xDE, // -.
        0x46  // F.
    }
};