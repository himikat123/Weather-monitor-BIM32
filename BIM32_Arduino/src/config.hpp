#define ALARMS                 12 // Number of alarms

#define SEPARATOR "**********************************************************************"

#define FW "v6.0a"                    // Firmware version
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

class Config {
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
    // Touch calibration data
    uint16_t _calData[5] = { 0 };

    // Localization
    char _lang[3] = "en";
    uint8_t _units_pres = 0;

    // Sound
    unsigned int _sound_vol = 15; // Sound volume
    unsigned int _sound_eq = 0; // Equalizer: 0-Normal, 1-Pop, 2-Rock, 3-Jazz, 4-Classic, 5-Bass
    unsigned int _sound_hourly = 2; // Hourly signal: 0-Always ON, 1-Always OFF, 2-On from dawn to dusk, 3-Enabled by time
    char _sound_hour_from[6] = "07:00"; // The hour from which the hourly signal is turned on
    char _sound_hour_to[6] = "22:00"; // The hour from which the hourly signal is turned off

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
    float _wsensor_wind_speed_corr[WSENSORS] = {0.0, 0.0}; // Wireless sensor wind speed correction
    float _wsensor_wind_dir_corr[WSENSORS]= {0.0, 0.0}; // Wireless sensor wind direction correction
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

    struct Comfort {
        struct Iaq {
            private:
                unsigned int _source = 0;
                bool _sound = false;
                friend class Config;

            public:
                unsigned int source() const { return _source; }
                bool sound() const { return _sound; }
        };

        struct CO2 : public Iaq {
            private: unsigned int _wsensNum = 0; friend class Config;
            public: unsigned int wsensNum() const { return _wsensNum; }
        };

        struct Hum : public CO2 {
            private:
                unsigned int _thing = 0;
                float _min = 0.0;
                float _max = 0.0;
                float _minHysteresis = 0.0;
                float _maxHysteresis = 0.0;
                friend class Config;

            public:
                unsigned int thing() const { return _thing; }
                float min() const { return _min; }
                float max() const { return _max; }
                float minHysteresis() const { return _minHysteresis; }
                float maxHysteresis() const { return _maxHysteresis; }
        };

        struct Temp : public Hum {
            private: unsigned int _sens = 0; friend class Config;
            public: unsigned int sens() const  { return _sens; }
        };

        public:
            Temp temp;
            Hum hum;
            Iaq iaq;
            CO2 co2;
    }; Comfort comfort;

    struct Network {
        private:
            char _ssid[NETWORKS][33] = { "", "", "" }; // SSID list
            char _pass[NETWORKS][33] = { "", "", "" }; // Password list
            bool _type = false; // Connection type: false = dynamic IP, true = static IP
            char _ip[16] = ""; // Static IP address
            char _mask[16] = ""; // Static Subnet mask
            char _gw[16] = ""; // Static default gateway
            char _dns1[16] = ""; // Static DNS1 address
            char _dns2[16] = ""; // Static DNS2 address
            friend class Config;

        public:
            const char* ssid(unsigned int num) const {
                if(num >= NETWORKS) return (char*) "";
                return _ssid[num];
            }

            const char* pass(unsigned int num) const {
                if(num >= NETWORKS) return (char*) "";
                return _pass[num];
            }

            const bool type() const { return _type; }
            const char* ip() const { return _ip; }
            const char* mask() const { return _mask; }
            const char* gw() const { return _gw; }
            const char* dns1() const { return _dns1; }
            const char* dns2() const { return _dns2; }
    }; Network network;

    struct AccessPoint {
        private:
            char _ssid[33] = "BIM32"; // SSID
            char _pass[33] = "1234567890"; // Password
            unsigned int _chnl = 1; // WiFi channel number 1...13
            char _ip[33] = "192.168.4.1"; // IP address
            char _mask[33] = "255.255.255.0"; // Subnet mask
            friend class Config;

        public:
            const char* ssid() const { return _ssid; }
            const char* pass() const { return _pass; }
            const unsigned int chnl() const { if(_chnl < 1 or _chnl > 13) return 1; return _chnl; }
            const char* ip() const { return _ip; }
            const char* mask() const { return _mask; }
    }; AccessPoint accessPoint;

    struct Weather {
        private:
            char _appid[PROVIDERS][33] = { "", "" }; // [0] -> APPID openweathermap.org, [1] -> KEY weatherbit.io
            float _lon = 0.0; // Longitude
            float _lat = 0.0; // Latitude
            unsigned int _provider = 0; // Weather forecast provider. 0: openweathermap.org, 1: weatherbit.io, 2: open-meteo.com 
            char _city[41] = ""; // City name
            unsigned int _cityid = 0; // City ID
            unsigned int _citysearch = 0; // The way to recognize a city. 0 = by name, 1 = by ID, 2 = by coordinates
            float _tempCorr = 0; // Weather temperature correction
            float _humCorr = 0; // Weather humidity correction
            float _presCorr = 0; // Weather pressure correction
            friend class Config;

        public:
            const char* appid(unsigned int num) const {
                if(num >= PROVIDERS) return (char*) "";
                return _appid[num];
            }

            unsigned int provider() const {
                if(_provider > 2) return 0;
                return _provider;
            }

            unsigned int citysearch() const {
                if(_citysearch > 2) return 0;
                return _citysearch;
            }

            const char* city() const { return _city; }
            unsigned int cityid() const { return _cityid; }
            float lon() const { return _lon; }
            float lat() const { return _lat; }
            float tempCorr() const { return _tempCorr; }
            float humCorr() const { return _humCorr; }
            float presCorr() const { return _presCorr; }
    }; Weather weather;

    struct Clock {
        private:
            unsigned int _format = 0; // Clock format: 0 = 12 hour wo leading zero, 1 = 12 hour with leading zero, 2 = 24 hour wo leading zero, 3 = 24 hour with leading zero
            char _ntp[65] = "time.nist.gov"; // NTP server address
            int _utc = 0; // Timezone -12...13
            bool _dlst = false; // Auto daylight saving time
            unsigned int _ntpPeriod = 15; // NTP update period (minutes) 0...90000
            friend class Config;

        public:
            unsigned int format() const {
                if(_format > 3) return 0;
                return _format;
            }

            const char* ntp() const {
                if(String(_ntp) == "") return (char*) "time.nist.gov";
                return _ntp;
            }

            const int utc() const {
                if(_utc < -12 or _utc > 13) return 0;
                return _utc;
            }

            const unsigned int ntpPeriod() const {
                if(_ntpPeriod > 90000) return 15;
                return _ntpPeriod;
            }

            const bool dlst() const { return _dlst; }
    }; Clock clock;

    struct Display {
        struct Animation {
            private:
                unsigned int _type[DISPLAYS] = {0, 0}; // Display animation number 0...9
                unsigned int _speed[DISPLAYS] = {10, 10}; // Display animation speed 1...30
                unsigned int _points[DISPLAYS] = {0, 0}; // Display animation clock points 0...4
                friend class Config;

            public:
                const unsigned int type(unsigned int num) const { if(num >= DISPLAYS) return 0; return _type[num]; }
                const unsigned int speed(unsigned int num) const { if(num >= DISPLAYS) return 0; return _speed[num]; }
                const unsigned int points(unsigned int num) const { if(num >= DISPLAYS) return 0; return _points[num]; }
        };

        struct NightOff {
            private:
                bool _need[DISPLAYS] = {false, false}; // Turn off display at night
                char _from[DISPLAYS][6] = {"23:00", "23:00"}; // The hour from which the display is turned off
                char _to[DISPLAYS][6] = {"07:00", "07:00"}; // The hour from which the display is turned on
                friend class Config;

            public:
                const bool need(unsigned int num) const { if(num >= DISPLAYS) return false; return _need[num]; }
                const unsigned int from(unsigned int num, bool level) const { if(num >= DISPLAYS) return 0; return _get_time(level, _from[num]); }
                const unsigned int to(unsigned int num, bool level) const { if(num >= DISPLAYS) return 0; return _get_time(level, _to[num]); }
        };

        struct Brightness {
            private:
                unsigned int _method[DISPLAYS] = {3, 3}; // Display brightness adjustment method: 0-Auto, 1-By light sensor, 2-By time, 3-Constant
                unsigned int _day[DISPLAYS] = {50, 50}; // Day mode brightness 1...100
                unsigned int _night[DISPLAYS] = {50, 50}; // Night mode brightness 1...100
                unsigned int _min[DISPLAYS] = {1, 1}; // Minimum brightness limit 0...255
                unsigned int _max[DISPLAYS] = {255, 255}; // Maximum brightness limit 0...255
                friend class Config;

            public:
                const unsigned int method(unsigned int num) const { if(num >= DISPLAYS) return 3; if(_method[num] > 3) return 3; return _method[num]; }
                const unsigned int day(unsigned int num) const { if(num >= DISPLAYS) return 100; if(_day[num] < 1 or _day[num] > 100) return 100; return _day[num]; }
                const unsigned int night(unsigned int num) const { if(num >= DISPLAYS) return 100; if(_night[num] < 1 or _night[num] > 100) return 100; return _night[num]; }
                const unsigned int min(unsigned int num) const { if(num >= DISPLAYS) return 1; return _min[num]; }
                const unsigned int max(unsigned int num) const { if(num >= DISPLAYS) return 255; return _max[num]; }
        };

        struct LightSensor {
            private:
                unsigned int _type[DISPLAYS] = {1, 1}; // Sensor type for brightness adjust: 0-Analog input, 1-MAX44009, 2-BH1750
                unsigned int _sensitivity[DISPLAYS] = {50, 50}; // Ambient light sensor sensibility 1...100
                friend class Config;

            public:
                const unsigned int type(unsigned int num) const { if(num >= DISPLAYS) return 0; if(_type[num] > 2) return 0; return _type[num]; }
                const unsigned int sensitivity(unsigned int num) const { if(num >= DISPLAYS) return 50; if(_sensitivity[num] < 1 or _sensitivity[num] > 100) return 50; return _sensitivity[num]; }
        };

        struct Source {
            struct BaseSensor {
                private:
                    unsigned int _sens = 0;
                    unsigned int _wsensNum = 0;
                    unsigned int _thing = 0;
                    friend class Config;

                public:
                    const unsigned int sens() const { return _sens; }
                    const unsigned int wsensNum() const { return _wsensNum; }
                    const unsigned int thing() const { return _thing; }
            };

            struct TempSensor : public BaseSensor {
                private: unsigned int _temp = 0; friend class Config;
                public: const unsigned int temp() const { return _temp }
            };

            struct VoltSensor : public BaseSensor {
                private:
                    unsigned int _volt = 0;
                    unsigned int _thingType = 0;
                    friend class Config;

                public:
                    const unsigned int volt() const { return _volt; }
                    const unsigned int thingType() const { return _thingType; }
            };

            public:
                BaseSensor bat;
                BaseSensor humOut;
                BaseSensor humIn;
                BaseSensor presOut;
                TempSensor tempOut;
                TempSensor tempIn;
                VoltSensor volt;
        };

    unsigned int _type[DISPLAYS] = {0, 0}; // Display type
    unsigned int _model[DISPLAYS] = {0, 0}; // Display model
    unsigned int _order[DISPLAYS][8] = { {1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 4, 5, 6, 7, 8} }; // Digits order
    char _dayTime[DISPLAYS][6] = {"07:00", "07:00"}; // Time to switch to day mode
    char _nightTime[DISPLAYS][6] = {"21:00", "21:00"}; // Time to switch to night mode
    unsigned int _autoOff[DISPLAYS] = {0, 0}; // Display auto-off time 0...1440
    unsigned int _source_descr = 0; // Additional description data source: 0-Nothing, 1-Comfort level, 2-Sequence
    unsigned int _source_wind_speed_sens = 1; // Wind speed data source: 0-Nothing, 1-Forecast, 2-Wireless sensor, 3-Thingspeak
    unsigned int _source_wind_speed_wsensNum = 0; // Wireless sensor number for wind speed
    unsigned int _source_wind_speed_thing = 0; // Thingspeak field number for wind speed
    unsigned int _source_wind_dir_sens = 1; // Wind direction data source: 0-Nothing, 1-Forecast, 2-Wireless sensor, 3-Thingspeak
    unsigned int _source_wind_dir_wsensNum = 0; // Wireless sensor number for wind direction
    unsigned int _source_wind_dir_thing = 0; // Thingspeak field number for wind direction
    unsigned int _source_sequence_dur = 2; // Sequence data display duration (seconds)
    char _source_sequence_name[SEQUENCES][33] = {"", "", "", ""}; // Sequence data names
    unsigned int _source_sequence_temp[SEQUENCES] = {0, 0, 0, 0}; // Sequence data sources for the temperature sequence: 0-Nothing, 1-Thingspeak, 2-Wireless sensor, 3-BME280, 4-BMP180, 5-SHT21, 6-DHT22, 7-DS18B20, 8-ESP32, 9-Forecast, 10-BME680
    unsigned int _source_sequence_thngtemp[SEQUENCES] = {0, 0, 0, 0}; // Thingspeak field number for the temperature sequence
    unsigned int _source_sequence_wsenstemp[SEQUENCES][WSENSORS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Wireless sensor number and its sensor for the temperature sequence
    unsigned int _source_sequence_hum[SEQUENCES] = {0, 0, 0, 0}; // Sequence data sources for the humidity sequence: 0-Nothing, 1-Thingspeak, 2-Wireless sensor, 3-BME280, 4-SHT21, 5-DHT22, 6-Forecast, 7-BME680
    unsigned int _source_sequence_thnghum[SEQUENCES] = {0, 0, 0, 0}; // Thingspeak field number for the humidity sequence
    unsigned int _source_sequence_wsenshum[SEQUENCES] =  {0, 0, 0, 0}; // Wireless sensor number for the humidity sequence
    unsigned int _timeSlot_period[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot durations
    unsigned int _timeSlot_sensor[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot data sources: 0-Time, 1-Date, 2-BME280, 3-BMP180, 4-SHT21, 5-DHT22, 6-DS18B20, 7-ESP32, 8-Thingspeak, 9-Weather forecast, 10-Wireless sensor, 11-BME680 
    unsigned int _timeSlot_data[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot sensors types
    unsigned int _timeSlot_thing[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot thingspeak field number
    unsigned int _timeSlot_wsensor_num[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot wireless sensor number
    unsigned int _timeSlot_wsensor_type[TIMESLOTS][DISPLAYS] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // Segment display timeslot wireless sensor type 
    char _timeSlot_color[TIMESLOTS][DISPLAYS][8] = { // Segment display timeslot colors 
        {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"},
        {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}
    };
        public:
            Animation animation;
            NightOff nightOff;
            Brightness brightness;
            LightSensor lightSensor;
    }; Display display;

    struct Account {
        private:
            char _name[32] = ""; // Web interface username
            char _pass[64] = ""; // Web interface password
            bool _required = false; // Require username and password to access the web interface
            friend class Config;

        public:
            const char* name() const { return _name; }
            const char* pass() const { return _pass; }
            bool required() const { return _required; }
    }; Account account;

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
                //String json = file.readString();
                JsonDocument conf;

                DeserializationError error = deserializeJson(conf, /*json*/ file);
                if(!error) {
                    //WiFi network
                    for(unsigned int i=0; i<NETWORKS; i++) {
                        COPYSTR(conf["network"]["ssid"][i], network._ssid[i]);
                        COPYSTR(conf["network"]["pass"][i], network._pass[i]);
                    }
                    COPYBOOL(conf["network"]["type"], network._type);
                    COPYSTR(conf["network"]["ip"], network._ip);
                    COPYSTR(conf["network"]["mask"], network._mask);
                    COPYSTR(conf["network"]["gw"], network._gw);
                    COPYSTR(conf["network"]["dns1"], network._dns1);
                    COPYSTR(conf["network"]["dns2"], network._dns2);

                    // Access point
                    COPYSTR(conf["accessPoint"]["ssid"], accessPoint._ssid);
                    COPYSTR(conf["accessPoint"]["pass"], accessPoint._pass);
                    COPYNUM(conf["accessPoint"]["chnl"], accessPoint._chnl);
                    COPYSTR(conf["accessPoint"]["ip"], accessPoint._ip);
                    COPYSTR(conf["accessPoint"]["mask"], accessPoint._mask);

                    // Weather
                    for(unsigned int i=0; i<PROVIDERS; i++) COPYSTR(conf["weather"]["appid"][i], weather._appid[i]);
                    COPYSTR(conf["weather"]["city"], weather._city);
                    COPYNUM(conf["weather"]["cityid"], weather._cityid);
                    COPYNUM(conf["weather"]["lon"], weather._lon);
                    COPYNUM(conf["weather"]["lat"], weather._lat);
                    COPYNUM(conf["weather"]["provider"], weather._provider);
                    COPYNUM(conf["weather"]["citysearch"], weather._citysearch);
                    COPYNUM(conf["weather"]["corr"]["t"], weather._tempCorr);
                    COPYNUM(conf["weather"]["corr"]["h"], weather._humCorr);
                    COPYNUM(conf["weather"]["corr"]["p"], weather._presCorr);

                    // Localization 
                    COPYSTR(conf["lang"], _lang);
                    COPYNUM(conf["units"]["pres"], _units_pres);

                    // Clock
                    COPYNUM(conf["clock"]["format"], clock._format);
                    COPYSTR(conf["clock"]["ntp"], clock._ntp);
                    COPYNUM(conf["clock"]["utc"], clock._utc);
                    COPYBOOL(conf["clock"]["dlst"], clock._dlst);
                    COPYNUM(conf["clock"]["ntp_period"], clock._ntpPeriod);

                    // Display
                    for(unsigned int i=0; i<DISPLAYS; i++) {
                        COPYNUM(conf["display"]["type"][i], _display_type[i]);
                        COPYNUM(conf["display"]["model"][i], _display_model[i]);
                        for(unsigned int o=0; o<8; o++) {
                            COPYNUM(conf["display"]["order"][i][o], _display_order[i][o]);
                        }
                        COPYSTR(conf["display"]["dayTime"][i], _display_dayTime[i]);
                        COPYSTR(conf["display"]["nightTime"][i], _display_nightTime[i]);
                        COPYNUM(conf["display"]["autoOff"][i], _display_autoOff[i]);
                        COPYBOOL(conf["display"]["nightOff"]["need"][i], display.nightOff._need[i]);
                        COPYSTR(conf["display"]["nightOff"]["from"][i], display.nightOff._from[i]);
                        COPYSTR(conf["display"]["nightOff"]["to"][i], display.nightOff._to[i]);
                        COPYNUM(conf["display"]["brightMethod"][i], display.brightness._method[i]);
                        COPYNUM(conf["display"]["brightness"]["day"][i], display.brightness._day[i]);
                        COPYNUM(conf["display"]["brightness"]["night"][i], display.brightness._night[i]);
                        COPYNUM(conf["display"]["brightness"]["min"][i], display.brightness._min[i]);
                        COPYNUM(conf["display"]["brightness"]["max"][i], display.brightness._max[i]);
                        COPYNUM(conf["display"]["lightSensor"][i], display.lightSensor._type[i]);
                        COPYNUM(conf["display"]["lightSensor_sensitivity"][i], display.lightSensor._sensitivity[i]);
                        COPYNUM(conf["display"]["animation"]["type"][i], display.animation._type[i]);
                        COPYNUM(conf["display"]["animation"]["speed"][i], display.animation._speed[i]);
                        COPYNUM(conf["display"]["animation"]["points"][i], display.animation._points[i]);
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
                    COPYNUM(conf["display"]["source"]["wind"]["speed"]["sens"], _display_source_wind_speed_sens);
                    COPYNUM(conf["display"]["source"]["wind"]["speed"]["wsensNum"], _display_source_wind_speed_wsensNum);
                    COPYNUM(conf["display"]["source"]["wind"]["speed"]["thing"], _display_source_wind_speed_thing);
                    COPYNUM(conf["display"]["source"]["wind"]["dir"]["sens"], _display_source_wind_dir_sens);
                    COPYNUM(conf["display"]["source"]["wind"]["dir"]["wsensNum"], _display_source_wind_dir_wsensNum);
                    COPYNUM(conf["display"]["source"]["wind"]["dir"]["thing"], _display_source_wind_dir_thing);
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
                    COPYSTR(conf["sound"]["hour"]["from"], _sound_hour_from);
                    COPYSTR(conf["sound"]["hour"]["to"], _sound_hour_to);

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
                            COPYNUM(conf["wsensor"]["temp"][i][k], _wsensor_temp_corr[i][k]);
                        }
                        COPYNUM(conf["wsensor"]["hum"][i], _wsensor_hum_corr[i]);
                        COPYNUM(conf["wsensor"]["pres"][i], _wsensor_pres_corr[i]);
                        COPYNUM(conf["wsensor"]["wind"]["speed"], _wsensor_wind_speed_corr[i]);
                        COPYNUM(conf["wsensor"]["wind"]["dir"], _wsensor_wind_dir_corr[i]);
                        COPYNUM(conf["wsensor"]["light"][i], _wsensor_light_corr[i]);
                        COPYNUM(conf["wsensor"]["volt"][i], _wsensor_volt_corr[i]);
                        COPYNUM(conf["wsensor"]["curr"][i], _wsensor_curr_corr[i]);
                        COPYNUM(conf["wsensor"]["pow"][i], _wsensor_pow_corr[i]);
                        COPYNUM(conf["wsensor"]["enrg"][i], _wsensor_enrg_corr[i]);
                        COPYNUM(conf["wsensor"]["freq"][i], _wsensor_freq_corr[i]);
                        COPYNUM(conf["wsensor"]["co2"][i], _wsensor_co2_corr[i]);
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
                    COPYNUM(conf["comfort"]["temp"]["source"], comfort.temp._source);
                    COPYNUM(conf["comfort"]["temp"]["wsensNum"], comfort.temp._wsensNum);
                    COPYNUM(conf["comfort"]["temp"]["sens"], comfort.temp._sens);
                    COPYNUM(conf["comfort"]["temp"]["thing"], comfort.temp._thing);
                    COPYNUM(conf["comfort"]["temp"]["min"][0], comfort.temp._min);
                    COPYNUM(conf["comfort"]["temp"]["max"][0], comfort.temp._max);
                    COPYNUM(conf["comfort"]["temp"]["min"][1], comfort.temp._minHysteresis);
                    COPYNUM(conf["comfort"]["temp"]["max"][1], comfort.temp._maxHysteresis);
                    COPYBOOL(conf["comfort"]["temp"]["sound"], comfort.temp._sound);
                    COPYNUM(conf["comfort"]["hum"]["source"], comfort.hum._source);
                    COPYNUM(conf["comfort"]["hum"]["wsensNum"], comfort.hum._wsensNum);
                    COPYNUM(conf["comfort"]["hum"]["thing"], comfort.hum._thing);
                    COPYNUM(conf["comfort"]["hum"]["min"][0], comfort.hum._min);
                    COPYNUM(conf["comfort"]["hum"]["max"][0], comfort.hum._max);
                    COPYNUM(conf["comfort"]["hum"]["min"][1], comfort.hum._minHysteresis);
                    COPYNUM(conf["comfort"]["hum"]["max"][1], comfort.hum._maxHysteresis);
                    COPYBOOL(conf["comfort"]["hum"]["sound"], comfort.hum._sound);
                    COPYNUM(conf["comfort"]["iaq"]["source"], comfort.iaq._source);
                    COPYBOOL(conf["comfort"]["iaq"]["sound"], comfort.iaq._sound);
                    COPYNUM(conf["comfort"]["co2"]["source"], comfort.co2._source);
                    COPYNUM(conf["comfort"]["co2"]["wsensNum"], comfort.co2._wsensNum);
                    COPYBOOL(conf["comfort"]["co2"]["sound"], comfort.co2._sound);

                    // Account
                    COPYSTR(conf["account"]["name"], account._name);
                    COPYBOOL(conf["account"]["required"], account._required);

                    Serial.println("done");
                }
                else Serial.println(" Configuration file corrupted");
            }
        }
        else Serial.println(" No configuration file found");

        /* Read alarm file */
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
        Serial.print("Read user file... ");
        file = LittleFS.open("/user.us");
        if(file) {
            while(file.available()) {
                String json = file.readString();
                JsonDocument conf;
                DeserializationError error = deserializeJson(conf, json);
                if(!error) {
                    strlcpy(account._pass, conf["pass"] | account._pass, sizeof(account._pass));
                    Serial.println("done");
                }
                else Serial.println(" User file corrupted");
            }
        }
        else Serial.println(" No user file found");

        /* Read touch calibration file */
        Serial.print("Read touch calibration file... ");
        file = LittleFS.open("/touch.json");
        if(file) {
            while(file.available()) {
                String json = file.readString();
                JsonDocument conf;
                DeserializationError error = deserializeJson(conf, json);
                if(!error) {
                    for(uint8_t i=0; i<5; i++) {
                        COPYNUM(conf["calData"][i], _calData[i]);
                    }
                    Serial.println("done");
                }
                else Serial.println(" Touch calibration file corrupted");
            }
        }
        else Serial.println(" No touch calibration file found");
    }
  
    /**
     * Getters
     */

    uint16_t calData(uint8_t num) {
        if(num > 4) return 0;
        return _calData[num];
    }

    String lang() {
        if(String(_lang) == "") return "en";
        return String(_lang);
    }

    uint8_t units_pres() {
        return _units_pres ? 1 : 0;
    }

    unsigned int display_type(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        return _display_type[num];
    }

    unsigned int display_model(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        return _display_model[num];
    }

    unsigned int display_order(unsigned int num, unsigned int dig) {
        if(num >= DISPLAYS || dig > 7) return 0;
        return _display_order[num][dig];
    }

    unsigned int display_dayTime(unsigned int num, bool level) {
        if(num >= DISPLAYS) return 0;
        return _get_time(level, _display_dayTime[num]); 
    }

    unsigned int display_nightTime(unsigned int num, bool level) {
        if(num >= DISPLAYS) return 0;
        return _get_time(level, _display_nightTime[num]);
    }

    unsigned int display_autoOff(unsigned int num) {
        if(num >= DISPLAYS) return 0;
        if(_display_autoOff[num] > 1440) return 0; 
        return _display_autoOff[num];
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
        if(_display_source_volt_sens > 13) return 0;
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

    unsigned int display_source_wind_speed_sens() {
        if(_display_source_wind_speed_sens > 3) return 0;
        return _display_source_wind_speed_sens;
    }

    unsigned int display_source_wind_speed_wsensNum() {
        if(_display_source_wind_speed_wsensNum >= WSENSORS) return 0;
        return _display_source_wind_speed_wsensNum; 
    }

    unsigned int display_source_wind_speed_thing() {
        if(_display_source_wind_speed_thing >= THNG_FIELDS) return 0;
        return _display_source_wind_speed_thing;
    }

    unsigned int display_source_wind_dir_sens() {
        if(_display_source_wind_dir_sens > 3) return 0;
        return _display_source_wind_dir_sens;
    }

    unsigned int display_source_wind_dir_wsensNum() {
        if(_display_source_wind_dir_wsensNum >= WSENSORS) return 0;
        return _display_source_wind_dir_wsensNum; 
    }

    unsigned int display_source_wind_dir_thing() {
        if(_display_source_wind_dir_thing >= THNG_FIELDS) return 0;
        return _display_source_wind_dir_thing;
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
        if(_display_timeSlot_data[slot][displayNum] > 4) return 0;
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
  
    unsigned int sound_hour_from(bool level) {
        return _get_time(level, _sound_hour_from);
    }
  
    unsigned int sound_hour_to(bool level) {
        return _get_time(level, _sound_hour_to);
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
        if(num >= WSENSORS) return 0.0;
        if(sens >= WSENSOR_TEMPS) return 0.0;
        return _wsensor_temp_corr[num][sens];
    }

    float wsensor_hum_corr(unsigned int num) {
        if(num >= WSENSORS) return 0.0;
        return _wsensor_hum_corr[num];
    }

    float wsensor_pres_corr(unsigned int num) {
        if(num >= WSENSORS) return 0.0;
        return _wsensor_pres_corr[num];
    }

    float wsensor_wind_speed_corr(unsigned int num) {
        if(num >= WSENSORS) return 0.0;
        return _wsensor_wind_speed_corr[num];
    }

    float wsensor_wind_dir_corr(unsigned int num) {
        if(num >= WSENSORS) return 0.0;
        return _wsensor_wind_dir_corr[num];
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
        if(_thingspeakSend_types[num] > 5) return 0;
        return _thingspeakSend_types[num];
    }

    unsigned int thingspeakSend_wsensors(unsigned int num) {
        if(num >= THNG_FIELDS) return 0;
        if(_thingspeakSend_wsensors[num] >= WSENSORS) return 0;
        return _thingspeakSend_wsensors[num];
    }

    unsigned int thingspeakSend_wtypes(unsigned int num) {
        if(num >= THNG_FIELDS) return 0;
        if(_thingspeakSend_wtypes[num] > 18) return 0;
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
        if(_narodmonSend_types[num] > 13) return 0;
        return _narodmonSend_types[num];
    }

    unsigned int narodmonSend_wsensors(unsigned int num) {
        if(num >= NAROD_FIELDS) return 0;
        if(_narodmonSend_wsensors[num] >= WSENSORS) return 0;
        return _narodmonSend_wsensors[num];
    }

    unsigned int narodmonSend_wtypes(unsigned int num) {
        if(num >= NAROD_FIELDS) return 0;
        if(_narodmonSend_wtypes[num] > 18) return 0;
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
        if(_mqttSend_types[num] > 13) return 0;
        return _mqttSend_types[num];
    }

    unsigned int mqttSend_wsensors(unsigned int num) {
        if(num >= MQTT_TOPICS) return 0;
        if(_mqttSend_wsensors[num] >= WSENSORS) return 0;
        return _mqttSend_wsensors[num];
    }

    unsigned int mqttSend_wtypes(unsigned int num) {
        if(num >= MQTT_TOPICS) return 0;
        if(_mqttSend_wtypes[num] > 18) return 0;
        return _mqttSend_wtypes[num];
    }

    char* mqttSend_topics(unsigned int num) {
        if(num >= MQTT_TOPICS) return (char*) "";
        return _mqttSend_topics[num];
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

    void set_alarm_state(uint8_t alarm_num, uint8_t state) {
        if(alarm_num > ALARMS) return;
        _alarm_states[alarm_num] = state ? 1 : 0;
    }

    void set_alarm_time(uint8_t alarm_num, uint8_t level, uint8_t val) {
        if(alarm_num > ALARMS || level > 1 || val > (level == 0 ? 23 : 59)) return;
        _alarm_time[alarm_num][level] = val;
    }
  
    void set_alarm_weekday(uint8_t alarm_num, uint8_t week_day, uint8_t val) {
        if(alarm_num > ALARMS or week_day > 6) return;
        _alarm_weekdays[alarm_num][week_day] = val ? 1 : 0;
    };

    void save_alarm_file() {
        String json = "{\"alarm\":{\"time\":[";
        for(uint8_t i=0; i<ALARMS; i++) {
            json += "[" + String(_alarm_time[i][0]) + "," + String(_alarm_time[i][1]) + "]";
            if(i < ALARMS - 1) json += ",";
        }
        json += "],\"weekdays\":[";
        for(uint8_t i=0; i<ALARMS; i++) {
            json += "[";
            for(uint8_t n=0; n<7; n++) {
                json += String(_alarm_weekdays[i][n]);
                if(n < 6) json += ",";
            }
            json += "]";
            if(i < ALARMS - 1) json += ","; 
        }
        json += "],\"states\":[";
        for(uint8_t i=0; i<ALARMS; i++) {
            json += String(_alarm_states[i]);
            if(i < ALARMS - 1) json += ",";
        }
        json += "],\"melodies\":[";
        for(uint8_t i=0; i<ALARMS; i++) {
            json += String(_alarm_melodies[i]);
            if(i < ALARMS - 1) json += ",";
        }
        json += "]}}";
        File file = LittleFS.open("/alarm.json", "w");
        if(file) file.print(json);
        file.close();
    }
};