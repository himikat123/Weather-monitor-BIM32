#include <Arduino.h>
#include "./thingspeak.hpp"

#include "../../config.hpp"
#include "../../state.hpp"

/**
 * check if data is not expired
 */
bool Thingspeak::dataRelevance() {
    return (now() - state.thing.time) < (config.cloud.thingspeakReceive.expire() * 60);
}

/**
 * Get data from a field
 * @param field number
 * @return field data or obviously erroneous value
 */
float Thingspeak::get_field(unsigned int num) {
    if(num > 7) return -40400.0;
    return state.thing.data[num];
}

/**
 * Get timestamp of last update
 * @return timestamp
 */
time_t Thingspeak::get_updated() {
    return state.thing.time;
}

/**
 * Get data from a history repository field
 * @param sensor number
 * @param timeslot number
 * @return field data or obviously erroneous value
 */
float Thingspeak::get_historyField(unsigned int sensor, unsigned int slot) {
    if(sensor >= 7) return -99.0;
    if(slot >= 24) return -99.0;
    return state.thing.historyData[sensor][slot];
}

/**
 * Get timestamp of history repository timeslot update
 * @return timestamp
 */
time_t Thingspeak::get_historyUpdated(unsigned int slot) {
    if(slot >= 24) return 0;
    return state.thing.historyTime[slot];
}