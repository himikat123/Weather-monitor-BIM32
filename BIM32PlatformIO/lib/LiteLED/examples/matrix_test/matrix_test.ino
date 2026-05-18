// Matrix panel test
// Using the LiteLED library
// Runs a series of tests on an square RGB LED matrix panel

#include <LiteLED.h>

// Choose the LED type from the list below.
// Un-comment the line below that matches your LED strip type.
// #define LED_TYPE        LED_STRIP_WS2812
// #define LED_TYPE        LED_STRIP_SK6812
// #define LED_TYPE        LED_STRIP_APA106
// #define LED_TYPE        LED_STRIP_SM16703

#define LED_TYPE_IS_RGBW    0   // if the LED matrix uses RGBW type LED's, change the 0 to 1

#define LED_GPIO        99  // change this number to be the GPIO pin connected to DIN of the matrix panel

// Change the MTRIX_X_SIZE #define below to match the size of one side of the matrix panel
// Only square panels are supported.
// The panel must be wired "row by row" fashion, not "serpintine".
#define MTRIX_X_SIZE     8

#define MTRIX_LEDS       ( MTRIX_X_SIZE * MTRIX_X_SIZE )    // total number of LED's in the matrix panel

#define A_DELAY         75      // # of ms delay between animations within a test
#define TST_PAUSE       1000    // # of ms to pause between tests
#define REPEAT_DELAY    3000    // # of ms to delay before repeating all tests 

static const uint8_t currBright = 20;    // change this to set the brightness level of the matrix

size_t c = 0;   // picker for the colour of the LEDs for the currrent test

static const crgb_t L_BLACK = 0x000000;
static const crgb_t colors[] = {
    0x2f0000,   /* red */
    0x002f00,   /* green */
    0x00002f,   /* blue */
    0x0f0f0f    /* white */
};  // the tests will cycle through the colours above; add more if you'd like
#define COLORS_TOTAL ( sizeof( colors ) / sizeof( crgb_t ) )

LiteLED myDisplay( LED_TYPE, LED_TYPE_IS_RGBW );    // create the LiteLED object

void setup() {
    myDisplay.begin( LED_GPIO, MTRIX_LEDS );        // initialze the myDisplay object.
    myDisplay.brightness( currBright );
}

void loop() {
    // fill the display with a color
    myDisplay.fill( colors[ c ], 1 );       // fill the matrix with a single colour and show it
    delay( TST_PAUSE );

    // flash the display
    for ( uint8_t k = 0; k < 3; k++ ) {       
        myDisplay.brightness( 0, 1 );
        delay( 250 );

        myDisplay.brightness( currBright, 1 );
        delay( 250 );
    }
    delay( TST_PAUSE );
    myDisplay.clear( 1 ); 

    // pixel walk - sequentially light up each pixel
    for ( size_t pxlNum = 0; pxlNum < MTRIX_LEDS; pxlNum++ ) {
        myDisplay.setPixel( pxlNum, colors[ c ], 1 );
        delay( A_DELAY );                       // pause before moving to the next pixel
        myDisplay.setPixel( pxlNum, L_BLACK );  // turn off the pixel we just light up
    }
    myDisplay.clear( 1 );   // clear the LED buffer, setting all colours to black and show it
    
    delay( TST_PAUSE );

    //  row test - sequentially light up each row
    for ( size_t i = 0; i < MTRIX_X_SIZE; i++ ) {
        for ( size_t j = 0; j < MTRIX_X_SIZE; j++ ) {
            size_t pxl = i * MTRIX_X_SIZE + j;
            myDisplay.setPixel( pxl, colors[ c ] );
        }
        myDisplay.show();
        delay( A_DELAY * 2 );       // pause before moving to the next row
        myDisplay.clear();
    }
    myDisplay.clear( 1 );

    delay( TST_PAUSE );

    //  column test - sequentially light up each column
    for ( size_t i = 0; i < MTRIX_X_SIZE; i++ ) {
        for ( size_t j = 0; j < MTRIX_X_SIZE; j++ ) {
            size_t pxl = i + j * MTRIX_X_SIZE;
            myDisplay.setPixel( pxl, colors[ c ] );
        }
        myDisplay.show();
        delay( A_DELAY *2 );       // pause before moving to the next column
        myDisplay.clear();
    }
    myDisplay.clear( 1 );
    
    if ( ++c >= COLORS_TOTAL ) {
        c = 0;
        delay( REPEAT_DELAY );
    }
    else {
        delay ( TST_PAUSE );
    }

}
