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

 /*
現在の指定パターンを取得する
戻り値：指定パターン(1-8)
*/
int	mode::getCurrnetPattern(){
	return _pattern;
}

/*
指定スイッチが押下したタイミングか取得する
引数  ：class enum Switch値(intに変換して設定する)
戻り値：true:押下したタイミング false:その他の状態
*/
bool	mode::isSwitchOnClick(int _SwIndex){
	bool _ret	=	(( false == _currentSwtich[_SwIndex]) &&  (true == _panelManager->getSwitch(_SwIndex)));
	return _ret;
}

/*
指定スイッチが押下⇒離れたタイミングか取得する
引数  ：class enum Switch値(intに変換して設定する)
戻り値：true:押下⇒離れたタイミング false:その他の状態
*/
bool	mode::isSwitchOffClick(int _SwIndex){
	bool _ret	=	(( true == _currentSwtich[_SwIndex]) &&  (false == _panelManager->getSwitch(_SwIndex)));
	return _ret;
}

/*
ステップ:指定ステップ数に応じたLEDを設定する
*/
void	mode::setStepLED(int value){

	_panelManager->setLED(static_cast<int>(LED::STEP_8), false);	
	_panelManager->setLED(static_cast<int>(LED::STEP_4), false);	
	_panelManager->setLED(static_cast<int>(LED::STEP_2), false);	
	_panelManager->setLED(static_cast<int>(LED::STEP_1), false);	

	if ( value & 0x0008){
		_panelManager->setLED(static_cast<int>(LED::STEP_8), true);	
	}

	if ( value & 0x0004){
		_panelManager->setLED(static_cast<int>(LED::STEP_4), true);	
	}

	if ( value & 0x0002){
		_panelManager->setLED(static_cast<int>(LED::STEP_2), true);	
	}

	if ( value & 0x0001){
		_panelManager->setLED(static_cast<int>(LED::STEP_1), true);	
	}
}
