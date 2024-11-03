#include "trackWrite.h"

/*
コンストラクタ
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
*/
trackWrite::trackWrite(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, trackMap* ptTrackMap, int _initTrack) :mode(MODE_NAME::TRACK_WRITE, ptPanelManager, ptVoltage, ptSequenceMap, ptTrackMap) {


	//各状態を初期値に変更する
	_track = _initTrack;						//指定トラック
	_trackStep = TRACK_STEP_START_IDX;			//現在ステップ
	_LEDCount = 0;								//LED点滅カウント
	_pushRunSW = false;							//ラン/ストップ前回状態フラグ
	_midiclock_16note = MIDICLOCK_START_16NOTE;	//16音符毎MIDIクロックカウント
	_LEDtempo=true;                             //テンポカウント時LED点灯フラグ
	_LEDstep=0;									//テンポカウント時ステップカウンタ
	_Playstep = STEP_START_IDX;					//演奏時ステップを1に戻す
	
	//指定トラックから演奏するバンク/パターンを取得する
	getTrack2Pattern(_track, _trackStep, &_bank, &_pattern, &_transport, &_tracklastStep);

	//ラン/ストップフラグ←ストップ
	_run_stop = RUN_STOP::STOP;

	//voltageクラス 出力内容をリセット
	_voltage->reset();

	//LED出力をクリア
	_panelManager->setLEDRow(LED_ROW_0, 0x0000);
	_panelManager->setLEDRow(LED_ROW_1, 0x0000);
	_panelManager->setLED(static_cast<int>(LED::TRACK), true);
	_panelManager->setLED(static_cast<int>(LED::PLAY_WRITE), true);

}

/*
[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
*/
void trackWrite::runSequence() {

	//現状入力情報を取得
	//ボタン押下中変数と比較
	for ( int i=0 ; i<SW_INDEX_MAX ; i++){
		_onClickSwtich[i]=false;
		_offClickSwtich[i]=false;

		if ( _panelManager->getSwitch(i) != _currentSwtich[i] ){
			/*
			Serial.print("paternwrite::runSequence() change index:");
			Serial.print(i);
			Serial.print(" _currentSwtich[i]:");
			Serial.print(_currentSwtich[i]);
			Serial.print(" _panelManager->getSwitch(i):");
			Serial.print(_panelManager->getSwitch(i));
			Serial.println("");*/
			_onClickSwtich[i] = isSwitchOnClick(i);
			_offClickSwtich[i] = isSwitchOffClick(i);
			_currentSwtich[i] = _panelManager->getSwitch(i);
		}
	}

	//ラン/ストップ切替チェック
	//changeRunStop();

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
void trackWrite::runClock() {

	//16音符毎MIDIクロックカウントを更新
	_midiclock_16note++;

	//開始ステップをクリア
	_StartStep	=	false;

	//16音符毎MIDIクロックカウントが7カウント目なら1カウントに戻す
	if (_midiclock_16note > MIDICLOCK_STOP_16NOTE) {
		_midiclock_16note = MIDICLOCK_START_16NOTE;

		//開始ステップを設定
		_StartStep	=	true;
	}

	/*
	Serial.print("runClock() _midiclock_16note:");
	Serial.print(_midiclock_16note);
	Serial.println("");
    */

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
ラン/ストップ切替チェック
*/
void	trackWrite::changeRunStop() {
	/*演奏なし*/
}


/*
ラン/ストップフラグ:ラン シークエンス処理を実行
*/
void	trackWrite::execRunSequence() {
	execStopSequence();
}

/*
ラン/ストップフラグ:ストップ シークエンス処理を実行
*/
void	trackWrite::execStopSequence() {
	
	int i;

	//指定トラックから演奏するバンク/パターンを取得する
	getTrack2Pattern(_track, _trackStep, &_bank, &_pattern, &_transport, &_tracklastStep);

	//LED 表示更新
	_panelManager->setLEDRow(LED_ROW_0, 0x0000);
	_panelManager->setLEDRow(LED_ROW_1, 0x0000);
	_panelManager->setLED(static_cast<int>(LED::TRACK), true);
	_panelManager->setLED(static_cast<int>(LED::PLAY_WRITE), true);
	
	//バンク:指定バンク数に応じたLEDを設定する
	setBackLED(_bank);

	_panelManager->setLED(static_cast<int>(LED::UP), _up);
	_panelManager->setLED(static_cast<int>(LED::DOWN), _down);
	_panelManager->setLED(static_cast<int>(LED::SLIDE), _slide);
	_panelManager->setLED(static_cast<int>(LED::ACC), _acc);
	_panelManager->setLED(static_cast<int>(LED::LENMAX), _laststep);
	_panelManager->setLED(_noteOnLED[_note_relative], true);




	if	(	STEP_NOTE_ON_NORMAL	==	_note_on	)	{
		_panelManager->setLED(static_cast<int>(LED::NOTE_ON), true);
	} else if (	STEP_NOTE_ON_TIE	==	_note_on	)	{
		_panelManager->setLED(static_cast<int>(LED::NOTE_TIE), true);
	}


	if (_run_stop == RUN_STOP::RUN){
		_panelManager->setLED(static_cast<int>(LED::RUN_STOP), true);
	}	else	if (_run_stop == RUN_STOP::STOP) {
	}

	//ステップ:指定ステップ数に応じたLEDを設定する
	setStepLED(_step);

	//バンク:指定バンク数に応じたLEDを設定する
	setBackLED(_bank);

	//SW ノート:発音ノートを更新
	for (i=static_cast<int>(Switch::C2) ; i>=static_cast<int>(Switch::C) ; i--){
		if (_onClickSwtich[i]) {
			_sequenceMap->paterns[_bank][_pattern].steps[_step].note = static_cast<unsigned char>(i) + static_cast<unsigned char>(NOTE_PWM_INDEX::NOTE_C2);
			break;
		}
	}

	//SW ノート:ノートオンを更新
	if	(	_onClickSwtich[static_cast<int>(Switch::NOTE)]	)	{
		if	(	STEP_NOTE_ON_NORMAL	==	_note_on	)	{
			_sequenceMap->paterns[_bank][_pattern].steps[_step].note_on	=	STEP_NOTE_ON_TIE;
		} else if (	STEP_NOTE_ON_TIE	==	_note_on	)	{
			_sequenceMap->paterns[_bank][_pattern].steps[_step].note_on	=	STEP_NOTE_OFF;
		} else if (	STEP_NOTE_OFF	==	_note_on	)	{
			_sequenceMap->paterns[_bank][_pattern].steps[_step].note_on	=	STEP_NOTE_ON_NORMAL;
		}
	}

	//SW アクセントを更新
	if	(	_onClickSwtich[static_cast<int>(Switch::ACC)]	)	{
		_sequenceMap->paterns[_bank][_pattern].steps[_step].acc	=	!_acc;
	}
	

	//SW スライドを更新
	if	(	_onClickSwtich[static_cast<int>(Switch::SLIDE)]	)	{
		_sequenceMap->paterns[_bank][_pattern].steps[_step].slide	=	!_slide;
	}

	//SW ラストステップを更新
	if	(	_onClickSwtich[static_cast<int>(Switch::LENMAX)]	)	{

		if ( MIDI_STEP_MAX > (_step + 1)){
		_sequenceMap->paterns[_bank][_pattern].steps[_step].lastStep	=	!_laststep;
		}
	}

	//SW UPを更新
	if	(	_onClickSwtich[static_cast<int>(Switch::UP)]	)	{
		_sequenceMap->paterns[_bank][_pattern].steps[_step].up	=	!_up;
		_sequenceMap->paterns[_bank][_pattern].steps[_step].down =false;
	//SW DOWNを更新
	}	 else if	(	_onClickSwtich[static_cast<int>(Switch::DOWN)]	)	{
		_sequenceMap->paterns[_bank][_pattern].steps[_step].up	=false;
		_sequenceMap->paterns[_bank][_pattern].steps[_step].down	=	!_down;
	}




	//SW NEXT 次のステップに移行
	if	(	_onClickSwtich[static_cast<int>(Switch::NEXT)]	)	{

		Serial.print(" paternWrite::execStopSequence Switch::NEXT");
		Serial.print(" bank:");
		Serial.print(_bank);
		Serial.print(" _pattern:");
		Serial.print(_pattern);
		Serial.print(" _step:");
		Serial.print(_step);
		Serial.print(" _laststep:");
		Serial.print(_laststep);
		
		if(_laststep)	{
			_step	=	STEP_START_IDX;
		} else if (!_laststep) {
			_step++;
		}


		Serial.print(" next _step:");
		Serial.print(_step);
		Serial.println("");

	//SW BACK 前のステップに移行
	} else if (	_onClickSwtich[static_cast<int>(Switch::BACK)]	)	{

		Serial.print(" paternWrite::execStopSequence Switch::BACK");
		Serial.print(" bank:");
		Serial.print(_bank);
		Serial.print(" _pattern:");
		Serial.print(_pattern);
		Serial.print(" _step:");
		Serial.print(_step);

		_step--;
		if( _step < STEP_START_IDX)	{
			
			_step	=	STEP_START_IDX;

			for (i=STEP_START_IDX ; i<PATERN_STEP_LENGTH ; i++){
				if(_sequenceMap->paterns[_bank][_pattern].steps[i].lastStep){
					_step	=	i;
					break;
				}
			}

		}

		Serial.print(" next _step:");
		Serial.print(_step);
		Serial.println("");
	}


	//SW NEXT/BACK 設定したノートを発音する
	if	(	(	_currentSwtich[static_cast<int>(Switch::NEXT)]	)	||
			(	_currentSwtich[static_cast<int>(Switch::BACK)]	) )	{
		
 		int _note_CV=_sequenceMap->paterns[_bank][_pattern].steps[_step].note -	static_cast<int>(NOTE_PWM_INDEX::NOTE_C2);
		
		if ( _sequenceMap->paterns[_bank][_pattern].steps[_step].up ) {
			_note_CV	=	_note_CV	+	static_cast<int>(NOTE_PWM_INDEX::NOTE_C3);
		} else if ( _sequenceMap->paterns[_bank][_pattern].steps[_step].down ) {
			_note_CV	=	_note_CV	+	static_cast<int>(NOTE_PWM_INDEX::NOTE_C1);
		} else{
			_note_CV	=	_note_CV	+	static_cast<int>(NOTE_PWM_INDEX::NOTE_C2);
		}

		_voltage->cv(_note_CV);  //CVを設定する
		_voltage->gate(GATE_ON);	//gate
		_voltage->accent(!_sequenceMap->paterns[_bank][_pattern].steps[_step].acc);	//acc
		_voltage->slide(_sequenceMap->paterns[_bank][_pattern].steps[_step].slide);//slide

	} else {

		if (_run_stop == RUN_STOP::STOP)	{
			_voltage->gate(GATE_OFF);	//gate
			_voltage->accent(ACCENT_OFF);	//acc
			_voltage->slide(SLIDE_OFF);//slide
		}
	}

	
}

/*
ラン/ストップフラグ:ラン MIDIクロックカウント処理を実行
*/
void	trackWrite::execRunClock() {

    /*
	Serial.print("execRunClock() _midiclock_16note:");
	Serial.print(_midiclock_16note);
	Serial.print(" _Playstep:");
	Serial.print(_Playstep);
	Serial.println("");
    */

	//16音符毎MIDIクロックカウントが最初ならゲートをオンする
	//_gate_on_16note();

	//16音符毎MIDIクロックカウントが後半クロックになったらゲートをオフする
	//_gate_off_16note();

	//16音符毎MIDIクロックカウントが最終クロックになったら次に演奏するステップを決定する
	//_next_step_16note();

}

/*
ラン/ストップフラグ:ストップ MIDIクロックカウント処理を実行
*/
void	trackWrite::execStopClock() {
}


/*
16音符毎MIDIクロックカウントが最初ならゲートをオンする
*/
void trackWrite::_gate_on_16note() {
}

/*
16音符毎MIDIクロックカウントが後半クロックになったらゲートをオフする
*/
void trackWrite::_gate_off_16note() {
}

/*
16音符毎MIDIクロックカウントが最終クロックになったら次に演奏するパターン&ステップを決定する
*/
void trackWrite::_next_step_16note() {
}






