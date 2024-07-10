#include "debugMode.h"

debugMode::debugMode(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) :mode(MODE_NAME::DEBUG, ptPanelManager, ptVoltage, ptSequenceMap) {

	//各状態を初期値に変更する
	_run_stop = RUN_STOP::STOP;		//ラン/ストップフラグ
    _midiClock=1;	//MIDIクロック数
    _Step=1;		//現在ステップ

    for ( int i=0 ; i<SW_ROW_MAX ; i++){
		_currentSwtich[i]=false;
	}

  	//<LEDマトリクス>
    //全部消灯
	ptPanelManager->setLEDRow(LED_ROW_0,0x00);
	ptPanelManager->setLEDRow(LED_ROW_1,0x00);
	ptPanelManager->setLEDRow(LED_ROW_2,0x00);
	ptPanelManager->setLEDRow(LED_ROW_3,0x00);

    //<C::voltage>
    //reset()
    //<LED gate/acc/slide>
    //全部消灯
	ptVoltage->init();
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
		if ( ptPanelManager->get(i) != _currentSwtich[i] ){
			currentSwtich[i] = ptPanelManager->get(i);
		}

		keyOn = keyOn || currentSwtich[i];

		//<LEDマトリクス>
		//ボタン押下中変数の内容で表示を更新する
		ptPanelManager->setLED(i,currentSwtich[i]);
	}


	/*
    <LED acc>
    ボタン押下中変数で1個以上押下あり
　　⇒点灯
    ボタン押下中変数で押下なし
　　⇒消灯
	*/
	ptVoltage->gate(keyOn);


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
