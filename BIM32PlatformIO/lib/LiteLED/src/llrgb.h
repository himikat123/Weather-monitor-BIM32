/*
    Helper stuff for the LiteLED library
        - Ported from FastLED
        - MIT Licensed as described in the file LICENSE
*/

#ifndef __LLRGB_H__
#define __LLRGB_H__

#define LIB8STATIC               __attribute__ ((unused)) static inline
#define LIB8STATIC_ALWAYS_INLINE __attribute__ ((always_inline)) static inline
typedef uint8_t fract8;   // ANSI: unsigned short _Fract

//  scale one byte by a second one, which is treated as
//  the numerator of a fraction whose denominator is 256
//  In other words, it computes i * (scale / 256)
//  4 clocks AVR with MUL, 2 clocks ARM
LIB8STATIC_ALWAYS_INLINE uint8_t scale8( uint8_t i, fract8 scale ) {
    return ( ( ( uint16_t ) i ) * ( 1 + ( uint16_t )( scale ) ) ) >> 8;
}

//  The "video" version of scale8 guarantees that the output will
//  be only be zero if one or both of the inputs are zero.  If both
//  inputs are non-zero, the output is guaranteed to be non-zero.
//  This makes for better 'video'/LED dimming, at the cost of
//  several additional cycles.
LIB8STATIC_ALWAYS_INLINE uint8_t scale8_video( uint8_t i, fract8 scale ) {
    return ( ( ( int ) i * ( int ) scale ) >> 8 ) + ( ( i && scale ) ? 1 : 0 );
}

// RGB color representation (array style)
typedef struct {
    union {
        uint8_t r;
        uint8_t red;
    };
    union {
        uint8_t g;
        uint8_t green;
    };
    union {
        uint8_t b;
        uint8_t blue;
    };
} rgb_t;

// RGB color representation (colour code style)
//   where colour code = 0xRRGGBB
typedef uint32_t crgb_t;

// This allows testing a RGB for zero-ness
static inline bool rgb_is_zero( rgb_t a ) {
    return !( a.r | a.g | a.b );
}

// Create rgb_t color from 24-bit color code 0xRRGGBB
static inline rgb_t rgb_from_code( crgb_t color_code ) {
    rgb_t res = {
        .r = ( uint8_t )( ( color_code >> 16 ) & 0xff ),
        .g = ( uint8_t )( ( color_code >> 8 ) & 0xff ),
        .b = ( uint8_t )( color_code & 0xff ),
    };
    return res;
}

// Create rgb_t color from values
static inline rgb_t rgb_from_values( uint8_t r, uint8_t g, uint8_t b ) {
    rgb_t res = {
        .r = r,
        .g = g,
        .b = b,
    };
    return res;
}

// Convert RGB color to 24-bit color code 0x00RRGGBB
static inline crgb_t rgb_to_code( rgb_t color ) {
    return ( ( crgb_t )color.r << 16 ) | ( ( crgb_t )color.g << 8 ) | color.b;
}

// Get the 'luma' of a RGB color - aka roughly how much light the RGB pixel
// is putting out (from 0 to 255).
static inline uint8_t rgb_luma( rgb_t a ) {
    return scale8( a.r, 54 ) + scale8( a.g, 183 ) + scale8( a.b, 18 );
}

#endif
// llrgb.h
