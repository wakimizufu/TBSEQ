#include "utilityMode.h"

/*
コンストラクタ
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
*/
utilityMode::utilityMode(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, trackMap* ptTrackMap) :mode(MODE_NAME::UTILITY, ptPanelManager, ptVoltage, ptSequenceMap, ptTrackMap) {

	_syncPolarity	=	SYNC_TRIGER_POSITIVE;		//シンク信号極性   (false:立ち上がり, true:立下り)
	_syncTempo	=	SYNC_TEMPO_MIDI_IN;				//テンポ同期ソース (false:シンク信号, true:MIDI IN)

	//各状態を初期値に変更する
	_midiclock_16note = MIDICLOCK_START_16NOTE;	//16音符毎MIDIクロックカウント

	//ラン/ストップフラグ←ストップ
	_run_stop = RUN_STOP::STOP;

	//voltageクラス 出力内容をリセット
	_voltage->reset();

	//LED出力をクリア
	_panelManager->setLEDRow(LED_ROW_0, 0x0000);
	_panelManager->setLEDRow(LED_ROW_1, 0x0000);
	_panelManager->setLED(static_cast<int>(LED::TRACK), true);
	_panelManager->setLED(static_cast<int>(LED::PATTERN), true);
	_panelManager->setLED(static_cast<int>(LED::PLAY_WRITE), true);

}

/*
[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
*/
void utilityMode::runSequence() {

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
void utilityMode::runClock() {

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
ラン/ストップフラグ:ラン シークエンス処理を実行
*/
void	utilityMode::execRunSequence() {
	execStopSequence();
}

/*
ラン/ストップフラグ:ストップ シークエンス処理を実行
*/
void	utilityMode::execStopSequence() {
	
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
	_panelManager->setLED(static_cast<int>(LED::LENMAX), _tracklastStep);
	_panelManager->setLED(_scanPatternLED[_pattern], true);


	_transport	=	_transport - static_cast<unsigned char>(NOTE_PWM_INDEX::NOTE_C2);
	if ( _transport >= 1){
		_panelManager->setLED(static_cast<int>(LED::SLIDE), true);
	}

	//ステップ:指定ステップ数に応じたLEDを設定する
	setStepLED(_trackStep);


	//SW ノート:演奏バンクを更新
	for (i=static_cast<int>(Switch::BANK_D) ; i>=static_cast<int>(Switch::BANK_A) ; i--){
		if (_onClickSwtich[i]) {
			_trackMap->tracks[_track].trackSteps[_trackStep].bank = i - static_cast<int>(Switch::BANK_A);
			break;
		}
	}

	//パターン:演奏するパターンを選択
	for (i=0;i<SEQUENCE_PATTERN_LENGTH ;i++){
		int _patern_index;
		_patern_index=_scanPatternSwich[i];	

		//押下中ノートボタンがあれば、パターン選択中とみなす
		if (_currentSwtich[_patern_index]){
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

		Serial.print(" utilityMode::execStopSequence Switch::NEXT");
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

		Serial.print(" utilityMode::execStopSequence Switch::BACK");
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
				if(_trackMap->tracks[_track].trackSteps[i].lastStep){
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
void	utilityMode::execRunClock() {
}

/*
ラン/ストップフラグ:ストップ MIDIクロックカウント処理を実行
*/
void	utilityMode::execStopClock() {
}

