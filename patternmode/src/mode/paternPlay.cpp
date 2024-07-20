#include "paternPlay.h"

/*
コンストラクタ
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
*/
paternPlay::paternPlay(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) :mode(MODE_NAME::PATERN_PLAY, ptPanelManager, ptVoltage, ptSequenceMap) {

	//各状態を初期値に変更する
	_pattern = PATTERN_START_IDX;				//指定パターン
	_step = STEP_START_IDX;						//現在ステップ
	_LEDCount = 0;								//LED点滅カウント
	_pushRunSW = false;							//ラン/ストップ前回状態フラグ
	_midiclock_16note = MIDICLOCK_START_16NOTE;	//16音符毎MIDIクロックカウント
    _LEDtempo=true;                             //テンポカウント時LED点灯フラグ
	_LEDstep=0;									//テンポカウント時ステップカウンタ
	_next_pattern=_pattern;						//次に演奏する指定パターン(1-8)

	//ラン/ストップフラグ←ストップ
	_run_stop = RUN_STOP::STOP;

	//voltageクラス 出力内容をリセット
	_voltage->reset();

	//LED出力をクリア
	_panelManager->setLEDRow(LED_ROW_0, 0x00);
	_panelManager->setLEDRow(LED_ROW_1, 0x00);
	_panelManager->setLEDRow(LED_ROW_2, 0x60);
	_panelManager->setLEDRow(LED_ROW_3, 0x00);
}

/*
[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
*/
void paternPlay::runSequence() {

	//現状入力情報を取得
	//ボタン押下中変数と比較
	for ( int i=0 ; i<SW_INDEX_MAX ; i++){
		if ( _panelManager->getSwitch(i) != _currentSwtich[i] ){
			/*
			Serial.print("paternPlay::runSequence() change index:");
			Serial.print(i);
			Serial.print(" _currentSwtich[i]:");
			Serial.print(_currentSwtich[i]);
			Serial.print(" _panelManager->getSwitch(i):");
			Serial.print(_panelManager->getSwitch(i));
			Serial.println("");*/

			_currentSwtich[i] = _panelManager->getSwitch(i);
		}
	}

	//ラン/ストップ切替チェック
	changeRunStop();

	//ラン/ストップフラグ:ラン
	if (_run_stop == RUN_STOP::RUN) {
		execRunSequence();

		//ラン/ストップフラグ:ストップ
	}
	else	if (_run_stop == RUN_STOP::STOP) {
		execStopSequence();
	}
}

/*
[仮想関数]MIDIクロックカウント閾値達成時に実行されるアプリケーションを実施する
*/
void paternPlay::runClock() {

	//16音符毎MIDIクロックカウントを更新
	_midiclock_16note++;

	//16音符毎MIDIクロックカウントが7カウント目なら1カウントに戻す
	if (_midiclock_16note > MIDICLOCK_STOP_16NOTE) {
		_midiclock_16note = MIDICLOCK_START_16NOTE;

		//16音符毎MIDIクロックアップしたらテンポカウント時ステップカウンタをインクリメント
		_LEDstep++;	

		//指定パターンLED点灯状態を反転
		if ( 0 == (_LEDstep%2)){
			_LEDtempo = !_LEDtempo;	
			_LEDstep=0;
		}
	}


	//ラン/ストップフラグ:ラン
	if (_run_stop == RUN_STOP::RUN) {
		execRunClock();

	//ラン/ストップフラグ:ストップ
	}
	else	if (_run_stop == RUN_STOP::STOP) {
		execStopClock();
	}
}


/*
ラン/ストップフラグ:ラン シークエンス処理を実行
*/
void	paternPlay::execRunSequence() {

	int i;
	int _patern_index;

	_panelManager->setLEDRow(LED_ROW_0, 0x00);
	_panelManager->setLEDRow(LED_ROW_1, 0x00);
	_panelManager->setLEDRow(LED_ROW_2, 0x00);
	_panelManager->setLEDRow(LED_ROW_3, 0x00);
	_panelManager->setLED(static_cast<int>(LED::PATTERN), true);
	_panelManager->setLED(static_cast<int>(LED::RUN_STOP), true);
	_panelManager->setLED(_scanPatternLED[_pattern], _LEDtempo);

	//次に演奏するパターンを決定する
	for (i=0;i<SEQUENCE_PATTERN_LENGTH ;i++){
		_patern_index=_scanPatternSwich[i];	

		//押下中ノートボタンがあれば、パターン選択中とみなす
		if (_currentSwtich[_patern_index]){
			_next_pattern = i;
			break;
		}
	}

	//次に演奏するパターンが選択されていたらLEDを点灯
	if ( _pattern != _next_pattern){
		_panelManager->setLED(_scanPatternLED[_next_pattern], true);
	}
}

/*
ラン/ストップフラグ:ストップ シークエンス処理を実行
*/
void	paternPlay::execStopSequence() {

	int i;
	int _patern_index;

	//キーボード機能 切り替えチェック＆キーボード機能実行
	bool _execKeyborad = execKeyborad();

	//パターン選択中をチェック
	if ( false==_execKeyborad ){

        /*
		Serial.print("paternPlay::execStopSequence()");
		Serial.print(" _pattern:");
		Serial.print(_pattern);
		Serial.print(" _scanPatternLED[_pattern]:");
		Serial.print(_scanPatternLED[_pattern]);
		Serial.println("");
        */

		_panelManager->setLEDRow(LED_ROW_0, 0x00);
		_panelManager->setLEDRow(LED_ROW_1, 0x00);
		_panelManager->setLEDRow(LED_ROW_2, 0x00);
		_panelManager->setLEDRow(LED_ROW_3, 0x00);
		_panelManager->setLED(static_cast<int>(LED::PATTERN), true);
		_panelManager->setLED(_scanPatternLED[_pattern], _LEDtempo);		//直前に選択したパターンのLEDを点灯
	
		for (i=0;i<SEQUENCE_PATTERN_LENGTH ;i++){
			_patern_index=_scanPatternSwich[i];	

			//押下中ノートボタンがあれば、パターン選択中とみなす
			if (_currentSwtich[_patern_index]){
				_pattern = i;
				_next_pattern = _pattern;
				_panelManager->setLED(_scanPatternLED[_pattern], _LEDtempo);		//直前に選択したパターンのLEDを点灯
				break;
			}
		}
	}

}

/*
ラン/ストップフラグ:ラン MIDIクロックカウント処理を実行
*/
void	paternPlay::execRunClock() {

	//16音符毎MIDIクロックカウントが最初ならゲートをオンする
	_gate_on_16note();

	//16音符毎MIDIクロックカウントが後半クロックになったらゲートをオフする
	_gate_off_16note();

	//16音符毎MIDIクロックカウントが最終クロックになったら次に演奏するステップを決定する
	_next_step_16note();

}

/*
ラン/ストップフラグ:ストップ MIDIクロックカウント処理を実行
*/
void	paternPlay::execStopClock() {
}

/*
ラン/ストップフラグ:ストップ キーボード機能
*/
bool	paternPlay::execKeyborad() {

	//NOTEボタン押下状態より機能切り替えをチェック⇒未押下なら通常処理に戻る
	bool _exec_keyborad	= _panelManager->getSwitch(static_cast<int>(Switch::NOTE));
	if (!_exec_keyborad) {
		_voltage->gate(false);
		return false;
	}

	//LED点灯初期化
	_panelManager->setLEDRow(LED_ROW_0, 0x00);
	_panelManager->setLEDRow(LED_ROW_1, 0x00);
	_panelManager->setLEDRow(LED_ROW_2, 0x00);
	_panelManager->setLEDRow(LED_ROW_3, 0x00);
	_panelManager->setLED(static_cast<int>(LED::NOTE), true);
	_panelManager->setLED(static_cast<int>(LED::PATTERN), true);

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
	int i;
	int  currentSwitch=0xFF;
    bool note_on = false;
	for (i=static_cast<int>(Switch::C2) ; i>=static_cast<int>(Switch::C) ; i--){
		if (_currentSwtich[i]) {
			note_on=true;
			_panelManager->setLED(i, true);
			break;
		}
	}

	//UP/DOWNボタン 押下中ならLED点灯
	if ( _currentSwtich[static_cast<int>(Switch::UP)] ) {
		_panelManager->setLED(static_cast<int>(LED::UP), true);
	} else if ( _currentSwtich[static_cast<int>(Switch::DOWN)] ) {
		_panelManager->setLED(static_cast<int>(LED::DOWN), true);
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
		Serial.print(" =>note_CV:");
		Serial.print(note_CV);
		Serial.println("");

		_voltage->cv(note_CV);  //CVを設定する
	}

		/*
	<LED gate>
	C1~C2ボタン押下中変数で1個以上押下あり
	　　⇒点灯
		C1~C2ボタン押下中変数で押下なし
	　　⇒消灯
	*/
	_voltage->gate(note_on);


	return true;
}


/*
ラン/ストップ切替チェック
*/
void	paternPlay::changeRunStop() {

	//現在のラン/ストップSW状態
	bool	nowRunSW = _panelManager->getSwitch(static_cast<int>(Switch::RUN_STOP));

	//前回状態=OFF,現在状態=ON ならモード切替を行う
	if ((!_pushRunSW) && (nowRunSW)) {
		bool _LEDRunStop = false;

		//ラン/ストップフラグ:ラン
		if (_run_stop == RUN_STOP::STOP) {
			_run_stop = RUN_STOP::RUN;
			_LEDRunStop	= true;

		//ラン/ストップフラグ:ストップ
		}
		else	if (_run_stop == RUN_STOP::RUN) {
			_run_stop = RUN_STOP::STOP;
			_LEDRunStop	= false;

		}

		////現在ステップを1に戻す
		_step=1;					

		//_midiclock_16note⇒初期化
		_midiclock_16note = MIDICLOCK_START_16NOTE;

		//LED::RUN_STOP⇒状態に合わせて設定
		_panelManager->setLED(static_cast<int>(LED::RUN_STOP), _LEDRunStop);
	}

	//前回状態←現在状態
	_pushRunSW = nowRunSW;

}


/*
16音符毎MIDIクロックカウントが最初ならゲートをオンする
*/
void paternPlay::_gate_on_16note() {

	bool _note_on	=	_sequenceMap->paterns[_pattern].steps[_step].note_on;
	bool _up		=	_sequenceMap->paterns[_pattern].steps[_step].up;
	bool _down		=	_sequenceMap->paterns[_pattern].steps[_step].down;
	bool _acc		=	_sequenceMap->paterns[_pattern].steps[_step].acc;
	bool _slide		=	_sequenceMap->paterns[_pattern].steps[_step].slide;
	unsigned char _note_relative = _sequenceMap->paterns[_pattern].steps[_step].note - static_cast<unsigned char>(NOTE_PWM_INDEX::NOTE_C2);
	


	if(_midiclock_16note == MIDICLOCK_START_16NOTE){
		Serial.print("_gate_on_16note() MIDICLOCK_START_16NOTE ");
		Serial.print(" pattern:");
		Serial.print(_pattern);
		Serial.print(" step:");
		Serial.print(_step);
		Serial.print(" gate:");
		Serial.print(_note_on);
		Serial.print(" acc:");
		Serial.print(_acc);
		Serial.print(" slide:");
		Serial.print(_slide);

		_voltage->gate(_note_on);	//gate

		if (_note_on){
			_voltage->accent(_acc);	//acc
			_voltage->slide(_slide);//slide

			int _note_CV=0;
			if ( _up ) {
				_note_CV=static_cast<int>(NOTE_PWM_INDEX::NOTE_C3);
			} else if ( _down ) {
				_note_CV=static_cast<int>(NOTE_PWM_INDEX::NOTE_C1);
			} else{
				_note_CV = static_cast<int>(NOTE_PWM_INDEX::NOTE_C2);
			}

			//NOTE_PWM_INDEX のインデックス値を算出する
			_note_CV	=	_note_CV	+	_note_relative;	
			Serial.print(" CV:");
			Serial.print(_note_CV);
			_voltage->cv(_note_CV);  //CVを設定する

		} else {
			_voltage->accent(false);//acc
			_voltage->slide(false);	//slide			
		}

		Serial.println("");
	}
}

/*
16音符毎MIDIクロックカウントが後半クロックになったらゲートをオフする
*/
void paternPlay::_gate_off_16note() {

	bool _note_on	=	_sequenceMap->paterns[_pattern].steps[_step].note_on;
	bool _slide 	=	_sequenceMap->paterns[_pattern].steps[_step].slide;

	if (_midiclock_16note == MIDICLOCK_GATEOFF_16NOTE) {

		Serial.print("_gate_off_16note() ");
		Serial.print(" pattern:");
		Serial.print(_pattern);
		Serial.print(" step:");
		Serial.print(_step);
		Serial.print(" note_on:");
		Serial.print(_note_on);
		Serial.print(" slide:");
		Serial.print(_slide);

		if ((_note_on) && (!_slide)){
			_voltage->gate(false);
			_voltage->accent(false);//acc
		}

		Serial.println("");
	}

}

/*
16音符毎MIDIクロックカウントが最終クロックになったら次に演奏するパターン&ステップを決定する
*/
void paternPlay::_next_step_16note() {

	bool _laststep 		=	_sequenceMap->paterns[_pattern].steps[_step].lastStep;
	bool _nextPattern 	=	false;

	if (_midiclock_16note == MIDICLOCK_STOP_16NOTE) {

		Serial.print("_next_step_16note() ");
		Serial.print(" pattern:");
		Serial.print(_pattern);
		Serial.print(" step:");
		Serial.print(_step);
		Serial.print(" laststep:");
		Serial.print(_laststep);

		//現在のステップが最終ステップなら「ステップ=1」に設定する
		if (_laststep)	{
			_step=1;
			_nextPattern = true;

		//現在のステップが通常ステップなら次ステップに設定する
		} else if (!_laststep){
			_step++;

			if(_step>=PATERN_STEP_LENGTH){
				_step=1;
				_nextPattern = true;
			}
		}

		//次に演奏するパターンを決定する
		if (_nextPattern) {
			_pattern = _next_pattern;
		}

		Serial.print(" EDITED:_pattern:");
		Serial.print(_pattern);
		Serial.print(" EDITED:_next_pattern:");
		Serial.print(_next_pattern);
		Serial.println("");
	}


}




