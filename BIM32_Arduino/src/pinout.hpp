#if !defined(BIM32_CYD)
    uint8_t dummy = 1;
    /* 
    * BIM32 pinout 
    */
    #define SETTINGS_BUTTON_PIN     0 // Settings button pin
    #define DISPLAY1_BUTTON_PIN    34 // Display 1 button pin
    #define DISPLAY2_BUTTON_PIN    35 // Display 2 button pin
    #define ALARM_BUTTON_PIN       26 // Alarm button pin

    #define NEXTION_TX_PIN         32 // Nextion display RX pin
    #define NEXTION_RX_PIN         33 // Nextion display TX pin

    #define TFT_BACKLIGHT          13 // ILI9341 LED pin

    #define WS2812_1_DAT_PIN       33 // WS2812b display 1 pin
    #define WS2812_2_DAT_PIN       12 // WS2812b display 2 pin

    #define TM1637_1_CLK_PIN       32 // TM1637 display 1 CLK pin
    #define TM1637_1_DAT_PIN       14 // TM1637 display 1 DAT pin
    #define TM1637_2_CLK_PIN       19 // TM1637 display 2 CLK pin
    #define TM1637_2_DAT_PIN       15 // TM1637 display 2 DAT pin

    #define MAX7219_1_CLK_PIN      32 // MAX7219 display 1 CLK pin
    #define MAX7219_1_DAT_PIN      14 // MAX7219 display 1 DAT pin
    #define MAX7219_1_LOAD_PIN      5 // MAX7219 display 1 LOAD pin
    #define MAX7219_2_CLK_PIN      19 // MAX7219 display 2 CLK pin
    #define MAX7219_2_DAT_PIN      15 // MAX7219 display 2 DAT pin
    #define MAX7219_2_LOAD_PIN      2 // MAX7219 display 2 LOAD pin

    #define PCF8575_1_SCL_PIN      32 // PCF8575 display 1 SCL pin
    #define PCF8575_1_SDA_PIN      14 // PCF8575 display 1 SDA pin
    #define NUMITRON_1_PWM_PIN      5 // NUMITRON display 1 PWM pin
    #define PCF8575_2_SCL_PIN      19 // PCF8575 display 2 SCL pin
    #define PCF8575_2_SDA_PIN      15 // PCF8575 display 2 SDA pin
    #define NUMITRON_2_PWM_PIN      2 // NUMITRON display 2 PWM pin

    #define DHT22_PIN               4 // DHT22 sensor pin
    #define PHOTORESISTOR_PIN      36 // Photoresistor pin
    #define ONE_WIRE_BUS_PIN       27 // DS18B20 one-wire bus pin

    #define HC12_RX_PIN            16 // HC12 wireless module TX pin
    #define HC12_TX_PIN            17 // HC12 wireless module RX pin
    #define HC12_SET_PIN           25 // HC12 wireless module SET pin

    #define MP3_TX_PIN             23 // DFplayer RX pin
    #define MP3_BUSY_PIN           18 // DFplayer BUSY pin
#else
    uint8_t dummy = 2;
    /* 
     * BIM32_CYD pinout
     */
    #define SETTINGS_BUTTON_PIN     0 // Settings button pin
    #define DISPLAY1_BUTTON_PIN   134 // Display 1 button pin
    #define DISPLAY2_BUTTON_PIN   135 // Display 2 button pin
    #define ALARM_BUTTON_PIN      126 // Alarm button pin

    #define TFT_BACKLIGHT          21 // TFT LED pin
    #define XPT2046_IRQ            36 // TOUCH IRQ pin
    #define XPT2046_MOSI           32 // TOUCH DIN pin
    #define XPT2046_MISO           39 // TOUCH OUT pin
    #define XPT2046_CLK            25 // TOUCH CLK pin
    #define XPT2046_CS             33 // TOUCH CS pin

    #define WS2812_2_DAT_PIN      112 // WS2812b display 2 pin

    #define TM1637_2_CLK_PIN      119 // TM1637 display 2 CLK pin
    #define TM1637_2_DAT_PIN      115 // TM1637 display 2 DAT pin

    #define MAX7219_2_CLK_PIN     119 // MAX7219 display 2 CLK pin
    #define MAX7219_2_DAT_PIN     115 // MAX7219 display 2 DAT pin
    #define MAX7219_2_LOAD_PIN    112 // MAX7219 display 2 LOAD pin

    #define PCF8575_1_SCL_PIN     119 // PCF8575 display 1 SCL pin
    #define PCF8575_1_SDA_PIN     115 // PCF8575 display 1 SDA pin
    #define NUMITRON_1_PWM_PIN    112 // NUMITRON display 1 PWM pin
    #define PCF8575_2_SCL_PIN     119 // PCF8575 display 2 SCL pin
    #define PCF8575_2_SDA_PIN     115 // PCF8575 display 2 SDA pin
    #define NUMITRON_2_PWM_PIN    112 // NUMITRON display 2 PWM pin

    #define DHT22_PIN              27 // DHT22 sensor pin
    #define PHOTORESISTOR_PIN      34 // Photoresistor pin
    #define ONE_WIRE_BUS_PIN      127 // DS18B20 one-wire bus pin

    #define HC12_RX_PIN           116 // HC12 wireless module TX pin
    #define HC12_TX_PIN           117 // HC12 wireless module RX pin
    #define HC12_SET_PIN          125 // HC12 wireless module SET pin

    #define MP3_TX_PIN            123 // DFplayer RX pin
    #define MP3_BUSY_PIN          118 // DFplayer BUSY pin
#endif