#include <Arduino.h>
#include <TimeLib.h>

#include "../taskServer.hpp"
#include "../../displays/nextion/nextion.hpp"
#include "../../sensors/sensors.hpp"
#include "../../state/state.hpp"

/**
 * Initialize clock syncronization with NTP server
 */
void TaskServer::_webSyncClock() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("ntp")) {
            state.clockSynchronized = false;
            _server.send(200, "text/plain", "OK");
        }
        else {
            setTime(
                (_server.arg("h")).toInt(), 
                (_server.arg("i")).toInt(),
                (_server.arg("s")).toInt(),
                (_server.arg("d")).toInt(),
                (_server.arg("m")).toInt(),
                (_server.arg("y")).toInt()
            );
            nextion.setDisplayRTC();
            sensors.set_ds3231_timeDate();
            _server.send(200, "text/plain", _webTimeString(now()));
        }
    }
}

/**
 * Send via AJAX the information about the clock synchronization process
 */
void TaskServer::_webSyncdialog() {
    if(_webIsLogged(true)) {
        if(!state.clockSynchronized) _server.send(200, "text/plain", ".");
        else _server.send(200, "text/plain", _webTimeString(now()));
    }
}

/**
 * Get current date and time as a string
 */
String TaskServer::_webTimeString(unsigned int tm) {
    char buf[20];
    sprintf(buf, "%02d:%02d:%02d %02d.%02d.%d", hour(tm), minute(tm), second(tm), day(tm), month(tm), year(tm));
    return String(buf);
}