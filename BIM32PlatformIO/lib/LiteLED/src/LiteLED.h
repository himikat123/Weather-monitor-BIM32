/*
    RMT-based ESP-IDF driver for WS2812B/SK6812/APA106/SM16703 LED strips
    MIT Licensed as described in the file LICENSE
*/

#pragma once

#ifndef __LITELED_H__
#define __LITELED_H__

#include <hal/rmt_types.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <driver/rmt.h>
#include "llrgb.h"

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL( 4, 4, 1 ) || ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL( 5, 0, 0 )
    #error "LiteLED requires arduino-esp32 core version to be at least 2.0.3 but less than 3.0.0."
#endif

// following defines are 'magic numbers' from the esp-idf-library led_strip driver
#define LED_STRIP_FLUSH_TIMEOUT  1000UL
#define LED_STRIP_PAUSE_LENGTH   50UL
#define LED_STRIP_SHOW_TIMEOUT   1000UL

enum led_strip_type_t {
    LED_STRIP_WS2812 = 0,
    LED_STRIP_SK6812,
    LED_STRIP_APA106,
    LED_STRIP_SM16703,
    LED_STRIP_TYPE_MAX
};

// LED strip descriptor
typedef struct {
    led_strip_type_t type;      // < LED type
    bool is_rgbw;               // < set true for RGBW strips
    bool auto_w;                // < set false to disable auto W channel setting for RBGW strips
    uint8_t brightness;         // < brightness 0..255, pending next call to ::show() after change.
    uint8_t bright_act;         // < actual brightness as set after last ::show
    size_t length;              // < number of LEDs in strip
    gpio_num_t gpio;            // < GPIO pin connected to the strip DIN
    rmt_channel_t channel;      // < ESP32 RMT channel to use
    uint8_t* buf;               // < pointer to the buffer that holds the LED data
} led_strip_t;

class LiteLED {
public:
    // @brief Class constructor. Set the LED parameters for the RMT driver
    // @param led_type Enumerated value for the type of LED's in the strip
    // @param rgbw Set true if the strip is RGBW type
    // @param channel Optional. Enumerated RMT channel to use for this strip
    // @return `ESP_OK` on success
    LiteLED( led_strip_type_t led_type, bool rgbw, rmt_channel_t channel = RMT_CHANNEL_0 );

    // @brief Initialize the strip
    // @param data_pin GPIO pin connected to the DIN pin of the strip
    // @param length Number of LED's in the strip
    // @param auto_w Optional. Only used for RGBW strips. Set false to not use the automatic W channel value set by the library
    // @return `ESP_OK` on success
    esp_err_t begin( uint8_t data_pin, size_t length, bool auto_w = 1 );

    // @brief Send the LED buffer to the strip
    esp_err_t show();

    // @brief Set color of single LED in strip, optionally send buffer to the strip
    // @param num Position of the LED in the strip, 0-based
    // @param color Colour to set the LED to
    // @param show Optional. Set true to send the buffer to the strip before returning
    // @return `ESP_OK` on success
    esp_err_t setPixel( size_t num, rgb_t color, bool show = 0 );
    esp_err_t setPixel( size_t num, crgb_t color, bool show = 0 );

    // @brief Set colors of multiple consecutive LEDs, optionally send buffer to the strip
    // @param start First LED index, 0-based
    // @param len The number of consecutive LEDs in the strip to which we are writing
    // @param data Pointer to data. Layout must match the colour type
    // @param show Optional. Set true to send the buffer to the strip before returning
    // @return `ESP_OK` on success
    esp_err_t setPixels( size_t start, size_t len, rgb_t* data, bool show = 0 );
    esp_err_t setPixels( size_t start, size_t len, crgb_t* data, bool show = 0 );

    // @brief Set the entire strip to a color, optionally send buffer to the LEDs
    // @param color rgb_t or crgb_t Colour value to set the strip to
    // @param show Optional. Set true to send the buffer to the strip before returning
    // @return `ESP_OK` on success
    esp_err_t fill( rgb_t color, bool show = 0 );
    esp_err_t fill( crgb_t color, bool show = 0 );

    // @brief Clear the strip buffer, optionally send buffer to the strip
    // @param show Optional. Set true to send the buffer to the strip before returning
    // @return `ESP_OK` on success
    esp_err_t clear( bool show = 0 );

    // @brief Set the intensity of the LEDs, optionally send buffer to the strip
    // @param bright Brightness value, 0-255
    // @param show Optional. Set true to set strip intensity to `bright` before returning
    // @return `ESP_OK` on success
    esp_err_t brightness( uint8_t bright, bool show = 0 );

    // @brief Get the intensity value of the LEDs
    // @return The `bright` value of the strip
    uint8_t getBrightness( );

    // @brief Get, in rgb_t format, the color of single LED in strip
    // @param num Position of the LED in the strip, 0-based
    // @return The rgb_t color value of the LED
    rgb_t getPixel( size_t num );

    // @brief Get, in crgb_t format, the color of single LED in strip
    // @param num Position of the LED in the strip, 0-based
    // @return The crgb_t color value of the LED
    crgb_t getPixelC( size_t num );

private:
    led_strip_t theStrip;   // LED strip object for this class


};   // class LiteLED

#endif /* __LITELED_H__ */

