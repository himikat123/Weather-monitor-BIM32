#define SET_HC12  25
const uint16_t PixelCount = 30;
const uint8_t PixelPin = 12;
#define colorSaturation 25
char fw[5] = "v1.1";

struct{
  char lang[5] = "ru";
  float wsens_temp_corr = 0.0; // wireless sensor temperature correction
  float wsens_hum_corr = 0.0; // wireless sensor humidity correction
  float wsens_pres_corr = 0.0; // wireless sensor pressure correction
  float wsens_light_corr = 0.0; // wireless sensor ambient light correction
  uint8_t wsens_channel = 1; // channel number of the wireless sensor
  uint8_t wexpire = 5; // expiration date of wireless sensor in minutes
  uint8_t bat_type = 1; // wireless sensor battery type, 0 = 4.5v, 1 = 3.7v
  uint16_t bat_k = 142; // wireless sensor battery voltage correction
  
  char ssid[40] = ""; // WiFi network name
  char pass[40] = ""; // WiFi network password
  bool type = false; // WiFi network type 0 = dynamic, 1 = static
  char ip[40] = ""; // WiFi IP address
  char mask[40] = ""; // WiFi Subnet mask
  char gw[40] = ""; // WiFi default gateway
  char dns1[40] = ""; // WiFi DNS1 address
  char dns2[40] = ""; // WiFi DNS2 address
  
  uint8_t brt = 0; // way to control backlight brightness 0 = auto, 1 = by light sensor, 2 = by time, 3 = constant 
  uint32_t brday = 100; // backlight brightness during the day
  uint32_t brnight = 100; // backlight brightness during the night
  uint8_t hd = 8; // day start hours
  uint8_t md = 0; // day start minutes
  uint8_t hn = 21; // night start hours
  uint8_t mn = 0; // might start minutes
  
  uint8_t temp_out = 0; // temperature source for displaying outdoor temperature
  // 0 = internet, 1 = wireless sensor, 2 = BME280, 3 = BMP180, 4 = SHT21, 5 = DHT22
  uint8_t hum_out = 0; // humidity source for outdoor humidity display
  // 0 = internet, 1 = wireless sensor, 2 = BME280, 4 = SHT21, 5 = DHT22
  uint8_t pres_out = 0; // pressure source for displaying outdoor pressure
  // 0 = internet, 1 = wireless sensor, 2 = BME280, 3 = BMP180
  uint8_t temp_in = 1; // temperature source for displaying temperature in the house
  // 0 = internet, 1 = wireless sensor, 2 = BME280, 3 = BMP180, 4 = SHT21, 5 = DHT22
  uint8_t hum_in = 1; // humidity source for displaying humidity in the house
  // 0 = internet, 1 = wireless sensor, 2 = BME280, 4 = SHT21, 5 = DHT22
  uint8_t light_in = 1; // light source for backlight brightness control
  // 1 = wireless sensor, 8 = MAX44009, 9 = BH1750
  
  int utc = 0; // Timezone
  bool daylight = false; // auto daylight saving time
  char ntp[40] = ""; // NTP server address
  char city[40] = "Moskva,RU"; // city name
  char cityid[12] = "6609999"; // City ID
  char lat[32] = "55.751"; // latitude
  char lon[32] = "37.617"; // lоngitude
  char appid[40] = ""; // APPID
  uint8_t citysearch = 0; // the way to recognize a city. 0 = by name, 1 = by ID, 2 = by coordinates
  
  uint8_t tupd = 5;       // period of sending data to the cloud, in minutes
  bool mqttsend = false;  // whether to send data over MQTT 
  bool thngsend = false;  // whether to send data to Thingspeak
  bool nrdmsend = false;  // whether to send data to Narodmon

  bool mqttrcv = false;   // whether to receive data by MQTT
  bool thngrcv = false;   // whether to receive data from Thingspeak

  uint8_t nrd_tout = 0; // outdoor temperature source to send to Narodmon. 0 = internet, 1 = wireless sensor, 2 = bme280, 3 = bmp180, 4 = sht21, 5 = dht22
  uint8_t nrd_hout = 0; // a source of outdoor humidity to send to Narodmon. 0 = internet, 1 = wireless sensor, 2 = bme280, 4 = sht21, 5 = dht22
  uint8_t nrd_pout = 0; // pressure source to send to Narodmon. 0 = internet, 1 = wireless sensor, 2 = bme280, 3 = bmp180
  uint8_t nrd_tin = 0; // home temperature source to send to Narodmon. 0 = internet, 1 = wireless sensor, 2 = bme280, 3 = bmp180, 4 = sht21, 5 = dht22
  uint8_t nrd_hin = 0; // home humidity source to send to Narodmon. 0 = internet, 1 = wireless sensor, 2 = bme280, 4 = sht21, 5 = dht22
  uint8_t nrd_lin = 0; // ambient light source to send to Narodmon. 0 = internet, 1 = wireless sensor, 8 = max44009, 9 = bh1750
  
  char mqttServer[40] = ""; // broker mqtt address 
  uint32_t mqttPort = 1883; // broker mqtt port number 
  char mqttLogin[40] = ""; // broker mqtt login
  char mqttPass[40] = ""; // broker mqtt password
  bool fq1 = false; // whether to send data to 1 topic mqtt
  bool fq2 = false; // whether to send data to 2 topic mqtt
  bool fq3 = false; // whether to send data to 3 topic mqtt
  bool fq4 = false; // whether to send data to 4 topic mqtt
  bool fq5 = false; // whether to send data to 5 topic mqtt
  bool fq6 = false; // whether to send data to 6 topic mqtt
  bool fq7 = false; // whether to send data to 7 topic mqtt
  bool fq8 = false; // whether to send data to 8 topic mqtt
  char mqttF1[40] = ""; // mqtt topic 1 name (for sending data)
  char mqttF2[40] = ""; // mqtt topic 2 name (for sending data)
  char mqttF3[40] = ""; // mqtt topic 3 name (for sending data)
  char mqttF4[40] = ""; // mqtt topic 4 name (for sending data)
  char mqttF5[40] = ""; // mqtt topic 5 name (for sending data)
  char mqttF6[40] = ""; // mqtt topic 6 name (for sending data)
  char mqttF7[40] = ""; // mqtt topic 7 name (for sending data)
  char mqttF8[40] = ""; // mqtt topic 8 name (for sending data)
  uint8_t fd1[2] = {0,0}; // 
  uint8_t fd2[2] = {0,0}; // 
  uint8_t fd3[2] = {0,0}; // 
  uint8_t fd4[2] = {0,0}; // 
  uint8_t fd5[2] = {0,0}; // 
  uint8_t fd6[2] = {0,0}; // 
  uint8_t fd7[2] = {0,0}; // 
  uint8_t fd8[2] = {0,0}; // 

  uint8_t f1[2] = {0,0}; // 
  uint8_t f2[2] = {0,0}; // 
  uint8_t f3[2] = {0,0}; // 
  uint8_t f4[2] = {0,0}; // 
  uint8_t f5[2] = {0,0}; // 
  uint8_t f6[2] = {0,0}; // 
  uint8_t f7[2] = {0,0}; // 
  uint8_t f8[2] = {0,0}; // 
  char wrkey[20] = ""; // 
  char rdkey[20] = ""; // 
  uint32_t chid = 0; // 

  uint8_t mto = 0; // 
  uint8_t mho = 0; // 
  uint8_t mpo = 0; // 
  uint8_t mti = 0; // 
  uint8_t mhi = 0; // 
  uint8_t mli = 0; // 

  uint8_t tto = 0; // 
  uint8_t tho = 0; // 
  uint8_t tpo = 0; // 
  uint8_t tti = 0; // 
  uint8_t thi = 0; // 
  uint8_t tli = 0; // 

  bool tq1 = false; // 
  bool tq2 = false; // 
  bool tq3 = false; // 
  bool tq4 = false; // 
  bool tq5 = false; // 
  bool tq6 = false; // 
  bool tq7 = false; // 
  bool tq8 = false; // 
  char mqttT1[40] = ""; // 
  char mqttT2[40] = ""; // 
  char mqttT3[40] = ""; // 
  char mqttT4[40] = ""; // 
  char mqttT5[40] = ""; // 
  char mqttT6[40] = ""; // 
  char mqttT7[40] = ""; // 
  char mqttT8[40] = ""; //

  uint8_t dp[6] = {6,2,0,0,0,0}; //extra display period
  uint8_t dt[6] = {1,1,0,0,0,0}; //extra display sensor type
  uint16_t dc[6] = {65535,65504,12710,12710,12710,12710}; //extra display color
  char ds[6][2] = {"C","D","C","C","C","C"}; //extra display data type
  
} config;

struct{
  int ant = 0;
  int old_ant = 0;
  uint8_t page = 0;
  char networks[1024];
  char wtemp_sens[20] = "";
  char whum_sens[20] = "";
  char wpres_sens[20] = "";
  char wlight_sens[20] = "";
  time_t time_wsens = 0;
  float temp_wsens = 404.0;
  float hum_wsens = 202.0;
  float pres_wsens = 4040.0;
  float light_wsens = -1.0;
  uint16_t bat_adc = 0;
  uint8_t bat_level = 0;
  float bat_voltage = -1.0;
  //uint32_t upd_sens = 1500;
  time_t w_updated = 0;
  char description[128] = "";
  float temp[10] = {404.0, 404.0, 404.0, 404.0, 404.0, 404.0, 404.0, 404.0, 404.0, 404.0};
  float old_htemp = 404.0;
  float old_otemp = 0;
  uint8_t hum[2] = {202, 202};
  uint8_t old_hhum = 202;
  uint8_t old_ohum = 0;
  uint16_t pres = 4040;
  uint16_t old_opres = 0;
  float old_w_speed = 0;
  float old_temp_d = 0;
  float old_temp_n = 0;
  float light = 0.0;
  uint16_t bright = 20;
  float temp_web = 404.0;
  uint8_t hum_web = 202;
  uint16_t pres_web = 4040;
  uint8_t icon[5] = {0, 0, 0, 0, 0};
  uint8_t old_icon = 0;
  float w_speed[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
  uint8_t w_dir[5] = {-1, -1, -1, -1, -1};
  time_t sunrise = 0;
  time_t sunset = 0;
  char country[5] = "";
  char city[128] = "";
  bool isDay = false;
  time_t date3hourly[40];
  uint8_t icon3hourly[40];
  float temp3hourly[40];
  uint16_t pres3hourly[40];
  uint8_t wspd3hourly[40];
  uint16_t wdeg3hourly[40];
  uint8_t prec3hourly[40];
  bool is_settings = false;
  bool clock_synchronized = false;
  bool net_connected = false;
  bool mqtt_sending = false;
  float mqtt_temp_out = 404.0;
  float mqtt_temp_in = 404.0;
  float mqtt_hum_out = 404.0;
  float mqtt_hum_in = 404.0;
  float mqtt_pres_out = 404.0;
  float mqtt_light_in = 404.0;
  float thng_temp_out = 404.0;
  float thng_temp_in = 404.0;
  float thng_hum_out = 404.0;
  float thng_hum_in = 404.0;
  float thng_pres_out = 404.0;
  float thng_light_in = 404.0;
  uint8_t clock_dig[4] = {10,10,10,10};
} datas;

struct{
  bool bme280_det = false;        // обнаружен BME280
  bool bmp180_det = false;        // обнаружен BMP180
  bool sht21_det = false;         // обнаружен SHT21
  bool dht22_det = false;         // обнаружен DHT22
  bool max44009_det = false;      // обнаружен MAX44009
  bool bh1750_det = false;        // обнаружен BH1750
  float bme280_temp = 404.0;      // температура BME280
  float bme280_temp_corr = 0.0;   // коррекция температуры BME280
  float bme280_hum = 202.0;       // влажность BME280
  float bme280_hum_corr = 0;      // коррекция влажности BME280
  float bme280_pres = 4040.0;     // давление BME280
  float bme280_pres_corr = 0;     // коррекция давления BME280
  float bmp180_temp = 404.0;      // температура BMP180
  float bmp180_temp_corr = 0.0;   // коррекция температуры BMP180
  float bmp180_pres = 4040.0;     // давление BMP180
  float bmp180_pres_corr = 0;     // коррекция давления BMP180
  float sht21_temp = 404.0;       // температура SHT21
  float sht21_temp_corr = 0.0;    // коррекция температуры SHT21
  float sht21_hum = 202.0;        // влажность SHT21
  float sht21_hum_corr = 0;       // коррекция влажности SHT21
  float dht22_temp = 404.0;       // температура DHT22
  float dht22_temp_corr = 0.0;    // коррекция температуры DHT22
  float dht22_hum = 202.0;        // влажность DHT22
  float dht22_hum_corr = 0;       // коррекция влажности DHT22
  float max44009_light = -1.0;    // освещенность MAX44009
  float max44009_light_corr = 0.0;// коррекция освещенности MAX44009
  float bh1750_light = -1.0;      // освещенность BH1750
  float bh1750_light_corr = 0.0;  // коррекция освещенности BH1750
} sensors;

#ifdef __cplusplus
  extern "C"{
#endif
  uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();
