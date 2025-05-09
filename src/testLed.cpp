#include  <Arduino.h>


/*In the main method, we let the led of the esp dev board blink, so we can test the overall setup
    and connection to the computer while working. */

void setup() {
  pinMode(2, OUTPUT);  // Set GPIO2 as output (onboard LED on NodeMCU)
  Serial.begin(115200);  // Start serial communication
  Serial.println("LED Blink Test started");
}

void loop() {
  digitalWrite(2, HIGH);  // Turn the LED on
  Serial.println("LED ON");
  delay(1000);             // Wait for 1 second
  digitalWrite(2, LOW);   // Turn the LED off
  Serial.println("LED OFF");
  delay(1000);             // Wait for 1 second
}
