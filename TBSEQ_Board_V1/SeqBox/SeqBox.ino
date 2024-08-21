#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <Wire.h>  

 #include "src/raspberryPiPico/panelManager.h"
 #include "src/raspberryPiPico/voltage.h"
 #include "src/trigger/tempo.h"
 #include "src/midi/midiClock.h"
 #include "src/mode/sequenceMap.h"
 #include "src/mode/modeManager.h"

//グローバル変数定義
panelManager _panelManager(0);
voltage _voltage;
tempo _tempo(0);
midiClock _midiClock(_tempo.getCountThd(),0);
modeManager _modeManager( &_panelManager, &_voltage,0,0);


//タイマー割り込み関連変数定義
struct repeating_timer st_timer;
bool timer_flag = false;

//タイマー割り込み関数
bool toggle_panelWR(struct repeating_timer *t) {
  timer_flag = true;
  return true; // repeat? true
}



void setup() {

delay(2000);

//UART println()ポート
Serial.begin(115200);

//各種I/Oへの初期化処理を行う
_panelManager.init();
_voltage.reset();

//MIDIClock初期化
_midiClock.setTempo2Threshold(_tempo.getCountThd());

//シークエンスマップをFRAMからロードして設定する(ロード出来なかったらプリセットを設定する)
_modeManager.presetSequence();

//タイマー割り込み/* タイマーの初期化(割込み間隔はusで指定) */
add_repeating_timer_us(-32, toggle_panelWR, NULL, &st_timer);

Serial.println("SeqBox.ino setup() finish");
}

void loop() {

  //タイマー割り込み時処理
  if(timer_flag){
    timer_flag = false;

   //MIDIクロック
    _midiClock.countUp();
    if(_midiClock.getCountUp()){
      _midiClock.clear();
      _midiClock.setTempo2Threshold(_tempo.getCountThd());

      //モード:MIDIクロック時処理を実行
      _modeManager.clockCountUp();
    }

   //パネル:情報更新
    _panelManager.countUp();
    if(_panelManager.getSequenceUp()){
      _panelManager.clear();
    }

    //モード:アプリケーション実行
    _modeManager.countUp();

    //テンポ更新    
    _tempo.countUp();
    if(_tempo.getTempoUp()){
      _tempo.clear();
      _tempo.setTempo(_panelManager.getTempoADC());
    }

  }
}
