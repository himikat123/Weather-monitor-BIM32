#define DISP4 0
#define DISP6 1

class SegmentDisplay {
    public:
        void setDotFreq(unsigned int freq);
        void displayToggle();
        void displayOn();
        void displayOff();
        bool isDisplayOn();
        void brightness(unsigned int intensity, bool reduc);

    protected:
        bool _dots = false;                                     // time points
        bool _clockdots = false;                                // need for time points
        bool _dot1 = false;
        bool _dot2 = false;
        unsigned int _dotfreq = 500;                            // time points period
        bool _power = true;                                     // display on/off flag
        unsigned int _brightness = 40;                          // display brightness
        int _dispImg[6] = {                                     // display digits
            SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
        };
        char _dispColors[6][8] = {"", "", "", "", "", ""};
        uint8_t _dispNum = 0;
        bool _animIsRunnung = false;

        void _slotSwitch();
        void _segAnimations();

    private:
        uint8_t _slot = 0;
        uint8_t _prevSlot = 0;
        uint8_t _animSlot = 0;
        uint32_t _animMillis = 0;
        uint32_t _prevSlotMillis = 0;
        const uint8_t _FRAMES[8] = {1, 5, 5, 8, 8, 4, 4, 4}; // number of frames in effect
        const int8_t _SHIFTS[8][8][4] = { /* shifts[effect num][frame num][segment num] =0: blank, >0: new data, <0: old data. */
            {{1, 2, 3, 4},    {0, 0, 0, 0},   {0, 0, 0, 0},  {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
            {{0, -1, -2, -3}, {4, 0, -1, -2}, {3, 4, 0, -1}, {2, 3, 4, 0}, {1, 2, 3, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
            {{-2, -3, -4, 0}, {-3, -4, 0, 1}, {-4, 0, 1, 2}, {0, 1, 2, 3}, {1, 2, 3, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
            {{0, -1, -2, -3}, {0, 0, -1, -2}, {0, 0, 0, -1}, {0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 2}, {0, 1, 2, 3}, {1, 1, 3, 4}},
            {{-2, -3, -4, 0}, {-3, -4, 0, 0}, {-4, 0, 0, 0}, {0, 0, 0, 0}, {4, 0, 0, 0}, {3, 4, 0, 0}, {2, 3, 4, 0}, {1, 2, 3, 4}},
            {{-2, 0, 0, -3},  {0, 0, 0, 0},   {2, 0, 0, 3},  {1, 2, 3, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
            {{-1, -2, -3, 4}, {-1, -2, 3, 4}, {-1, 2, 3, 4}, {1, 2, 3, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
            {{1, -2, -3, -4}, {1, 2, -3, -4}, {1, 2, 3, -4}, {1, 2, 3, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
        };

        void _segGetData(int* segImg, uint8_t slot, bool dots);
        void _clock(int* segImg);
        void _date(int* segImg);
        void _temp(float t, uint8_t digs, int* segImg);
        void _hum(float h, uint8_t digs, int* segImg);
        void _pres(float p, uint8_t digs, int* segImg);
        void _iaq(float i, uint8_t digs, int* segImg);
        void _co2(float co2, uint8_t digs, int* segImg);
};

/**
 * Set time points frequency (period)
 * @param frequency (period)
 */
void SegmentDisplay::setDotFreq(unsigned int freq) {
    _dotfreq = freq;
}

/**
 * Toggle display (on/off)
 */
void SegmentDisplay::displayToggle() {
    _power = !_power;
}

/**
 * Turn display on
 */
void SegmentDisplay::displayOn() {
    _power = true;
}

/*
 * Turn display off
 */
void SegmentDisplay::displayOff() {
    _power = false;
}

/**
 * Return true if display is on
 */
bool SegmentDisplay::isDisplayOn() {
    return _power;
}

/**
 * Set display brightness
 * @param intensity brightness
 * @param reduc brightness is reduced or not
 */
void SegmentDisplay::brightness(unsigned int intensity, bool reduc) {
    _brightness = reduc ? round(intensity / 2) : intensity;
}

/**
 * Preparing data for displaying the clock
 */
void SegmentDisplay::_clock(int* segImg) {
    segImg[0] = hour() < 10 ? SYMB_SPACE : hour() / 10; // second() / 10;
    segImg[1] = hour() % 10; // second() % 10;
    segImg[2] = minute() / 10; // millis() % 1000 / 100;
    segImg[3] = minute() % 10; // millis() % 100 / 10;
    segImg[4] = second() / 10;
    segImg[5] = second() % 10;
}

/**
 * Preparing data for displaying the date
 */
void SegmentDisplay::_date(int* segImg) {
    segImg[0] = day() / 10;
    segImg[1] = day() % 10;
    segImg[2] = month() / 10;
    segImg[3] = month() % 10;
    segImg[4] = floor(year() % 100 / 10);
    segImg[5] = year() % 10;
}

/**
 * Preparing data for displaying the temperature
 * @param t temperature
 */
void SegmentDisplay::_temp(float t, uint8_t digs, int* segImg) {
    int tmp = round(t);
    if(validate.temp(t)) { // if temperature is in normal range
        unsigned int th = floor(abs(tmp) / 10);
        unsigned int tl = abs(tmp) % 10;
        if(th == 0) th = SYMB_SPACE;
        segImg[0] = digs == DISP4 ? (tmp < 0 ? SYMB_MINUS : tmp > 9 ? th : SYMB_SPACE) : SYMB_SPACE;
        segImg[1] = digs == DISP4 ? (tmp < 0 ? tmp < -9 ? th : tl : tl) : (tmp < 0 ? SYMB_MINUS : tmp > 9 ? th : SYMB_SPACE);
        segImg[2] = digs == DISP4 ? (tmp < 0 ? tmp < -9 ? tl : SYMB_DEGREE : SYMB_DEGREE) : (tmp < 0 ? tmp < -9 ? th : tl : tl);
        segImg[3] = digs == DISP4 ? (tmp < 0 ? tmp < -9 ? SYMB_DEGREE : SYMB_C : SYMB_C) : (tmp < 0 ? tmp < -9 ? tl : SYMB_DEGREE : SYMB_DEGREE);
        segImg[4] = tmp < 0 ? tmp < -9 ? SYMB_DEGREE : SYMB_C : SYMB_C;
        segImg[5] = tmp < 0 ? tmp < -9 ? SYMB_C : SYMB_SPACE : SYMB_SPACE;
    }
    else { // if temperature is incorrect
        segImg[0] = digs == DISP4 ? SYMB_MINUS : SYMB_SPACE;
        segImg[1] = SYMB_MINUS;
        segImg[2] = digs == DISP4 ? SYMB_DEGREE : SYMB_MINUS;
        segImg[3] = digs == DISP4 ? SYMB_C : SYMB_DEGREE;
        segImg[4] = SYMB_C;
        segImg[5] = SYMB_SPACE;
    }
}

/**
 * Preparing data for displaying the humidity
 * @param h humidity
 */
void SegmentDisplay::_hum(float h, uint8_t digs, int* segImg) {
    int hm = round(h);
    if(validate.hum(h)) { // if humidity is in normal range
        unsigned int hh = floor(hm / 10);
        unsigned int hl = hm % 10;
        if(hh == 0) hh = SYMB_SPACE;
        segImg[0] = digs == DISP4 ? (hm > 99 ? 1 : hh) : SYMB_SPACE;
        segImg[1] = digs == DISP4 ? (hm > 99 ? 0 : hl) : (hm > 99 ? 1 : hh);
        segImg[2] = digs == DISP4 ? (hm > 99 ? 0 : SYMB_SPACE) : (hm > 99 ? 0 : hl);
        segImg[3] = digs == DISP4 ? SYMB_H : (hm > 99 ? 0 : SYMB_SPACE);
        segImg[4] = hm > 99 ? SYMB_SPACE : SYMB_H;
        segImg[5] = hm > 99 ? SYMB_H : SYMB_SPACE;        
    }
    else { // if humidity is incorrect
        segImg[0] = digs == DISP4 ? SYMB_MINUS : SYMB_SPACE;
        segImg[1] = SYMB_MINUS;
        segImg[2] = digs == DISP4 ? SYMB_SPACE : SYMB_MINUS;
        segImg[3] = digs == DISP4 ? SYMB_H : SYMB_SPACE;
        segImg[4] = SYMB_H;
        segImg[5] = SYMB_SPACE;
    }
}

/**
 * Preparing data for displaying the pressure
 * @param p pressure
 */
void SegmentDisplay::_pres(float p, uint8_t digs, int* segImg) {
    int prs = round(p * 0.75);
    if(validate.pres(p)) { // if pressure is in normal range
        segImg[0] = digs == DISP4 ? floor(prs / 100) : SYMB_SPACE;
        segImg[1] = digs == DISP4 ? floor(prs % 100 / 10) : floor(prs / 100);
        segImg[2] = digs == DISP4 ? prs % 10 : floor(prs % 100 / 10);
        segImg[3] = digs == DISP4 ? SYMB_P : prs % 10;
        segImg[4] = SYMB_SPACE;
        segImg[5] = SYMB_P;
    }
    else { // if pressure is incorrect
        segImg[0] = digs == DISP4 ? SYMB_MINUS : SYMB_SPACE;
        segImg[1] = SYMB_MINUS;
        segImg[2] = SYMB_MINUS;
        segImg[3] = digs == DISP4 ? SYMB_P : SYMB_SPACE;
        segImg[4] = SYMB_SPACE;
        segImg[5] = SYMB_P;
    }
}

/**
 * Preparing data for displaying the IAQ
 * @param i Index Air Quality
 */
void SegmentDisplay::_iaq(float i, uint8_t digs, int* segImg) {
    int iaq = round(i);
    if(validate.iaq(i)) { // if IAQ is in normal range
        segImg[0] = digs == DISP4 ? SYMB_A : SYMB_SPACE;
        segImg[1] = digs == DISP4 ? (iaq < 100 ? SYMB_SPACE : floor(iaq / 100)) : SYMB_A;
        segImg[2] = digs == DISP4 ? (iaq < 10 ? SYMB_SPACE : floor(iaq % 100 / 10)) : (iaq < 100 ? SYMB_SPACE : floor(iaq / 100));
        segImg[3] = digs == DISP4 ? (iaq % 10) : (iaq < 10 ? SYMB_SPACE : floor(iaq % 100 / 10));
        segImg[4] = (iaq % 10);
        segImg[5] = SYMB_SPACE;
    }
    else { // if IAQ is incorrect
        segImg[0] = digs == DISP4 ? SYMB_A : SYMB_SPACE;
        segImg[1] = digs == DISP4 ? SYMB_MINUS : SYMB_A;
        segImg[2] = SYMB_MINUS;
        segImg[3] = SYMB_MINUS;
        segImg[4] = SYMB_MINUS;
        segImg[5] = SYMB_SPACE;
    }
}

/**
 * Preparing data for displaying CO2
 * @param c CO2 level
 */
void SegmentDisplay::_co2(float c, uint8_t digs, int* segImg) {
    int co2 = round(c);
    if(validate.co2(c)) { // if CO2 is in normal range
        segImg[0] = co2 < 1000 ? SYMB_SPACE : floor(co2 / 1000);
        segImg[1] = co2 < 100 ? SYMB_SPACE : floor(co2 % 1000 / 100);
        segImg[2] = co2 < 10 ? SYMB_SPACE : floor(co2 % 100 / 10);
        segImg[3] = co2 % 10;
        segImg[4] = SYMB_C;
        segImg[5] = 0;
    }
    else { // if CO2 is incorrect
        segImg[0] = SYMB_MINUS;
        segImg[1] = SYMB_MINUS;
        segImg[2] = SYMB_MINUS;
        segImg[3] = SYMB_MINUS;
        segImg[4] = SYMB_C;
        segImg[5] = 0;
    }
}

void SegmentDisplay::_slotSwitch() {
    unsigned int period = config.display_timeSlot_period(_slot, _dispNum);
    if((millis() - _prevSlotMillis) > (period * 1000) or period == 0) {
        _prevSlot = _slot;
        _slot++;
        _animSlot = 0;
        _animMillis = millis();
        for(uint8_t i=_slot; i<8; i++) {
            if(config.display_timeSlot_period(_slot, _dispNum) == 0) {
                _slot++;
                _animSlot = 0;
                _animMillis = millis();
            }
            else break;
        }
        if(_slot > 7) {
            _slot = 0;
            _animSlot = 0;
            _animMillis = millis();
        }
        _prevSlotMillis = millis();
    }
    _dots = millis() % (_dotfreq * 2) > _dotfreq;
}

void SegmentDisplay::_segAnimations() {
    int segImg[6] = {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};
    int segImgPrev[6] = {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE}; 

    _segGetData(segImg, _slot, true);
    _segGetData(segImgPrev, _prevSlot, false);
    String color = config.display_timeSlot_color(_slot, _dispNum);
    String prevColor = config.display_timeSlot_color(_prevSlot, _dispNum);

    _animIsRunnung = true;
    unsigned int type = config.display_animation_type(_dispNum);

    for(uint8_t i=0; i<4; i++) {
        uint8_t shf = abs(_SHIFTS[type][_animSlot][i]) - 1;
        if(_SHIFTS[type][_animSlot][i] == 0) _dispImg[i] = SYMB_SPACE;
        else {
            if(_SHIFTS[type][_animSlot][i] < 0) _dispImg[i] = segImgPrev[shf]; 
            else _dispImg[i] = segImg[shf];
        }
        
        if(_SHIFTS[type][_animSlot][i] < 0) prevColor.toCharArray(_dispColors[i], 8);
        else color.toCharArray(_dispColors[i], 8);
    }

    if(millis() - _animMillis > 1000 / config.display_animation_speed(_dispNum)) {
        _animMillis = millis();
        if(_animSlot < _FRAMES[type] - 1) _animSlot++;
    }

    if(_animSlot >= _FRAMES[type] - 1) _animIsRunnung = false;

    switch(config.display_animation_points(_dispNum)) {
        case 0: _dot1 = _dot2 = _clockdots and !_animIsRunnung and _dots; break;
        case 1: {
            if(_clockdots and !_animIsRunnung) {
                _dot1 = _dots;
                _dot2 = !_dots;
            }
            else _dot1 = _dot2 = false;
        }; break;
        case 2: _dot1 = _dot2 = _clockdots and !_animIsRunnung; break;
        default: _dot1 = _dot2 = false; break;
    }
}

void SegmentDisplay::_segGetData(int* segImg, uint8_t slot, bool dots) {
    if(config.display_timeSlot_period(slot, _dispNum) > 0) {
        uint8_t digs = config.display_model(_dispNum) < 3 ? DISP4 : DISP6;
        uint8_t dType = 0;
        bool dummy = false;
        float data = agregateSegmentData.slotData(
            config.display_timeSlot_sensor(slot, _dispNum),
            config.display_timeSlot_data(slot, _dispNum),
            slot, _dispNum, &dType, dots ? &_clockdots : &dummy
        );

        switch(dType) {
            case CLOCK: _clock(segImg); break;
            case DATE: _date(segImg); break;
            case TEMP: _temp(data, digs, segImg); break;
            case HUM: _hum(data, digs, segImg); break;
            case PRES: _pres(data, digs, segImg); break;
            case CO2: _co2(data, digs, segImg); break;
            case IAQ: _iaq(data, digs, segImg); break;
            default: ; break;
        }
    }
}