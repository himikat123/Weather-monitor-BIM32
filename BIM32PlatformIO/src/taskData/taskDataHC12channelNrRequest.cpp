#include <Arduino.h>

#include "./taskData.hpp"
#include "../pinout.hpp"

/*
 * HC12 wireless module channel number request
 */
void TaskData::_HC12channelNrRequest() {
    digitalWrite(HC12_SET_PIN, LOW);
    vTaskDelay(50);
    Serial2.println("AT+RC");
    Serial2.flush();
    vTaskDelay(100);
    digitalWrite(HC12_SET_PIN, HIGH);
}