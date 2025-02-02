// Blinky program for Arduino (emulated with QEMU)
#include <Arduino.h>

void setup() {
  pinMode(13, OUTPUT);  // Set pin 13 as an output (LED)
  Serial.begin(9600);
}

void loop() {
  digitalWrite(13, HIGH);   // Turn LED on
  Serial.println("ON");
  delay(1000);               // Wait for 1 second
  digitalWrite(13, LOW);    // Turn LED off
  delay(1000);               // Wait for 1 second
  Serial.println("OFF");
}

