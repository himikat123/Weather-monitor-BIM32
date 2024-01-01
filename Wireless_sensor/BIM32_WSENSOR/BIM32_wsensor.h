#define BAT           A2 // ADC battery voltage pin (PC2 -> pin #25)
#define SET            5 // HC-12 wireless module SET pin (PD5 -> pin #9)
#define DONE           3 // TPL5110 DONE pin (PD3 -> pin #1)
#define ONE_WIRE_1     2 // DS18B20 #1 bus pin (PD2 -> pin #32)
#define ONE_WIRE_2     4 // DS18B20 #2 bus pin (PD4 -> pin #2)
#define ONE_WIRE_3     6 // DS18B20 #3 bus pin (PD6 -> pin #18) 
#define ONE_WIRE_4     7 // DS18B20 #4 bus pin (PD7 -> pin #11)
#define SENSEAIR_RX    8 // SENSEAIR_S8 UART TX pin (PB0 -> pin #12)
#define SENSEAIR_TX    9 // SENSEAIR_S8 UART RX pin (PB1 -> pin #13)
#define PZEM_RX       10 // PZEM_004T UART TX pin (PB2 -> pin #14)
#define PZEM_TX       11 // PZEM_004T UART RX pin (PB3 -> pin #15)

struct{
  bool bme280 = false;
  bool sht21 = false;
  bool ds18b20_1 = false;
  bool ds18b20_2 = false;
  bool ds18b20_3 = false;
  bool ds18b20_4 = false;
  bool max44009 = false;
  bool s8 = false;
} detected;

struct{
  float temp = 404.0;
  float temp_1 = 404.0;
  float temp_2 = 404.0;
  float temp_3 = 404.0;
  float temp_4 = 404.0;
  float hum = 202.0;
  float pres = 4040.0;
  float light = -1.0;
  uint16_t adc = 0;
  float voltage = -1.0;
  float current = -1.0;
  float power = -1.0;
  float energy = -1.0;
  float frequency = -1.0;
  float pf = -1.0;
  uint16_t co2 = 0;
} datas;
