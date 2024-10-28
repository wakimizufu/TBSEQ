#include "trackWrite.h"

/*
コンストラクタ
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
*/
trackWrite::trackWrite(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, trackMap* ptTrackMap) :mode(MODE_NAME::TRACK_WRITE, ptPanelManager, ptVoltage, ptSequenceMap, ptTrackMap) {
}

/*
[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
*/
void trackWrite::runSequence() {
}

/*
[仮想関数]MIDIクロックカウント閾値達成時に実行されるアプリケーションを実施する
*/
void trackWrite::runClock() {
}