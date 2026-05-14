#include "./ili9341.hpp"

#include "../globals.hpp"
#include "../state.hpp"
#include "../config.hpp"
#include "../weather.hpp"

void ILI9341::getTouch() {
    if(state.touch_calibrate) {
        _touch_calibrate();
        state.touch_calibrate = false;
    }

    bool pressed = false;
    #if defined(BIM32_CYD)
        pressed = touchscreen.tirqTouched() && touchscreen.touched();
    #else
        pressed = tft.getTouch(&_touchX, &_touchY);
    #endif
    if(pressed) {
        #if defined(BIM32_CYD)
            TS_Point p = touchscreen.getPoint();
            _touchX = map(p.x, _calData[0], _calData[2], 0, 319);
            _touchY = map(p.y, _calData[1], _calData[3], 0, 239);
        #endif
        if(state.debugTouch) {
            tft.drawPixel(_touchX, _touchY, 0xFFFF);
            #if defined(BIM32_CYD)
                Serial.printf("raw X: %d, raw Y: %d\r\n", p.x, p.y);
            #endif
            Serial.printf("X: %d, Y: %d\r\n", _touchX, _touchY);
        }

        if(digitalRead(SETTINGS_BUTTON_PIN) == 0) _touch_calibrate();
        else {
            uint8_t page = 100;

            if(_touchX > 284 && _touchY < 30) {
                // Switch to Network page
                if(_page == PAGE_MAIN) {
                    if(millis() - _pageSwitchedTime > 1000) {
                        page = PAGE_NETWORK;
                        _networkSkeleton = false;
                    }
                }
                // Switch to Main page
                else if(millis() - _pageSwitchedTime > 1000) {
                    page = PAGE_MAIN;
                    _calendarShiftSeconds = 0;
                }
            }

            // On the Main page
            if(_page == PAGE_MAIN) {
                // Switch to Big clock page
                if(_touchX < 140 && _touchY < 80) {
                    page = PAGE_BIG_CLOCK;
                    _bigClockSkeleton = false;
                }

                // Switch to Calendar page
                if(_touchX > 145 && _touchX < 180 && _touchY < 33) {
                    page = PAGE_CALENDAR;
                    _calendarSkeleton = false;
                }

                // Switch to History inside page
                if(_touchX > 145 && _touchY > 33 && _touchY < 80) {
                    page = PAGE_HISTORY_IN;
                    _historyInSkeleton = false;
                    _historyInShift = 16;
                }

                // Switch to History outside page
                if(_touchX < 284 && _touchY > 81 && _touchY < 160) {
                    page = PAGE_HISTORY_OUT;
                    _historyOutSkeleton = false;
                    _historyOutShift = 16;
                }

                // Switch to alarm page
                if(_touchX > 284 && _touchY > 130 && _touchY < 162) {
                    page = PAGE_ALARM;
                    _alarmSkeleton = false;
                }
                
                // Switch to Hourly weather forecast page
                if(_touchY > 162) {
                    uint8_t dayLinks[6];
                    uint8_t dayLink = 0;
                    for(uint8_t i=0; i<40; i++) {
                        if(hour(weather.get_hourlyDate(i)) == 0 && i != 0) dayLinks[dayLink++] = i;
                        if(dayLink > 1) break;
                    }
                    if(_touchX < 106) _hourlyShift = 0;
                    if(_touchX > 106 && _touchX < 208) _hourlyShift = dayLinks[0];
                    if(_touchX > 208) _hourlyShift = dayLinks[1];

                    page = PAGE_HOURLY;
                    _hourlySkeleton = false;
                }
            }

            // On the Clock page
            if(_page == PAGE_BIG_CLOCK || _page == PAGE_SMALL_CLOCK) {
                // Big/Small clock switch 
                if(_touchY > 55 && _touchY < 185) {
                    if(millis() - _pageSwitchedTime > 1000) {
                        page = _page == PAGE_BIG_CLOCK ? PAGE_SMALL_CLOCK : PAGE_BIG_CLOCK;
                        _bigClockSkeleton = false;
                        _smallClockSkeleton = false;
                    }
                }
                // Switch to calendar page
                if((_touchX > 40 && _touchX < 250 && _touchY < 36) || _touchY > 188) {
                    page = PAGE_CALENDAR;
                    _calendarSkeleton = false;
                }
            }

            // On the Alarm page
            if(_page == PAGE_ALARM) {
                uint8_t alarmNr = 0;
                for(uint8_t v=0; v<3; v++) {
                    for(uint8_t h=0; h<4; h++) {
                        uint16_t x = h * 72 + 48, y = v * 79 + 25;
                        if(_touchX >= x && _touchX <= x + 18 && _touchY >= y && _touchY <= y + 18) {
                            if(millis() - _pageSwitchedTime > 1000) {
                                config.alarm.setState(alarmNr, config.alarm.state(alarmNr) ? 0 : 1);
                                _forced = true;
                                config.saveAlarmFile();
                            }
                        }
                        alarmNr++;
                    }
                }
            }

            // Left button
            if(_touchX < 32 && _touchY > 100 && _touchY < 136) {
                // Calendar page
                if(_page == PAGE_CALENDAR) _calendarShiftDirection = -1;
                // Hourly weather forecast page
                if(_page == PAGE_HOURLY) {
                    if(_hourlyShift >= 4) _hourlyShift -= 4; 
                    else _hourlyShift = 0;
                }
                /* History outside page */
                if(_page == PAGE_HISTORY_OUT) {
                    if(_historyOutShift >= 4) _historyOutShift -= 4; 
                    else _historyOutShift = 0;
                }
                /* History inside page */
                if(_page == PAGE_HISTORY_IN) {
                    if(_historyInShift >= 4) _historyInShift -= 4; 
                    else _historyInShift = 0;
                }
            }
            /* Right button */
            if(_touchX > 286 && _touchY > 100 && _touchY < 136) {
                // Calendar page
                if(_page == PAGE_CALENDAR) _calendarShiftDirection = 1;
                /* Hourly weather forecast page */
                if(_page == PAGE_HOURLY) {
                    if(_hourlyShift <= 28) _hourlyShift += 4;
                    else _hourlyShift = 32;
                }
                /* History outside page */
                if(_page == PAGE_HISTORY_OUT) {
                    if(_historyOutShift <= 12) _historyOutShift += 4;
                    else _historyOutShift = 16;
                }
                /* History inside page */
                if(_page == PAGE_HISTORY_IN) {
                    if(_historyInShift <= 12) _historyInShift += 4;
                    else _historyInShift = 16;
                }
            }

            if(_page != page) {
                _forced = true;
                if(page != 100) {
                    _page = page;
                    _pageSwitchedTime = millis();
                }
                if(page == PAGE_MAIN) showHomeScreen();
            }
        }
    }
}