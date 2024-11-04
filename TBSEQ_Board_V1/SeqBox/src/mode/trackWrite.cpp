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


	/*
	ラン/ストップフラグ:ストップ 転調設定を実行
	*/
	if (	execTransport()	){
		//転調設定を行ったら以下の処理は行わずそのまま終了
		return;
	}


	//LED 表示更新
	_panelManager->setLEDRow(LED_ROW_0, 0x0000);
	_panelManager->setLEDRow(LED_ROW_1, 0x0000);
	_panelManager->setLED(static_cast<int>(LED::TRACK), true);
	_panelManager->setLED(static_cast<int>(LED::PLAY_WRITE), true);
	
	//バンク:指定バンク数に応じたLEDを設定する
	setBackLED(_bank);
	_panelManager->setLED(static_cast<int>(LED::SLIDE), _slide);
	_panelManager->setLED(static_cast<int>(LED::LENMAX), _tracklastStep);
	_panelManager->setLED(_noteOnLED[_pattern], true);

	//ステップ:指定ステップ数に応じたLEDを設定する
	setStepLED(_trackStep);


	//SW ノート:演奏バンクを更新
	for (i=static_cast<int>(Switch::BANK_D) ; i>=static_cast<int>(Switch::BANK_A) ; i--){
		if (_onClickSwtich[i]) {
			_trackMap->tracks[_track].trackSteps[_trackStep].bank = i - static_cast<int>(Switch::BANK_A);
			break;
		}
	}

	//SW ノート:演奏パターンを更新
	for (i=static_cast<int>(Switch::C2) ; i>=static_cast<int>(Switch::C) ; i--){
		if (_onClickSwtich[i]) {
			_trackMap->tracks[_track].trackSteps[_trackStep].pattern = i;
			break;
		}
	}

	//SW ラストステップを更新
	if	(	_onClickSwtich[static_cast<int>(Switch::LENMAX)]	)	{

		if ( TRACK_STEP_LENGTH > (_trackStep + 1)){
		_trackMap->tracks[_track].trackSteps[_trackStep].lastStep = !_tracklastStep;
		}
	}





	//SW NEXT 次のステップに移行
	if	(	_onClickSwtich[static_cast<int>(Switch::NEXT)]	)	{

		Serial.print(" trackWrite::execStopSequence Switch::NEXT");
		Serial.print(" _trackStep:");
		Serial.print(_trackStep);
		Serial.print(" bank:");
		Serial.print(_bank);
		Serial.print(" _pattern:");
		Serial.print(_pattern);
		Serial.print(" _tracklastStep:");
		Serial.print(_tracklastStep);
		
		if(_tracklastStep)	{
			_trackStep	=	TRACK_STEP_START_IDX;
		} else if (!_tracklastStep) {
			_trackStep++;
		}


		Serial.print(" next _trackStep:");
		Serial.print(_trackStep);
		Serial.println("");

	//SW BACK 前のステップに移行
	} else if (	_onClickSwtich[static_cast<int>(Switch::BACK)]	)	{

		Serial.print(" trackWrite::execStopSequence Switch::BACK");
		Serial.print(" _trackStep:");
		Serial.print(_trackStep);
		Serial.print(" bank:");
		Serial.print(_bank);
		Serial.print(" _pattern:");
		Serial.print(_pattern);
		Serial.print(" _tracklastStep:");
		Serial.print(_tracklastStep);

		_trackStep--;
		if( _trackStep < TRACK_STEP_START_IDX)	{
			
			_trackStep	=	TRACK_STEP_START_IDX;

			for (i=TRACK_STEP_START_IDX ; i<TRACK_STEP_LENGTH ; i++){
				if(_trackMap->tracks[_track].trackSteps[_trackStep].lastStep){
					_trackStep	=	i;
					break;
				}
			}

		}

		Serial.print(" next _trackStep:");
		Serial.print(_trackStep);
		Serial.println("");
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

/*
ラン/ストップフラグ:ストップ 転調設定を実行
*/
bool	trackWrite::execTransport() {

	//NOTEボタン押下状態より機能切り替えをチェック⇒未押下なら通常処理に戻る
	bool _execTransport	= _panelManager->getSwitch(static_cast<int>(Switch::SLIDE));
	if (!_execTransport) {
		return false;
	}


	//LED点灯初期化
	_panelManager->setLEDRow(LED_ROW_0, 0x0000);
	_panelManager->setLEDRow(LED_ROW_1, 0x0000);
	_panelManager->setLED(static_cast<int>(LED::PLAY_WRITE), true);
	
	//バンク:指定バンク数に応じたLEDを設定する
	setBackLED(_bank);
	_panelManager->setLED(static_cast<int>(LED::SLIDE), true);

	//ステップ:指定ステップ数に応じたLEDを設定する
	setStepLED(_trackStep);

	unsigned char _transport_relative = _trackMap->tracks[track].trackSteps[trackStep].transport - static_cast<unsigned char>(NOTE_PWM_INDEX::NOTE_C2);
	_panelManager->setLED(_noteOnLED[_transport_relative], true);


	//各ノートボタンを高音優先で押下状態を取得する
	int i;
	int  currentSwitch=0xFF;
	for (i=static_cast<int>(Switch::C2) ; i>=static_cast<int>(Switch::C) ; i--){
		if (_currentSwtich[i]) {
			_trackMap->tracks[track].trackSteps[trackStep].transport=i;
			break;
		}
	}



	return true;
}




