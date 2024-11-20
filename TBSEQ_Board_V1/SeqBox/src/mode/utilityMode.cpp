#include "utilityMode.h"

/*
コンストラクタ
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
*/
utilityMode::utilityMode(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, trackMap* ptTrackMap, bool bSyncPolarity, bool bSyncTempo) :mode(MODE_NAME::UTILITY, ptPanelManager, ptVoltage, ptSequenceMap, ptTrackMap) {

	_syncPolarity	=	bSyncPolarity;	//シンク信号極性   (false:立ち上がり, true:立下り)
	_syncTempo		=	bSyncTempo;		//テンポ同期ソース (false:シンク信号, true:MIDI IN)

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
	
	//LED 表示更新
	_panelManager->setLEDRow(LED_ROW_0, 0x0000);
	_panelManager->setLEDRow(LED_ROW_1, 0x0000);
	_panelManager->setLED(static_cast<int>(LED::TRACK), true);
	_panelManager->setLED(static_cast<int>(LED::PATTERN), true);
	_panelManager->setLED(static_cast<int>(LED::PLAY_WRITE), true);
	
	//LED シンク信号極性を表示
	if (_syncPolarity==SYNC_TRIGER_POSITIVE){
		_panelManager->setLED(static_cast<int>(LED::C), true);
	} else if (	_syncPolarity==SYNC_TRIGER_NEGATIVE	)	{
		_panelManager->setLED(static_cast<int>(LED::D), true);
	}

	//LED テンポ同期ソースを表示
	if (_syncTempo==SYNC_TEMPO_SYNC_SIGNAL){
		_panelManager->setLED(static_cast<int>(LED::E), true);
	} else if (	_syncTempo==SYNC_TEMPO_MIDI_IN	)	{
		_panelManager->setLED(static_cast<int>(LED::F), true);
	}


	//SW シンク信号極性を設定
	if	(	_onClickSwtich[static_cast<int>(Switch::C)]	)	{
		_syncPolarity=SYNC_TRIGER_POSITIVE;		//false:立ち上がり
	} else if (	_onClickSwtich[static_cast<int>(Switch::D)]	)	{
		_syncPolarity=SYNC_TRIGER_NEGATIVE;		//true:立下り
	}

	//SW テンポ同期ソースを設定
	if	(	_onClickSwtich[static_cast<int>(Switch::E)]	)	{
		_syncTempo=SYNC_TEMPO_SYNC_SIGNAL;	//シンク信号同期
	} else if (	_onClickSwtich[static_cast<int>(Switch::F)]	)	{
		_syncTempo=SYNC_TEMPO_MIDI_IN;	//MIDI IN同期
	}


	//SW NEXT＋SW BACK 設定を書き込んでリセットする
	if	(	(	_currentSwtich[static_cast<int>(Switch::NEXT)]	)	&&
			(	_currentSwtich[static_cast<int>(Switch::BACK)]	)	) {

		//パターン配列からビットストリームに設定する =>トラックライトのデバッグが終わったら書き込めるようにする
		unsigned char _current_utility_bitstream[UTILITY_ALLBYTE];
		_current_utility_bitstream[0] = 0x00;

		//シンク信号極性   (false:立ち上がり, true:立下り)
		if (_syncPolarity){
			_current_utility_bitstream[0] |= 0x01;
		}

		//テンポ同期ソース (false:シンク信号, true:MIDI IN)
		if (_syncTempo){
			_current_utility_bitstream[0] |= 0x02;
		}

		int _startAddr = UTILITY_START_ADDRESS;
		_panelManager->saveFRAM( _startAddr, _current_utility_bitstream , UTILITY_ALLBYTE);		

		//リセットする
    	rp2040.reboot();
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

