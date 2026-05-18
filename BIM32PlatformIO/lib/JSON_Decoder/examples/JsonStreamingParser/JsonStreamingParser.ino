#include "JSON_Decoder.h"
#include "JsonListener.h"
#include "ExampleParser.h"

JSON_Decoder parser;
ExampleListener listener;

void setup() {
  Serial.begin(115200);
#ifdef ARDUINO_ARCH_ESP8266
  Serial.println(String(ESP.getFreeHeap()));
#endif
  parser.setListener(&listener);
  // put your setup code here, to run once:
  char json[] = "{\"a\":3, \"b\":{\"c\":\"d\"}}";
  for (int i = 0; i < sizeof(json); i++) {
    parser.parse(json[i]); 
  }
#ifdef ARDUINO_ARCH_ESP8266
  Serial.println(String(ESP.getFreeHeap()));
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

}
