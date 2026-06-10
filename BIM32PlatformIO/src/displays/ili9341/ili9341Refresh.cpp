#include "./ili9341.hpp"
#include "../../state/state.hpp"
#include "../../globals.hpp"

/**
 * Display refresh
 */
void ILI9341::refresh() {
    if(state.disp_on_off[DISPLAY_1]) {
        _getData();

        if(_page == PAGE_MAIN) {
            _sequenceSlotSkip();
            _showTemperatureInside();
            _showTemperatureOutside();
            _showHumidityInside();
            _showHumidityOutside();
            _showPressure();
            _showWeekday();
            _showForecastTemps();
            _showDescription();
            _showWeekdays();
            _showForecastWinds();
            _showVoltageOrPercentage();
            _showComfort();
            _showWindSpeed();
            _showUpdTime();
            _showTime();
            _clockPoints();
            _showBatteryLevel();
            _showAntenna();
            _showWeatherIcon();
            _showWindDirection();
            _showAlarmIcon();
            _showForecastIcons();
            _sequenceSlotNext();
        }
        if(_page == PAGE_NETWORK) _networkPage();
        if(_page == PAGE_BIG_CLOCK) _bigClockPage();
        if(_page == PAGE_SMALL_CLOCK) _smallClockPage();
        if(_page == PAGE_CALENDAR) _calendarPage();
        if(_page == PAGE_HOURLY) _hourlyPage();
        if(_page == PAGE_HISTORY_IN) _historyInPage();
        if(_page == PAGE_HISTORY_OUT) _historyOutPage();
        if(_page == PAGE_ALARM) _alarmPage();
    }
}