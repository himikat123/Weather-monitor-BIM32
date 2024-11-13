#define DISP4 0
#define DISP6 1
#define DISP8 2
#define DOT 100

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
        bool _points[8] = {false, false, false, false, false, false, false, false};
        bool _prevPoints[8] = {false, false, false, false, false, false, false, false};
        unsigned int _dotfreq = 500;    // time points period
        bool _power = true;             // display on/off flag
        uint8_t _brightness = 40;       // display brightness
        int _dispImg[8] = {             // display digits
            SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
        };
        char _dispColors[8][8] = {"", "", "", "", "", "", "", ""};
        uint8_t _dispNum = 0;
        bool _animIsRunnung = false;

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
        bool _pointsState = false;
        uint8_t _dispLength = DISP4;

        void _segGetData(int* segImg, uint8_t slot, bool dots);
        void _clock(int* segImg, uint8_t slot);
        void _date(int* segImg, uint8_t slot);
        void _temp(float t, int* segImg);
        void _hum(float h, int* segImg);
        void _pres(float p, int* segImg);
        void _iaq(float i, int* segImg);
        void _co2(float c, int* segImg);
        void _apMode(int* segImg);
};

/**
 * Set display model
 */
void SegmentDisplay::_setModel(uint8_t model) {
    _dispLength = config.display_type(_dispNum) == 2
        ? (model < 3 ? DISP4 : DISP6)
        : ((model == 0 || model == 2) ? DISP4 : (model == 1 || model == 3) ? DISP6 : DISP8);
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
void SegmentDisplay::_clock(int* segImg, uint8_t slot) {
    uint8_t hr = config.clock_format() ? hour() : hourFormat12();
    uint8_t hrH = floor(hr < 10 ? SYMB_SPACE : hr / 10);
    uint8_t hrL = hr % 10;
    uint8_t mnH = floor(minute() / 10), mnL = minute() % 10;
    uint8_t scH = floor(second() / 10), scL = second() % 10;
    int32_t ml = millis() - _millisShift;
    uint8_t msH = floor(ml % 1000 / 100), msL = floor(ml % 100 / 10);
    bool point1 = false, point2 = false;

    switch(config.display_animation_points(_dispNum)) {
        case 0: point1 = point2 = _pointsState; break;
        case 1: point1 = _pointsState; point2 = !point1; break;
        case 2: point1 = point2 = true; break;
        case 3: point1 = point2 = false; break;
        default: ; break;
    }

    int disp4Img[8] = {
        config.display_type(_dispNum) == 2 && point1 ? (hrH + DOT) : hrH, 
        point2 ? (hrL + DOT) : hrL, 
        mnH, mnL, 
        SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
    };
    int disp6Img[3][8] = {
        {
            SYMB_SPACE, SYMB_SPACE, config.display_type(_dispNum) == 2 && point1 ? (hrH + DOT) : hrH, 
            point2 ? (hrL + DOT) : hrL, mnH, mnL, SYMB_SPACE, SYMB_SPACE
        },
        {
            config.display_type(_dispNum) == 2 && point1 ? (hrH + DOT) : hrH, point2 ? (hrL + DOT) : hrL, 
            config.display_type(_dispNum) == 2 && point1 ? (mnH + DOT) : mnH, point2 ? (mnL + DOT) : mnL, 
            scH, scL, SYMB_SPACE, SYMB_SPACE
        },
        {SYMB_SPACE, hrH, hrL, point1 ? SYMB_MINUS : SYMB_SPACE, mnH, mnL}
    };
    int disp8Img[5][8] = {
        {SYMB_SPACE, SYMB_SPACE, hrH, point1 ? hrL + DOT : hrL, mnH, mnL, SYMB_SPACE, SYMB_SPACE},
        {SYMB_SPACE, hrH, point1 ? hrL + DOT : hrL, mnH, point1 ? mnL + DOT : mnL, scH, scL, SYMB_SPACE},
        {SYMB_SPACE, SYMB_SPACE, hrH, hrL, point1 ? SYMB_MINUS : SYMB_SPACE, mnH, mnL, SYMB_SPACE},
        {hrH, hrL, point1 ? SYMB_MINUS : SYMB_SPACE, mnH, mnL, point1 ? SYMB_MINUS : SYMB_SPACE, scH, scL},
        {hrH, point1 ? hrL + DOT : hrL, mnH, point1 ? mnL + DOT : mnL, scH, point1 ? scL + DOT : scL, msH, msL}
    };

    uint8_t sens = config.display_timeSlot_data(slot, _dispNum);
    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == DISP4 ? disp4Img[i] : _dispLength == DISP6 ? disp6Img[sens][i] : disp8Img[sens][i];
    }
}

/**
 * Preparing data for displaying the date
 */
void SegmentDisplay::_date(int* segImg, uint8_t slot) {
    uint8_t dtH = floor(day() / 10), dtL = day() % 10;
    uint8_t mtH = floor(month() / 10), mtL = month() % 10;
    uint8_t yr1 = floor(year() / 1000), yr2 = floor(year() % 1000 / 100), yr3 = floor(year() % 100 / 10), yr4 = floor(year() % 10);

    int disp4Img[8] = {dtH, dtL + DOT, mtH, mtL, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};
    int disp6Img[2][8] = {
        {SYMB_SPACE, SYMB_SPACE, dtH, dtL + DOT, mtH, mtL, SYMB_SPACE, SYMB_SPACE},
        {dtH, dtL + DOT, mtH, mtL + DOT, yr3, yr4, SYMB_SPACE, SYMB_SPACE}
    };
    int disp8Img[3][8] = {
        {SYMB_SPACE, SYMB_SPACE, dtH, dtL + DOT, mtH, mtL, SYMB_SPACE, SYMB_SPACE},
        {SYMB_SPACE, dtH, dtL + DOT, mtH, mtL + DOT, yr3, yr4, SYMB_SPACE},
        {dtH, dtL + DOT, mtH, mtL + DOT, yr1, yr2, yr3, yr4}
    };

    uint8_t sens = config.display_timeSlot_data(slot, _dispNum);
    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == DISP4 ? disp4Img[i] : _dispLength == DISP6 ? disp6Img[sens][i] : disp8Img[sens][i];
    }
}

/**
 * Preparing data for displaying the temperature
 * @param t temperature
 */
void SegmentDisplay::_temp(float t, int* segImg) {
    bool valid = validate.temp(t);
    int tmp = round(t);
    uint8_t th = floor(abs(tmp) / 10), tl = abs(tmp) % 10;
    if(th == 0) th = SYMB_SPACE;

    int disp4Img[8] = {
        valid ? (tmp < 0 ? SYMB_MINUS : tmp > 9 ? th : SYMB_SPACE) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? th : tl : tl) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? tl : SYMB_DEGREE : SYMB_DEGREE) : SYMB_DEGREE,
        valid ? (tmp < 0 ? tmp < -9 ? SYMB_DEGREE : SYMB_C : SYMB_C) : SYMB_C,
        SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
    };
    int disp6Img[8] = {
        SYMB_SPACE,
        valid ? (tmp < 0 ? SYMB_MINUS : tmp > 9 ? th : SYMB_SPACE) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? th : tl : tl) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? tl : SYMB_DEGREE : SYMB_DEGREE) : SYMB_DEGREE,
        valid ? (tmp <-9 ? SYMB_DEGREE : SYMB_C) : SYMB_C,
        valid ? (tmp <-9 ? SYMB_C : SYMB_SPACE) : SYMB_SPACE,
        SYMB_SPACE, SYMB_SPACE
    };
    int disp8Img[8] = {
        SYMB_SPACE, SYMB_SPACE,
        valid ? (tmp < 0 ? SYMB_MINUS : tmp > 9 ? th : SYMB_SPACE) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? th : tl : tl) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? tl : SYMB_DEGREE : SYMB_DEGREE) : SYMB_DEGREE,
        valid ? (tmp <-9 ? SYMB_DEGREE : SYMB_C) : SYMB_C,
        valid ? (tmp <-9 ? SYMB_C : SYMB_SPACE) : SYMB_SPACE,
        SYMB_SPACE
    };

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == DISP4 ? disp4Img[i] : _dispLength == DISP6 ? disp6Img[i] : disp8Img[i];
    }
}

/**
 * Preparing data for displaying the humidity
 * @param h humidity
 */
void SegmentDisplay::_hum(float h, int* segImg) {
    bool valid = validate.hum(h);
    int hm = round(h);
    uint8_t hh = floor(hm / 10), hl = hm % 10;
    if(hh == 0) hh = SYMB_SPACE;

    int disp4Img[8] = {
        valid ? (hm > 99 ? 1 : hh) : SYMB_MINUS, valid ? (hm > 99 ? 0 : hl) : SYMB_MINUS,
        valid ? (hm > 99 ? 0 : SYMB_SPACE) : SYMB_SPACE, SYMB_H, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
    };
    int disp6Img[8] = {
        SYMB_SPACE, valid ? (hm > 9 ? hm > 99 ? 1 : hh : SYMB_SPACE) : SYMB_MINUS,
        valid ? (hm > 99 ? 0 : hl) : SYMB_MINUS, valid ? (hm > 99 ? 0 : SYMB_SPACE) : SYMB_SPACE,
        valid ? (hm > 99 ? SYMB_SPACE : SYMB_H) : SYMB_H, valid ? (hm > 99 ? SYMB_H : SYMB_SPACE) : SYMB_SPACE, 
        SYMB_SPACE, SYMB_SPACE
    };
    int disp8Img[8] = {
        SYMB_SPACE, SYMB_SPACE, valid ? (hm > 9 ? hm > 99 ? 1 : hh : SYMB_SPACE) : SYMB_MINUS,
        valid ? (hm > 99 ? 0 : hl) : SYMB_MINUS, valid ? (hm > 99 ? 0 : SYMB_SPACE) : SYMB_SPACE,
        valid ? (hm > 99 ? SYMB_SPACE : SYMB_H) : SYMB_H, valid ? (hm > 99 ? SYMB_H : SYMB_SPACE) : SYMB_SPACE, SYMB_SPACE
    };

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == DISP4 ? disp4Img[i] : _dispLength == DISP6 ? disp6Img[i] : disp8Img[i];
    }
}

/**
 * Preparing data for displaying the pressure
 * @param p pressure
 */
void SegmentDisplay::_pres(float p, int* segImg) {
    bool valid = validate.pres(p);
    int prs = round(p * 0.75);
    uint8_t p100 = valid ? floor(prs / 100) : SYMB_MINUS;
    uint8_t p10 = valid ? floor(prs % 100 / 10) : SYMB_MINUS;
    uint8_t p1 = valid ? prs % 10 : SYMB_MINUS;

    int disp4Img[8] = {p100, p10, p1, SYMB_P, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};
    int disp6Img[8] = {SYMB_SPACE, p100, p10, p1, SYMB_SPACE, SYMB_P, SYMB_SPACE, SYMB_SPACE};
    int disp8Img[8] = {SYMB_SPACE, SYMB_SPACE, p100, p10, p1, SYMB_SPACE, SYMB_P, SYMB_SPACE};

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == DISP4 ? disp4Img[i] : _dispLength == DISP6 ? disp6Img[i] : disp8Img[i];
    }
}

/**
 * Preparing data for displaying the IAQ
 * @param i Index Air Quality
 */
void SegmentDisplay::_iaq(float i, int* segImg) {
    bool valid = validate.iaq(i);
    int iaq = round(i);
    uint8_t i100 = valid ? iaq < 100 ? SYMB_SPACE : floor(iaq / 100) : SYMB_MINUS;
    uint8_t i10 = valid ? iaq < 10 ? SYMB_SPACE : floor(iaq % 100 / 10) : SYMB_MINUS;
    uint8_t i1 = valid ? iaq % 10 : SYMB_MINUS;

    int disp4Img[8] = {
        iaq < 10 ? SYMB_SPACE : SYMB_A, iaq < 100 ? iaq < 10 ? SYMB_A : SYMB_SPACE : i100, 
        iaq < 10 ? SYMB_SPACE : i10, i1, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
    };
    int disp6Img[8] = {
        SYMB_SPACE, iaq < 10 ? SYMB_SPACE : SYMB_A, 
        iaq < 10 ? SYMB_A : SYMB_SPACE, iaq < 100 ? iaq < 10 ? SYMB_SPACE : i10 : i100,
        iaq < 100 ? i1 : i10, iaq < 100 ? SYMB_SPACE : i1, SYMB_SPACE, SYMB_SPACE
    };
    int disp8Img[8] = {
        SYMB_SPACE, SYMB_SPACE, iaq < 10 ? SYMB_SPACE : SYMB_A, 
        iaq < 10 ? SYMB_A : SYMB_SPACE, iaq < 100 ? iaq < 10 ? SYMB_SPACE : i10 : i100, 
        iaq < 100 ? i1 : i10, iaq < 100 ? SYMB_SPACE : i1, SYMB_SPACE
    };

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == DISP4 ? disp4Img[i] : _dispLength == DISP6 ? disp6Img[i] : disp8Img[i];
    }
}

/**
 * Preparing data for displaying CO2
 * @param c CO2 level
 */
void SegmentDisplay::_co2(float c, int* segImg) {
    bool valid = validate.co2(c);
    int co2 = round(c);
    uint8_t c1000 = co2 < 1000 ? SYMB_SPACE : floor(co2 / 1000);
    uint8_t c100 = co2 < 100 ? SYMB_SPACE : floor(co2 % 1000 / 100);
    uint8_t c10 = co2 < 10 ? SYMB_SPACE : floor(co2 % 100 / 10);
    uint8_t c1 = co2 % 10;

    int disp4Img[8] = {
        valid ? co2 < 1000 ? SYMB_C : c1000 : SYMB_C, valid ? co2 < 100 ? 0 : c100 : 0, 
        valid ? co2 < 10 ? SYMB_SPACE : c10 : 2, valid ? c1 : SYMB_MINUS, 
        SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
    };
    int disp6Img[8] = {
        co2 < 10 ? SYMB_SPACE : SYMB_C, co2 < 10 ? SYMB_C : 0, 
        valid ? co2 < 1000 ? co2 < 100 ? co2 < 10 ? 0 : 2 : SYMB_SPACE : c1000 : 2, 
        valid ? co2 < 100 ? co2 < 10 ? 2 : SYMB_SPACE : c100 : SYMB_SPACE, 
        valid ? co2 < 10 ? SYMB_SPACE : c10 : SYMB_MINUS, valid ? c1 : SYMB_MINUS, 
        SYMB_SPACE, SYMB_SPACE
    };
    int disp8Img[8] = {
        valid ? co2 < 1000 ? SYMB_SPACE : SYMB_C : SYMB_C,
        valid ? co2 < 1000 ? co2 < 10 ? SYMB_SPACE : SYMB_C : 0 : 0,
        valid ? co2 < 1000 ? co2 < 10 ? SYMB_C : 0 : 2 : 2,
        valid ? co2 < 1000 ? co2 < 10 ? 0 : 2 : SYMB_SPACE : SYMB_SPACE,
        valid ? co2 < 1000 ? co2 < 10 ? 2 : SYMB_SPACE : c1000 : SYMB_MINUS,
        valid ? co2 < 100 ? co2 < 10 ? SYMB_SPACE : c10 : c100 : SYMB_MINUS,
        valid ? co2 < 100 ? c1 : c10 : SYMB_MINUS,
        valid ? co2 < 100 ? SYMB_SPACE : c1 : SYMB_MINUS
    };

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == DISP4 ? disp4Img[i] : _dispLength == DISP6 ? disp6Img[i] : disp8Img[i];
    }
}

/**
 * Preparing data for displaying AP mode
 */
void SegmentDisplay::_apMode(int* segImg) {
    int disp4Img[8] = {SYMB_SPACE, SYMB_A, SYMB_P, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};
    int disp6Img[8] = {SYMB_SPACE, SYMB_SPACE, SYMB_A, SYMB_P, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};
    int disp8Img[8] = {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_A, SYMB_P, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == DISP4 ? disp4Img[i] : _dispLength == DISP6 ? disp6Img[i] : disp8Img[i];
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
    if(_prevSecond != second()) {
        _millisShift = millis() % 1000;
        _prevSecond = second();
    }
    _pointsState = !((millis() - _millisShift) % (_dotfreq * 2) > _dotfreq);
}

void SegmentDisplay::_segAnimations() {
    int segImg[8] = {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};
    int segImgPrev[8] = {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE}; 

    _segGetData(segImgPrev, _prevSlot, false);
    _segGetData(segImg, _slot, true);
    String color = config.display_timeSlot_color(_slot, _dispNum);
    String prevColor = config.display_timeSlot_color(_prevSlot, _dispNum);
    if(global.apMode) {
        color = "#FFFFFF";
        prevColor = "#FFFFFF";
    }

    _animIsRunnung = true;
    unsigned int type = config.display_animation_type(_dispNum);

    for(uint8_t i=0; i<(_dispLength == DISP4 ? 4 : _dispLength == DISP6 ? 6 : 8); i++) {
        uint8_t shf = abs(SHIFTS[_dispLength][type][_animSlot][i]) - 1;
        if(SHIFTS[_dispLength][type][_animSlot][i] == 0) _dispImg[i] = SYMB_SPACE;
        else {
            if(SHIFTS[_dispLength][type][_animSlot][i] < 0) _dispImg[i] = segImgPrev[shf]; 
            else _dispImg[i] = segImg[shf];
        }
        
        if(SHIFTS[_dispLength][type][_animSlot][i] < 0) prevColor.toCharArray(_dispColors[i], 8);
        else color.toCharArray(_dispColors[i], 8);
    }

    if(millis() - _animMillis > 1000 / config.display_animation_speed(_dispNum)) {
        _animMillis = millis();
        if(_animSlot < FRAMES[_dispLength][type] - 1) _animSlot++;
    }

    if(_animSlot >= FRAMES[_dispLength][type] - 1) _animIsRunnung = false;
}

void SegmentDisplay::_segGetData(int* segImg, uint8_t slot, bool dots) {
    if(global.apMode) {
        _apMode(segImg);
    }
    else if(config.display_timeSlot_period(slot, _dispNum) > 0) {
        uint8_t dType = 0;
        float data = agregateSegmentData.slotData(
            config.display_timeSlot_sensor(slot, _dispNum),
            config.display_timeSlot_data(slot, _dispNum),
            slot, _dispNum, &dType
        );

        switch(dType) {
            case CLOCK: _clock(segImg, slot); break;
            case DATE: _date(segImg, slot); break;
            case TEMP: _temp(data, segImg); break;
            case HUM: _hum(data, segImg); break;
            case PRES: _pres(data, segImg); break;
            case CO2: _co2(data, segImg); break;
            case IAQ: _iaq(data, segImg); break;
            default: ; break;
        }
    }
}