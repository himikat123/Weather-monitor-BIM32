#include <Adafruit_PCF8574.h>

/* Example for 8 output LEDs that are connected from power to the GPIO expander pins
 * Note the LEDs must be connected with the CATHODES to the expander, to SINK current!
 * The PCF8574 cannot SOURCE current!
 */

Adafruit_PCF8574 pcf;

void setup() {
  while (!Serial) { delay(10); }
  Serial.begin(115200);
  Serial.println("Adafruit PCF8574 LED blink test");

  if (!pcf.begin(0x20, &Wire)) {
    Serial.println("Couldn't find PCF8574");
    while (1);
  }
  for (uint8_t p=0; p<8; p++) {
    pcf.pinMode(p, OUTPUT);
  }
}

void loop() {
  for (uint8_t p=0; p<8; p++) {
    pcf.digitalWrite(p, LOW);  // turn LED on by sinking current to ground
    delay(100);
    pcf.digitalWrite(p, HIGH); // turn LED off by turning off sinking transistor
  }
}
