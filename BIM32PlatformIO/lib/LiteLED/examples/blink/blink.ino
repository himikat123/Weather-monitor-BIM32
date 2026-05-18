// blink example
// Using the LiteLED library
// - blinks a colour RGB LED attached to a GPIO pin of the ESP32

#include <LiteLED.h>

// Choose the LED type from the list below.
// Comment out all but one LED_TYPE.
#define LED_TYPE        LED_STRIP_WS2812
// #define LED_TYPE        LED_STRIP_SK6812
// #define LED_TYPE        LED_STRIP_APA106
// #define LED_TYPE        LED_STRIP_SM16703

#define LED_TYPE_IS_RGBW 0   // if the LED is an RGBW type, change the 0 to 1

#define LED_GPIO 42     // change this number to be the GPIO pin connected to the LED

#define LED_BRIGHT 30   // sets how bright the LED is. O is off; 255 is burn your eyeballs out (not recommended)

// pick the colour you want from the list here and change it in setup()
static const crgb_t L_RED = 0xff0000;
static const crgb_t L_GREEN = 0x00ff00;
static const crgb_t L_BLUE = 0x0000ff;
static const crgb_t L_WHITE = 0xe0e0e0;

LiteLED myLED( LED_TYPE, LED_TYPE_IS_RGBW );    // create the LiteLED object; we're calling it "myLED"

void setup() {
    myLED.begin( LED_GPIO, 1 );         // initialze the myLED object. Here we have 1 LED attached to the LED_GPIO pin
    myLED.brightness( LED_BRIGHT );     // set the LED photon intensity level
    myLED.setPixel( 0, L_GREEN, 1 );    // set the LED colour and show it
    delay( 2000 );
}

void loop() {
    // flash the LED
    myLED.brightness( 0, 1 );           // turn the LED off
    delay( 1000 );

    myLED.brightness( LED_BRIGHT, 1 );  // turn the LED on
    delay( 1000 );
} 
