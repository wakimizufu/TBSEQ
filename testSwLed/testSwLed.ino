#include "pico/stdlib.h"
#include <Wire.h>  


/*c:\Users\yosem\OneDrive\ドキュメント\Arduino\testSwLed\src\raspberryPiPico\voltage.cpp
テストモード：スイッチ&LEDパネル
入力：マトリクススイッチ
出力：マトリクスLED
*/
 #include "src/raspberryPiPico/panelManager.h"
 #include "src/raspberryPiPico/voltage.h"
 #include "src/trigger/tempo.h"
 #include "src/mode/sequenceMap.h"


//グローバル変数定義
panelManager _panelManager(0);
sequenceMap _sequenceMap;
voltage _voltage;
tempo _tempo(0);
int note=0;
bool bLED=false;

//タイマー割り込み関連変数定義
struct repeating_timer st_timer;
bool timer_flag = false;

//タイマー割り込み関数
bool toggle_panelWR(struct repeating_timer *t) {
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
Wire.setClock(I2C_CLOCK);
Wire.setSDA(I2C_WIRE0_SDA);   //I2C0はWireオブジェクトを使用します。
Wire.setSCL(I2C_WIRE0_SCL);

//PWM
//PWM信号とローパスフィルタを用いた簡易D/A：http://okawa-denshi.jp/blog/?th=2009072200
analogWriteFreq(10000); //fc=10kHz
analogWriteRange(256);  //解像度=8bit
pinMode(PIN_CV, OUTPUT);  //CV
analogWrite(PIN_CV,100);

//タイマー割り込み/* タイマーの初期化(割込み間隔はusで指定) */
//add_repeating_timer_us(1000000, toggle_panelWR, NULL, &st_timer);
add_repeating_timer_us(100, toggle_panelWR, NULL, &st_timer);


//UART println()ポート
Serial.begin(115200);
Serial.println("Serial Start.");

//voltage 初期化
_voltage.reset();


//MCP23017 初期化 addr:0x20 マトリクススイッチ
//⇒IODIRA(FF) 全て入力ポート
Wire.beginTransmission(I2C_ADDR_SW);
Wire.write(0x00); 
Wire.write(0x00); 
Wire.endTransmission();

//⇒IODIRB(FF) 全て出力ポート
Wire.beginTransmission(I2C_ADDR_SW);
Wire.write(0x01); 
Wire.write(0xFF); 
Wire.endTransmission();


//MCP23017 初期化 addr:0x21 LEDマトリクス
//⇒IODIRA(00) 全て出力ポート
Wire.beginTransmission(I2C_ADDR_LED);
Wire.write(0x00); 
Wire.write(0x00); 
Wire.endTransmission();

//⇒IODIRB(00) 全て出力ポート
Wire.beginTransmission(I2C_ADDR_LED);
Wire.write(0x01); 
Wire.write(0x00); 
Wire.endTransmission();

/*
//LED出力用アドレスRow選択書き込み
//⇒GPIOA(12) Aポート値 (0x01, 0x02, 0x04, 0x08)
Wire.beginTransmission(I2C_ADDR_LED);
Wire.write(0x12); 
Wire.write(0x08); 
Wire.endTransmission();

//LED出力用アドレスCol値書き込み
//⇒GPIOB(13) Bポート値 0⇒点灯 1⇒消灯
Wire.beginTransmission(I2C_ADDR_LED);
Wire.write(0x13); 
Wire.write(0x00); 
Wire.endTransmission();
*/

_panelManager.setLEDRow(0,0x01);
_panelManager.setLEDRow(1,0x86);
_panelManager.setLEDRow(2,0x0F);
_panelManager.setLEDRow(3,0x35);

}

void loop() {

  //タイマー割り込み時処理
  if(timer_flag){
    timer_flag = false;

    /*
    Serial.print("Interbal Timer. note:");
    Serial.println(note);
    */

    bLED = !gpio_get(LED_BUILTIN);
    gpio_put(LED_BUILTIN, bLED); // toggle the LED

    _voltage.accent(bLED);
    _voltage.gate(bLED);
    _voltage.slide(bLED);

    _voltage.cv(note);
    note++;
    if ( note >= SIZE_NOTE2PWM){
      note=0;
    }

    //_tempo.countUp();
    _panelManager.countUp();

    if(_panelManager.getSequenceUp()){
      _panelManager.clear();
    }
    

  }
}
