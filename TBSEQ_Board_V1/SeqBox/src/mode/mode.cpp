#include "mode.h"

/*
コンストラクタ
modename      :モード名(Class MODE_NAME)
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
	ptSequenceMap :sequenceMapクラスポインタ
	ptTrackMap    :trackMapクラスポインタ
*/
mode::mode(MODE_NAME modename, panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, trackMap* ptTrackMap) {
	_modeName = modename;
	_panelManager = ptPanelManager;		//panelManagerクラスポインタ
	_voltage = ptVoltage;						//【コンストラクタで設定】voltageクラスポインタ
	_sequenceMap = ptSequenceMap;		//【コンストラクタで設定】sequenceMapクラスポインタ
	_trackMap = ptTrackMap;             //【コンストラクタで設定】trackMapクラスポインタ

	_run_stop = RUN_STOP::STOP;
	_midiclock_16note = MIDICLOCK_START_16NOTE;
	_MIDI_Start	=	false;	//MIDI:スタート受信フラグ
	_MIDI_Stop	=	false;	//MIDI:ストップ受信フラグ
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
現在の指定トラックを取得する
戻り値：指定トラック(1-13)
*/
int	mode::getCurrnetTrack(){
	return _track;
}

 /*
現在の指定パターンを取得する
戻り値：指定パターン(1-8)
*/
int	mode::getCurrnetPattern(){
	return _pattern;
}

 /*
現在の指定パターンを取得する
戻り値：指定パターン(1-8)
*/
int	mode::getCurrnetBank(){
	return _bank;
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

/*
バンク:指定ステップ数に応じたLEDを設定する
引数  ：指定バンク
*/
void	mode::setBank(int value){

	if ( (value < BANK_START_IDX) || (value >= SEQUENCE_BANK_LENGTH) ) {
		_bank = BANK_START_IDX;
	} else {
		_bank = value;
	}
}

/*
バンク:指定バンク数に応じたLEDを設定する
引数  ：バンク数
*/
void	mode::setBackLED(int value){

	_panelManager->setLED(static_cast<int>(LED::BANK_A), false);	
	_panelManager->setLED(static_cast<int>(LED::BANK_B), false);	
	_panelManager->setLED(static_cast<int>(LED::BANK_C), false);	
	_panelManager->setLED(static_cast<int>(LED::BANK_D), false);	

	if ( 0 == value){
		_panelManager->setLED(static_cast<int>(LED::BANK_A), true);	
	}

	if ( 1 == value){
		_panelManager->setLED(static_cast<int>(LED::BANK_B), true);	
	}

	if ( 2 == value){
		_panelManager->setLED(static_cast<int>(LED::BANK_C), true);	
	}

	if ( 3 == value){
		_panelManager->setLED(static_cast<int>(LED::BANK_D), true);	
	}
}


/*
パターン:指定したパターンへ変更する
戻り値：指定パターン
*/
void	mode::setPattern(int value){

	if ( (value < PATTERN_START_IDX) || (value >= SEQUENCE_PATTERN_LENGTH) ) {
		_pattern = PATTERN_START_IDX;
	} else {
		_pattern = value;
	}
}

/*
トラック:指定したトラックへ変更する
戻り値：指定トラック(1-13)
*/
void	mode::setTrack(int value){

	if ( (value < TRACKMAP_START_IDX) || (value >= TRACKMAP_TRACK_LENGTH) ) {
		_track = TRACKMAP_START_IDX;
	} else {
		_track = value;
	}
}

/*
現在のステップ開始を取得する
戻り値：ステップ開始
*/
bool	mode::getFirstStep(){
	return	_StartStep;
}


/*
ステップ:ステップ開始を設定する
引数：ステップ開始
*/
void	mode::setFirstStep(bool value){
	_StartStep	=	value;
}


/*
MIDI:スタート受信フラグを設定
引数：true=>受信済, false=>未受信
*/
void	mode::setMIDIStart(bool value){
	_MIDI_Start=value;
}

/*
MIDI:ストップ受信フラグを設定
引数：true=>受信済, false=>未受信
*/
void	mode::setMIDIStop(bool value){
	_MIDI_Stop=value;
}



/*
指定トラックから演奏するバンク/パターンを取得する
引数:指定トラック(1-13)
	演奏トラックステップ(1-16)
	演奏バンク(1-4)
	演奏パターン(1-8)
*/
void	mode::getTrack2Pattern(int track, 
								int trackStep,  
								int* ptBank , 
								int* ptPattern , 
								int* ptTransport , 
								bool* ptLastStep){
	*ptBank			=	_trackMap->tracks[track].trackSteps[trackStep].bank;
	*ptPattern		=	_trackMap->tracks[track].trackSteps[trackStep].pattern;
	*ptTransport	=	_trackMap->tracks[track].trackSteps[trackStep].transport;
	*ptLastStep		=	_trackMap->tracks[track].trackSteps[trackStep].lastStep;
}

