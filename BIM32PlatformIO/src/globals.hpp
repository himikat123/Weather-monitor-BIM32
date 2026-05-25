#pragma once

#include <Arduino.h> 

#define FW "v6.0a"                    // Firmware version
#define REMOTE_HOST "www.google.com" // Remote host to ping

#define ALARMS              12       // Number of alarms

#define SEPARATOR "**********************************************************************"
#define UNDEFINED_FLOAT  -40400.0

#define UNDEFINED            0
#define COMFORTABLE          1
#define HOT                  2
#define COLD                 3
#define HUMID                4
#define DRY                  5
#define HOT_HUMID            6
#define HOT_DRY              7
#define COLD_HUMID           8
#define COLD_DRY             9
#define TEMP_UNDEFINED      -1
#define TEMP_COMFORTABLE     0
#define TEMP_TOO_HOT         1
#define TEMP_TOO_COLD        2
#define HUM_UNDEFINED       -1
#define HUM_COMFORTABLE      0
#define HUM_TOO_HUMID        1
#define HUM_TOO_DRY          2
#define AIR_UNDEFINED        0
#define AIR_CLEAN            1
#define AIR_POLLUTED         2
#define AIR_HEAVILY_POLLUTED 3

#define HC12                 0
#define DFPlayer             1

#define LCD_DISPLAY          1
#define PIXEL_LEDS_DISPLAY   2
#define SEGMENT_DISPLAY      3
#define NUMITRON_DISPLAY     4

#define D_NX4832K035         0
#define D_NX4832T035         1
#define D_NX4827K043         2
#define D_ILI9341            3
#define D_TM1637             1
#define D_MAX7219            2

#define OPENWEATHERMAP       0
#define WEATHERBIT           1
#define OPEN_METEO           2
#define DAYS                 5

#define NETWORKS             3
#define PROVIDERS            2
#define DISPLAYS             2
#define SEQUENCES            4
#define TIMESLOTS            8
#define WSENSORS             2
#define WSENSOR_TEMPS        5
#define THNG_FIELDS          8
#define NAROD_FIELDS         12
#define MQTT_TOPICS          12

#define SSID_COUNT           30
#define SSID_LEN             33
#define IP_LEN               16
#define MAC_LEN              18
#define W_NAME_LEN           32
#define HOURLY_COUNT         40

#define DISPLAY_1             0
#define DISPLAY_2             1

#define HOUR                  0
#define MINUTE                1

#define DISP4                 0
#define DISP6                 1
#define DISP8                 2
#define DOT                   100
#define SYMBOLS               0
#define WITH_DOT              1

#define MMHG                  0
#define HPA                   1

// Division of time into hours and minutes
inline unsigned int getTime(bool level, const char* time) {
    if(!time || time[2] != ':') return 0;
    unsigned int hour = (time[0] - '0') * 10 + (time[1] - '0');
    unsigned int minute = (time[3] - '0') * 10 + (time[4] - '0');
    if(hour > 23) hour = 0;
    if(minute > 59) minute = 0;

    return level ? minute : hour;
}

extern TaskHandle_t task_display1_handle;
extern TaskHandle_t task_display2_handle;
extern TaskHandle_t task_server_handle;
extern TaskHandle_t task_sensors_handle;

extern SemaphoreHandle_t sensorsSemaphore;