#include <Arduino.h>
#include <TimerOne.h>

#include <countTriger.h>
#include <midiClock.h>
#include <tempo.h>

void setup() {
  Timer1.initialize(32);  // 32us
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}