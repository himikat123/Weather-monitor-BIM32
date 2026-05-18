#include <Adafruit_PCF8575.h>

/* Example for 1 button that is connected from PCF GPIO #0 to ground, 
 * and one LED connected from power to PCF GPIO #7
 * We also have the IRQ output connected to an Interrupt input pin on the 
 * Arduino so we are not constantly polling from the PCF8575 expander
 */

Adafruit_PCF8575 pcf;

#define PCF_BUTTON  0  // on the GPIO expander!
#define PCF_LED     7  // on the GPIO expander!

#define ARDUINO_IRQ 2  // make sure this pin is possible to make IRQ input

void setup() {
  while (!Serial) { delay(10); }
  Serial.begin(115200);
  Serial.println("Adafruit PCF8575 button/led IRQ test");

  if (!pcf.begin(0x20, &Wire)) {
    Serial.println("Couldn't find PCF8575");
    while (1);
  }

  pcf.pinMode(PCF_BUTTON, INPUT_PULLUP);
  pcf.pinMode(PCF_LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // set up the interrupt pin on IRQ signal toggle
  pinMode(ARDUINO_IRQ, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ARDUINO_IRQ), button_detect, CHANGE);
}

// We use a flag to make sure we don't enter the interrupt more than once
volatile bool in_irq = false;

// called when the button is pressed!
void button_detect(void) {
  if (in_irq) return; // we are already handling an irq so don't collide!
  
  in_irq = true;
  interrupts(); // Arduino UNO seems to require that we turn on interrupts for I2C to work!
  bool val = pcf.digitalRead(PCF_BUTTON);
  pcf.digitalWrite(PCF_LED, val);
  in_irq = false;
}

void loop() {
  delay(100); // we do nothing here!
}
