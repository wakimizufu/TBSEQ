#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <Wire.h>  

 #include "src/raspberryPiPico/panelManager.h"
 #include "src/raspberryPiPico/voltage.h"
 #include "src/trigger/tempo.h"
 #include "src/trigger/syncTriger.h"
 #include "src/midi/midiClock.h"
 #include "src/midi/midiReceive.h"
 #include "src/mode/sequenceMap.h"
 #include "src/mode/trackMap.h"
 #include "src/mode/modeManager.h"

/*
[Todo 2024/8/26]
<パターンプレイ/ライト>
・【仮実装済】バンクA,B,C,Dの対応
  ⇒バンクに対応したsequenceMapの改修
  ⇒FRAMへのプリセット書き込み/読込/パターン変更時の書き込みの改修
  ⇒パターンプレイ時のバンクボタンをクリック時の挙動
  ⇒パターンライト時のバンクボタンをクリック時の挙動

<Sync>
・SyncOutの出力テスト
・SyncInの入力テスト
・SyncInとテンポを同期

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
midiReceive _midiReceive;


//タイマー割り込み関連変数定義
struct repeating_timer st_timer;
bool timer_flag = false;
uint64_t debugCount = 1;
bool syncTriger_flag = false;
uint32_t waitLoopCount = 0;
uint32_t overCount = 0;

//タイマー割り込み関数
bool toggle_panelWR(struct repeating_timer *t) {
  timer_flag = true;
  //return true; // repeat? true
  return false; // repeat? true
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

//トラックマップをFRAMからロードして設定する(ロード出来なかったらプリセットを設定する)
_modeManager.presetTrack();

//ユーティリティーモードフラグをFRAMからロードして設定する(ロード出来なかったらプリセットを設定する)
_modeManager.presetUtility();

//ユーティリティーモードに切り替える
if (_panelManager.utilityMode){
  _modeManager.changeUtilityMode();
}

//タイマー割り込み/* タイマーの初期化(割込み間隔はusで指定) */
add_repeating_timer_us(-32, toggle_panelWR, NULL, &st_timer);

//シンク極性の初期設定
if( _modeManager.getSyncPolarity() == SYNC_TRIGER_POSITIVE){
  Serial.println("getSyncPolarity:SYNC_TRIGER_POSITIVE");
  _voltage.syncPolarity(SYNC_TRIGER_POSITIVE);  //シンク信号極性  false:立ち上がり
} else if ( _modeManager.getSyncPolarity() == SYNC_TRIGER_NEGATIVE){
  Serial.println("getSyncPolarity:SYNC_TRIGER_NEGATIVE");
  _voltage.syncPolarity(SYNC_TRIGER_NEGATIVE);  //シンク信号極性  true:立下り)
}

//テンポ同期ソース設定
if( _modeManager.getSyncTempo() == SYNC_TEMPO_SYNC_SIGNAL){
  Serial.println("getSyncTempo:SYNC_TEMPO_SYNC_SIGNAL");
  _midiReceive.setReceiveEnable(false);  //シンク信号同期
} else if( _modeManager.getSyncTempo() == SYNC_TEMPO_MIDI_IN){
  Serial.println("getSyncTempo:SYNC_TEMPO_MIDI_IN");
  _midiReceive.setReceiveEnable(true);  //MIDI IN同期
}

if (_midiReceive.isEnable()){
  //UART0 MIDI受信ポート
  Serial1.begin(31250);   // UART0初期化 TX:GP0 / RX:GP1
}

Serial.println("SeqBox.ino setup() finish");
}

void loop() {

  //タイマー割り込み時処理
  if(timer_flag){
    timer_flag = false;
    //gpio_put(LED_BUILTIN, !gpio_get(LED_BUILTIN)); // toggle the LED

    //タイマー割込みを解除する
    cancel_repeating_timer( &st_timer);

    debugCount++;

    if ( 0 == debugCount % 25000){
      //Serial.print("debugCount:");
      //Serial.println(debugCount);
      /*
      Serial.print("waitLoopCount:");
      Serial.print(waitLoopCount);
      Serial.print(" overCount:");
      Serial.println(overCount);
      */
    }

    if (50000<debugCount){
      debugCount=1;
    }

   
   //MIDIクロック
   //MIDI受信開始⇒タイミングクロック/スタート/ストップ受信時に実行
   if (_midiReceive.isEnable()){
      _midiReceive.receiveMidiMessage();

      //スタート
      if(_midiReceive.isStart()){
        _modeManager.setMIDIStart(true);
      //ストップ  
      } else if(_midiReceive.isStop()){
        _modeManager.setMIDIStop(true);
      }

      //タイミングクロック
      if (_midiReceive.isTimmingClock()){
        //モード:MIDIクロック時処理を実行
        _modeManager.clockCountUp(); 
      }

   } else if (!_midiReceive.isEnable()){
      _midiClock.countUp();
      if(_midiClock.getCountUp()){
        _midiClock.clear();
        _midiClock.setTempo2Threshold(_tempo.getCountThd());

        //モード:MIDIクロック時処理を実行
        _modeManager.clockCountUp();
      }
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
    
    //タイマー割り込み/* タイマーの初期化(割込み間隔はusで指定) */
    add_repeating_timer_us(-32, toggle_panelWR, NULL, &st_timer);

  //メインルーチン未実施時はコールバック呼び出し出来ない対策を実装する
  } else if (!timer_flag){

    //オーバーフロー時の跨ぎ対応は簡略して再度32us経過を待つ
    if ( waitLoopCount > time_us_32()){
      waitLoopCount = time_us_32();
    }

    //前回計測から64us以上ならコールバック呼び出しが機能不全したとみなしてtimer_flagを強制でtrueにする
    if ( 64 <= (time_us_32()-waitLoopCount)){
      timer_flag = true;
      overCount++;

      if ( 100 <= overCount){
        Serial.println("SeqBox.ino loop() Can't call callback overcount=100");
        overCount = 0;
      }
    }
  }

  //現在の32usカウンタを設定
  waitLoopCount = time_us_32();
}
