#include <Adafruit_PCF8574.h>

/* Example for 8 input buttons that are connected from the GPIO expander pins to ground.
 * Note the buttons must be connected with the other side of the switch to GROUND. There is
 * a built in pull-up 'resistor' on each input, but no pull-down resistor capability.
 */

Adafruit_PCF8574 pcf;

void setup() {
  while (!Serial) { delay(10); }
  Serial.begin(115200);
  Serial.println("Adafruit PCF8574 button read test");

  if (!pcf.begin(0x20, &Wire)) {
    Serial.println("Couldn't find PCF8574");
    while (1);
  }
  for (uint8_t p=0; p<8; p++) {
    pcf.pinMode(p, INPUT_PULLUP);
  }
}

void loop() {
  for (uint8_t p=0; p<8; p++) {
    if (! pcf.digitalRead(p)) {
      Serial.print("Button on GPIO #");
      Serial.print(p);
      Serial.println(" pressed!");
    }
  }
  delay(10); // a short debounce delay
}
