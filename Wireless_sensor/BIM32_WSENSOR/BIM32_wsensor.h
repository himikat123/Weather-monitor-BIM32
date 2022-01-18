#define BAT           A2
#define SET            5
#define DONE           3
#define ONE_WIRE       7

struct{
  bool bme280 = false;
  bool sht21 = false;
  bool ds18b20 = false;
  bool max44009 = false;
} detected;

struct{
  float temp = 404.0;
  float hum = 202;
  float pres = 4040;
  float light = -1.0;
  uint16_t adc = 0;
} datas;
