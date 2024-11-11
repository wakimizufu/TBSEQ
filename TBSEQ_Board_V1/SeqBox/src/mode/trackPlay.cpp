#include "trackPlay.h"

/*
コンストラクタ
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
	ptSequenceMap :sequenceMapクラスポインタ
	ptTrackMap    :trackMapクラスポインタ
*/
trackPlay::trackPlay(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, trackMap* ptTrackMap) :mode(MODE_NAME::TRACK_PLAY, ptPanelManager, ptVoltage, ptSequenceMap, ptTrackMap) {

	//各状態を初期値に変更する
	_track		=	TRACKMAP_START_IDX;			//指定トラック
	_trackStep	=	TRACK_STEP_START_IDX;		//指定トラックステップ
	_pattern = PATTERN_START_IDX;				//指定パターン
	_step = STEP_START_IDX;						//現在ステップ
	_LEDCount = 0;								//LED点滅カウント
	_pushRunSW = false;							//ラン/ストップ前回状態フラグ
	_midiclock_16note = MIDICLOCK_START_16NOTE;	//16音符毎MIDIクロックカウント
    _LEDtempo=true;                             //テンポカウント時LED点灯フラグ
	_LEDstep=0;									//テンポカウント時ステップカウンタ
	_next_track=_track;							//次に演奏する指定トラック(1-13)

	//ラン/ストップフラグ←ストップ
	_run_stop = RUN_STOP::STOP;

	//voltageクラス 出力内容をリセット
	_voltage->reset();

	//LED出力をクリア
	_panelManager->setLEDRow(LED_ROW_0, 0x0000);
	_panelManager->setLEDRow(LED_ROW_1, 0x0000);
	_panelManager->setLED(static_cast<int>(LED::TRACK), true);
}

/*
[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
*/
void trackPlay::runSequence() {

	//現状入力情報を取得
	//ボタン押下中変数と比較
	for ( int i=0 ; i<SW_INDEX_MAX ; i++){
		_onClickSwtich[i]=false;
		
		if ( _panelManager->getSwitch(i) != _currentSwtich[i] ){
			/*
			Serial.print("trackPlay::runSequence() change index:");
			Serial.print(i);
			Serial.print(" _currentSwtich[i]:");
			Serial.print(_currentSwtich[i]);
			Serial.print(" _panelManager->getSwitch(i):");
			Serial.print(_panelManager->getSwitch(i));
			Serial.println("");*/

			_onClickSwtich[i] = isSwitchOnClick(i);
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
void trackPlay::runClock() {

	//16音符毎MIDIクロックカウントを更新
	_midiclock_16note++;

	//開始ステップをクリア
	_StartStep	=	false;

	//16音符毎MIDIクロックカウントが7カウント目なら1カウントに戻す
	if (_midiclock_16note > MIDICLOCK_STOP_16NOTE) {
		_midiclock_16note = MIDICLOCK_START_16NOTE;

		//開始ステップを設定
		_StartStep	=	true;

		//16音符毎MIDIクロックアップしたらテンポカウント時ステップカウンタをインクリメント
		//_LEDstep++;	

		//指定パターンLED点灯状態を反転
		//if ( 0 == (_LEDstep%2)){
			_LEDtempo = !_LEDtempo;	
		//	_LEDstep=0;
		//}
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
void	trackPlay::execRunSequence() {

	int i;
	int _track_index;

	_panelManager->setLEDRow(LED_ROW_0, 0x0000);
	_panelManager->setLEDRow(LED_ROW_1, 0x0000);
	_panelManager->setLED(static_cast<int>(LED::TRACK), true);
	_panelManager->setLED(static_cast<int>(LED::RUN_STOP), true);
	_panelManager->setLED(_scanTrackLED[_track], _LEDtempo);

	//指定トラックから演奏するバンク/パターンを取得する
	getTrack2Pattern(_track, _trackStep, &_bank, &_pattern, &_transport, &_tracklastStep);

	//ステップ:指定ステップ数に応じたLEDを設定する
	setStepLED(_step);

	//バンク:指定バンク数に応じたLEDを設定する
	setBackLED(_bank);

	//トラックステップ:指定トラックステップ数に応じたLEDを設定する
	setTrackStepLED(_trackStep);

	//トラックステップ:最終トラックステップならLEDを設定する
	if (_tracklastStep){
		_panelManager->setLED(static_cast<int>(LED::LENMAX), true);	
	}

	//次に演奏するパターンを決定する
	for (i=0;i<TRACKMAP_TRACK_LENGTH ;i++){
		_track_index=_scanTrackSwich[i];	

		//押下中ノートボタンがあれば、パターン選択中とみなす
		if (_currentSwtich[_track_index]){
			_next_track = i;
			break;
		}
	}

	//次に演奏するパターンが選択されていたらLEDを点灯
	if ( _track != _next_track){
		_panelManager->setLED(_scanTrackLED[_next_track], true);
	}
}


/*
ラン/ストップフラグ:ストップ シークエンス処理を実行
*/
void	trackPlay::execStopSequence() {

	int i;
	int _track_index, _bank_index;

	/*
	Serial.print("trackPlay::execStopSequence()");
	Serial.print(" _track:");
	Serial.print(_track);
	Serial.print(" _scanTrackLED[_track]:");
	Serial.print(_scanTrackLED[_track]);
	Serial.println("");
	*/

	_panelManager->setLEDRow(LED_ROW_0, 0x0000);
	_panelManager->setLEDRow(LED_ROW_1, 0x0000);
	_panelManager->setLED(static_cast<int>(LED::TRACK), true);
	_panelManager->setLED(_scanTrackLED[_track], _LEDtempo);		//直前に選択したパターンのLEDを点灯

	//トラック:演奏するトラックを選択
	for (i=0;i<TRACKMAP_TRACK_LENGTH ;i++){
		_track_index=_scanTrackSwich[i];	

		//押下中ノートボタンがあれば、パターン選択中とみなす
		if (_currentSwtich[_track_index]){
			_track = i;
			_next_track = _track;
			_panelManager->setLED(_scanTrackLED[_track], _LEDtempo);		//直前に選択したパターンのLEDを点灯
			break;
		}
	}
}


/*
ラン/ストップフラグ:ラン MIDIクロックカウント処理を実行
*/
void	trackPlay::execRunClock() {

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
void	trackPlay::execStopClock() {
}


/*
ラン/ストップ切替チェック
*/
void	trackPlay::changeRunStop() {

	//現在のラン/ストップSW状態
	bool	nowRunSW = _panelManager->getSwitch(static_cast<int>(Switch::RUN_STOP));

	if ( (_MIDI_Start) || (_MIDI_Stop) ){
		Serial.print("trackPlayPlay::changeRunStop()");
		Serial.print(" _MIDI_Start:");
		Serial.print(_MIDI_Start);
		Serial.print(" _MIDI_Stop:");
		Serial.print(_MIDI_Stop);
		Serial.print(" _run_stop:");
		Serial.print(static_cast<int>(_run_stop));
		Serial.println("");
	}


	//MIDIスタート/ストップ受信に応じてラン/ストップSW状態を設定
	if ( (_run_stop == RUN_STOP::RUN) && (_MIDI_Stop)){
		nowRunSW	=	true;
		_MIDI_Stop  =   false;
	} else if ( (_run_stop == RUN_STOP::STOP) && (_MIDI_Start)){
		nowRunSW	=	true;
		_MIDI_Start = false;
	}


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

		////現在トラックステップを1に戻す
		_trackStep=TRACK_STEP_START_IDX;	

		////現在ステップを1に戻す
		_step=STEP_START_IDX;					

		//_midiclock_16note⇒初期化
		_midiclock_16note = MIDICLOCK_STOP_16NOTE;

		//LED::RUN_STOP⇒状態に合わせて設定
		_panelManager->setLED(static_cast<int>(LED::RUN_STOP), _LEDRunStop);
	}

	//前回状態←現在状態
	_pushRunSW = nowRunSW;

}


/*
指定トラックを取得する
戻り値:指定トラック(1-13)
*/
int	trackPlay::getCurrnetTrack(){
	return	_track;
}


/*
16音符毎MIDIクロックカウントが最初ならゲートをオンする
*/
void trackPlay::_gate_on_16note() {

	//指定トラックから演奏するバンク/パターンを取得する
	getTrack2Pattern(_track, _trackStep, &_bank, &_pattern, &_transport, &_tracklastStep);

	bool _up		=	_sequenceMap->paterns[_bank][_pattern].steps[_step].up;
	bool _down		=	_sequenceMap->paterns[_bank][_pattern].steps[_step].down;
	bool _acc		=	_sequenceMap->paterns[_bank][_pattern].steps[_step].acc;
	bool _slide		=	_sequenceMap->paterns[_bank][_pattern].steps[_step].slide;
	unsigned char _note_on	=	_sequenceMap->paterns[_bank][_pattern].steps[_step].note_on;
	unsigned char _note_relative = _sequenceMap->paterns[_bank][_pattern].steps[_step].note - static_cast<unsigned char>(NOTE_PWM_INDEX::NOTE_C2);
	_transport	=	_transport - static_cast<unsigned char>(NOTE_PWM_INDEX::NOTE_C2);

	if(_midiclock_16note == MIDICLOCK_START_16NOTE){
		Serial.print("_gate_on_16note() MIDICLOCK_START_16NOTE ");
		Serial.print(" track:");
		Serial.print(_track);
		Serial.print(" _trackStep:");
		Serial.print(_trackStep);
		Serial.print(" bank:");
		Serial.print(_bank);
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

		if (( STEP_NOTE_ON_NORMAL == _note_on)||( STEP_NOTE_ON_TIE == _note_on)){
			_voltage->gate(true);	//gate
		} else if ( STEP_NOTE_OFF == _note_on){
			_voltage->gate(false);	//gate
		}

		if (_note_on){
			_voltage->accent(!_acc);	//acc
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
			_note_CV	=	_note_CV	+	_note_relative + _transport;	
			Serial.print(" CV:");
			Serial.print(_note_CV);
			_voltage->cv(_note_CV);  //CVを設定する

		} else {
			_voltage->accent(ACCENT_OFF);//acc
			_voltage->slide(SLIDE_OFF);	//slide			
		}

		Serial.println("");
	}
}


/*
16音符毎MIDIクロックカウントが後半クロックになったらゲートをオフする
*/
void trackPlay::_gate_off_16note() {

	//指定トラックから演奏するバンク/パターンを取得する
	getTrack2Pattern(_track, _trackStep, &_bank, &_pattern, &_transport, &_tracklastStep);

	unsigned char  _note_on	=	_sequenceMap->paterns[_bank][_pattern].steps[_step].note_on;
	bool _slide 	=	_sequenceMap->paterns[_bank][_pattern].steps[_step].slide;

	if (_midiclock_16note == MIDICLOCK_GATEOFF_16NOTE) {

		Serial.print("_gate_off_16note() ");
		Serial.print(" bank:");
		Serial.print(_bank);
		Serial.print(" pattern:");
		Serial.print(_pattern);
		Serial.print(" step:");
		Serial.print(_step);
		Serial.print(" note_on:");
		Serial.print(_note_on);
		Serial.print(" slide:");
		Serial.print(_slide);

		if (( STEP_NOTE_ON_NORMAL == _note_on) || ( STEP_NOTE_OFF == _note_on)) {
			_voltage->gate(GATE_OFF);	//gate
		}
		Serial.println("");
	}

}

/*
16音符毎MIDIクロックカウントが最終クロックになったら次に演奏するパターン&ステップを決定する
*/
void trackPlay::_next_step_16note() {

	//指定トラックから演奏するバンク/パターンを取得する
	getTrack2Pattern(_track, _trackStep, &_bank, &_pattern, &_transport, &_tracklastStep);

	bool _laststep 		=	_sequenceMap->paterns[_bank][_pattern].steps[_step].lastStep;
	bool _nextTrackStep	=	false;
	bool _nextTrack	=	false;

	if (_midiclock_16note == MIDICLOCK_STOP_16NOTE) {

		Serial.print("_next_step_16note() ");
		Serial.print(" bank:");
		Serial.print(_bank);
		Serial.print(" pattern:");
		Serial.print(_pattern);
		Serial.print(" step:");
		Serial.print(_step);
		Serial.print(" laststep:");
		Serial.print(_laststep);

		//現在のステップが最終ステップなら「ステップ=1」に設定する
		if (_laststep)	{
			_step=STEP_START_IDX;
			_nextTrackStep = true;

		//現在のステップが通常ステップなら次ステップに設定する
		} else if (!_laststep){
			_step++;

			if(_step>=PATERN_STEP_LENGTH){
				_step=STEP_START_IDX;
				_nextTrackStep = true;
			}
		}


		//次のトラックステップを演奏する
		if ( _nextTrackStep ){
			//現在のトラックステップが最終トラックなら「トラックステップ=1」に設定する
			if (_tracklastStep)	{
				_trackStep = TRACK_STEP_START_IDX;
				_nextTrack = true;

			//現在のトラックステップが通常ステップなら次トラックステップに設定する
			} else if (!_tracklastStep){
				_trackStep++;

				//16トラックステップを超えたら「トラックステップ=1」に設定する
				if(_step>=TRACK_STEP_LENGTH){
					_trackStep = TRACK_STEP_START_IDX;
					_nextTrack = true;
				}
			}
		}

		//次のトラックに切り替える
		if ( _nextTrack ){
			_track = _next_track;
		}


		Serial.print(" EDITED:_track:");
		Serial.print(_track);
		Serial.print(" EDITED:_next_track:");
		Serial.print(_next_track);
		Serial.println("");
	}


}

