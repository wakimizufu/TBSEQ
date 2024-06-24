#include "trackPlay.h"

/*
コンストラクタ
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
*/
trackPlay::trackPlay(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) :mode(MODE_NAME::TRACK_PLAY, ptPanelManager, ptVoltage, ptSequenceMap) {
}

/*
[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
*/
void trackPlay::runSequence() {
}

/*
[仮想関数]MIDIクロックカウント閾値達成時に実行されるアプリケーションを実施する
*/
void trackPlay::runClock() {
}
