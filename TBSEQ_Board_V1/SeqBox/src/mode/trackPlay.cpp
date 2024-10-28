#include "trackPlay.h"

/*
コンストラクタ
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
	ptSequenceMap :sequenceMapクラスポインタ
	ptTrackMap    :trackMapクラスポインタ
*/
trackPlay::trackPlay(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, trackMap* ptTrackMap) :mode(MODE_NAME::TRACK_PLAY, ptPanelManager, ptVoltage, ptSequenceMap, trackMap* ptTrackMap) {

	//各状態を初期値に変更する
	_track = TRACKMAP_START_IDX;				//指定トラック
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

	
	//ステップ:指定ステップ数に応じたLEDを設定する
	setStepLED(_step);

	//バンク:指定バンク数に応じたLEDを設定する
	setBackLED(_bank);

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

	//パターン選択中をチェック
	if ( false==_execKeyborad ){

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
	
		//パターン:演奏するパターンを選択
		for (i=0;i<TRACKMAP_TRACK_LENGTH ;i++){
			_track_index=_scanPatternSwich[i];	

			//押下中ノートボタンがあれば、パターン選択中とみなす
			if (_currentSwtich[_track_index]){
				_track = i;
				_next_track = _track;
				_panelManager->setLED(_scanTrackLED[_track], _LEDtempo);		//直前に選択したパターンのLEDを点灯
				break;
			}
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

