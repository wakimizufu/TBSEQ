#include "debugMode.h"

debugMode::debugMode(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) :mode(MODE_NAME::DEBUG, ptPanelManager, ptVoltage, ptSequenceMap) {

	//各状態を初期値に変更する
	_run_stop = RUN_STOP::STOP;		//ラン/ストップフラグ
	_pattern = 0;	//指定パターン
	_step = 0;	//現在ステップ
}

/*
[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
*/
void debugMode::runSequence() {

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
void debugMode::runClock() {

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
void	debugMode::execRunSequence() {
}

/*
ラン/ストップフラグ:ストップ シークエンス処理を実行
*/
void	debugMode::execStopSequence() {
}

/*
ラン/ストップフラグ:ラン MIDIクロックカウント処理を実行
*/
void	debugMode::execRunClock() {
}

/*
ラン/ストップフラグ:ストップ MIDIクロックカウント処理を実行
*/
void	debugMode::execStopClock() {
}
