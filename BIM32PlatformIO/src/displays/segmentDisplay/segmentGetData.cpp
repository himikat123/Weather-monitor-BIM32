#include <TimeLib.h>

#include "./segmentDisplay.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"
#include "../../agregateData/segmentData/agregateSegmentData.hpp"

void SegmentDisplay::_segGetData(int* segImg, uint8_t slot, bool dots) {
    if(state.apMode) {
        _apMode(segImg);
    }
    else if(config.display.timeSlot.period(slot, _dispNum) > 0) {
        uint8_t dType = 0;
        float data = agregateSegmentData.slotData(
            config.display.timeSlot.sensor(slot, _dispNum),
            config.display.timeSlot.data(slot, _dispNum),
            slot, _dispNum, &dType
        );

        switch(dType) {
            case agregateSegmentData.CLOCK_TYPE: _clock(segImg, slot); break;
            case agregateSegmentData.DATE_TYPE: _date(segImg, slot); break;
            case agregateSegmentData.TEMP_TYPE: _temp(data, segImg); break;
            case agregateSegmentData.HUM_TYPE: _hum(data, segImg); break;
            case agregateSegmentData.PRES_TYPE: _pres(data, segImg); break;
            case agregateSegmentData.CO2_TYPE: _co2(data, segImg); break;
            case agregateSegmentData.IAQ_TYPE: _iaq(data, segImg); break;
            default: ; break;
        }
    }
}