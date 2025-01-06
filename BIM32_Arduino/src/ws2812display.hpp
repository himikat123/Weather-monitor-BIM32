#include <LiteLED.h> // v1.2.0 https://github.com/Xylopyrographer/LiteLED/tree/main

LiteLED strip_1(LED_STRIP_WS2812, false, RMT_CHANNEL_2);
LiteLED strip_2(LED_STRIP_WS2812, false, RMT_CHANNEL_3);

#define DISP4 0
#define DISP6 1
#define DOT 100

class WS2812b : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, uint8_t pin);
        void refresh();

    private:
        LiteLED* _strip;
        uint8_t _pixelCount = 1;                                // number of pixels in display
        byte _pixels[6] = {0, 0, 0, 0, 0, 0};
        byte _pixelsPrev[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _reds[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _redsPrev[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _greens[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _greensPrev[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _blues[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _bluesPrev[6] = {0, 0, 0, 0, 0, 0};

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

    _strip = _dispNum == 0 ? &strip_1 : &strip_2;
    _strip->begin(pin, _pixelCount);
    _strip->clear(true);
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
        _points[i] = _dispImg[i] >= DOT;
        _pixels[i] = font_ws2812[_dispImg[i] >= DOT ? _dispImg[i] - DOT : _dispImg[i]];
        unsigned int colors = strtol(&_dispColors[i][1], NULL, 16);
        _reds[i] = colors >> 16;
        _greens[i] = colors >> 8 & 0xFF;
        _blues[i] = colors & 0xFF;

        if(_pixelsPrev[i] != _pixels[i] or _redsPrev[i] != _reds[i] or
           _greensPrev[i] != _greens[i] or _bluesPrev[i] != _blues[i]
        ) {
            _pixelsPrev[i] = _pixels[i];
            _redsPrev[i] = _reds[i];
            _greensPrev[i] = _greens[i];
            _bluesPrev[i] = _blues[i];
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
    uint8_t bright = (uint8_t)map(_brightness, 0, 100, 0, 255);
    bright = constrain(bright, 0, 255);
    if(bright < config.display_brightness_min(_dispNum)) bright = config.display_brightness_min(_dispNum);
    if(bright > config.display_brightness_max(_dispNum)) bright = config.display_brightness_max(_dispNum);  
    _strip->brightness(bright, false);

    rgb_t black = { .r = 0, .g = 0, .b = 0 };
    rgb_t dotsColor = { .r = _reds[0], .g = _greens[0], .b = _blues[0] };
    _strip->setPixel(0, black, false);
    uint8_t lastPixel = _sendTwoDigits(black, 0, 1);
    _strip->setPixel(lastPixel++, (_power && _points[0] && !_animIsRunnung) ? dotsColor : black, false);
    _strip->setPixel(lastPixel++, (_power && _points[1] && !_animIsRunnung) ? dotsColor : black, false);
    lastPixel = _sendTwoDigits(black, 2, lastPixel++);

    if(config.display_model(_dispNum) > 2) {
        _strip->setPixel(lastPixel++, (_power && _points[2] && !_animIsRunnung) ? dotsColor : black, false);
        _strip->setPixel(lastPixel++, (_power && _points[3] && !_animIsRunnung) ? dotsColor : black, false);
        lastPixel = _sendTwoDigits(black, 4, lastPixel++);
    }

    _strip->show();
    vTaskDelay(1);
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

                if(bitRead(_pixels[imgNr], bitNr) and _power) {
                    rgb_t color = { .r = _reds[imgNr], .g = _greens[imgNr], .b = _blues[imgNr] };
                    _strip->setPixel(pixelNr++, color, false);
                }
                else _strip->setPixel(pixelNr++, black, false);
            }
        }
    }

    return pixelNr;
}