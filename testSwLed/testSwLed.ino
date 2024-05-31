#include <Wire.h>

/*c:\Users\yosem\OneDrive\ドキュメント\Arduino\testSwLed\src\raspberryPiPico\voltage.cpp
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

//GPIO
pinMode(PIN_GATE, OUTPUT);  //GATE
pinMode(PIN_ACCENT, OUTPUT);  //ACCENT
pinMode(PIN_SLIDE, OUTPUT);   //SLIDE

//PWM
//PWM信号とローパスフィルタを用いた簡易D/A：http://okawa-denshi.jp/blog/?th=2009072200
analogWriteFreq(10000); //fc=10kHz
analogWriteRange(256);  //解像度=8bit
pinMode(PIN_CV, OUTPUT);  //CV



}

void loop() {

}
