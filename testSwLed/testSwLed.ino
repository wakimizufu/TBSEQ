/*
テストモード：スイッチ&LEDパネル
入力：マトリクススイッチ
出力：マトリクスLED
*/


#include <Wire.h>

#include "matrixLED.h"
#include "matrixSW.h"

//I2Cアドレス
const char I2C_ADDR_SW  = 0x20; //MCP27017:マトリクススイッチ
const char I2C_ADDR_LED = 0x21; //MCP27017:LEDスイッチ
const char I2C_ADDR_EEPROM = 0x50; //24LC256

//マトリクスLED
matrixLED cMatrixLed;

//マトリクススイッチ
Switch cSwitch;

bool toggle_panelWR(void *) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle the LED
  return true; // repeat? true
}



void setup() {

  pinMode(LED_BUILTIN, OUTPUT); // set LED pin to OUTPUT
  Wire.begin(); //I2C使用開始

}

void loop() {

}
