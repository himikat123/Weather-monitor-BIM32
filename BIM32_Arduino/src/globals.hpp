#define FW "v6.0a"                    // Firmware version
#define REMOTE_HOST "www.google.com" // Remote host to ping

#define ALARMS                 12 // Number of alarms

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

TaskHandle_t task_display1_handle = NULL;
TaskHandle_t task_display2_handle = NULL;
TaskHandle_t task_server_handle = NULL;
TaskHandle_t task_sensors_handle = NULL;

SemaphoreHandle_t sensorsSemaphore = NULL;