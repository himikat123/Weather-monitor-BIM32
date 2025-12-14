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

    // Division of time into hours and minutes
    static unsigned int _get_time(bool level, const char* time) {
        if(!time || time[2] != ':') return 0;
        unsigned int hour = (time[0] - '0') * 10 + (time[1] - '0');
        unsigned int minute = (time[3] - '0') * 10 + (time[4] - '0');
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
        const char* ssid(unsigned int num) const { if(num >= NETWORKS) return (char*) ""; return _ssid[num]; }
        const char* pass(unsigned int num) const { if(num >= NETWORKS) return (char*) ""; return _pass[num]; }
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
        const char* appid(unsigned int num) const { if(num >= PROVIDERS) return (char*) ""; return _appid[num]; }
        unsigned int provider() const { if(_provider > 2) return 0; return _provider; }
        unsigned int citysearch() const { if(_citysearch > 2) return 0; return _citysearch; }
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
        unsigned int format() const { if(_format > 3) return 0; return _format; }
        const char* ntp() const { if(String(_ntp) == "") return (char*) "time.nist.gov"; return _ntp; }
        const int utc() const { if(_utc < -12 or _utc > 13) return 0; return _utc; }
        const unsigned int ntpPeriod() const { if(_ntpPeriod > 90000) return 15; return _ntpPeriod; }
        const bool dlst() const { return _dlst; }
    }; Clock clock;

    struct Display {
        struct Animation {
            private:
            unsigned int _type[DISPLAYS] = { 0 }; // Display animation number 0...9
            unsigned int _speed[DISPLAYS] = { 10, 10 }; // Display animation speed 1...30
            unsigned int _points[DISPLAYS] = { 0 }; // Display animation clock points 0...4
            friend class Config;

            public:
            const unsigned int type(unsigned int num) const { if(num >= DISPLAYS) return 0; return _type[num]; }
            const unsigned int speed(unsigned int num) const { if(num >= DISPLAYS) return 0; return _speed[num]; }
            const unsigned int points(unsigned int num) const { if(num >= DISPLAYS) return 0; return _points[num]; }
            void setType(unsigned int type, unsigned int displayNum) { if(type <= 9 and displayNum < DISPLAYS) _type[displayNum] = type; }
            void setSpeed(unsigned int speed, unsigned int displayNum) { if(speed >= 1 and speed <= 30 and displayNum < DISPLAYS) _speed[displayNum] = speed; }
            void setPoints(unsigned int points, unsigned int displayNum) { if(points <= 7 and displayNum < DISPLAYS) _points[displayNum] = points; }
        };

        struct NightOff {
            private:
            bool _need[DISPLAYS] = {false, false}; // Turn off display at night
            char _from[DISPLAYS][6] = {"23:00", "23:00"}; // The hour from which the display is turned off
            char _to[DISPLAYS][6] = {"07:00", "07:00"}; // The hour from which the display is turned on
            friend class Config;

            public:
            const bool need(unsigned int num) const { if(num >= DISPLAYS) return false; return _need[num]; }
            const unsigned int from(unsigned int num, bool level) const { if(num >= DISPLAYS) return 0; return Config::_get_time(level, _from[num]); }
            const unsigned int to(unsigned int num, bool level) const { if(num >= DISPLAYS) return 0; return Config::_get_time(level, _to[num]); }
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
            void setBrightLimit(unsigned int min, unsigned int max, unsigned int num) { if(num >= DISPLAYS) return; if(min <= 255) _min[num] = min; if(max <= 255) _max[num] = max; }
            void setBright(int bright, unsigned int num) { if(num >= DISPLAYS) return; if(bright >= 0 and bright <= 255) { _day[num] = bright; _night[num] = bright; } }
        };

        struct LightSensor {
            private:
            unsigned int _type[DISPLAYS] = {1, 1}; // Sensor type for brightness adjust: 0-Analog input, 1-MAX44009, 2-BH1750
            unsigned int _sensitivity[DISPLAYS] = {50, 50}; // Ambient light sensor sensibility 1...100
            friend class Config;

            public:
            const unsigned int type(unsigned int num) const { if(num >= DISPLAYS) return 0; if(_type[num] > 2) return 0; return _type[num]; }
            const unsigned int sensitivity(unsigned int num) const { if(num >= DISPLAYS) return 50; if(_sensitivity[num] < 1 or _sensitivity[num] > 100) return 50; return _sensitivity[num]; }
            void setSensitivity(int snstv, unsigned int num) { if(num >= DISPLAYS) return; if(snstv >= 1 and snstv <= 200) _sensitivity[num] = snstv; }
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
                const unsigned int wsensNum() const { if(_wsensNum >= WSENSORS) return 0; return _wsensNum; }
                const unsigned int thing() const { if(_thing >= THNG_FIELDS) return 0; return _thing; }
            };

            struct TempSensor : public BaseSensor {
                private: unsigned int _temp = 0; friend class Config;
                public: const unsigned int temp() const { if(_temp >= WSENSOR_TEMPS) return 0; return _temp; }
            };

            struct VoltSensor : public BaseSensor {
                private:
                unsigned int _volt = 0;
                unsigned int _thingType = 0;
                friend class Config;

                public:
                const unsigned int volt() const { if(_volt > 3) return 0; return _volt; }
                const unsigned int thingType() const { return _thingType; }
            };

            struct Wind {
                BaseSensor speed;
                BaseSensor dir;
            };

            struct Sequence {
                private:
                unsigned int _dur = 2; // Sequence data display duration (seconds)
                char _name[SEQUENCES][33] = {"", "", "", ""}; // Sequence data names
                unsigned int _temp[SEQUENCES] = { 0 }; // Sequence data sources for the temperature sequence: 0-Nothing, 1-Thingspeak, 2-Wireless sensor, 3-BME280, 4-BMP180, 5-SHT21, 6-DHT22, 7-DS18B20, 8-ESP32, 9-Forecast, 10-BME680
                unsigned int _thngtemp[SEQUENCES] = { 0 }; // Thingspeak field number for the temperature sequence
                unsigned int _wsenstemp[SEQUENCES][WSENSORS] = { 0 }; // Wireless sensor number and its sensor for the temperature sequence
                unsigned int _hum[SEQUENCES] = { 0 }; // Sequence data sources for the humidity sequence: 0-Nothing, 1-Thingspeak, 2-Wireless sensor, 3-BME280, 4-SHT21, 5-DHT22, 6-Forecast, 7-BME680
                unsigned int _thnghum[SEQUENCES] = { 0 }; // Thingspeak field number for the humidity sequence
                unsigned int _wsenshum[SEQUENCES] =  { 0 }; // Wireless sensor number for the humidity sequence
                friend class Config;

                public:
                const unsigned int dur() const { return _dur; }
                const char* name(unsigned int slot) const { if(slot >= SEQUENCES) return ""; return _name[slot]; }
                const unsigned int temp(unsigned int slot) const { if(slot >= SEQUENCES) return 0; if(_temp[slot] > 10) return 0; return _temp[slot]; }
                const unsigned int thngtemp(unsigned int slot) const { if(slot >= SEQUENCES) return 0; if(_thngtemp[slot] >= THNG_FIELDS) return 0; return _thngtemp[slot]; }
                const unsigned int wsenstemp(unsigned int slot, unsigned int ws) const { if(slot >= SEQUENCES) return 0; if(ws > 1) return 0; if(ws == 0 and _wsenstemp[slot][0] >= WSENSORS) return 0; if(ws == 1 and _wsenstemp[slot][1] >= WSENSOR_TEMPS) return 0; return _wsenstemp[slot][ws]; }
                const unsigned int hum(unsigned int slot) const { if(slot >= SEQUENCES) return 0; if(_hum[slot] > 7) return 0; return _hum[slot]; }
                const unsigned int thnghum(unsigned int slot) const { if(slot >= SEQUENCES) return 0; if(_thnghum[slot] >= THNG_FIELDS) return 0; return _thnghum[slot]; }
                const unsigned int wsenshum(unsigned int slot) const { if(slot >= SEQUENCES) return 0; if(_wsenshum[slot] >= WSENSORS) return 0; return _wsenshum[slot]; }
            };

            private:
            unsigned int _descr = 0; // Additional description data source: 0-Nothing, 1-Comfort level, 2-Sequence
            friend class Config;

            public:
            BaseSensor bat;
            BaseSensor humOut;
            BaseSensor humIn;
            BaseSensor presOut;
            TempSensor tempOut;
            TempSensor tempIn;
            VoltSensor volt;
            Wind wind;
            Sequence sequence;
            const unsigned int descr() const { if(_descr > 2) return 0; return _descr; }
        };

        struct TimeSlot {
            struct Wsensor {
                private:
                unsigned int _num[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot wireless sensor number
                unsigned int _type[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot wireless sensor type 
                friend class Config;

                public:
                const unsigned int num(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; return _num[slot][displayNum]; }
                const unsigned int type(unsigned int slot, unsigned int displayNum) { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; return _type[slot][displayNum]; }
            };

            private:
            unsigned int _period[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot durations
            unsigned int _sensor[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot data sources: 0-Time, 1-Date, 2-BME280, 3-BMP180, 4-SHT21, 5-DHT22, 6-DS18B20, 7-ESP32, 8-Thingspeak, 9-Weather forecast, 10-Wireless sensor, 11-BME680 
            unsigned int _data[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot sensors types
            unsigned int _thing[TIMESLOTS][DISPLAYS] = { 0 }; // Segment display timeslot thingspeak field number
            char _color[TIMESLOTS][DISPLAYS][8] = { // Segment display timeslot colors 
                {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"},
                {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}, {"#FFFFFF", "#FFFFFF"}
            };
            friend class Config;

            public:
            Wsensor wsensor;
            const unsigned int period(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; if(_period[slot][displayNum] > 99) return 0; return _period[slot][displayNum]; }
            const unsigned int sensor(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; if(_sensor[slot][displayNum] > 11) return 0; return _sensor[slot][displayNum]; }
            const unsigned int data(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; if(_data[slot][displayNum] > 4) return 0; return _data[slot][displayNum]; }
            const unsigned int thing(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return 0; if(displayNum >= DISPLAYS) return 0; if(_thing[slot][displayNum] > 7) return 0; return _thing[slot][displayNum]; }
            const char* color(unsigned int slot, unsigned int displayNum) const { if(slot >= TIMESLOTS) return (char*) ""; if(displayNum >= DISPLAYS) return (char*) ""; return _color[slot][displayNum]; }
            void setColor(char color[6], unsigned int slotNum, unsigned int displayNum) {
                if(slotNum >= TIMESLOTS) return;
                if(displayNum >= DISPLAYS) return;
                _color[slotNum][displayNum][0] = '#';
                for(unsigned int i=1; i<7; i++) _color[slotNum][displayNum][i] = color[i - 1];
                _color[slotNum][displayNum][7] = '\0';
            }
        };

        private:
        unsigned int _type[DISPLAYS] = { 0 }; // Display type
        unsigned int _model[DISPLAYS] = { 0 }; // Display model
        unsigned int _order[DISPLAYS][8] = { {1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 4, 5, 6, 7, 8} }; // Digits order
        char _dayTime[DISPLAYS][6] = {"07:00", "07:00"}; // Time to switch to day mode
        char _nightTime[DISPLAYS][6] = {"21:00", "21:00"}; // Time to switch to night mode
        unsigned int _autoOff[DISPLAYS] = { 0 }; // Display auto-off time 0...1440
        friend class Config;

        public:
        Animation animation;
        NightOff nightOff;
        Brightness brightness;
        LightSensor lightSensor;
        Source source;
        TimeSlot timeSlot;
        const unsigned int type(unsigned int num) const { if(num >= DISPLAYS) return 0; return _type[num]; }
        const unsigned int model(unsigned int num) const { if(num >= DISPLAYS) return 0; return _model[num]; }
        const unsigned int order(unsigned int num, unsigned int dig) const { if(num >= DISPLAYS || dig > 7) return 0; return _order[num][dig]; }
        const unsigned int dayTime(unsigned int num, bool level) const { if(num >= DISPLAYS) return 0; return Config::_get_time(level, _dayTime[num]); }
        const unsigned int nightTime(unsigned int num, bool level) const { if(num >= DISPLAYS) return 0; return Config::_get_time(level, _nightTime[num]); }
        const unsigned int autoOff(unsigned int num) const { if(num >= DISPLAYS) return 0; if(_autoOff[num] > 1440) return 0; return _autoOff[num]; }
    }; Display display;

    struct Sound {
        private:
        unsigned int _vol = 15; // Sound volume
        unsigned int _eq = 0; // Equalizer: 0-Normal, 1-Pop, 2-Rock, 3-Jazz, 4-Classic, 5-Bass
        unsigned int _hourly = 2; // Hourly signal: 0-Always ON, 1-Always OFF, 2-On from dawn to dusk, 3-Enabled by time
        char _hourFrom[6] = "07:00"; // The hour from which the hourly signal is turned on
        char _hourTo[6] = "22:00"; // The hour from which the hourly signal is turned off
        friend class Config;

        public:
        const unsigned int vol() const { return _vol; }
        const unsigned int eq() const { return _eq; }
        const unsigned int hourly() const { return _hourly; }
        const unsigned int hourFrom(bool level) const { return Config::_get_time(level, _hourFrom); }
        const unsigned int hourTo(bool level) { return Config::_get_time(level, _hourTo); }
        void setVol(unsigned int vol) { _vol = vol; }
        void setEq(unsigned int eq) { _eq = eq; }
    }; Sound sound;

    struct Sensors {
        struct Temp {
            private: float _tempCorr = 0.0; friend class Config;
            public: const float tempCorr() const { return _tempCorr; }
        };

        struct Light {
            private: float _lightCorr = 0.0; friend class Config;
            public: const float lightCorr() const { return _lightCorr; }
        };

        struct Voltage {
            private: float _voltageCorr = 0.0; friend class Config;
            public: const float voltageCorr() const { return _voltageCorr; }
        };

        struct TempHum : public Temp {
            private: float _humCorr = 0.0; friend class Config;
            public: const float humCorr() const { return _humCorr; }
        };

        struct TempPres : public Temp {
            private: float _presCorr = 0.0; friend class Config;
            public: const float presCorr() const { return _presCorr; }
        };

        struct TempHumPres : public TempHum {
            private: float _presCorr = 0.0; friend class Config;
            public: const float presCorr() const { return _presCorr; }
        };

        struct TempHumPresIaq : public TempHumPres {
            private: float _iaqCorr = 0.0; friend class Config;
            public: const float iaqCorr() const { return _iaqCorr; }
        };

        public:
        TempHumPres bme280;
        TempPres bmp180;
        TempHum sht21;
        TempHum dht22;
        Temp ds18b20;
        Temp esp32;
        Light max44009;
        Light bh1750;
        Voltage analog;
        TempHumPresIaq bme680;
    }; Sensors sensors;

    struct Wsensor {
        private:
        float _tempCorr[WSENSORS][WSENSOR_TEMPS] = { 0 }; // Wireless sensor temperature correction
        float _humCorr[WSENSORS] = { 0 }; // Wireless sensor humidity correction
        float _presCorr[WSENSORS] = { 0 }; // Wireless sensor pressure correction
        float _windSpeedCorr[WSENSORS] = { 0 }; // Wireless sensor wind speed correction
        float _windDirCorr[WSENSORS]= { 0 }; // Wireless sensor wind direction correction
        float _lightCorr[WSENSORS] = { 0 }; // Wireless sensor ambient light correction
        float _voltCorr[WSENSORS] = { 0 }; // Wireless sensor PZEM-004t voltage correction
        float _currCorr[WSENSORS] = { 0 }; // Wireless sensor PZEM-004t current correction
        float _powCorr[WSENSORS] = { 0 }; // Wireless sensor PZEM-004t power correction
        float _enrgCorr[WSENSORS] = { 0 }; // Wireless sensor PZEM-004t energy correction
        float _freqCorr[WSENSORS] = { 0 }; // Wireless sensor PZEM-004t frequency correction
        float _co2Corr[WSENSORS] = { 0 }; // Wireless sensor SenseAir S8 CO2 correction
        float _batK[WSENSORS] = { 125.0, 125.0 }; // Wireless sensor battery voltage ADC division factor
        unsigned int _batType[WSENSORS] = { 0 }; // Wireless sensor battery type: 0 - 3x Batteries (4.5V), 1 - LiIon battery (3.7V) 
        unsigned int _expire[WSENSORS] = { 10, 10 }; // Wireless sensor data expire (minutes) 1...100
        unsigned int _channel = 1; // Wireless sensors channel number 1...100
        friend class Config;

        public:
            const float tempCorr(unsigned int num, unsigned int sens) const { if(num >= WSENSORS) return 0.0; if(sens >= WSENSOR_TEMPS) return 0.0; return _tempCorr[num][sens]; }
            const float humCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _humCorr[num]; }
            const float presCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _presCorr[num]; }
            const float windSpeedCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _windSpeedCorr[num]; }
            const float windDirCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _windDirCorr[num]; }
            const float lightCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _lightCorr[num]; }
            const float voltCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _voltCorr[num]; }
            const float currCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _currCorr[num]; }
            const float powCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _powCorr[num]; }
            const float enrgCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _enrgCorr[num]; }
            const float freqCorr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _freqCorr[num]; }
            const float co2Corr(unsigned int num) const { if(num >= WSENSORS) return 0.0; return _co2Corr[num]; }
            const float batK(unsigned int num) const { if(num >= WSENSORS) return 0.0; if(_batK[num] < 10.0 or _batK[num] > 250.0) return 125.0; return _batK[num]; }
            const unsigned int batType(unsigned int num) const { if(num >= WSENSORS) return 0; if(_batType[num] > 1) return 0; return _batType[num]; }
            const unsigned int channel() const { if(_channel < 1 or _channel > 100) return 1; return _channel; }
            const unsigned int expire(unsigned int num) const { if(num >= WSENSORS) return 0; if(_expire[num] < 1 or _expire[num] > 100) return 10; return _expire[num]; }
    }; Wsensor wsensor;

    struct Cloud {
        struct ThingBase {
            private:
            unsigned int _period = 0; // Period data update (minutes) 0...999
            char _channelID[11] = ""; // channel ID
            char _rdkey[17] = ""; // Read API Key
            friend class Config;

            public:
            const unsigned int period() const { return _period; }
            const char* channelID() const { return _channelID; }
            const char* rdkey() const { return _rdkey; }
        };

        struct ThinspeakReceive : public ThingBase {
            private: unsigned int _expire = 10; friend class Config; // Thingspeak data expire (minutes) 1...100
            public: const unsigned int expire() const { return _expire; }
        };

        struct ThingspeakSend : public ThingBase {
            private:
            char _wrkey[17] = ""; // Write API Key
            unsigned int _fields[THNG_FIELDS] = { 0 }; // Data sources
            unsigned int _types[THNG_FIELDS] = { 0 };
            unsigned int _wsensors[THNG_FIELDS] = { 0 }; // Wireless sensor numbers
            unsigned int _wtypes[THNG_FIELDS] = { 0 }; // Wireless sensor data types
            friend class Config;

            public:
            const char* wrkey() const { return _wrkey; }
            const unsigned int fields(unsigned int num) const { return (num >= THNG_FIELDS) ? 0 : _fields[num]; }
            const unsigned int types(unsigned int num) const { return (num >= THNG_FIELDS) ? 0 : _types[num]; }
            const unsigned int wsensors(unsigned int num) const { return (num >= THNG_FIELDS or _wsensors[num] >= WSENSORS) ? 0 : _wsensors[num]; }
            const unsigned int wtypes(unsigned int num) const { return (num >= THNG_FIELDS) ? 0 : _wtypes[num]; }
        };

        struct NarodmonMqttBase {
            private:
            unsigned int _period = 0; // Period for data sending (minutes) 0...999
            unsigned int _sensors[NAROD_FIELDS] = { 0 }; // Data sources to send
            unsigned int _types[NAROD_FIELDS] = { 0 }; // Wired sensor data types to send
            unsigned int _wsensors[NAROD_FIELDS] = { 0 }; // Wireless sensor numbers
            unsigned int _wtypes[NAROD_FIELDS] = { 0 }; // Wireless sensor data types
            friend class Config;

            public:
            const unsigned int period() const { return (_period > 999) ? 5 : _period; }
            const unsigned int sensors(unsigned int num) const { return (num >= NAROD_FIELDS) ? 0 : _sensors[num]; }
            const unsigned int types(unsigned int num) const { return (num >= NAROD_FIELDS) ? 0 : _types[num]; }
            const unsigned int wsensors(unsigned int num) const { return (num >= NAROD_FIELDS or _wsensors[num] >= WSENSORS) ? 0 : _wsensors[num]; }
            const unsigned int wtypes(unsigned int num) const { return (num >= NAROD_FIELDS) ? 0 : _wtypes[num]; }
        };

        struct NarodmonSend : public NarodmonMqttBase {
            private:
            char _lon[11] = ""; // Longitude for Narodmon
            char _lat[11] = ""; // Latitude for Narodmon
            char _name[17] = "BIM"; // Sensor name
            char _metrics[NAROD_FIELDS][17] { 0 }; // Sensor metrics to send to Narodmon fields
            friend class Config;

            public:
            const char* lon() const { return _lon; }
            const char* lat() const { return _lat; }
            const char* name() const { return _name; }
            const char* metrics(unsigned int num) const { return (num >= NAROD_FIELDS) ? (char*) "" : _metrics[num]; }
        };

        struct MQTTSend : public NarodmonMqttBase{
            private:
            char _broker[33] = ""; // MQTT broker address
            unsigned int _port = 1883; // MQTT port
            char _user[33] = ""; // MQTT username
            char _pass[33] = ""; // MQTT password
            char _topics[12][17] = { 0 }; // Topics for sending via MQTT
            friend class Config;

            public:
            const char* broker() const { return _broker; }
            const unsigned int port() const { return (_port > 65535) ? 1883 : _port; }
            const char* user() const { return _user; }
            const char* pass() const { return _pass; }
            const char* topics(unsigned int num) const { return (num >= MQTT_TOPICS) ? (char*) "" : _topics[num]; }
        };

        public:
        ThinspeakReceive thingspeakReceive;
        ThingspeakSend thingspeakSend;
        ThingspeakSend history;
        NarodmonSend narodmonSend;
        MQTTSend mqttSend;
    }; Cloud cloud;

    struct Alarm {
        private:
        unsigned int _time[ALARMS][2] = { // Alarm time [hour, minute]
            {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}, {7, 0}
        };
        unsigned int _weekdays[ALARMS][7] = { 0 }; // Alarm days of the week [mo, tu, we, th, fr, sa, su]
        unsigned int _states[ALARMS] = { 0 }; // Alarm state (0-Off, 1-On)
        unsigned int _melodies[ALARMS] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }; // Alarm selected melody (1...20)
        friend class Config;

        public:
        const unsigned int time(unsigned int num, unsigned int level) const {return (num > ALARMS or level > 1) ? 7 : _time[num][level]; }  
        const unsigned int weekday(unsigned int num, unsigned int day) const { return (num > ALARMS or day > 6) ? 0 : _weekdays[num][day]; }
        const unsigned int state(unsigned int num) const { return (num > ALARMS) ? 0 : _states[num]; }
        const unsigned int melodie(unsigned int num) const { return (num > ALARMS) ? 0 : _melodies[num]; }
        void setState(uint8_t num, uint8_t state) { if(num > ALARMS) return; _states[num] = state ? 1 : 0; }
        void setTime(uint8_t num, uint8_t level, uint8_t val) { if(num > ALARMS or level > 1 or val > (level == 0 ? 23 : 59)) return; _time[num][level] = val; }
        void setWeekday(uint8_t num, uint8_t day, uint8_t val) { if(num > ALARMS or day > 6) return; _weekdays[num][day] = val ? 1 : 0; };
    }; Alarm alarm;

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
                        COPYNUM(conf["display"]["type"][i], display._type[i]);
                        COPYNUM(conf["display"]["model"][i], display._model[i]);
                        for(unsigned int o=0; o<8; o++) {
                            COPYNUM(conf["display"]["order"][i][o], display._order[i][o]);
                        }
                        COPYSTR(conf["display"]["dayTime"][i], display._dayTime[i]);
                        COPYSTR(conf["display"]["nightTime"][i], display._nightTime[i]);
                        COPYNUM(conf["display"]["autoOff"][i], display._autoOff[i]);
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
                            COPYNUM(conf["display"]["timeSlot"]["period"][t][i], display.timeSlot._period[t][i]);
                            COPYNUM(conf["display"]["timeSlot"]["sensor"][t][i], display.timeSlot._sensor[t][i]);
                            COPYNUM(conf["display"]["timeSlot"]["data"][t][i], display.timeSlot._data[t][i]);
                            COPYNUM(conf["display"]["timeSlot"]["thing"][t][i], display.timeSlot._thing[t][i]);
                            COPYSTR(conf["display"]["timeSlot"]["color"][t][i], display.timeSlot._color[t][i]);
                            COPYNUM(conf["display"]["timeSlot"]["wsensor"]["num"][t][i], display.timeSlot.wsensor._num[t][i]);
                            COPYNUM(conf["display"]["timeSlot"]["wsensor"]["type"][t][i], display.timeSlot.wsensor._type[t][i]); 
                        }
                    }
                    COPYNUM(conf["display"]["source"]["tempOut"]["sens"], display.source.tempOut._sens);
                    COPYNUM(conf["display"]["source"]["tempOut"]["wsensNum"], display.source.tempOut._wsensNum);
                    COPYNUM(conf["display"]["source"]["tempOut"]["temp"], display.source.tempOut._temp);
                    COPYNUM(conf["display"]["source"]["tempOut"]["thing"], display.source.tempOut._thing);
                    COPYNUM(conf["display"]["source"]["humOut"]["sens"], display.source.humOut._sens);
                    COPYNUM(conf["display"]["source"]["humOut"]["wsensNum"], display.source.humOut._wsensNum);
                    COPYNUM(conf["display"]["source"]["humOut"]["thing"], display.source.humOut._thing);
                    COPYNUM(conf["display"]["source"]["presOut"]["sens"], display.source.presOut._sens);
                    COPYNUM(conf["display"]["source"]["presOut"]["wsensNum"], display.source.presOut._wsensNum);
                    COPYNUM(conf["display"]["source"]["presOut"]["thing"], display.source.presOut._thing);
                    COPYNUM(conf["display"]["source"]["tempIn"]["sens"], display.source.tempIn._sens);
                    COPYNUM(conf["display"]["source"]["tempIn"]["wsensNum"], display.source.tempIn._wsensNum);
                    COPYNUM(conf["display"]["source"]["tempIn"]["temp"], display.source.tempIn._temp);
                    COPYNUM(conf["display"]["source"]["tempIn"]["thing"], display.source.tempIn._thing);
                    COPYNUM(conf["display"]["source"]["humIn"]["sens"], display.source.humIn._sens);
                    COPYNUM(conf["display"]["source"]["humIn"]["wsensNum"], display.source.humIn._wsensNum);
                    COPYNUM(conf["display"]["source"]["humIn"]["thing"], display.source.humIn._thing);
                    COPYNUM(conf["display"]["source"]["volt"]["sens"], display.source.volt._sens);
                    COPYNUM(conf["display"]["source"]["volt"]["wsensNum"], display.source.volt._wsensNum);
                    COPYNUM(conf["display"]["source"]["volt"]["volt"], display.source.volt._volt);
                    COPYNUM(conf["display"]["source"]["volt"]["thing"], display.source.volt._thing);
                    COPYNUM(conf["display"]["source"]["volt"]["thingType"], display.source.volt._thingType);
                    COPYNUM(conf["display"]["source"]["bat"]["sens"], display.source.bat._sens);
                    COPYNUM(conf["display"]["source"]["bat"]["wsensNum"], display.source.bat._wsensNum);
                    COPYNUM(conf["display"]["source"]["bat"]["thing"], display.source.bat._thing);
                    COPYNUM(conf["display"]["source"]["descr"], display.source._descr);
                    COPYNUM(conf["display"]["source"]["wind"]["speed"]["sens"], display.source.wind.speed._sens);
                    COPYNUM(conf["display"]["source"]["wind"]["speed"]["wsensNum"], display.source.wind.speed._wsensNum);
                    COPYNUM(conf["display"]["source"]["wind"]["speed"]["thing"], display.source.wind.speed._thing);
                    COPYNUM(conf["display"]["source"]["wind"]["dir"]["sens"], display.source.wind.dir._sens);
                    COPYNUM(conf["display"]["source"]["wind"]["dir"]["wsensNum"], display.source.wind.dir._wsensNum);
                    COPYNUM(conf["display"]["source"]["wind"]["dir"]["thing"], display.source.wind.dir._thing);
                    COPYNUM(conf["display"]["source"]["sequence"]["dur"], display.source.sequence._dur);
                    for(unsigned int i=0; i<SEQUENCES; i++) {
                        COPYSTR(conf["display"]["source"]["sequence"]["name"][i], display.source.sequence._name[i]);
                        COPYNUM(conf["display"]["source"]["sequence"]["temp"][i], display.source.sequence._temp[i]);
                        COPYNUM(conf["display"]["source"]["sequence"]["thngtemp"][i], display.source.sequence._thngtemp[i]);
                        COPYNUM(conf["display"]["source"]["sequence"]["hum"][i], display.source.sequence._hum[i]);
                        COPYNUM(conf["display"]["source"]["sequence"]["thnghum"][i], display.source.sequence._thnghum[i]);
                        COPYNUM(conf["display"]["source"]["sequence"]["wsenshum"][i], display.source.sequence._wsenshum[i]);
                        for(unsigned int k=0; k<WSENSORS; k++) {
                            COPYNUM(conf["display"]["source"]["sequence"]["wsenstemp"][i][k], display.source.sequence._wsenstemp[i][k]);
                        }
                    }

                    // Sound
                    COPYNUM(conf["sound"]["vol"], sound._vol);
                    COPYNUM(conf["sound"]["eq"], sound._eq);
                    COPYNUM(conf["sound"]["hourly"], sound._hourly);
                    COPYSTR(conf["sound"]["hour"]["from"], sound._hourFrom);
                    COPYSTR(conf["sound"]["hour"]["to"], sound._hourTo);

                    // Sensors
                    COPYNUM(conf["sensors"]["bme280"]["t"], sensors.bme280._tempCorr);
                    COPYNUM(conf["sensors"]["bme280"]["h"], sensors.bme280._humCorr);
                    COPYNUM(conf["sensors"]["bme280"]["p"], sensors.bme280._presCorr);
                    COPYNUM(conf["sensors"]["bmp180"]["t"], sensors.bmp180._tempCorr);
                    COPYNUM(conf["sensors"]["bmp180"]["p"], sensors.bmp180._presCorr);
                    COPYNUM(conf["sensors"]["sht21"]["t"], sensors.sht21._tempCorr);
                    COPYNUM(conf["sensors"]["sht21"]["h"], sensors.sht21._humCorr);
                    COPYNUM(conf["sensors"]["dht22"]["t"], sensors.dht22._tempCorr);
                    COPYNUM(conf["sensors"]["dht22"]["h"], sensors.dht22._humCorr);
                    COPYNUM(conf["sensors"]["ds18b20"]["t"], sensors.ds18b20._tempCorr);
                    COPYNUM(conf["sensors"]["esp32"]["t"], sensors.esp32._tempCorr);
                    COPYNUM(conf["sensors"]["max44009"]["l"], sensors.max44009._lightCorr);
                    COPYNUM(conf["sensors"]["bh1750"]["l"], sensors.bh1750._lightCorr);
                    COPYNUM(conf["sensors"]["analog"]["v"], sensors.analog._voltageCorr);
                    COPYNUM(conf["sensors"]["bme680"]["t"], sensors.bme680._tempCorr);
                    COPYNUM(conf["sensors"]["bme680"]["h"], sensors.bme680._humCorr);
                    COPYNUM(conf["sensors"]["bme680"]["p"], sensors.bme680._presCorr);
                    COPYNUM(conf["sensors"]["bme680"]["i"], sensors.bme680._iaqCorr);

                    // Wireless sensors
                    for(unsigned int i=0; i<WSENSORS; i++) {
                        for(unsigned int k=0; k<WSENSOR_TEMPS; k++) {
                            COPYNUM(conf["wsensor"]["temp"][i][k], wsensor._tempCorr[i][k]);
                        }
                        COPYNUM(conf["wsensor"]["hum"][i], wsensor._humCorr[i]);
                        COPYNUM(conf["wsensor"]["pres"][i], wsensor._presCorr[i]);
                        COPYNUM(conf["wsensor"]["wind"]["speed"], wsensor._windSpeedCorr[i]);
                        COPYNUM(conf["wsensor"]["wind"]["dir"], wsensor._windDirCorr[i]);
                        COPYNUM(conf["wsensor"]["light"][i], wsensor._lightCorr[i]);
                        COPYNUM(conf["wsensor"]["volt"][i], wsensor._voltCorr[i]);
                        COPYNUM(conf["wsensor"]["curr"][i], wsensor._currCorr[i]);
                        COPYNUM(conf["wsensor"]["pow"][i], wsensor._powCorr[i]);
                        COPYNUM(conf["wsensor"]["enrg"][i], wsensor._enrgCorr[i]);
                        COPYNUM(conf["wsensor"]["freq"][i], wsensor._freqCorr[i]);
                        COPYNUM(conf["wsensor"]["co2"][i], wsensor._co2Corr[i]);
                        COPYNUM(conf["wsensor"]["bat"]["k"][i], wsensor._batK[i]);
                        COPYNUM(conf["wsensor"]["bat"]["type"][i], wsensor._batType[i]);
                        COPYNUM(conf["wsensor"]["expire"][i], wsensor._expire[i]); 
                    }
                    COPYNUM(conf["wsensor"]["channel"], wsensor._channel);

                    // Weather history repository
                    COPYNUM(conf["history"]["period"], cloud.history._period);
                    COPYSTR(conf["history"]["channelID"], cloud.history._channelID);
                    COPYSTR(conf["history"]["wrkey"], cloud.history._wrkey);
                    COPYSTR(conf["history"]["rdkey"], cloud.history._rdkey);
                    for(unsigned int i=0; i<7; i++) {
                        COPYNUM(conf["history"]["fields"][i], cloud.history._fields[i]);
                        COPYNUM(conf["history"]["wSensors"][i], cloud.history._wsensors[i]);
                        COPYNUM(conf["history"]["wTypes"][i], cloud.history._wtypes[i]);
                        COPYNUM(conf["history"]["tFields"][i], cloud.history._types[i]);
                    }

                    // Thingspeak send
                    COPYNUM(conf["thingspeakSend"]["period"], cloud.thingspeakSend._period);
                    COPYSTR(conf["thingspeakSend"]["channelID"], cloud.thingspeakSend._channelID);
                    COPYSTR(conf["thingspeakSend"]["wrkey"], cloud.thingspeakSend._wrkey);
                    COPYSTR(conf["thingspeakSend"]["rdkey"], cloud.thingspeakSend._rdkey);
                    for(unsigned int i=0; i<THNG_FIELDS; i++) {
                        COPYNUM(conf["thingspeakSend"]["fields"][i], cloud.thingspeakSend._fields[i]);
                        COPYNUM(conf["thingspeakSend"]["types"][i], cloud.thingspeakSend._types[i]);
                        COPYNUM(conf["thingspeakSend"]["wsensors"][i], cloud.thingspeakSend._wsensors[i]);
                        COPYNUM(conf["thingspeakSend"]["wtypes"][i], cloud.thingspeakSend._wtypes[i]);
                    }

                    // Thingspeak receive
                    COPYNUM(conf["thingspeakReceive"]["period"], cloud.thingspeakReceive._period);
                    COPYSTR(conf["thingspeakReceive"]["channelID"], cloud.thingspeakReceive._channelID);
                    COPYSTR(conf["thingspeakReceive"]["rdkey"], cloud.thingspeakReceive._rdkey);
                    COPYNUM(conf["thingspeakReceive"]["expire"], cloud.thingspeakReceive._expire);

                    // Narodmon send
                    COPYNUM(conf["narodmonSend"]["period"], cloud.narodmonSend._period);
                    COPYSTR(conf["narodmonSend"]["lon"], cloud.narodmonSend._lon);
                    COPYSTR(conf["narodmonSend"]["lat"], cloud.narodmonSend._lat);
                    COPYSTR(conf["narodmonSend"]["name"], cloud.narodmonSend._name);
                    for(unsigned int i=0; i<NAROD_FIELDS; i++) {
                        COPYNUM(conf["narodmonSend"]["sensors"][i], cloud.narodmonSend._sensors[i]);
                        COPYSTR(conf["narodmonSend"]["metrics"][i], cloud.narodmonSend._metrics[i]);
                        COPYNUM(conf["narodmonSend"]["types"][i], cloud.narodmonSend._types[i]);
                        COPYNUM(conf["narodmonSend"]["wsensors"][i], cloud.narodmonSend._wsensors[i]);
                        COPYNUM(conf["narodmonSend"]["wtypes"][i], cloud.narodmonSend._wtypes[i]);
                    }

                    // MQTT send
                    COPYNUM(conf["mqttSend"]["period"], cloud.mqttSend._period);
                    COPYSTR(conf["mqttSend"]["broker"], cloud.mqttSend._broker);
                    COPYNUM(conf["mqttSend"]["port"], cloud.mqttSend._port);
                    COPYSTR(conf["mqttSend"]["user"], cloud.mqttSend._user);
                    COPYSTR(conf["mqttSend"]["pass"], cloud.mqttSend._pass);
                    for(unsigned int i=0; i<MQTT_TOPICS; i++) {
                        COPYNUM(conf["mqttSend"]["sensors"][i], cloud.mqttSend._sensors[i]);
                        COPYNUM(conf["mqttSend"]["types"][i], cloud.mqttSend._types[i]);
                        COPYNUM(conf["mqttSend"]["wsensors"][i], cloud.mqttSend._wsensors[i]);
                        COPYNUM(conf["mqttSend"]["wtypes"][i], cloud.mqttSend._wtypes[i]);
                        COPYSTR(conf["mqttSend"]["topics"][i], cloud.mqttSend._topics[i]);
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
                        COPYNUM(alarms["alarm"]["states"][i], alarm._states[i]);
                        COPYNUM(alarms["alarm"]["melodies"][i], alarm._melodies[i]);
                        for(unsigned int k=0; k<2; k++) COPYNUM(alarms["alarm"]["time"][i][k], alarm._time[i][k]);
                        for(unsigned int k=0; k<7; k++) COPYNUM(alarms["alarm"]["weekdays"][i][k], alarm._weekdays[i][k]);
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

    /* 
     * Setters
     */

    void set_lang(String lng) {
        lng.toCharArray(_lang, 3);
    }

    void saveAlarmFile() {
        String json = "{\"alarm\":{\"time\":[";
        for(uint8_t i=0; i<ALARMS; i++) {
            json += "[" + String(alarm._time[i][0]) + "," + String(alarm._time[i][1]) + "]";
            if(i < ALARMS - 1) json += ",";
        }
        json += "],\"weekdays\":[";
        for(uint8_t i=0; i<ALARMS; i++) {
            json += "[";
            for(uint8_t n=0; n<7; n++) {
                json += String(alarm._weekdays[i][n]);
                if(n < 6) json += ",";
            }
            json += "]";
            if(i < ALARMS - 1) json += ","; 
        }
        json += "],\"states\":[";
        for(uint8_t i=0; i<ALARMS; i++) {
            json += String(alarm._states[i]);
            if(i < ALARMS - 1) json += ",";
        }
        json += "],\"melodies\":[";
        for(uint8_t i=0; i<ALARMS; i++) {
            json += String(alarm._melodies[i]);
            if(i < ALARMS - 1) json += ",";
        }
        json += "]}}";
        File file = LittleFS.open("/alarm.json", "w");
        if(file) file.print(json);
        file.close();
    }
};