#define DISP4 0
#define DISP6 1

class SegmentDisplay {
    public:
        void setDotFreq(unsigned int freq);
        void displayToggle();
        void displayOn();
        void displayOff();
        bool isDisplayOn();
        void brightness(uint8_t intensity, bool reduc);

    protected:
        uint8_t _slot = 0;
        bool _dots = false;                                     // time points
        bool _clockdots = false;                                // need for time points
        bool _dummy = false;
        bool _dot1 = false;
        bool _dot2 = false;
        bool _dot3 = false;
        bool _dot4 = false;
        unsigned int _dotfreq = 500;                            // time points period
        bool _power = true;                                     // display on/off flag
        uint8_t _brightness = 40;                               // display brightness
        int _dispImg[6] = {                                     // display digits
            SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
        };
        char _dispColors[6][8] = {"", "", "", "", "", ""};
        uint8_t _dispNum = 0;
        bool _animIsRunnung = false;
        uint8_t _model = DISP4;

        void _slotSwitch();
        void _segAnimations();
        void _setModel(uint8_t model);

    private:
        uint8_t _prevSlot = 0;
        uint8_t _animSlot = 0;
        uint32_t _animMillis = 0;
        uint32_t _prevSlotMillis = 0;
        uint16_t _millisShift = 0;
        uint8_t _prevSecond = 60;

        void _segGetData(int* segImg, uint8_t slot, bool dots);
        void _clock(int* segImg);
        void _date(int* segImg);
        void _temp(float t, int* segImg);
        void _hum(float h, int* segImg);
        void _pres(float p, int* segImg);
        void _iaq(float i, int* segImg);
        void _co2(float c, int* segImg);
        void _pointsTogether(bool dots);
        void _pointsInTurn();
};

/**
 * Set display model
 */
void SegmentDisplay::_setModel(uint8_t model) {
    _model = model < 3 ? DISP4 : DISP6;
}

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
void SegmentDisplay::brightness(uint8_t intensity, bool reduc) {
    _brightness = reduc ? round(intensity / 2) : intensity;
}

/**
 * Preparing data for displaying the clock
 */
void SegmentDisplay::_clock(int* segImg) {
    uint8_t hrH = hour() < 10 ? SYMB_SPACE : floor(hour() / 10);
    uint8_t hrL = hour() % 10;
    uint8_t mnH = floor(minute() / 10);
    uint8_t mnL = minute() % 10;
    uint8_t scH = floor(second() / 10);
    uint8_t scL = second() % 10;
    uint8_t sens = config.display_timeSlot_data(_slot, _dispNum);

    segImg[0] = _model == DISP4 ? hrH : sens == 0 ? SYMB_SPACE : hrH;
    segImg[1] = _model == DISP4 ? hrL : sens == 0 ? SYMB_SPACE : hrL;
    segImg[2] = _model == DISP4 ? mnH : sens == 0 ? hrH : mnH;
    segImg[3] = _model == DISP4 ? mnL : sens == 0 ? hrL : mnL;
    segImg[4] = _model == DISP4 ? scH : sens == 0 ? mnH : scH;
    segImg[5] = _model == DISP4 ? scL : sens == 0 ? mnL : scL;
}

/**
 * Preparing data for displaying the date
 */
void SegmentDisplay::_date(int* segImg) {
    uint8_t dtH = floor(day() / 10);
    uint8_t dtL = day() % 10;
    uint8_t mtH = floor(month() / 10);
    uint8_t mtL = month() % 10;
    uint8_t yrH = floor(year() % 100 / 10);
    uint8_t yrL = year() % 10;
    uint8_t sens = config.display_timeSlot_data(_slot, _dispNum);

    segImg[0] = _model == DISP4 ? dtH : sens == 0 ? SYMB_SPACE : dtH;
    segImg[1] = _model == DISP4 ? dtL : sens == 0 ? SYMB_SPACE : dtL;
    segImg[2] = _model == DISP4 ? mtH : sens == 0 ? dtH : mtH;
    segImg[3] = _model == DISP4 ? mtL : sens == 0 ? dtL : mtL;
    segImg[4] = _model == DISP4 ? SYMB_SPACE : sens == 0 ? mtH : yrH;
    segImg[5] = _model == DISP4 ? SYMB_SPACE : sens == 0 ? mtL : yrL;
}

/**
 * Preparing data for displaying the temperature
 * @param t temperature
 */
void SegmentDisplay::_temp(float t, int* segImg) {
    bool valid = validate.temp(t);
    int tmp = round(t);
    unsigned int th = floor(abs(tmp) / 10);
    unsigned int tl = abs(tmp) % 10;
    if(th == 0) th = SYMB_SPACE;

    segImg[0] = _model == DISP4 ? valid ? (tmp < 0 ? SYMB_MINUS : tmp > 9 ? th : SYMB_SPACE) : SYMB_MINUS : SYMB_SPACE;
    segImg[1] = _model == DISP4 ? valid ? tmp < -9 ? th : tl : SYMB_MINUS : valid ? tmp < -9 ? SYMB_MINUS : SYMB_SPACE : SYMB_SPACE;
    segImg[2] = _model == DISP4 ? valid ? tmp < -9 ? tl : SYMB_DEGREE : SYMB_DEGREE : valid ? th : SYMB_MINUS;
    segImg[3] = _model == DISP4 ? valid ? tmp < -9 ? SYMB_DEGREE : SYMB_C : SYMB_C : valid ? tl : SYMB_MINUS;
    segImg[4] = SYMB_DEGREE;
    segImg[5] = SYMB_C;
}

/**
 * Preparing data for displaying the humidity
 * @param h humidity
 */
void SegmentDisplay::_hum(float h, int* segImg) {
    bool valid = validate.hum(h);
    int hm = round(h);
    unsigned int hh = floor(hm / 10);
    unsigned int hl = hm % 10;
    if(hh == 0) hh = SYMB_SPACE;

    segImg[0] = _model == DISP4 ? valid ? (hm > 99 ? 1 : hh) : SYMB_MINUS : SYMB_SPACE;
    segImg[1] = _model == DISP4 ? valid ? (hm > 99 ? 0 : hl) : SYMB_MINUS : (valid && hm > 99) ? 1 : SYMB_SPACE;
    segImg[2] = _model == DISP4 ? valid ? (hm > 99 ? 0 : SYMB_SPACE) : SYMB_SPACE : valid ? (hm > 99 ? 0 : hh) : SYMB_MINUS;
    segImg[3] = _model == DISP4 ? SYMB_H : valid ? (hm > 99 ? 0 : hl) : SYMB_MINUS;
    segImg[4] = SYMB_SPACE;
    segImg[5] = SYMB_H;
}

/**
 * Preparing data for displaying the pressure
 * @param p pressure
 */
void SegmentDisplay::_pres(float p, int* segImg) {
    bool valid = validate.pres(p);
    int prs = round(p * 0.75);
    unsigned int p100 = floor(prs / 100);
    unsigned int p10 = floor(prs % 100 / 10);
    unsigned int p1 = prs % 10;

    segImg[0] = _model == DISP4 ? valid ? p100 : SYMB_MINUS : SYMB_SPACE;
    segImg[1] = _model == DISP4 ? valid ? p10 : SYMB_MINUS : valid ? p100 : SYMB_MINUS;
    segImg[2] = _model == DISP4 ? valid ? p1 : SYMB_MINUS : valid ? p10 : SYMB_MINUS;
    segImg[3] = _model == DISP4 ? SYMB_P : valid ? p1 : SYMB_MINUS;
    segImg[4] = SYMB_SPACE;
    segImg[5] = SYMB_P;
}

/**
 * Preparing data for displaying the IAQ
 * @param i Index Air Quality
 */
void SegmentDisplay::_iaq(float i, int* segImg) {
    bool valid = validate.iaq(i);
    int iaq = round(i);
    unsigned int i100 = iaq < 100 ? SYMB_SPACE : floor(iaq / 100);
    unsigned int i10 = iaq < 10 ? SYMB_SPACE : floor(iaq % 100 / 10);
    unsigned int i1 = iaq % 10;

    segImg[0] = _model == DISP4 ? SYMB_A : SYMB_SPACE;
    segImg[1] = _model == DISP4 ? valid ? i100 : SYMB_MINUS : (valid && iaq < 100) ? SYMB_SPACE : SYMB_A;
    segImg[2] = _model == DISP4 ? valid ? i10 : SYMB_MINUS : (valid && iaq < 100) ? SYMB_A : SYMB_SPACE;
    segImg[3] = _model == DISP4 ? valid ? i1 : SYMB_MINUS : valid ? i100 : SYMB_MINUS;
    segImg[4] = valid ? i10 : SYMB_MINUS;
    segImg[5] = valid ? i1 : SYMB_MINUS;
}

/**
 * Preparing data for displaying CO2
 * @param c CO2 level
 */
void SegmentDisplay::_co2(float c, int* segImg) {
    bool valid = validate.co2(c);
    int co2 = round(c);
    unsigned int c1000 = valid ? (co2 < 1000 ? SYMB_SPACE : floor(co2 / 1000)) : SYMB_MINUS;
    unsigned int c100 = valid ? (co2 < 100 ? SYMB_SPACE : floor(co2 % 1000 / 100)) : SYMB_MINUS;
    unsigned int c10 = valid ? (co2 < 10 ? SYMB_SPACE : floor(co2 % 100 / 10)) : SYMB_MINUS;
    unsigned int c1 = valid ? co2 % 10 : SYMB_MINUS;

    segImg[0] = _model == DISP4 ? c1000 : SYMB_C;
    segImg[1] = _model == DISP4 ? c100 : SYMB_o;
    segImg[2] = _model == DISP4 ? c10 : c1000;
    segImg[3] = _model == DISP4 ? c1 : c100;
    segImg[4] = c10;
    segImg[5] = c1;
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
    if(_prevSecond != second()) {
        _millisShift = millis() % 1000;
        _prevSecond = second();
    }
    _dots = !((millis() - _millisShift) % (_dotfreq * 2) > _dotfreq);
}

void SegmentDisplay::_segAnimations() {
    int segImg[6] = {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};
    int segImgPrev[6] = {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE}; 

    _segGetData(segImgPrev, _prevSlot, false);
    _segGetData(segImg, _slot, true);
    String color = config.display_timeSlot_color(_slot, _dispNum);
    String prevColor = config.display_timeSlot_color(_prevSlot, _dispNum);

    _animIsRunnung = true;
    unsigned int type = config.display_animation_type(_dispNum);

    for(uint8_t i=0; i<(_model == DISP4 ? 4 : 6); i++) {
        uint8_t shf = abs(SHIFTS[_model][type][_animSlot][i]) - 1;
        if(SHIFTS[_model][type][_animSlot][i] == 0) _dispImg[i] = SYMB_SPACE;
        else {
            if(SHIFTS[_model][type][_animSlot][i] < 0) _dispImg[i] = segImgPrev[shf]; 
            else _dispImg[i] = segImg[shf];
        }
        
        if(SHIFTS[_model][type][_animSlot][i] < 0) prevColor.toCharArray(_dispColors[i], 8);
        else color.toCharArray(_dispColors[i], 8);
    }

    if(millis() - _animMillis > 1000 / config.display_animation_speed(_dispNum)) {
        _animMillis = millis();
        if(_animSlot < FRAMES[_model][type] - 1) _animSlot++;
    }

    if(_animSlot >= FRAMES[_model][type] - 1) _animIsRunnung = false;

    /* clock points */
    switch(config.display_animation_points(_dispNum)) {
        case 0: _pointsTogether(_dots); break;
        case 1: _pointsInTurn(); break;
        case 2: _pointsTogether(true); break;
        default: _pointsTogether(false); break;
    }

    /* power on bottom points for date */
    if(config.display_timeSlot_sensor(_slot, _dispNum) == 1) {
        _dot1 = false;
        _dot2 = config.display_model(_dispNum) < 3 
            ? true 
            : config.display_timeSlot_data(_slot, _dispNum) > 0 
                ? true 
                : false;
        _dot3 = false;
        _dot4 = true;
    }
}

void SegmentDisplay::_segGetData(int* segImg, uint8_t slot, bool dots) {
    if(config.display_timeSlot_period(slot, _dispNum) > 0) {
        uint8_t dType = 0;
        float data = agregateSegmentData.slotData(
            config.display_timeSlot_sensor(slot, _dispNum),
            config.display_timeSlot_data(slot, _dispNum),
            slot, _dispNum, &dType, dots ? &_clockdots : &_dummy
        );

        switch(dType) {
            case CLOCK: _clock(segImg); break;
            case DATE: _date(segImg); break;
            case TEMP: _temp(data, segImg); break;
            case HUM: _hum(data, segImg); break;
            case PRES: _pres(data, segImg); break;
            case CO2: _co2(data, segImg); break;
            case IAQ: _iaq(data, segImg); break;
            default: ; break;
        }
    }
}

void SegmentDisplay::_pointsTogether(bool dots) {
    _dot3 = _dot4 = _clockdots and !_animIsRunnung and dots;
    if(_model == DISP4) _dot1 = _dot2 = _dot3;
    else {
        if(config.display_timeSlot_data(_slot, _dispNum) > 0) _dot1 = _dot2 = _dot3;
        else _dot1 = _dot2 = false;
    }
}

void SegmentDisplay::_pointsInTurn() {
    if(_clockdots and !_animIsRunnung) {
        _dot3 = _dots;
        _dot4 = !_dots;
        if(config.display_model(_dispNum) < 3) {
            _dot1 = _dot3;
            _dot2 = _dot4;
        }
        else {
            if(config.display_timeSlot_data(_slot, _dispNum) > 0) {
                _dot1 = _dot3;
                _dot2 = _dot4;
            }
            else _dot1 = _dot2 = false;
        }
    }
    else _dot1 = _dot2 = _dot3 = _dot4 = false;
}