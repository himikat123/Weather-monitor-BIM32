#include <Arduino.h>
#include "./taskSensors.hpp"
#include "../state/state.hpp"

void IRAM_ATTR TaskSensors::isr_display1(void* arg) {
    static_cast<TaskSensors*>(arg)->display1_toggle();
}
void IRAM_ATTR TaskSensors::isr_display2(void* arg) {
    static_cast<TaskSensors*>(arg)->display2_toggle();
}
void IRAM_ATTR TaskSensors::isr_alarm(void* arg) {
    static_cast<TaskSensors*>(arg)->alarm_button();
}
void IRAM_ATTR TaskSensors::isr_mp3(void* arg) {
    static_cast<TaskSensors*>(arg)->mp3_busy();
}

/**
 * Interrupt from display 1 button
 */
void IRAM_ATTR TaskSensors::display1_toggle() {
    unsigned int now = (unsigned int)(esp_timer_get_time() / 1000); 
    if(now - state.btnMillis[0] > 500) {
        state.btnMillis[0] = now;
        state.display_btn_pressed[0] = true;
    }
}

/**
 * Interrupt from display 2 button
 */
void IRAM_ATTR TaskSensors::display2_toggle() {
    unsigned int now = (unsigned int)(esp_timer_get_time() / 1000);
    if(now - state.btnMillis[1] > 500) {
        state.btnMillis[1] = now;
        state.display_btn_pressed[1] = true;
    }
}

/**
 * Interrupt from alarm button
 */
void IRAM_ATTR TaskSensors::alarm_button() {
    state.alarm_but_pressed = true;
}

/**
 * Interrupt from mp3 player busy pin
 */
void IRAM_ATTR TaskSensors::mp3_busy() {
    state.mp3_busy = false;
}