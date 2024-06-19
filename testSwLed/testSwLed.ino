#include "pico/stdlib.h"
#include <Wire.h>

/*c:\Users\yosem\OneDrive\ドキュメント\Arduino\testSwLed\src\raspberryPiPico\voltage.cpp
テストモード：スイッチ&LEDパネル
入力：マトリクススイッチ
出力：マトリクスLED
*/
 #include "src/raspberryPiPico/panelManager.h"
 #include "src/raspberryPiPico/voltage.h"
 #include "src/mode/sequenceMap.h"


//グローバル変数定義
panelManager _panelManager(0);
sequenceMap _sequenceMap;
voltage _voltage;
tempo _tempo(0);

//タイマー割り込み関連変数定義
struct repeating_timer st_timer;
bool timer_flag = false;

//タイマー割り込み関数
bool toggle_panelWR(void *) {
  timer_flag = true;
  return true; // repeat? true
}



void setup() {

//GPIO
/*
pinMode(LED_BUILTIN, OUTPUT); // set LED pin to OUTPUT
pinMode(PIN_GATE, OUTPUT);  //GATE
pinMode(PIN_ACCENT, OUTPUT);  //ACCENT
pinMode(PIN_SLIDE, OUTPUT);   //SLIDE
*/
gpio_init(LED_BUILTIN); // set LED pin to OUTPUT
gpio_set_dir(LED_BUILTIN,GPIO_OUT);

gpio_init(PIN_GATE);  //GATE
gpio_set_dir(PIN_GATE,GPIO_OUT);

gpio_init(PIN_ACCENT); //ACCENT
gpio_set_dir(PIN_ACCENT,GPIO_OUT);

gpio_init(PIN_SLIDE); //SLIDE
gpio_set_dir(PIN_SLIDE,GPIO_OUT);

//I2C:I2C0に GP0(SDA)とGP1(SCL)を設定します。
Wire.begin();     //I2C使用開始
Wire.setSDA(I2C_WIRE0_SDA);   //I2C0はWireオブジェクトを使用します。
Wire.setSCL(I2C_WIRE0_SCL);

//PWM
//PWM信号とローパスフィルタを用いた簡易D/A：http://okawa-denshi.jp/blog/?th=2009072200
analogWriteFreq(10000); //fc=10kHz
analogWriteRange(256);  //解像度=8bit
pinMode(PIN_CV, OUTPUT);  //CV

//タイマー割り込み/* タイマーの初期化(割込み間隔はusで指定) */
add_repeating_timer_us(1000000, toggle_panelWR, NULL, &st_timer);

//UART println()ポート
Serial.begin(115200);
Serial.println("Serial Start.");
}

void loop() {

  int note=0;
  _voltage.reset();

  //タイマー割り込み時処理
  if(timer_flag){
    timer_flag = false;
    gpio_put(LED_BUILTIN, !gpio_get(LED_BUILTIN)); // toggle the LED

    _voltage.accent(gpio_get(LED_BUILTIN));
    _voltage.gate(gpio_get(LED_BUILTIN));
    _voltage.slide(gpio_get(LED_BUILTIN));

    _voltage.cv(note);
    note++;
    if ( note >= SIZE_NOTE2PWM){
      note=0;
    }

    /*
    _tempo.countUp();
    _panelManager.countUp();

    if(_panelManager.getSequenceUp()){
      _panelManager.clear();
    }
    */

  }
}
