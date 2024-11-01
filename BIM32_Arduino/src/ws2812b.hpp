#include <LiteLED.h> // v1.2.0 https://github.com/Xylopyrographer/LiteLED/tree/main

LiteLED strip_1(LED_STRIP_WS2812, false, RMT_CHANNEL_2);
LiteLED strip_2(LED_STRIP_WS2812, false, RMT_CHANNEL_3);

#define DISP4 0
#define DISP6 1

class WS2812b : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, uint8_t pin);
        void refresh();

    private:
        LiteLED* strip;
        uint8_t _pixelCount = 1;                                // number of pixels in display
        byte pixels[6] = {0, 0, 0, 0, 0, 0};
        byte pixelsPrev[6] = {0, 0, 0, 0, 0, 0};
        uint8_t reds[6] = {0, 0, 0, 0, 0, 0};
        uint8_t redsPrev[6] = {0, 0, 0, 0, 0, 0};
        uint8_t greens[6] = {0, 0, 0, 0, 0, 0};
        uint8_t greensPrev[6] = {0, 0, 0, 0, 0, 0};
        uint8_t blues[6] = {0, 0, 0, 0, 0, 0};
        uint8_t bluesPrev[6] = {0, 0, 0, 0, 0, 0};

        void _print();
        void _sendToDisplay();
        uint8_t _sendTwoDigits(rgb_t black, uint8_t digShift, uint8_t pixelShift);
};

/**
 * Display initialize
 * @param dispNum display number
 * @param pin display pin number
 */
void WS2812b::init(uint8_t dispNum, uint8_t pin) {
    _dispNum = dispNum;
 
    switch(config.display_model(dispNum)) {
        case 0: _pixelCount = 4 * 7 * 1 + 2 + 1; break; // 4 digits, 7 segments, 1 led per segment, 2 clock points, 1 sacrificial led
        case 1: _pixelCount = 4 * 7 * 2 + 2 + 1; break;
        case 2: _pixelCount = 4 * 7 * 3 + 2 + 1; break;
        case 3: _pixelCount = 6 * 7 * 1 + 4 + 1; break;
        case 4: _pixelCount = 6 * 7 * 2 + 4 + 1; break;
        case 5: _pixelCount = 6 * 7 * 3 + 4 + 1; break;
        default: _pixelCount = 1; break;
    } 

    strip = _dispNum == 0 ? &strip_1 : &strip_2;
    strip->begin(pin, _pixelCount);
    strip->clear(true);
    _setModel(config.display_model(dispNum));
}

/**
 * Displays the next display slot
 */
void WS2812b::refresh() {
    _slotSwitch();
    _segAnimations();
    _print();
}

/**
 * Display data on the display
 */
void WS2812b::_print() {
    bool updated = false;

    for(uint8_t i=0; i<6; i++) {
        _points[i] = _dispImg[i] >= 100;
        pixels[i] = font_ws2812[_dispImg[i] >= 100 ? _dispImg[i] - 100 : _dispImg[i]];
        unsigned int colors = strtol(&_dispColors[i][1], NULL, 16);
        reds[i] = colors >> 16;
        greens[i] = colors >> 8 & 0xFF;
        blues[i] = colors & 0xFF;

        if(pixelsPrev[i] != pixels[i] or redsPrev[i] != reds[i] or
           greensPrev[i] != greens[i] or bluesPrev[i] != blues[i]
        ) {
            pixelsPrev[i] = pixels[i];
            redsPrev[i] = reds[i];
            greensPrev[i] = greens[i];
            bluesPrev[i] = blues[i];
            updated |= true;
        }
    }

    for(uint8_t i=0; i<4; i++) {
        if(_prevPoints[i] != _points[i]) {
            _prevPoints[i] = _points[i];
            updated |= true;
        }
    }

    if(updated) _sendToDisplay();
}

/**
 * Send data to display
 */
void WS2812b::_sendToDisplay() {
    uint8_t bright = _brightness;
    if(bright < config.display_brightness_min(_dispNum)) bright = config.display_brightness_min(_dispNum);
    if(bright > config.display_brightness_max(_dispNum)) bright = config.display_brightness_max(_dispNum);  
    strip->brightness(bright, false);

    rgb_t black = { .r = 0, .g = 0, .b = 0 };
    rgb_t dotsColor = { .r = reds[0], .g = greens[0], .b = blues[0] };
    uint8_t lastPixel = 0;
    strip->setPixel(lastPixel++, black, false);
    lastPixel = _sendTwoDigits(black, 0, lastPixel++);
    strip->setPixel(lastPixel++, (_power && _points[0] && !_animIsRunnung) ? dotsColor : black, false);
    strip->setPixel(lastPixel++, (_power && _points[1] && !_animIsRunnung) ? dotsColor : black, false);
    lastPixel = _sendTwoDigits(black, 2, lastPixel++);

    if(config.display_model(_dispNum) > 2) {
        strip->setPixel(lastPixel++, (_power && _points[2] && !_animIsRunnung) ? dotsColor : black, false);
        strip->setPixel(lastPixel++, (_power && _points[3] && !_animIsRunnung) ? dotsColor : black, false);
        lastPixel = _sendTwoDigits(black, 4, lastPixel++);
    }

    strip->show();
}

uint8_t WS2812b::_sendTwoDigits(rgb_t black, uint8_t digShift, uint8_t pixelNr) {
    uint8_t repeats = 1;

    switch(config.display_model(_dispNum)) {
        case 1: 
        case 4: repeats = 2; break;
        case 2:
        case 5: repeats = 3; break;
        default: ; break;
    }

    for(uint8_t digNr=0; digNr<2; digNr++) {
        for(uint8_t bitNr=0; bitNr<7; bitNr++) {
            for(uint8_t repeat=0; repeat<repeats; repeat++) {
                uint8_t imgNr = digNr + digShift;

                if(bitRead(pixels[imgNr], bitNr) and _power) {
                    rgb_t color = { .r = reds[imgNr], .g = greens[imgNr], .b = blues[imgNr] };
                    strip->setPixel(pixelNr++, color, false);
                }
                else strip->setPixel(pixelNr++, black, false);
            }
        }
    }

    return pixelNr;
}