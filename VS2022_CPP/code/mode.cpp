#include "mode.h"

/*
コンストラクタ
modename      :モード名(Class MODE_NAME)
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
	ptSequenceMap :sequenceMapクラスポインタ
*/
mode::mode(MODE_NAME modename, panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) {
	_modeName = modename;
	_panelManager = ptPanelManager;		//panelManagerクラスポインタ
	_voltage = ptVoltage;						//【コンストラクタで設定】voltageクラスポインタ
	_sequenceMap = ptSequenceMap;		//【コンストラクタで設定】sequenceMapクラスポインタ

	_run_stop = RUN_STOP::STOP;
	_midiclock_16note = MIDICLOCK_START_16NOTE;
}

/*
モード名を取得する
戻り値：モード名(Class MODE_NAME)
*/
MODE_NAME	mode::getModeName() {
	return	_modeName;
}

/*
ラン/ストップフラグを取得する
戻り値：ラン/ストップフラグ(Class RUN_STOP)
*/
RUN_STOP	mode::getRunStop() {
	return	_run_stop;
}