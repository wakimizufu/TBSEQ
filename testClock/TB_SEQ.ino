#include <TimerOne.h>
#include <Wire.h>
#include <MCP23017.h>
#include <I2C_eeprom.h>
#include <MIDI.h>


void setup() {
  Wire.begin(); // join I2C bus (address optional for master)
  pinMode(LED_BUILTIN, OUTPUT); // set LED pin to OUTPUT 
}

void loop() {
  // put your main code here, to run repeatedly:
}
