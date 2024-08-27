#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <Wire.h>  

 #include "src/raspberryPiPico/panelManager.h"
 #include "src/raspberryPiPico/voltage.h"
 #include "src/trigger/tempo.h"
 #include "src/trigger/syncTriger.h"
 #include "src/midi/midiClock.h"
 #include "src/mode/sequenceMap.h"
 #include "src/mode/modeManager.h"

/*
[Todo 2024/8/27]
<パターンプレイ/ライト>
・【仮実装済】バンクA,B,C,Dの対応
  ⇒バンクに対応したsequenceMapの改修
  ⇒FRAMへのプリセット書き込み/読込/パターン変更時の書き込みの改修
  ⇒パターンプレイ時のバンクボタンをクリック時の挙動
  ⇒パターンライト時のバンクボタンをクリック時の挙動

<Sync>
・【仮実装済】SyncOutの出力テスト
　 ⇒口述のSyncInは仕様上実装出来ないが、SyncOutは「1ステップ/1クロック」でも実装可能

・【調査済】SyncInの入力テスト
  ⇒RP2040は5Vトレランスではないので、3.3Vへ変換が必要
・【見送り】SyncInとテンポを同期
　⇒クロックはデフォルト「1ステップ/1クロック」なのでNOTE TIEを実現するには「1ステップ/2クロック」が必要
　⇒実質「1ステップ/2クロック」しか通用しないので仕様として見送り


<MIDI>
・【確認済】MIDI IN回路のテスト
　　⇒5Vが必要
・MIDIメッセージ受信実装
・システムリアルタイムメッセージとテンポを同期

<トラックプレイ/ライト>
・仕様を考え中
*/



//グローバル変数定義
panelManager _panelManager(0);
voltage _voltage;
tempo _tempo(0);
syncTriger _syncTriger(0);
midiClock _midiClock(_tempo.getCountThd(),0);
modeManager _modeManager( &_panelManager, &_voltage,0,0);


//タイマー割り込み関連変数定義
struct repeating_timer st_timer;
bool timer_flag = false;
bool syncTriger_flag = false;

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

//シンク極性の初期設定
_voltage.syncPolarity(SYNC_TRIGER_POSITIVE);
//_voltage.syncPolarity(SYNC_TRIGER_NEGATIVE);

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

    //開始ステップをチェック
    if (_modeManager.getFirstStep()){
      //Serial.println("SeqBox.ino loop() FirstStep");
      _modeManager.setFirstStep(false);
      syncTriger_flag = true;
      _voltage.syncOn();
    }

    //シンクトリガー更新
    if (syncTriger_flag) {
      _syncTriger.countUp();

      if(_syncTriger.getSyncUp()){
        _syncTriger.clear();
        syncTriger_flag = false;
        _voltage.syncReset();
      }
    }
    
  }
}
