#include "debugMode.h"

debugMode::debugMode(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) :mode(MODE_NAME::DEBUG, ptPanelManager, ptVoltage, ptSequenceMap) {

	//各状態を初期値に変更する
	_run_stop = RUN_STOP::STOP;		//ラン/ストップフラグ
    _midiClock=1;	//MIDIクロック数
    _Step=1;		//現在ステップ
	_tempoLed=false;//テンポ用LED

    for ( int i=0 ; i<SW_INDEX_MAX ; i++){
		_currentSwtich[i]=false;
	}

  	//<LEDマトリクス>
    //全部消灯
	ptPanelManager->setLEDRow(LED_ROW_0,0x0000);
	ptPanelManager->setLEDRow(LED_ROW_1,0x0000);

    //<C::voltage>
    //reset()
    //<LED gate/acc/slide>
    //全部消灯
	ptVoltage->reset();
}

/*
[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
*/
void debugMode::runSequence() {

	int i;
	bool keyOn = false;

	//現状入力情報を取得
	//ボタン押下中変数と比較
	for ( i=0 ; i<SW_INDEX_MAX ; i++){
		if ( _panelManager->getSwitch(i) != _currentSwtich[i] ){

			Serial.print("debugMode::runSequence() change index:");
			Serial.print(i);
			Serial.print(" _currentSwtich[i]:");
			Serial.print(_currentSwtich[i]);
			Serial.print(" _panelManager->getSwitch(i):");
			Serial.print(_panelManager->getSwitch(i));
			Serial.println("");

			_currentSwtich[i] = _panelManager->getSwitch(i);

			//<LEDマトリクス>
			//ボタン押下中変数の内容で表示を更新する
			_panelManager->setLED(i,_currentSwtich[i]);


		}

		keyOn = keyOn || _currentSwtich[i];
	}

	/*
    <LED slide>
    ボタン押下中変数で1個以上押下あり
　　⇒点灯
    ボタン押下中変数で押下なし
　　⇒消灯
	*/
	_voltage->slide(keyOn);


	/*
	<PWM note>
	・C1~C2ボタンが押下中ならnoteにCVを出力する
	C1:2V ~ C2:3V
	・C1~C2ボタン＆UPボタンが押下中なら以下のCV
	C1:3V ~ C2:4V
	・C1~C2ボタン＆DOWNボタンが押下中なら以下のCV
	C1:1V ~ C2:2V
	⇒複数押下されていれば一番最高音を出力する
	*/

	//各ノートボタンを高音優先で押下状態を取得する
    bool note_on = false;
	for (i=static_cast<int>(Switch::C2) ; i>=static_cast<int>(Switch::C) ; i--){
		if (_currentSwtich[i]) {
			note_on=true;
			break;
		}
	}

	//各ノートボタンが押されていたらボタンに応じたCVを設定する
    int note_CV = static_cast<int>(NOTE_PWM_INDEX::NOTE_C2);
	if ( note_on ){
		if ( _currentSwtich[static_cast<int>(Switch::UP)] ) {
			note_CV=static_cast<int>(NOTE_PWM_INDEX::NOTE_C3);
		} else if ( _currentSwtich[static_cast<int>(Switch::DOWN)] ) {
			note_CV=static_cast<int>(NOTE_PWM_INDEX::NOTE_C1);
		} else{
			note_CV = static_cast<int>(NOTE_PWM_INDEX::NOTE_C2);
		}


        //NOTE_PWM_INDEX のインデックス値を算出する
		note_CV=note_CV+i;	
		if (0<=note_CV){
			Serial.print(" =>note_CV:");
			Serial.print(note_CV);
			Serial.println("");

			_voltage->cv(note_CV);  //CVを設定する
		}
	}


	/*
	<LED accent>
	C1~C2ボタン押下中変数で1個以上押下あり
	　　⇒点灯
		C1~C2ボタン押下中変数で押下なし
	　　⇒消灯
	*/
	_voltage->accent(note_on);

	/*
	//ラン/ストップフラグ:ラン
	if (_run_stop == RUN_STOP::RUN) {
		execRunSequence();

		//ラン/ストップフラグ:ストップ
	}
	else	if (_run_stop == RUN_STOP::STOP) {
		execStopSequence();
	}
	*/
}


/*
[仮想関数]MIDIクロックカウント閾値達成時に実行されるアプリケーションを実施する
*/
void debugMode::runClock() {

//・1=MIDIクロック数mod6 ⇒16分音符スタート
	/*
    <LED gate>
      ⇒1,2=MIDIステップ数mod
        ⇒⇒点灯
      ⇒0.3=MIDIステップ数mod4 
        ⇒⇒点灯
	*/
	if ( 1== _Step ){
	_tempoLed =	!_tempoLed;
	}
	_voltage->gate(_tempoLed);

	/*    
	<MIDIステップ数>
      ⇒MIDIステップ数+1
      ⇒MIDIステップ>16なら1に設定
	*/
	_Step++;
	if (_Step > MIDI_STEP_MAX){
		_Step = MIDI_STEP_START;
	}

	/*
	・MIDIクロック数
	<MIDIクロック数>
	⇒MIDIクロック数+1
	⇒MIDIクロック>96なら1に設定
	*/
	_midiClock++;
	if (_midiClock > MIDICLOCK_MAX){
		_midiClock = MIDICLOCK_START;
	}



	/*
	//ラン/ストップフラグ:ラン
	if (_run_stop == RUN_STOP::RUN) {
		execRunClock();

		//ラン/ストップフラグ:ストップ
	}
	else	if (_r
	un_stop == RUN_STOP::STOP) {
		execStopClock();
	}
	*/
}


/*
【未使用】ラン/ストップフラグ:ラン シークエンス処理を実行
*/
void	debugMode::execRunSequence() {
}

/*
【未使用】ラン/ストップフラグ:ストップ シークエンス処理を実行
*/
void	debugMode::execStopSequence() {
}

/*
【未使用】ラン/ストップフラグ:ラン MIDIクロックカウント処理を実行
*/
void	debugMode::execRunClock() {
}

/*
【未使用】ラン/ストップフラグ:ストップ MIDIクロックカウント処理を実行
*/
void	debugMode::execStopClock() {
}
