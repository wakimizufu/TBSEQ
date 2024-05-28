#include <Wire.h>

/*
テストモード：スイッチ&LEDパネル
入力：マトリクススイッチ
出力：マトリクスLED
*/


 #include <Wire.h>

 #include "src/raspberryPiPico/panelManager.h"
 #include "src/raspberryPiPico/voltage.h"
 #include "src/mode/sequenceMap.h"



//グローバル変数定義
panelManager _panelManager(0);
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
