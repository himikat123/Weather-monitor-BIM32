/* Pin definitions */
#define SETTINGS_BUTTON_PIN    0 // Settings button pin
#define DISPLAY1_BUTTON_PIN   34 // Display 1 button pin
#define DISPLAY2_BUTTON_PIN   35 // Display 2 button pin
#define ALARM_BUTTON_PIN      26 // Alarm button pin

#define NEXTION_TX_PIN        32 // Nextion display RX pin
#define NEXTION_RX_PIN        33 // Nextion display TX pin
#define TFT_BACKLIGHT         13 // ILI9341 LED pin

// #define TFT_RESET          EN // ILI9341 RESET pin
// #define TFT_DC              5 // ILI9341 DC pin
// #define TFT_CS             14 // ILI9341 CS pin
// #define TFT_SCK            32 // ILI9341 SCK pin
// #define TFT_DATA           33 // ILI9341 SDI pin
// #define TFT_DATA_OUT       NC // ILI9341 SDO pin
// #define TFT_T_CLK          32 // ILI9341 T_CLK pin
// #define TFT_T_CS          RXD // ILI9341 T_CS pin
// #define TFT_T_DIN          33 // ILI9341 T_DIN pin
// #define TFT_T_DO           39 // ILI9341 T_DO pin
// #define TFT_T_IRQ          NC // ILI9341 T_IRQ pin

#define WS2812_1_DAT_PIN      33 // WS2812b display 1 pin
#define WS2812_2_DAT_PIN      12 // WS2812b display 2 pin

#define DHT22_PIN              4 // DHT22 sensor pin
#define PHOTORESISTOR_PIN     36 // Photoresistor pin
#define ONE_WIRE_BUS_PIN      27 // DS18B20 one-wire bus pin

#define HC12_RX_PIN           16 // HC12 wireless module TX pin
#define HC12_TX_PIN           17 // HC12 wireless module RX pin
#define HC12_SET_PIN          25 // HC12 wireless module SET pin

#define MP3_TX_PIN            23 // DFplayer RX pin
#define MP3_BUSY_PIN          18 // DFplayer BUSY pin

#define ALARMS                12 // Number of alarms

#define SEPARATOR "**********************************************************************"

#define FW "v4.3"                    // Firmware version
#define REMOTE_HOST "www.google.com" // Remote host to ping

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

#define RAW                  false // raw data without correction
#define CORRECTED            true  // corrected data

#define LCD                  1
#define NEOPIXEL             2
#define D_NX4832K035         0
#define D_NX4832T035         1
#define D_ILI9341            2

static struct {
    bool clockSynchronized = false; // Is the time synchronized with the ntp server?
    bool clockSynchronize = false; // Should the display RTC be updated?
    bool net_connected = false; // Is the device connected to the network?
    unsigned int disp_autoOff[2] = {0, 0}; // Displays auto off interval counter
    bool reduc[2] = {false, false};
    char ssids[30][33]; // List of available networks
    unsigned int rssis[30]; // List of signal strengths of available networks
    unsigned int nets = 0; // Number of available networks
    bool apMode = false; // Access point mode
    bool display_but_pressed[2] = {false, false}; // display (1, 2) button pressed flag
    bool alarm_but_pressed = false; // alarm button pressed flag
    bool mp3_busy = true; // mp3 player busy pin
    bool fsInfoUpdate = true; // FS info update flag
    unsigned int comfort = 0; // Comfort level code: 1-Comfortable, 2-Hot, 3-Cold, 4-Humid, 5-Dry, 6-Hot & Humid, 7-Hot & Dry, 8-Cold & Humid, 9-Cold & Dry
    unsigned int iaq_level = 0; // IAQ level code: 0-undefined, 1-Air clean, 2-Air pulluted, 3-Air heavily polluted
    unsigned int co2_level = 0; // CO2 level code: 0-undefined, 1-Air clean, 2-Air pulluted, 3-Air heavily polluted
    bool clockPoints = false; // ILI9341 clock points state
    uint8_t uart2_tx = HC12; // What is UART2 connected to
} global;

TaskHandle_t task_display1_handle = NULL;
TaskHandle_t task_display2_handle = NULL;
TaskHandle_t task_sensors_handle = NULL;

class Configuration {
    #define NETWORKS 3
    #define PROVIDERS 2
    #define DISPLAYS 2
    #define SEQUENCES 4
    #define TIMESLOTS 8
    #define WSENSORS 2
    #define WSENSOR_TEMPS 5
    #define THNG_FIELDS 8
    #define NAROD_FIELDS 12
    #define MQTT_TOPICS 12

    private:

    // Comfort
    unsigned int _comfort_temp_source = 0; // Comfort temperature source: 0-Nothing, 1-Forecast, 2-Wireless sensor, 3-Thingspeak, 4-BME280, 5-BMP180, 6-SHT21, 7-DHT22, 8-DS18B20, 9-BME680
    unsigned int _comfort_temp_wsensNum = 0; // Comfort temperature wireless sensor number
    unsigned int _comfort_temp_sens = 0; // Comfort temperature wireless sensor temperature sensor number
    unsigned int _comfort_temp_thing = 0; // Comfort temperature thingspeak field number
    float _comfort_temp_min = 23.0; // Minimum comfort temperature
    float _comfort_temp_max = 25.0; // Maximum comfort temperature
    boolean _comfort_temp_sound = false; // Sound notification when the temperature goes beyond comfort limits
    float _comfort_temp_min_hysteresis = 0.0; // Minimum comfort temperature hysteresis
    float _comfort_temp_max_hysteresis = 0.0; // Maximum comfort temperature hysteresis
    unsigned int _comfort_hum_source = 0; // Comfort humidity source: 0-Nothing, 1-Forecast, 2-Wireless sensor, 3-Thingspeak, 4-BME280, 5-SHT21, 6-DHT22, 7-BME680
    unsigned int _comfort_hum_wsensNum = 0; // Comfort humidity wireless sensor number
    unsigned int _comfort_hum_thing = 0; // Comfort humidity thingspeak field number
    float _comfort_hum_min = 40.0; // Minimum comfort humidity
    float _comfort_hum_max = 60.0; // Maximum comfort humidity
    boolean _comfort_hum_sound = 0; // Sound notification when the humidity goes beyond comfort limits
    float _comfort_hum_min_hysteresis = 0.0; // Minimum comfort humidity hysteresis
    float _comfort_hum_max_hysteresis = 0.0; // Maximum comfort humidity hysteresis
    unsigned int _comfort_iaq_source = 0; // Comfort IAQ source: 0-Nothing, 1-BME680
    boolean _comfort_iaq_sound = false; // Sound notification when the IAQ goes beyond comfort limits
    unsigned int _comfort_co2_source = 0; // Comfort CO2 source: 0-Nothing, 1-Wireless sensor
    unsigned int _comfort_co2_wsensNum = 0; // Comfort CO2 wireless sensor number
    boolean _comfort_co2_sound = false; // Sound notification when the CO2 goes beyond comfort limits

    //WiFi network
    char _network_ssid[NETWORKS][33] = { "", "", "" }; // SSID list
    char _network_pass[NETWORKS][33] = { "", "", "" }; // Password list
    bool _network_type = false; // Connection type: false = dynamic IP, true = static IP
    char _network_ip[16] = ""; // Static IP address
    char _network_mask[16] = ""; // Static Subnet mask
    char _network_gw[16] = ""; // Static default gateway
    char _network_dns1[16] = ""; // Static DNS1 address
    char _network_dns2[16] = ""; // Static DNS2 address

    // Access point
    char _accessPoint_ssid[33] = "BIM32"; // SSID
    char _accessPoint_pass[33] = "1234567890"; // Password
    unsigned int _accessPoint_chnl = 1; // WiFi channel number 1...13
    char _accessPoint_ip[33] = "192.168.4.1"; // IP address
    char _accessPoint_mask[33] = "255.255.255.0"; // Subnet mask

    // Weather
    char _weather_appid[PROVIDERS][33] = { "", "" }; // [0] -> APPID openweathermap.org, [1] -> KEY weatherbit.io
    float _weather_lon = 0.0; // Longitude
    float _weather_lat = 0.0; // Latitude
    uint8_t _weather_provider = 0; // Weather forecast provider. 0: openweathermap.org, 1: weatherbit.io, 2: open-meteo.com 
    char _weather_city[41] = ""; // City name
    unsigned int _weather_cityid = 0; // City ID
    unsigned int _weather_citysearch = 0; // The way to recognize a city. 0 = by name, 1 = by ID, 2 = by coordinates

    // Language
    char _lang[3] = "en";

    // Clock
    bool _clock_format = false; // Clock format: false = 12 hour, true = 24 hour 
    char _clock_ntp[65] = "time.nist.gov"; // NTP server address
    int _clock_utc = 0; // Timezone -12...13
    bool _clock_dlst = false; // Auto daylight saving time
    unsigned int _clock_ntp_period = 15; // NTP update period (minutes) 0...90000

    // Display
    unsigned int _display_type[DISPLAYS] = {0, 0}; // Display type
    unsigned int _display_model[DISPLAYS] = {0, 0}; // Display model
    unsigned int _display_animation_type[DISPLAYS] = {0, 0}; // Display animation number 0...9
    unsigned int _display_animation_speed[DISPLAYS] = {10, 10}; // Display animation speed 1...30
    unsigned int _display_animation_points[DISPLAYS] = {0, 0}; // Display animation clock points 0...4
    char _display_dayTime[DISPLAYS][6] = {"07:00", "07:00"}; // Time to switch to day mode
    char _display_nightTime[DISPLAYS][6] = {"21:00", "21:00"}; // Time to switch to night mode
    unsigned int _display_brightMethod[DISPLAYS] = {3, 3}; // Display brightness adjustment method: 0-Auto, 1-By light sensor, 2-By time, 3-Constant
    unsigned int _display_autoOff[DISPLAYS] = {0, 0}; // Display auto-off time 0...1440
    bool _display_nightOff[DISPLAYS] = {false, false}; // Turn off display at night
    unsigned int _display_nightOff_from[DISPLAYS] = {22, 22}; // The hour from which the display is turned off
    unsigned int _display_nightOff_to[DISPLAYS] = {7, 7}; // The hour from which the display is turned on
    unsigned int _display_brightness_day[DISPLAYS] = {50, 50}; // Day mode brightness 1...100
    unsigned int _display_brightness_night[DISPLAYS] = {50, 50}; // Night mode brightness 1...100
    unsigned int _display_brightness_min[DISPLAYS] = {1, 1}; // Minimum brightness limit 0...255
    unsigned int _display_brightness_max[DISPLAYS] = {255, 255}; // Maximum brightness limit 0...255
    unsigned int _display_lightSensor[DISPLAYS] = {1, 1}; // Sensor type for brightness adjust: 0-Analog input, 1-MAX44009, 2-BH1750
    unsigned int _display_lightSensor_sensitivity[DISPLAYS] = {50, 50}; // Ambient light sensor sensibility 1...100
    unsigned int _display_source_tempOut_sens = 0; // Outdoor temperature data source: 0-Nothing, 1-Forecast, 2-Wireless sensor, 3-Thingspeak, 4-BME280, 5-BMP180, 6-SHT21, 7-DHT22, 8-DS18B20, 9-BME680
    unsigned int _display_source_tempOut_wsensNum = 0; // Wireless sensor number for the outdoor temperature
    unsigned int _display_source_tempOut_temp = 0; // Sensor number for the outdoor temperature
    unsigned int _display_source_tempOut_thing = 0; // Thingspeak field number for the outdoor temperature
    unsigned int _display_source_humOut_sens = 0; // Outdoor humidity data source: 0-Nothing, 1-Forecast, 2-Wireless sensor, 3-Thingspeak, 4-BME280, 5-SHT21, 6-DHT22, 7-BME680
    unsigned int _display_source_humOut_wsensNum = 0; // Wireless sensor number for the outdoor humidity
    unsigned int _display_source_humOut_thing = 0; // Thingspeak field number for the outdoor humidity
    unsigned int _display_source_presOut_sens = 0; // Outdoor pressure data source: 0-Nothing, 1-Forecast, 2-Wireless sensor, 3-Thingspeak, 4-BME280, 5-BMP180, 6-BME680
    unsigned int _display_source_presOut_wsensNum = 0; // Wireless sensor number for the outdoor pressure
    unsigned int _display_source_presOut_thing = 0; // Thingspeak field number for the outdoor pressure
    unsigned int _display_source_tempIn_sens = 0; // Indoor temperature data source: 0-Nothing, 1-Forecast, 2-Wireless sensor, 3-Thingspeak, 4-Sequence, 5-BME280, 6-BMP180, 7-SHT21, 8-DHT22, 9-DS18B20, 10-BME680
    unsigned int _display_source_tempIn_wsensNum = 0; // Wireless sensor number for the indoor temperature
    unsigned int _display_source_tempIn_temp = 0; // Sensor number for the indoor temperature
    unsigned int _display_source_tempIn_thing = 0; // Thingspeak field number for the intdoor temperature
    unsigned int _display_source_humIn_sens = 0; // Indoor humidity data source: 0-Nothing, 1-Forecast, 2-Wireless sensor, 3-Thingspeak, 4-Sequence, 5-BME280, 6-SHT21, 7-DHT22, 8-BME680
    unsigned int _display_source_humIn_wsensNum = 0; // Wireless sensor number for the indoor humidity
    unsigned int _display_source_humIn_thing = 0; // Thingspeak field number for the outdoor humidity
    unsigned int _display_source_volt_sens = 0; // Voltage data source: 0-Nothing, 1-Wireless sensor, 2-Thingspeak, 3-BME680-IAQ
    unsigned int _display_source_volt_wsensNum = 0; // Wireless sensor number for the voltage
    unsigned int _display_source_volt_volt = 0; // Sensor type for the voltage: 0-Wireless sensor battery voltage, 1-Wireless sensor battery percentage, 2-PZEM-004t voltage, 3-SenseAir-S8
    unsigned int _display_source_volt_thing = 0; // Thingspeak field number for the voltage
    unsigned int _display_source_volt_thingType = 0; // Thingspeak field voltage data type: 0-Battery voltage, 1-Battery percentage
    unsigned int _display_source_bat_sens = 0; // Battery level data source: 0-Nothing, 1-Wireless sensor, 2-Thingspeak
    unsigned int _display_source_bat_wsensNum = 0; // Wireless sensor number for the battery level:
    unsigned int _display_source_bat_thing = 0; // Thingspeak field number for the battery level
    unsigned int _display_source_descr = 0; // Additional description data source: 0-Nothing, 1-Comfort level, 2-Sequence
    unsigned int _display_source_sequence_dur = 2; // Sequence data display duration (seconds)
    char _display_source_sequence_name[SEQUENCES][33] = {"", "", "", ""}; // Sequence data names
    unsigned int _display_source_sequence_temp[SEQUENCES] = {0, 0, 0, 0}; // Sequence data sources for the temperature sequence: 0-Nothing, 1-Thingspeak, 2-Wireless sensor, 3-BME280, 4-BMP180, 5-SHT21, 6-DHT22, 7-DS18B20, 8-ESP32, 9-Forecast, 10-BME680
    unsigned int _display_source_sequence_thngtemp[SEQUENCES] = {0, 0, 0, 0}; // Thingspeak field number for the temperature sequence
    unsigned int _display_source_sequence_wsenstemp[SEQUENCES][WSENSORS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Wireless sensor number and its sensor for the temperature sequence
    unsigned int _display_source_sequence_hum[SEQUENCES] = {0, 0, 0, 0}; // Sequence data sources for the humidity sequence: 0-Nothing, 1-Thingspeak, 2-Wireless sensor, 3-BME280, 4-SHT21, 5-DHT22, 6-Forecast, 7-BME680
    unsigned int _display_source_sequence_thnghum[SEQUENCES] = {0, 0, 0, 0}; // Thingspeak field number for the humidity sequence
    unsigned int _display_source_sequence_wsenshum[SEQUENCES] =  {0, 0, 0, 0}; // Wireless sensor number for the humidity sequence
    unsigned int _display_timeSlot_period[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot durations
    unsigned int _display_timeSlot_sensor[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot data sources: 0-Time, 1-Date, 2-BME280, 3-BMP180, 4-SHT21, 5-DHT22, 6-DS18B20, 7-ESP32, 8-Thingspeak, 9-Weather forecast, 10-Wireless sensor, 11-BME680 
    unsigned int _display_timeSlot_data[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot sensors types
    unsigned int _display_timeSlot_thing[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot thingspeak field number
    unsigned int _display_timeSlot_wsensor_num[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot wireless sensor number
    unsigned int _display_timeSlot_wsensor_type[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot wireless sensor type 
    char _display_timeSlot_color[TIMESLOTS][DISPLAYS][8] = { // Segment display timeslot colors 
        {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"},
        {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}
    };

    // Sound
    unsigned int _sound_vol = 15; // Sound volume
    unsigned int _sound_eq = 0; // Equalizer: 0-Normal, 1-Pop, 2-Rock, 3-Jazz, 4-Classic, 5-Bass
    unsigned int _sound_hourly = 2; // Hourly signal: 0-Always ON, 1-Always OFF, 2-On from dawn to dusk, 3-Enabled by time
    unsigned int _sound_hour_from = 8; // The hour from which the hourly signal is turned on
    unsigned int _sound_hour_to = 21; // The hour from which the hourly signal is turned off

    // Sensors
    float _bme280_temp_corr = 0.0; // BME280 temperature correction
    float _bme280_hum_corr = 0.0; // BME280 humidity correction
    float _bme280_pres_corr = 0.0; // BME280 pressure correction
    float _bmp180_temp_corr = 0.0; // BMP180 temperature correction
    float _bmp180_pres_corr = 0.0; // BMP180 pressure correction
    float _sht21_temp_corr = 0.0; // SHT21 temperature correction
    float _sht21_hum_corr = 0.0; // SHT21 humidity correction
    float _dht22_temp_corr = 0.0; // DHT22 temperature correction
    float _dht22_hum_corr = 0.0; // DHT22 humidity correction
    float _ds18b20_temp_corr = 0.0; // DS18B20 temperature correction
    float _esp32_temp_corr = 0.0; // ESP32 temperature correction
    float _max44009_light_corr = 0.0; // MAX44009 ambient light correction
    float _bh1750_light_corr = 0.0; // BH1750 ambient light correction
    float _analog_voltage_corr = 0.0; // Analog ambient light sensor voltage correction
    float _bme680_temp_corr = 0.0; // BME680 temperature correction
    float _bme680_hum_corr = 0.0; // BME680 humidity correction
    float _bme680_pres_corr = 0.0; // BME680 pressure correction
    float _bme680_iaq_corr = 0.0; // BME680 IAQ correction

    // Wireless sensors
    float _wsensor_temp_corr[WSENSORS][WSENSOR_TEMPS] = { {0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0, 0.0} }; // Wireless sensor temperature correction
    float _wsensor_hum_corr[WSENSORS] = {0.0, 0.0}; // Wireless sensor humidity correction
    float _wsensor_pres_corr[WSENSORS] = {0.0, 0.0}; // Wireless sensor pressure correction
    float _wsensor_light_corr[WSENSORS] = {0.0, 0.0}; // Wireless sensor ambient light correction
    float _wsensor_volt_corr[WSENSORS] = {0.0, 0.0}; // Wireless sensor PZEM-004t voltage correction
    float _wsensor_curr_corr[WSENSORS] = {0.0, 0.0}; // Wireless sensor PZEM-004t current correction
    float _wsensor_pow_corr[WSENSORS] = {0.0, 0.0}; // Wireless sensor PZEM-004t power correction
    float _wsensor_enrg_corr[WSENSORS] = {0.0, 0.0}; // Wireless sensor PZEM-004t energy correction
    float _wsensor_freq_corr[WSENSORS] = {0.0, 0.0}; // Wireless sensor PZEM-004t frequency correction
    float _wsensor_co2_corr[WSENSORS] = {0.0, 0.0}; // Wireless sensor SenseAir S8 CO2 correction
    float _wsensor_bat_k[WSENSORS] = {125, 125}; // Wireless sensor battery voltage ADC division factor
    unsigned int _wsensor_bat_type[WSENSORS] = {0, 0}; // Wireless sensor battery type: 0 - 3x Batteries (4.5V), 1 - LiIon battery (3.7V) 
    unsigned int _wsensor_expire[WSENSORS] = {10, 10}; // Wireless sensor data expire (minutes) 1...100
    unsigned int _wsensor_channel = 1; // Wireless sensors channel number 1...100

    // Weather history repository
    unsigned int _history_period = 0; // Period data update (minutes) 0...999
    char _history_channelID[11] = ""; // Weather repository channel ID
    char _history_wrkey[17] = ""; // Weather repository Write API Key
    char _history_rdkey[17] = ""; // Weather repository Read API Key
    unsigned int _history_fields[7] = {0,0,0,0,0,0,0}; // Weather repository fields data sources 
    unsigned int _history_wSensors[7] = {0,0,0,0,0,0,0}; // Weather repository wireless sensor numbers
    unsigned int _history_wTypes[7] = {0,0,0,0,0,0,0}; // Weather repository wireless sensor temperature sensor numbers
    unsigned int _history_tFields[7] = {0,0,0,0,0,0,0}; // Weather repository thingspeak field numbers

    // Thingspeak send
    unsigned int _thingspeakSend_period = 0; // Period for sending data to Thingspeak (minutes) 0...999
    char _thingspeakSend_channelID[11] = ""; // Channel ID for sending data to Thingspeak
    char _thingspeakSend_wrkey[17] = ""; // Write API Key for sending data to Thingspeak
    char _thingspeakSend_rdkey[17] = ""; // Read API Key for sending data to Thingspeak
    unsigned int _thingspeakSend_fields[THNG_FIELDS] = {0, 0, 0, 0, 0, 0, 0, 0}; // Data sources to send to Thingspeak fields
    unsigned int _thingspeakSend_types[THNG_FIELDS] = {0, 0, 0, 0, 0, 0, 0, 0}; // Wired sensor data types to send to Thingspeak
    unsigned int _thingspeakSend_wsensors[THNG_FIELDS] = {0, 0, 0, 0, 0, 0, 0, 0}; // Wireless sensor numbers to send to Thingspeak
    unsigned int _thingspeakSend_wtypes[THNG_FIELDS] = {0, 0, 0, 0, 0, 0, 0, 0}; // Wireless sensor data types to send to Thingspeak

    // Thingspeak receive
    unsigned int _thingspeakReceive_period = 0; // Period for receiving data from Thingspeak (minutes) 0...999
    char _thingspeakReceive_channelID[11] = ""; // Channel ID for receiving data from Thingspeak
    char _thingspeakReceive_rdkey[17] = ""; // Read API Key for receiving data from Thingspeak
    unsigned int _thingspeakReceive_expire = 10; // Thingspeak data expire (minutes) 1...100

    // Narodmon send
    unsigned int _narodmonSend_period = 0; // Period for sending data to Narodmon (minutes) 0...999
    char _narodmonSend_lon[11] = ""; // Longitude for Narodmon
    char _narodmonSend_lat[11] = ""; // Latitude for Narodmon
    char _narodmonSend_name[17] = "BIM"; // Sensor name
    unsigned int _narodmonSend_sensors[NAROD_FIELDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Data sources to send to Narodmon fields
    char _narodmonSend_metrics[NAROD_FIELDS][17] {"", "", "", "", "", "", "", "", "", "", "", ""}; // Sensor metrics to send to Narodmon fields
    unsigned int _narodmonSend_types[NAROD_FIELDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Wired sensor data types to send to Narodmon
    unsigned int _narodmonSend_wsensors[NAROD_FIELDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Wireless sensor numbers to send to Narodmon
    unsigned int _narodmonSend_wtypes[NAROD_FIELDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Wireless sensor data types to send to Narodmon

    // MQTT send
    unsigned int _mqttSend_period = 0; // Period for sending data via MQTT (seconds) 0...999
    char _mqttSend_broker[33] = ""; // MQTT broker address
    unsigned int _mqttSend_port = 1883; // MQTT port
    char _mqttSend_user[33] = ""; // MQTT username
    char _mqttSend_pass[33] = ""; // MQTT password
    unsigned int _mqttSend_sensors[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Data sources to send via MQTT
    unsigned int _mqttSend_types[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Wired sensor data types to send via MQTT
    unsigned int _mqttSend_wsensors[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Wireless sensor numbers to send via MQTT
    unsigned int _mqttSend_wtypes[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Wireless sensor data types to send via MQTT
    char _mqttSend_topics[12][17] = {"", "", "", "", "", "", "", "", "", "", "", ""}; // Topic names for sending via MQTT
    
    // Account
    char _account_name[32] = ""; // Web interface default username
    char _account_pass[64] = ""; // Web interface default password
    bool _account_required = false; // Require username and password to access the web interface

    // Alarm
    unsigned int _alarm_time[ALARMS][2] = { // Alarm time [hour, minute]
        {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}
    };
    unsigned int _alarm_weekdays[ALARMS][7] = { // Alarm days of the week [mo, tu, we, th, fr, sa, su]
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };
    unsigned int _alarm_states[ALARMS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Alarm state (0-Off, 1-On)
    unsigned int _alarm_melodies[ALARMS] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // Alarm selected melody (1...20)


    // Division of time into hours and minutes
    unsigned int _get_time(bool level, String time) {
        unsigned int hour = time.substring(0, 2).toInt();
        unsigned int minute = time.substring(3, 5).toInt();
        if(hour > 23) hour = 0;
        if(minute > 59) minute = 0;
        return level ? minute : hour; 
    }


    public:

    void readConfig(void) {
        #define COPYSTR(from, to) strlcpy(to, from | to, sizeof(to))
        #define COPYNUM(from, to) to = from | to
        #define COPYBOOL(from, to) to = from.as<bool>() | to

        /* Read config file */
        Serial.println(SEPARATOR);
        Serial.print("Read config file... ");
        File file = LittleFS.open("/config.json");
        if(file) {
            while(file.available()) {
                String json = file.readString();
                JsonDocument conf;

                DeserializationError error = deserializeJson(conf, json);
                if(!error) {
                    //WiFi network
                    for(unsigned int i=0; i<NETWORKS; i++) {
                        COPYSTR(conf["network"]["ssid"][i], _network_ssid[i]);
                        COPYSTR(conf["network"]["pass"][i], _network_pass[i]);
                    }
                    COPYBOOL(conf["network"]["type"], _network_type);
                    COPYSTR(conf["network"]["ip"], _network_ip);
                    COPYSTR(conf["network"]["mask"], _network_mask);
                    COPYSTR(conf["network"]["gw"], _network_gw);
                    COPYSTR(conf["network"]["dns1"], _network_dns1);
                    COPYSTR(conf["network"]["dns2"], _network_dns2);

                    // Access point
                    COPYSTR(conf["accessPoint"]["ssid"], _accessPoint_ssid);
                    COPYSTR(conf["accessPoint"]["pass"], _accessPoint_pass);
                    COPYNUM(conf["accessPoint"]["chnl"], _accessPoint_chnl);
                    COPYSTR(conf["accessPoint"]["ip"], _accessPoint_ip);
                    COPYSTR(conf["accessPoint"]["mask"], _accessPoint_mask);

                    // Weather
                    for(unsigned int i=0; i<PROVIDERS; i++) COPYSTR(conf["weather"]["appid"][i], _weather_appid[i]);
                    COPYSTR(conf["weather"]["city"], _weather_city);
                    COPYNUM(conf["weather"]["cityid"], _weather_cityid);
                    COPYNUM(conf["weather"]["lon"], _weather_lon);
                    COPYNUM(conf["weather"]["lat"], _weather_lat);
                    COPYNUM(conf["weather"]["provider"], _weather_provider);
                    COPYNUM(conf["weather"]["citysearch"], _weather_citysearch);

                    // Language
                    COPYSTR(conf["lang"], _lang);

                    // Clock
                    COPYBOOL(conf["clock"]["format"], _clock_format);
                    COPYSTR(conf["clock"]["ntp"], _clock_ntp);
                    COPYNUM(conf["clock"]["utc"], _clock_utc);
                    COPYBOOL(conf["clock"]["dlst"], _clock_dlst);
                    COPYNUM(conf["clock"]["ntp_period"], _clock_ntp_period);

                    // Display
                    for(unsigned int i=0; i<DISPLAYS; i++) {
                        COPYNUM(conf["display"]["type"][i], _display_type[i]);
                        COPYNUM(conf["display"]["model"][i], _display_model[i]);
                        COPYSTR(conf["display"]["dayTime"][i], _display_dayTime[i]);
                        COPYSTR(conf["display"]["nightTime"][i], _display_nightTime[i]);
                        COPYNUM(conf["display"]["brightMethod"][i], _display_brightMethod[i]);
                        COPYNUM(conf["display"]["autoOff"][i], _display_autoOff[i]);
                        COPYBOOL(conf["display"]["nightOff"]["need"][i], _display_nightOff[i]);
                        COPYNUM(conf["display"]["nightOff"]["from"][i], _display_nightOff_from[i]);
                        COPYNUM(conf["display"]["nightOff"]["to"][i], _display_nightOff_to[i]);
                        COPYNUM(conf["display"]["brightness"]["day"][i], _display_brightness_day[i]);
                        COPYNUM(conf["display"]["brightness"]["night"][i], _display_brightness_night[i]);
                        COPYNUM(conf["display"]["brightness"]["min"][i], _display_brightness_min[i]);
                        COPYNUM(conf["display"]["brightness"]["max"][i], _display_brightness_max[i]);
                        COPYNUM(conf["display"]["lightSensor"][i], _display_lightSensor[i]);
                        COPYNUM(conf["display"]["lightSensor_sensitivity"][i], _display_lightSensor_sensitivity[i]);
                        COPYNUM(conf["display"]["animation"]["type"][i], _display_animation_type[i]);
                        COPYNUM(conf["display"]["animation"]["speed"][i], _display_animation_speed[i]);
                        COPYNUM(conf["display"]["animation"]["points"][i], _display_animation_points[i]);
                        for(unsigned int t=0; t<TIMESLOTS; t++) {
                            COPYNUM(conf["display"]["timeSlot"]["period"][t][i], _display_timeSlot_period[t][i]);
                            COPYNUM(conf["display"]["timeSlot"]["sensor"][t][i], _display_timeSlot_sensor[t][i]);
                            COPYNUM(conf["display"]["timeSlot"]["data"][t][i], _display_timeSlot_data[t][i]);
                            COPYNUM(conf["display"]["timeSlot"]["thing"][t][i], _display_timeSlot_thing[t][i]);
                            COPYSTR(conf["display"]["timeSlot"]["color"][t][i], _display_timeSlot_color[t][i]);
                            COPYNUM(conf["display"]["timeSlot"]["wsensor"]["num"][t][i], _display_timeSlot_wsensor_num[t][i]);
                            COPYNUM(conf["display"]["timeSlot"]["wsensor"]["type"][t][i], _display_timeSlot_wsensor_type[t][i]); 
                        }
                    }
                    COPYNUM(conf["display"]["source"]["tempOut"]["sens"], _display_source_tempOut_sens);
                    COPYNUM(conf["display"]["source"]["tempOut"]["wsensNum"], _display_source_tempOut_wsensNum);
                    COPYNUM(conf["display"]["source"]["tempOut"]["temp"], _display_source_tempOut_temp);
                    COPYNUM(conf["display"]["source"]["tempOut"]["thing"], _display_source_tempOut_thing);
                    COPYNUM(conf["display"]["source"]["humOut"]["sens"], _display_source_humOut_sens);
                    COPYNUM(conf["display"]["source"]["humOut"]["wsensNum"], _display_source_humOut_wsensNum);
                    COPYNUM(conf["display"]["source"]["humOut"]["thing"], _display_source_humOut_thing);
                    COPYNUM(conf["display"]["source"]["presOut"]["sens"], _display_source_presOut_sens);
                    COPYNUM(conf["display"]["source"]["presOut"]["wsensNum"], _display_source_presOut_wsensNum);
                    COPYNUM(conf["display"]["source"]["presOut"]["thing"], _display_source_presOut_thing);
                    COPYNUM(conf["display"]["source"]["tempIn"]["sens"], _display_source_tempIn_sens);
                    COPYNUM(conf["display"]["source"]["tempIn"]["wsensNum"], _display_source_tempIn_wsensNum);
                    COPYNUM(conf["display"]["source"]["tempIn"]["temp"], _display_source_tempIn_temp);
                    COPYNUM(conf["display"]["source"]["tempIn"]["thing"], _display_source_tempIn_thing);
                    COPYNUM(conf["display"]["source"]["humIn"]["sens"], _display_source_humIn_sens);
                    COPYNUM(conf["display"]["source"]["humIn"]["wsensNum"], _display_source_humIn_wsensNum);
                    COPYNUM(conf["display"]["source"]["humIn"]["thing"], _display_source_humIn_thing);
                    COPYNUM(conf["display"]["source"]["volt"]["sens"], _display_source_volt_sens);
                    COPYNUM(conf["display"]["source"]["volt"]["wsensNum"], _display_source_volt_wsensNum);
                    COPYNUM(conf["display"]["source"]["volt"]["volt"], _display_source_volt_volt);
                    COPYNUM(conf["display"]["source"]["volt"]["thing"], _display_source_volt_thing);
                    COPYNUM(conf["display"]["source"]["volt"]["thingType"], _display_source_volt_thingType);
                    COPYNUM(conf["display"]["source"]["bat"]["sens"], _display_source_bat_sens);
                    COPYNUM(conf["display"]["source"]["bat"]["wsensNum"], _display_source_bat_wsensNum);
                    COPYNUM(conf["display"]["source"]["bat"]["thing"], _display_source_bat_thing);
                    COPYNUM(conf["display"]["source"]["descr"], _display_source_descr);
                    COPYNUM(conf["display"]["source"]["sequence"]["dur"], _display_source_sequence_dur);
                    for(unsigned int i=0; i<SEQUENCES; i++) {
                        COPYSTR(conf["display"]["source"]["sequence"]["name"][i], _display_source_sequence_name[i]);
                        COPYNUM(conf["display"]["source"]["sequence"]["temp"][i], _display_source_sequence_temp[i]);
                        COPYNUM(conf["display"]["source"]["sequence"]["thngtemp"][i], _display_source_sequence_thngtemp[i]);
                        COPYNUM(conf["display"]["source"]["sequence"]["hum"][i], _display_source_sequence_hum[i]);
                        COPYNUM(conf["display"]["source"]["sequence"]["thnghum"][i], _display_source_sequence_thnghum[i]);
                        COPYNUM(conf["display"]["source"]["sequence"]["wsenshum"][i], _display_source_sequence_wsenshum[i]);
                        for(unsigned int k=0; k<WSENSORS; k++) {
                            COPYNUM(conf["display"]["source"]["sequence"]["wsenstemp"][i][k], _display_source_sequence_wsenstemp[i][k]);
                        }
                    }

                    // Sound
                    COPYNUM(conf["sound"]["vol"], _sound_vol);
                    COPYNUM(conf["sound"]["eq"], _sound_eq);
                    COPYNUM(conf["sound"]["hourly"], _sound_hourly);
                    COPYNUM(conf["sound"]["hour"]["from"], _sound_hour_from);
                    COPYNUM(conf["sound"]["hour"]["to"], _sound_hour_to);

                    // Sensors
                    COPYNUM(conf["sensors"]["bme280"]["t"], _bme280_temp_corr);
                    COPYNUM(conf["sensors"]["bme280"]["h"], _bme280_hum_corr);
                    COPYNUM(conf["sensors"]["bme280"]["p"], _bme280_pres_corr);
                    COPYNUM(conf["sensors"]["bmp180"]["t"], _bmp180_temp_corr);
                    COPYNUM(conf["sensors"]["bmp180"]["p"], _bmp180_pres_corr);
                    COPYNUM(conf["sensors"]["sht21"]["t"], _sht21_temp_corr);
                    COPYNUM(conf["sensors"]["sht21"]["h"], _sht21_hum_corr);
                    COPYNUM(conf["sensors"]["dht22"]["t"], _dht22_temp_corr);
                    COPYNUM(conf["sensors"]["dht22"]["h"], _dht22_hum_corr);
                    COPYNUM(conf["sensors"]["ds18b20"]["t"], _ds18b20_temp_corr);
                    COPYNUM(conf["sensors"]["esp32"]["t"], _esp32_temp_corr);
                    COPYNUM(conf["sensors"]["max44009"]["l"], _max44009_light_corr);
                    COPYNUM(conf["sensors"]["bh1750"]["l"], _bh1750_light_corr);
                    COPYNUM(conf["sensors"]["analog"]["v"], _analog_voltage_corr);
                    COPYNUM(conf["sensors"]["bme680"]["t"], _bme680_temp_corr);
                    COPYNUM(conf["sensors"]["bme680"]["h"], _bme680_hum_corr);
                    COPYNUM(conf["sensors"]["bme680"]["p"], _bme680_pres_corr);
                    COPYNUM(conf["sensors"]["bme680"]["i"], _bme680_iaq_corr);

                    // Wireless sensors
                    for(unsigned int i=0; i<WSENSORS; i++) {
                        for(unsigned int k=0; k<WSENSOR_TEMPS; k++) {
                            COPYNUM(conf["wsensor"]["temp"]["corr"][i][k], _wsensor_temp_corr[i][k]);
                        }
                        COPYNUM(conf["wsensor"]["hum"]["corr"][i], _wsensor_hum_corr[i]);
                        COPYNUM(conf["wsensor"]["pres"]["corr"][i], _wsensor_pres_corr[i]);
                        COPYNUM(conf["wsensor"]["light"]["corr"][i], _wsensor_light_corr[i]);
                        COPYNUM(conf["wsensor"]["volt"]["corr"][i], _wsensor_volt_corr[i]);
                        COPYNUM(conf["wsensor"]["curr"]["corr"][i], _wsensor_curr_corr[i]);
                        COPYNUM(conf["wsensor"]["pow"]["corr"][i], _wsensor_pow_corr[i]);
                        COPYNUM(conf["wsensor"]["enrg"]["corr"][i], _wsensor_enrg_corr[i]);
                        COPYNUM(conf["wsensor"]["freq"]["corr"][i], _wsensor_freq_corr[i]);
                        COPYNUM(conf["wsensor"]["co2"]["corr"][i], _wsensor_co2_corr[i]);
                        COPYNUM(conf["wsensor"]["bat"]["k"][i], _wsensor_bat_k[i]);
                        COPYNUM(conf["wsensor"]["bat"]["type"][i], _wsensor_bat_type[i]);
                        COPYNUM(conf["wsensor"]["expire"][i], _wsensor_expire[i]); 
                    }
                    COPYNUM(conf["wsensor"]["channel"], _wsensor_channel);

                    // Weather history repository
                    COPYNUM(conf["history"]["period"], _history_period);
                    COPYSTR(conf["history"]["channelID"], _history_channelID);
                    COPYSTR(conf["history"]["wrkey"], _history_wrkey);
                    COPYSTR(conf["history"]["rdkey"], _history_rdkey);
                    for(unsigned int i=0; i<7; i++) {
                        COPYNUM(conf["history"]["fields"][i], _history_fields[i]);
                        COPYNUM(conf["history"]["wSensors"][i], _history_wSensors[i]);
                        COPYNUM(conf["history"]["wTypes"][i], _history_wTypes[i]);
                        COPYNUM(conf["history"]["tFields"][i], _history_tFields[i]);
                    }

                    // Thingspeak send
                    COPYNUM(conf["thingspeakSend"]["period"], _thingspeakSend_period);
                    COPYSTR(conf["thingspeakSend"]["channelID"], _thingspeakSend_channelID);
                    COPYSTR(conf["thingspeakSend"]["wrkey"], _thingspeakSend_wrkey);
                    COPYSTR(conf["thingspeakSend"]["rdkey"], _thingspeakSend_rdkey);
                    for(unsigned int i=0; i<THNG_FIELDS; i++) {
                        COPYNUM(conf["thingspeakSend"]["fields"][i], _thingspeakSend_fields[i]);
                        COPYNUM(conf["thingspeakSend"]["types"][i], _thingspeakSend_types[i]);
                        COPYNUM(conf["thingspeakSend"]["wsensors"][i], _thingspeakSend_wsensors[i]);
                        COPYNUM(conf["thingspeakSend"]["wtypes"][i], _thingspeakSend_wtypes[i]);
                    }

                    // Thingspeak receive
                    COPYNUM(conf["thingspeakReceive"]["period"], _thingspeakReceive_period);
                    COPYSTR(conf["thingspeakReceive"]["channelID"], _thingspeakReceive_channelID);
                    COPYSTR(conf["thingspeakReceive"]["rdkey"], _thingspeakReceive_rdkey);
                    COPYNUM(conf["thingspeakReceive"]["expire"], _thingspeakReceive_expire);

                    // Narodmon send
                    COPYNUM(conf["narodmonSend"]["period"], _narodmonSend_period);
                    COPYSTR(conf["narodmonSend"]["lon"], _narodmonSend_lon);
                    COPYSTR(conf["narodmonSend"]["lat"], _narodmonSend_lat);
                    COPYSTR(conf["narodmonSend"]["name"], _narodmonSend_name);
                    for(unsigned int i=0; i<NAROD_FIELDS; i++) {
                        COPYNUM(conf["narodmonSend"]["sensors"][i], _narodmonSend_sensors[i]);
                        COPYSTR(conf["narodmonSend"]["metrics"][i], _narodmonSend_metrics[i]);
                        COPYNUM(conf["narodmonSend"]["types"][i], _narodmonSend_types[i]);
                        COPYNUM(conf["narodmonSend"]["wsensors"][i], _narodmonSend_wsensors[i]);
                        COPYNUM(conf["narodmonSend"]["wtypes"][i], _narodmonSend_wtypes[i]);
                    }

                    // MQTT send
                    COPYNUM(conf["mqttSend"]["period"], _mqttSend_period);
                    COPYSTR(conf["mqttSend"]["broker"], _mqttSend_broker);
                    COPYNUM(conf["mqttSend"]["port"], _mqttSend_port);
                    COPYSTR(conf["mqttSend"]["user"], _mqttSend_user);
                    COPYSTR(conf["mqttSend"]["pass"], _mqttSend_pass);
                    for(unsigned int i=0; i<MQTT_TOPICS; i++) {
                        COPYNUM(conf["mqttSend"]["sensors"][i], _mqttSend_sensors[i]);
                        COPYNUM(conf["mqttSend"]["types"][i], _mqttSend_types[i]);
                        COPYNUM(conf["mqttSend"]["wsensors"][i], _mqttSend_wsensors[i]);
                        COPYNUM(conf["mqttSend"]["wtypes"][i], _mqttSend_wtypes[i]);
                        COPYSTR(conf["mqttSend"]["topics"][i], _mqttSend_topics[i]);
                    }

                    // Comfort
                    COPYNUM(conf["comfort"]["temp"]["source"], _comfort_temp_source);
                    COPYNUM(conf["comfort"]["temp"]["wsensNum"], _comfort_temp_wsensNum);
                    COPYNUM(conf["comfort"]["temp"]["sens"], _comfort_temp_sens);
                    COPYNUM(conf["comfort"]["temp"]["thing"], _comfort_temp_thing);
                    COPYNUM(conf["comfort"]["temp"]["min"][0], _comfort_temp_min);
                    COPYNUM(conf["comfort"]["temp"]["max"][0], _comfort_temp_max);
                    COPYNUM(conf["comfort"]["temp"]["min"][1], _comfort_temp_min_hysteresis);
                    COPYNUM(conf["comfort"]["temp"]["max"][1], _comfort_temp_max_hysteresis);
                    COPYNUM(conf["comfort"]["hum"]["source"], _comfort_hum_source);
                    COPYNUM(conf["comfort"]["hum"]["wsensNum"], _comfort_hum_wsensNum);
                    COPYNUM(conf["comfort"]["hum"]["thing"], _comfort_hum_thing);
                    COPYNUM(conf["comfort"]["hum"]["min"][0], _comfort_hum_min);
                    COPYNUM(conf["comfort"]["hum"]["max"][0], _comfort_hum_max);
                    COPYNUM(conf["comfort"]["hum"]["min"][1], _comfort_hum_min_hysteresis);
                    COPYNUM(conf["comfort"]["hum"]["max"][1], _comfort_hum_max_hysteresis);
                    COPYNUM(conf["comfort"]["iaq"]["source"], _comfort_iaq_source);
                    COPYNUM(conf["comfort"]["co2"]["source"], _comfort_co2_source);
                    COPYNUM(conf["comfort"]["co2"]["wsensNum"], _comfort_co2_wsensNum);
                    COPYBOOL(conf["comfort"]["temp"]["sound"], _comfort_temp_sound);
                    COPYBOOL(conf["comfort"]["hum"]["sound"], _comfort_hum_sound);
                    COPYBOOL(conf["comfort"]["iaq"]["sound"], _comfort_iaq_sound);
                    COPYBOOL(conf["comfort"]["co2"]["sound"], _comfort_co2_sound);

                    // Account
                    COPYSTR(conf["account"]["name"], _account_name);
                    COPYBOOL(conf["account"]["required"], _account_required);

                    Serial.println("done");
                }
                else Serial.println(" Configuration file corrupted");
            }
        }
        else Serial.println(" No configuration file found");

        /* Read alarm file */
        Serial.println(SEPARATOR);
        Serial.print("Read alarm file... ");
        file = LittleFS.open("/alarm.json");
        if(file) {
            while(file.available()) {
                String json = file.readString();
                JsonDocument alarms;

                DeserializationError error = deserializeJson(alarms, json);
                if(!error) {
                    // Alarm
                    for(unsigned int i=0; i<ALARMS; i++) {
                        COPYNUM(alarms["alarm"]["states"][i], _alarm_states[i]);
                        COPYNUM(alarms["alarm"]["melodies"][i], _alarm_melodies[i]);
                        for(unsigned int k=0; k<2; k++) COPYNUM(alarms["alarm"]["time"][i][k], _alarm_time[i][k]);
                        for(unsigned int k=0; k<7; k++) COPYNUM(alarms["alarm"]["weekdays"][i][k], _alarm_weekdays[i][k]);
                    }

                    Serial.println("done");
                }
                else Serial.println(" Alarm file corrupted");
            }
        }
        else Serial.println(" No alarm file found");

        /* Read user file */
        Serial.println(SEPARATOR);
        Serial.print("Read user file... ");
        file = LittleFS.open("/user.us");
        if(file) {
            while(file.available()) {
                String json = file.readString();
                JsonDocument conf;
                DeserializationError error = deserializeJson(conf, json);
                if(!error) {
                    strlcpy(_account_pass, conf["pass"] | _account_pass, sizeof(_account_pass));
                    Serial.println("done");
                }
                else Serial.println(" User file corrupted");
            }
        }
        else Serial.println(" No user file found");
    }
  
    /**
     * Getters
     */

    char* network_ssid(unsigned int num) {
        if(num >= NETWORKS) return (char*) "";
        return _network_ssid[num];
    }

    char* network_pass(unsigned int num) {
        if(num >= NETWORKS) return (char*) "";
        return _network_pass[num];
    }

    bool network_type() {
        return _network_type;
    }

    String network_ip() {
        return String(_network_ip);
    }

    String network_mask() {
        return String(_network_mask);
    }

    String network_gw() {
        return String(_network_gw);
    }

    String network_dns1() {
        return String(_network_dns1);
    }

    String network_dns2() {
        return String(_network_dns2);
    }

    char* accessPoint_ssid() {
        if(String(_accessPoint_ssid).length()) return _accessPoint_ssid;
        else return (char*) "BIM32";
    }

    char* accessPoint_pass() {
        return _accessPoint_pass;
    }

    unsigned int accessPoint_chnl() {
        if(_accessPoint_chnl < 1 or _accessPoint_chnl > 13) return 1;
        return _accessPoint_chnl;
    }

    String accessPoint_ip() {
        return String(_accessPoint_ip);
    }

    String accessPoint_mask() {
        return String(_accessPoint_mask);
    }

    String weather_appid(unsigned int num) {
        if(num >= PROVIDERS) return "";
        return String(_weather_appid[num]);
    }

    String weather_city() {
        return String(_weather_city);
    }

    String weather_cityid() {
        return String(_weather_cityid);
    }

    String weather_lon() {
        return String(_weather_lon);
    }

    String weather_lat() {
        return String(_weather_lat);
    }

    uint8_t weather_provider() {
        if(_weather_provider > 2) return 0;
        return _weather_provider;
    }

    unsigned int weather_citysearch() {
        if(_weather_citysearch > 2) return 0;
        return _weather_citysearch;
    }

    String lang() {
        if(String(_lang) == "") return "en";
        return String(_lang);
    }

    bool clock_format() {
        return _clock_format;
    }

    char* clock_ntp() {
        if(String(_clock_ntp) == "") return (char*) "time.nist.gov";
        return _clock_ntp;
    }

    int clock_utc() {
        if(_clock_utc < -12 or _clock_utc > 13) return 0;
        return _clock_utc;
    }

    bool clock_dlst() {
        return _clock_dlst;
    }

    unsigned int clock_ntp_period(){
        if(_clock_ntp_period > 90000) return 15;
        return _clock_ntp_period;
    }

    unsigned int display_type(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        return _display_type[num];
    }

    unsigned int display_model(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        return _display_model[num];
    }

    unsigned int display_animation_type(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        return _display_animation_type[num];
    }

    unsigned int display_animation_speed(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        return _display_animation_speed[num];
    }

    unsigned int display_animation_points(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        return _display_animation_points[num];
    }

    unsigned int display_dayTime(unsigned int num, bool level) {
        if(num >= DISPLAYS) return 0;
        return _get_time(level, _display_dayTime[num]); 
    }

    unsigned int display_nightTime(unsigned int num, bool level) {
        if(num >= DISPLAYS) return 0;
        return _get_time(level, _display_nightTime[num]);
    }

    unsigned int display_brightMethod(unsigned int num) {
        if(num >= DISPLAYS) return 3;
        if(_display_brightMethod[num] > 3) return 3;
        return _display_brightMethod[num];
    }

    unsigned int display_autoOff(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        if(_display_autoOff[num] > 1440) return 0; 
        return _display_autoOff[num];
    }

    bool display_nightOff(unsigned int num) {
        if(num >= DISPLAYS) return false;
        return _display_nightOff[num];
    }

    unsigned int display_nightOff_from(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        if(_display_nightOff_from[num] > 23) return 0; 
        return _display_nightOff_from[num];
    }

    unsigned int display_nightOff_to(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        if(_display_nightOff_to[num] > 23) return 0; 
        return _display_nightOff_to[num];
    }

    unsigned int display_brightness_day(unsigned int num) {
        if(num >= DISPLAYS) return 100;
        if(_display_brightness_day[num] < 1 or _display_brightness_day[num] > 100) return 100;
        return _display_brightness_day[num];
    }

    unsigned int display_brightness_night(unsigned int num) {
        if(num >= DISPLAYS) return 100;
        if(_display_brightness_night[num] < 1 or _display_brightness_night[num] > 100) return 100;
        return _display_brightness_night[num];
    }

    unsigned int display_brightness_min(unsigned int num) {
        if(num >= DISPLAYS) return 1;
        return _display_brightness_min[num];
    }

    unsigned int display_brightness_max(unsigned int num) {
        if(num >= DISPLAYS) return 255;
        return _display_brightness_max[num];
    }

    unsigned int display_lightSensor(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        if(_display_lightSensor[num] > 2) return 0;
        return _display_lightSensor[num];
    }

    unsigned int display_lightSensor_sensitivity(unsigned int num) {
        if(num >= DISPLAYS) return 50;
        if(_display_lightSensor_sensitivity[num] < 1 or _display_lightSensor_sensitivity[num] > 100) return 50;
        return _display_lightSensor_sensitivity[num];
    }

    unsigned int display_source_tempOut_sens() {
        if(_display_source_tempOut_sens > 9) return 0;
        return _display_source_tempOut_sens;
    }

    unsigned int display_source_tempOut_wsensNum() {
        if(_display_source_tempOut_wsensNum >= WSENSORS) return 0;
        return _display_source_tempOut_wsensNum;
    }

    unsigned int display_source_tempOut_temp() {
        if(_display_source_tempOut_temp >= WSENSOR_TEMPS) return 0;
        return _display_source_tempOut_temp; 
    }

    unsigned int display_source_tempOut_thing() {
        if(_display_source_tempOut_thing >= THNG_FIELDS) return 0;
        return _display_source_tempOut_thing; 
    }

    unsigned int display_source_humOut_sens() {
        if(_display_source_humOut_sens > 7) return 0;
        return _display_source_humOut_sens; 
    }

    unsigned int display_source_humOut_wsensNum() {
        if(_display_source_humOut_wsensNum >= WSENSORS) return 0;
        return _display_source_humOut_wsensNum; 
    }

    unsigned int display_source_humOut_thing() {
        if(_display_source_humOut_thing >= THNG_FIELDS) return 0;
        return _display_source_humOut_thing; 
    }

    unsigned int display_source_presOut_sens() {
        if(_display_source_presOut_sens > 6) return 0;
        return _display_source_presOut_sens;
    }

    unsigned int display_source_presOut_wsensNum() {
        if(_display_source_presOut_wsensNum >= WSENSORS) return 0;
        return _display_source_presOut_wsensNum;
    }

    unsigned int display_source_presOut_thing() {
        if(_display_source_presOut_thing >= THNG_FIELDS) return 0;
        return _display_source_presOut_thing; 
    }

    unsigned int display_source_tempIn_sens() {
        if(_display_source_tempIn_sens > 10) return 0;
        return _display_source_tempIn_sens; 
    }

    unsigned int display_source_tempIn_wsensNum() {
        if(_display_source_tempIn_wsensNum >= WSENSORS) return 0;
        return _display_source_tempIn_wsensNum;
    }

    unsigned int display_source_tempIn_temp() {
        if(_display_source_tempIn_temp >= WSENSOR_TEMPS) return 0;
        return _display_source_tempIn_temp; 
    }

    unsigned int display_source_tempIn_thing() {
        if(_display_source_tempIn_thing >= THNG_FIELDS) return 0;
        return _display_source_tempIn_thing;
    }

    unsigned int display_source_humIn_sens() {
        if(_display_source_humIn_sens > 8) return 0;
        return _display_source_humIn_sens; 
    }

    unsigned int display_source_humIn_wsensNum() {
        if(_display_source_humIn_wsensNum >= WSENSORS) return 0;
        return _display_source_humIn_wsensNum; 
    }

    unsigned int display_source_humIn_thing() {
        if(_display_source_humIn_thing >= THNG_FIELDS) return 0;
        return _display_source_humIn_thing; 
    }

    unsigned int display_source_volt_sens() {
        if(_display_source_volt_sens > 3) return 0;
        return _display_source_volt_sens; 
    }

    unsigned int display_source_volt_wsensNum() {
        if(_display_source_volt_wsensNum >= WSENSORS) return 0;
        return _display_source_volt_wsensNum;
    }

    unsigned int display_source_volt_volt() {
        if(_display_source_volt_volt > 3) return 0;
        return _display_source_volt_volt; 
    }

    unsigned int display_source_volt_thing() {
        if(_display_source_volt_thing >= THNG_FIELDS) return 0;
        return _display_source_volt_thing;
    }

    unsigned int display_source_volt_thingType() {
        return _display_source_volt_thingType;
    }

    unsigned int display_source_bat_sens() {
        if(_display_source_bat_sens > 2) return 0;
        return _display_source_bat_sens;
    }

    unsigned int display_source_bat_wsensNum() {
        if(_display_source_bat_wsensNum >= WSENSORS) return 0;
        return _display_source_bat_wsensNum; 
    }

    unsigned int display_source_bat_thing() {
        if(_display_source_bat_thing >= THNG_FIELDS) return 0;
        return _display_source_bat_thing;
    }

    unsigned int display_source_descr() {
        if(_display_source_descr > 2) return 0;
        return _display_source_descr; 
    }

    unsigned int display_source_sequence_dur() {
        return _display_source_sequence_dur;
    }

    String display_source_sequence_name(unsigned int slot) {
        if(slot >= SEQUENCES) return "";
        return String(_display_source_sequence_name[slot]);
    }

    unsigned int display_source_sequence_temp(unsigned int slot) {
        if(slot >= SEQUENCES) return 0;
        if(_display_source_sequence_temp[slot] > 10) return 0;
        return _display_source_sequence_temp[slot];
    }

    unsigned int display_source_sequence_thngtemp(unsigned int slot) {
        if(slot >= SEQUENCES) return 0;
        if(_display_source_sequence_thngtemp[slot] >= THNG_FIELDS) return 0;
        return _display_source_sequence_thngtemp[slot];
    }

    unsigned int display_source_sequence_wsenstemp(unsigned int slot, unsigned int ws) {
        if(slot >= SEQUENCES) return 0;
        if(ws > 1) return 0;
        if(ws == 0 and _display_source_sequence_wsenstemp[slot][0] >= WSENSORS) return 0;
        if(ws == 1 and _display_source_sequence_wsenstemp[slot][1] >= WSENSOR_TEMPS) return 0;
        return _display_source_sequence_wsenstemp[slot][ws];
    }

    unsigned int display_source_sequence_hum(unsigned int slot) {
        if(slot >= SEQUENCES) return 0;
        if(_display_source_sequence_hum[slot] > 7) return 0;
        return _display_source_sequence_hum[slot];
    }

    unsigned int display_source_sequence_thnghum(unsigned int slot) {
        if(slot >= SEQUENCES) return 0;
        if(_display_source_sequence_thnghum[slot] >= THNG_FIELDS) return 0;
        return _display_source_sequence_thnghum[slot];
    }

    unsigned int display_source_sequence_wsenshum(unsigned int slot) {
        if(slot >= SEQUENCES) return 0;
        if(_display_source_sequence_wsenshum[slot] >= WSENSORS) return 0;
        return _display_source_sequence_wsenshum[slot];
    }

    unsigned int display_timeSlot_period(unsigned int slot, unsigned int displayNum) {
        if(slot >= TIMESLOTS) return 0;
        if(displayNum >= DISPLAYS) return 0;
        if(_display_timeSlot_period[slot][displayNum] > 99) return 0;
        return _display_timeSlot_period[slot][displayNum];
    }

    unsigned int display_timeSlot_sensor(unsigned int slot, unsigned int displayNum) {
        if(slot >= TIMESLOTS) return 0;
        if(displayNum >= DISPLAYS) return 0;
        if(_display_timeSlot_sensor[slot][displayNum] > 11) return 0;
        return _display_timeSlot_sensor[slot][displayNum];
    }

    unsigned int display_timeSlot_data(unsigned int slot, unsigned int displayNum) {
        if(slot >= TIMESLOTS) return 0;
        if(displayNum >= DISPLAYS) return 0;
        if(_display_timeSlot_data[slot][displayNum] > 3) return 0;
        return _display_timeSlot_data[slot][displayNum];
    }

    unsigned int display_timeSlot_thing(unsigned int slot, unsigned int displayNum) {
        if(slot >= TIMESLOTS) return 0;
        if(displayNum >= DISPLAYS) return 0;
        if(_display_timeSlot_thing[slot][displayNum] > 7) return 0;
        return _display_timeSlot_thing[slot][displayNum];
    }

    String display_timeSlot_color(unsigned int slot, unsigned int displayNum) {
        if(slot >= TIMESLOTS) return "";
        if(displayNum >= DISPLAYS) return "";
        return String(_display_timeSlot_color[slot][displayNum]);
    }

    unsigned int display_timeSlot_wsensor_num(unsigned int slot, unsigned int displayNum) {
        if(slot >= TIMESLOTS) return 0;
        if(displayNum >= DISPLAYS) return 0;
        return _display_timeSlot_wsensor_num[slot][displayNum];
    }
  
    unsigned int display_timeSlot_wsensor_type(unsigned int slot, unsigned int displayNum) {
        if(slot >= TIMESLOTS) return 0;
        if(displayNum >= DISPLAYS) return 0;
        return _display_timeSlot_wsensor_type[slot][displayNum];
    }

    unsigned int sound_vol() {
        return _sound_vol;
    }
  
    unsigned int sound_eq() {
        return _sound_eq;
    }
  
    unsigned int sound_hourly() {
        return _sound_hourly;
    }
  
    unsigned int sound_hour_from() {
        return _sound_hour_from;
    }
  
    unsigned int sound_hour_to() {
        return _sound_hour_to;
    }

    float bme280_temp_corr() {
        return _bme280_temp_corr;
    }

    float bme280_hum_corr() {
        return _bme280_hum_corr;
    }

    float bme280_pres_corr() {
        return _bme280_pres_corr;
    }

    float bmp180_temp_corr() {
        return _bmp180_temp_corr;
    }

    float bmp180_pres_corr() {
        return _bmp180_pres_corr;
    }

    float sht21_temp_corr() {
        return _sht21_temp_corr;
    }

    float sht21_hum_corr() {
        return _sht21_hum_corr;
    }

    float dht22_temp_corr() {
        return _dht22_temp_corr;
    }

    float dht22_hum_corr() {
        return _dht22_hum_corr;
    }

    float ds18b20_temp_corr() {
        return _ds18b20_temp_corr;
    }

    float esp32_temp_corr() {
        return _esp32_temp_corr;
    }

    float max44009_light_corr() {
        return _max44009_light_corr;
    }

    float bh1750_light_corr() {
        return _bh1750_light_corr;
    }

    float analog_voltage_corr() {
        return _analog_voltage_corr;
    }

    float bme680_temp_corr() {
        return _bme680_temp_corr;
    }

    float bme680_hum_corr() {
        return _bme680_hum_corr;
    }

    float bme680_pres_corr() {
        return _bme680_pres_corr;
    }

    float bme680_iaq_corr() {
        return _bme680_iaq_corr;
    }

    float wsensor_temp_corr(unsigned int num, unsigned int sens) {
        if(num >= WSENSORS) return 0;
        if(sens >= WSENSOR_TEMPS) return 0;
        return _wsensor_temp_corr[num][sens];
    }

    float wsensor_hum_corr(unsigned int num) {
        if(num >= WSENSORS) return 0;
        return _wsensor_hum_corr[num];
    }

    float wsensor_pres_corr(unsigned int num) {
        if(num >= WSENSORS) return 0;
        return _wsensor_pres_corr[num];
    }

    float wsensor_light_corr(unsigned int num) {
        if(num >= WSENSORS) return 0;
        return _wsensor_light_corr[num];
    }

    float wsensor_volt_corr(unsigned int num) {
        if(num >= WSENSORS) return 0;
        return _wsensor_volt_corr[num];
    }

    float wsensor_curr_corr(unsigned int num) {
        if(num >= WSENSORS) return 0;
        return _wsensor_curr_corr[num];
    }

    float wsensor_pow_corr(unsigned int num) {
        if(num >= WSENSORS) return 0;
        return _wsensor_pow_corr[num];
    }

    float wsensor_enrg_corr(unsigned int num) {
        if(num >= WSENSORS) return 0;
        return _wsensor_enrg_corr[num];
    }

    float wsensor_freq_corr(unsigned int num) {
        if(num >= WSENSORS) return 0;
        return _wsensor_freq_corr[num];
    }

    float wsensor_co2_corr(unsigned int num) {
        if(num >= WSENSORS) return 0;
        return _wsensor_co2_corr[num];
    }

    float wsensor_bat_k(unsigned int num) {
        if(num >= WSENSORS) return 0.0;
        if(_wsensor_bat_k[num] < 10.0 or _wsensor_bat_k[num] > 250.0) return 120.0;
        return _wsensor_bat_k[num];
    };

    unsigned int wsensor_bat_type(unsigned int num) {
        if(num >= WSENSORS) return 0;
        if(_wsensor_bat_type[num] > 1) return 0;
        return _wsensor_bat_type[num];
    };

    unsigned int wsensor_channel() {
        if(_wsensor_channel < 1 or _wsensor_channel > 100) return 1;
        return _wsensor_channel;
    };

    unsigned int wsensor_expire(unsigned int num) {
        if(num >= WSENSORS) return 0;
        if(_wsensor_expire[num] < 1 or _wsensor_expire[num] > 100) return 10;
        return _wsensor_expire[num];
    }

    unsigned int history_period() {
        return _history_period;
    }
  
    String history_channelID() {
        return _history_channelID;
    }
  
    String history_wrkey() {
        return _history_wrkey;
    }
  
    String history_rdkey() {
        return _history_rdkey;
    }
  
    unsigned int history_fields(unsigned int sensType) {
        if(sensType >= 7) return 0;
        return _history_fields[sensType];
    }
  
    unsigned int history_wSensors(unsigned int sensType) {
        if(sensType >= 7) return 0;
        return _history_wSensors[sensType];
    }
  
    unsigned int history_wTypes(unsigned int sensType) {
        if(sensType >= 7) return 0;
        return _history_wTypes[sensType];
    }
  
    unsigned int history_tFields(unsigned int sensType) {
        if(sensType >= 7) return 0;
        return _history_tFields[sensType];
    }

    unsigned int thingspeakSend_period() {
        if(_thingspeakSend_period > 999) return 5;
        return _thingspeakSend_period;
    }

    String thingspeakSend_channelID() {
        return String(_thingspeakSend_channelID);
    }

    String thingspeakSend_wrkey() {
        return String(_thingspeakSend_wrkey);
    }

    String thingspeakSend_rdkey() {
        return String(_thingspeakSend_rdkey);
    }

    unsigned int thingspeakSend_fields(unsigned int num) {
        if(num >= THNG_FIELDS) return 0;
        if(_thingspeakSend_fields[num] > 12) return 0;
        return _thingspeakSend_fields[num];
    }

    unsigned int thingspeakSend_types(unsigned int num) {
        if(num >= THNG_FIELDS) return 0;
        if(_thingspeakSend_types[num] > 3) return 0;
        return _thingspeakSend_types[num];
    }

    unsigned int thingspeakSend_wsensors(unsigned int num) {
        if(num >= THNG_FIELDS) return 0;
        if(_thingspeakSend_wsensors[num] >= WSENSORS) return 0;
        return _thingspeakSend_wsensors[num];
    }

    unsigned int thingspeakSend_wtypes(unsigned int num) {
        if(num >= THNG_FIELDS) return 0;
        if(_thingspeakSend_wtypes[num] > 16) return 0;
        return _thingspeakSend_wtypes[num];
    }

    unsigned int thingspeakReceive_period() {
        if(_thingspeakReceive_period > 999) return 5;
        return _thingspeakReceive_period;
    }

    String thingspeakReceive_channelID() {
        return String(_thingspeakReceive_channelID);
    }

    String thingspeakReceive_rdkey() {
        return String(_thingspeakReceive_rdkey);
    }

    unsigned int thingspeakReceive_expire() {
        return _thingspeakReceive_expire;
    }

    unsigned int narodmonSend_period() {
        if(_narodmonSend_period > 999) return 5;
        return _narodmonSend_period;
    }

    String narodmonSend_lon() {
        return String(_narodmonSend_lon);
    }

    String narodmonSend_lat() {
        return String(_narodmonSend_lat);
    }

    String narodmonSend_name() {
        return String(_narodmonSend_name);
    }

    unsigned int narodmonSend_sensors(unsigned int num) {
        if(num >= NAROD_FIELDS) return 0;
        if(_narodmonSend_sensors[num] > 13) return 0;
        return _narodmonSend_sensors[num];
    }

    String narodmonSend_metrics(unsigned int num) {
        if(num >= NAROD_FIELDS) return "";
        return String(_narodmonSend_metrics[num]);
    }

    unsigned int narodmonSend_types(unsigned int num) {
        if(num >= NAROD_FIELDS) return 0;
        if(_narodmonSend_types[num] > 3) return 0;
        return _narodmonSend_types[num];
    }

    unsigned int narodmonSend_wsensors(unsigned int num) {
        if(num >= NAROD_FIELDS) return 0;
        if(_narodmonSend_wsensors[num] >= WSENSORS) return 0;
        return _narodmonSend_wsensors[num];
    }

    unsigned int narodmonSend_wtypes(unsigned int num) {
        if(num >= NAROD_FIELDS) return 0;
        if(_narodmonSend_wtypes[num] > 16) return 0;
        return _narodmonSend_wtypes[num];
    }

    unsigned int mqttSend_period() {
        if(_mqttSend_period > 999) return 60;
        return _mqttSend_period;
    }

    char* mqttSend_broker() {
        return _mqttSend_broker;
    }

    unsigned int mqttSend_port() {
        if(_mqttSend_port > 65535) return 1883;
        return _mqttSend_port;
    }

    char* mqttSend_user() {
        return _mqttSend_user;
    }

    char* mqttSend_pass() {
        return _mqttSend_pass;
    };

    unsigned int mqttSend_sensors(unsigned int num) {
        if(num >= MQTT_TOPICS) return 0;
        if(_mqttSend_sensors[num] > 13) return 0;
        return _mqttSend_sensors[num];
    }

    unsigned int mqttSend_types(unsigned int num) {
        if(num >= MQTT_TOPICS) return 0;
        if(_mqttSend_types[num] > 3) return 0;
        return _mqttSend_types[num];
    }

    unsigned int mqttSend_wsensors(unsigned int num) {
        if(num >= MQTT_TOPICS) return 0;
        if(_mqttSend_wsensors[num] >= WSENSORS) return 0;
        return _mqttSend_wsensors[num];
    }

    unsigned int mqttSend_wtypes(unsigned int num) {
        if(num >= MQTT_TOPICS) return 0;
        if(_mqttSend_wtypes[num] > 16) return 0;
        return _mqttSend_wtypes[num];
    }

    char* mqttSend_topics(unsigned int num) {
        if(num >= MQTT_TOPICS) return (char*) "";
        return _mqttSend_topics[num];
    }

    unsigned int comfort_temp_source() {
        return _comfort_temp_source;
    }
  
    unsigned int comfort_temp_wsensNum() {
        return _comfort_temp_wsensNum;
    }
  
    unsigned int comfort_temp_sens() {
        return _comfort_temp_sens;
    }
  
    unsigned int comfort_temp_thing() {
        return _comfort_temp_thing;
    }
  
    float comfort_temp_min() {
        return _comfort_temp_min;
    }
  
    float comfort_temp_max() {
        return _comfort_temp_max;
    }

    float comfort_temp_min_hysteresis() {
        return _comfort_temp_min_hysteresis;
    }

    float comfort_temp_max_hysteresis() {
        return _comfort_temp_max_hysteresis;
    }
  
    unsigned int comfort_hum_source() {
        return _comfort_hum_source;
    }

    unsigned int comfort_hum_wsensNum() {
        return _comfort_hum_wsensNum;
    }

    unsigned int comfort_hum_thing() {
        return _comfort_hum_thing;
    }

    float comfort_hum_min() {
        return _comfort_hum_min;
    }

    float comfort_hum_max() {
        return _comfort_hum_max;
    }

    float comfort_hum_min_hysteresis() {
        return _comfort_hum_min_hysteresis;
    }

    float comfort_hum_max_hysteresis() {
        return _comfort_hum_max_hysteresis;
    }

    unsigned int comfort_iaq_source() {
        return _comfort_iaq_source;
    }

    unsigned int comfort_co2_source() {
        return _comfort_co2_source;
    }

    unsigned int comfort_co2_wsensNum() {
        return _comfort_co2_wsensNum;
    }

    boolean comfort_temp_sound() {
        return _comfort_temp_sound;
    }
  
    boolean comfort_hum_sound() {
        return _comfort_hum_sound;
    }
 
    boolean comfort_iaq_sound() {
        return _comfort_iaq_sound;
    }
 
    boolean comfort_co2_sound() {
        return _comfort_co2_sound;
    } 
  
    String account_name() {
        return String(_account_name);
    }

    String account_pass() {
        return String(_account_pass);
    }

    bool account_required() {
        return _account_required;
    }

    unsigned int alarm_time(unsigned int alarm_num, unsigned int level) {
        if(alarm_num > ALARMS or level > 1) return 7;
        return _alarm_time[alarm_num][level];
    }
  
    unsigned int alarm_weekday(unsigned int alarm_num, unsigned int week_day) {
        if(alarm_num > ALARMS or week_day > 6) return 0;
        return _alarm_weekdays[alarm_num][week_day];
    };
  
    unsigned int alarm_state(unsigned int alarm_num) {
        if(alarm_num > ALARMS) return 0;
        return _alarm_states[alarm_num];
    }
  
    unsigned int alarm_melodie(unsigned int alarm_num) {
        if(alarm_num > ALARMS) return 0;
        return _alarm_melodies[alarm_num];
    }
  

    /* 
     * Setters
     */

    void set_brightLimit(unsigned int min, unsigned int max, unsigned int num) {
        if(num >= DISPLAYS) return;
        if(min <= 255) _display_brightness_min[num] = min;
        if(max <= 255) _display_brightness_max[num] = max;
    }

    void set_bright(int bright, unsigned int num) {
        if(num >= DISPLAYS) return;
        if(bright >= 0 and bright <= 255) {
            _display_brightness_day[num] = bright;
            _display_brightness_night[num] = bright;
        }
    }

    void set_sensitivity(int sensitivity, unsigned int num) {
        if(num >= DISPLAYS) return;
        if(sensitivity >= 1 and sensitivity <= 200) _display_lightSensor_sensitivity[num] = sensitivity;
    }

    void set_animation_type(unsigned int type, unsigned int displayNum) {
        if(type <= 9 and displayNum < DISPLAYS) _display_animation_type[displayNum] = type;
    }

    void set_animation_speed(unsigned int speed, unsigned int displayNum) {
        if(speed >= 1 and speed <= 30 and displayNum < DISPLAYS) _display_animation_speed[displayNum] = speed;
    }

    void set_animation_points(unsigned int points, unsigned int displayNum) {
        if(points <= 7 and displayNum < DISPLAYS) _display_animation_points[displayNum] = points;
    }
  
    void set_color(char color[6], unsigned int slotNum, unsigned int displayNum) {
        if(slotNum >= TIMESLOTS) return;
        if(displayNum >= DISPLAYS) return;
        _display_timeSlot_color[slotNum][displayNum][0] = '#';
        for(unsigned int i=1; i<7; i++) {
            _display_timeSlot_color[slotNum][displayNum][i] = color[i - 1];
        }
        _display_timeSlot_color[slotNum][displayNum][7] = '\0';
    }

    void set_vol(unsigned int vol) {
        _sound_vol = vol;
    }

    void set_eq(unsigned int eq) {
        _sound_eq = eq;
    }

    void set_lang(String lng) {
        lng.toCharArray(_lang, 3);
    }
};
