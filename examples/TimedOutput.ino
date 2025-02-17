/*
 * TimedOutput Library
 * By Gabriel Bernardes
 * 2025-02-17
 */

#include <Arduino.h>
#include <TimedOutput.hpp>

TimedOutput led(12);

void wait(uint32_t ms) {
  Serial.print(F("Delaying for "));
  Serial.print(ms);
  Serial.println(F("ms"));
  delay(ms);
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initializing LED..."));

  led.init();

  wait(3000);

  Serial.println(F("Manual on, 1000ms duration."));
  led.on();
  wait(1000);
  led.off();

  wait(3000);

  Serial.println(F("Single, 1000ms duration."));
  led.single(1000);

  do { led.update(); } while (!led.isOff());

  wait(3000);

  Serial.println(F("Repeat 5 times, 75% duty cycle, 1000 ms cycle duration."));
  led.repeatX(5, 0.75, 1000);

  do { led.update(); } while (!led.isOff());
  
  wait(3000);

  Serial.println(F("Repeat 3x, 1000ms cycle duration."));
  led.repeatX(3, 500);

  do { led.update(); } while (!led.isOff());

  wait(3000);

  Serial.println(F("Repeat indefinitely, 1000ms cycle duration."));
  led.repeat(1000);
  
}

void loop() {
  led.update();
}
