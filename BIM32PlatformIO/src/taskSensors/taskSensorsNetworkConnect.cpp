#include <WiFi.h>

#include "./taskSensors.hpp"
#include "../state/state.hpp"
#include "../network/network.hpp"

/**
 * Network connection if not connected and if not Access point mode
 */
void TaskSensors::_networkConnect() {
    if(WiFi.localIP().toString() == "0.0.0.0" or !network.isConnected()) {
        state.net_connected = false;
        network.connect();
        vTaskDelay(1000);
    }
    else state.net_connected = true;
}