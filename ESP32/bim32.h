#define SET_HC12 25
char fw[5] = "v1.0";

struct{
  char lang[5] = "ru";
  float wsens_temp_corr = 0.0; // коррекция температуры беспроводного датчика
  float wsens_hum_corr = 0.0; // коррекция влажности беспроводного датчика
  float wsens_pres_corr = 0.0; // коррекция давления беспроводного датчика
  float wsens_light_corr = 0.0; // коррекция освещенности беспроводного датчика
  uint8_t wsens_channel = 7; // номер канала связи беспроводного датчика
  uint8_t wexpire = 5; // срок годности данных беспроводного датчика в минутах
  uint8_t bat_type = 1; // тип батареи беспроводного датчика, 0=4.5в, 1=3.7в
  uint16_t bat_k = 142; // коррекция напряжения батареи беспроводного датчика
  
  char ssid[40] = ""; // имя WiFi сети
  char pass[40] = ""; // пароль WiFi сети
  bool type = false; // тип адреса WiFi сети 0=динамический, 1=статический
  char ip[40] = ""; // IP адрес WiFi сети
  char mask[40] = ""; // маска подсети WiFi сети
  char gw[40] = ""; // шлюз WiFi сети
  char dns1[40] = ""; // DNS1 адрес WiFi сети
  char dns2[40] = ""; // DNS2 адрес WiFi сети
  
  uint8_t brt = 0; // способ управления ярклстью подсветки 0=авто, 1=по датчику освещенности, 2=по времени, 3=постоянная 
  uint32_t brday = 100; // яркость подсветки днем
  uint32_t brnight = 100; // яркость подсветки ночью
  uint8_t hd = 8; // часы начала дневного режима
  uint8_t md = 0; // минуты начала дневного режима
  uint8_t hn = 21; // часы начала ночного режима
  uint8_t mn = 0; // минуты начала ночного режима
  
  uint8_t temp_out = 0; // источник температуры для отображения температуры на улице
  // 0=интернет, 1=беспроводной датчик, 2=BME280, 3=BMP180, 4=SHT21, 5=DHT22
  uint8_t hum_out = 0; // источник влажности для отображения влажности на улице
  // 0=интернет, 1=беспроводной датчик, 2=BME280, 4=SHT21, 5=DHT22
  uint8_t pres_out = 0; // источник давления для отображения давления на улице
  // 0=интернет, 1=беспроводной датчик, 2=BME280, 3=BMP180
  uint8_t temp_in = 1; // источник температуры для отображения температуры в доме
  // 0=интернет, 1=беспроводной датчик, 2=BME280, 3=BMP180, 4=SHT21, 5=DHT22
  uint8_t hum_in = 1; // источник влажности для отображения влажности в доме
  // 0=интернет, 1=беспроводной датчик, 2=BME280, 4=SHT21, 5=DHT22
  uint8_t light_in = 1; // источник освещенности для управления яркостью подсветки
  // 1=беспроводной датчик, 8=MAX44009, 9=BH1750
  
  int utc = 0; // часовой пояс
  bool daylight = false; // автопереход на летнее время
  char ntp[40] = ""; // адрес NTP сервера
  char city[40] = "Moskva,RU"; // название города
  char cityid[12] = "6609999"; // ID номер города
  char lat[32] = "55.751"; // широта
  char lon[32] = "37.617"; // долгота
  char appid[40] = ""; // APPID
  uint8_t citysearch = 0; // способ опознования города. 0=по имени, 1=по ID, 2=по координатам
  
  uint8_t tupd = 5;       // период отправки данных в облако, в минутах
  bool mqttsend = false;  // отправлять ли данные по MQTT 
  bool thngsend = false;  // отправлять ли данные на Thingspeak
  bool nrdmsend = false;  // отправлять ли данные на Narodmon

  bool mqttrcv = false;   // принимать ли данные по MQTT
  bool thngrcv = false;   // принимать ли данные с Thingspeak

  uint8_t nrd_tout = 0; // источник уличной температуры для отправки на Narodmon. 0=интернет, 1=беспроводной датчик, 2=bme280, 3=bmp180, 4=sht21, 5=dht22
  uint8_t nrd_hout = 0; // источник уличной влажности для отправки на Narodmon. 0=интернет, 1=беспроводной датчик, 2=bme280, 4=sht21, 5=dht22
  uint8_t nrd_pout = 0; // источник давления для отправки на Narodmon. 0=интернет, 1=беспроводной датчик, 2=bme280, 3=bmp180
  uint8_t nrd_tin = 0; // источник домашней температуры для отправки на Narodmon. 0=интернет, 1=беспроводной датчик, 2=bme280, 3=bmp180, 4=sht21, 5=dht22
  uint8_t nrd_hin = 0; // источник домашней влажности для отправки на Narodmon. 0=интернет, 1=беспроводной датчик, 2=bme280, 4=sht21, 5=dht22
  uint8_t nrd_lin = 0; // источник освещенности для отправки на Narodmon. 0=интернет, 1=беспроводной датчик, 8=max44009, 9=bh1750
  
  char mqttServer[40] = ""; // адрес mqtt брокера 
  uint32_t mqttPort = 1883; // номер порта mqtt брокера 
  char mqttLogin[40] = ""; // логин mqtt брокера
  char mqttPass[40] = ""; // пароль mqtt брокера
  bool fq1 = false; // отправлять ли данные в 1 топик mqtt
  bool fq2 = false; // отправлять ли данные в 2 топик mqtt
  bool fq3 = false; // отправлять ли данные в 3 топик mqtt
  bool fq4 = false; // отправлять ли данные в 4 топик mqtt
  bool fq5 = false; // отправлять ли данные в 5 топик mqtt
  bool fq6 = false; // отправлять ли данные в 6 топик mqtt
  bool fq7 = false; // отправлять ли данные в 7 топик mqtt
  bool fq8 = false; // отправлять ли данные в 8 топик mqtt
  char mqttF1[40] = ""; // имя топика 1 mqtt (для отправки данных)
  char mqttF2[40] = ""; // имя топика 2 mqtt (для отправки данных)
  char mqttF3[40] = ""; // имя топика 3 mqtt (для отправки данных)
  char mqttF4[40] = ""; // имя топика 4 mqtt (для отправки данных)
  char mqttF5[40] = ""; // имя топика 5 mqtt (для отправки данных)
  char mqttF6[40] = ""; // имя топика 6 mqtt (для отправки данных)
  char mqttF7[40] = ""; // имя топика 7 mqtt (для отправки данных)
  char mqttF8[40] = ""; // имя топика 8 mqtt (для отправки данных)
  uint8_t fd1[2] = {0,0}; //
  uint8_t fd2[2] = {0,0};
  uint8_t fd3[2] = {0,0};
  uint8_t fd4[2] = {0,0};
  uint8_t fd5[2] = {0,0};
  uint8_t fd6[2] = {0,0};
  uint8_t fd7[2] = {0,0};
  uint8_t fd8[2] = {0,0};

  uint8_t f1[2] = {0,0};
  uint8_t f2[2] = {0,0};
  uint8_t f3[2] = {0,0};
  uint8_t f4[2] = {0,0};
  uint8_t f5[2] = {0,0};
  uint8_t f6[2] = {0,0};
  uint8_t f7[2] = {0,0};
  uint8_t f8[2] = {0,0};
  char wrkey[20] = "";
  char rdkey[20] = "";
  uint32_t chid = 0;

  uint8_t mto = 0;
  uint8_t mho = 0;
  uint8_t mpo = 0;
  uint8_t mti = 0;
  uint8_t mhi = 0;
  uint8_t mli = 0;

  uint8_t tto = 0;
  uint8_t tho = 0;
  uint8_t tpo = 0;
  uint8_t tti = 0;
  uint8_t thi = 0;
  uint8_t tli = 0;

  bool tq1 = false;
  bool tq2 = false;
  bool tq3 = false;
  bool tq4 = false;
  bool tq5 = false;
  bool tq6 = false;
  bool tq7 = false;
  bool tq8 = false;
  char mqttT1[40] = "";
  char mqttT2[40] = "";
  char mqttT3[40] = "";
  char mqttT4[40] = "";
  char mqttT5[40] = "";
  char mqttT6[40] = "";
  char mqttT7[40] = "";
  char mqttT8[40] = "";
     
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
