/*
テストモード：スイッチ&LEDパネル
入力：マトリクススイッチ
出力：マトリクスLED
*/


//arduino ideでコンパイルする際は以下をコメント解除する
#define DEBUG_ARDUINO 0

#if defined(DEBUG_ARDUINO)
  #include <Wire.h>
#else
#endif


#include "panelManager.h"
#include "voltage.h"
#include "sequenceMap.h"

//I2Cアドレス
const char I2C_ADDR_SW  = 0x20; //MCP27017:マトリクススイッチ
const char I2C_ADDR_LED = 0x21; //MCP27017:LEDスイッチ
const char I2C_ADDR_EEPROM = 0x50; //24LC256

//グローバル変数定義
panelManager _panelManager;
sequenceMap _sequenceMap;
voltage _voltage;


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
