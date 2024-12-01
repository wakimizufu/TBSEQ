#include "modeManager.h"

/*
コンストラクタ
ptPanelManager:panelManagerクラスポインタ
ptVoltage     :voltageクラスポインタ
【未使用】noteThredhold :MIDIクロックカウンタ閾値
start         :カウンタ開始値(デフォルト=0)
*/
modeManager::modeManager(panelManager* ptPanelManager, voltage* ptVoltage, int noteThredhold, int start = 0) :countTriger(THD_MODE_MANAGER, start) {

	_panelManager	=	ptPanelManager;	//panelManagerクラスポインタ
	_voltage		=	ptVoltage;		//【コンストラクタで設定】voltageクラスポインタ
	_debugMode		=	false;			//デバッグフラグ (true->デバッグモード ,false->通常モード)
	_utilityMode	=	false;			//ユーティリティーフラグ (true->ユーティリティーモード ,false->通常モード)
	_bank			=	BANK_START_IDX;	//現在の指定バンク
	_syncPolarity	=	SYNC_TRIGER_POSITIVE;		//シンク信号極性   (false:立ち上がり, true:立下り)
	_syncTempo		=	SYNC_TEMPO_SYNC_SIGNAL;		//テンポ同期ソース (false:シンク信号, true:MIDI IN)

	//ボタン押下中変数を初期化
	for ( int i=0 ; i<SW_INDEX_MAX ; i++){
		_currentSwtich[i] = false;
	}


	//現在のモードクラスの初期値を設定する
	_currentMode = new paternPlay( _panelManager, _voltage, &_sequenceMap);
	_currentMode->setBank(_bank);

	_clockCount = 0;							//現在のMIDIクロックカウンタ値
	_noteThredhold = noteThredhold;	//MIDIクロックカウンタ閾値
}

/*
[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
*/
void modeManager::trigger() {


	//モード切替判定を行う
	_changeMode();

	//モードに対してカウント閾値達成時に実行されるアプリケーションを実施する
	_currentMode->runSequence();
}

/*
MIDIクロックカウンタをインクリメントする
⇒MIDIクロックカウンタをインクリメントした結果閾値に達したらMIDIクロックカウンタを0に設定する
戻り値：true=>MIDIクロックカウンタ＝閾値, false=>MIDIクロックカウンタ＜閾値
*/
bool modeManager::clockCountUp() {
	_currentMode->runClock();
	return	true;
}


/*
【未使用】MIDIクロックカウンタ閾値を設定する
noteThredhold :MIDIクロックカウンタ閾値
戻り値：なし
*/
void	modeManager::setNoteThredhold(int noteThredhold) {
	_noteThredhold = noteThredhold;

	if (_noteThredhold <= _clockCount) {
		_currentMode->runClock();
		_clockCount = 0;
	}
}

/*
押下されたボタン状況を反映する(true->押下中,false->未押下)
戻り値:modeモード名
*/
MODE_NAME modeManager::getModeName() {
	return	_currentMode->getModeName();
}

/*
デバッグモードに変更する
=>デバッグモードから通常モードには戻らない仕様
戻り値：なし
*/
void	modeManager::changeDebugMode(){
	_debugMode = true;	//デバッグフラグ (true->デバッグモード ,false->通常モード)

	//現在のモードクラスの初期値を設定する
	_currentMode = new debugMode( _panelManager, _voltage, &_sequenceMap, &_trackMap);
}

/*
ユーティリティーモードに変更する
=>ユーティリティーモードから通常モードには戻らない仕様
戻り値：なし
*/
void	modeManager::changeUtilityMode(){
	_utilityMode = true;	//ユーティリティーフラグ (true->ユーティリティーモード ,false->通常モード)

	//現在のモードクラスの初期値を設定する
	_currentMode = new utilityMode( _panelManager, _voltage, &_sequenceMap, &_trackMap, _syncPolarity, _syncTempo);
}


/*
シークエンスマップをFRAMからロードして設定する(ロード出来なかったらプリセットを設定する)
戻り値：なし
*/
void	modeManager::presetSequence(){

	int b,i;
	unsigned char _patern_load_bitstream[SEQUENCE_ALLBYTE];
	uint16_t _memAddr;

	//FRAMへの接続を開始する
	bool _connect	=	_panelManager->connectFRAM();

	//シークエンスマップをFRAMからロードして設定する
	if	( _connect ) {

		for (b = BANK_START_IDX; b < SEQUENCE_BANK_LENGTH; b++) {
			_memAddr = SEQUENCE_ALLBYTE * b;

			Serial.print("modeManager::presetSequence() _memAddr:");
			Serial.print(_memAddr,HEX);
			Serial.println("");
			
			_panelManager->loadFRAM(_patern_load_bitstream, _memAddr, SEQUENCE_ALLBYTE);
		
		    /*
			for (i=0 ; i<SEQUENCE_ALLBYTE ; i++){
			Serial.print("_patern_load_bitstream[");
			Serial.print(i,HEX);
			Serial.print("]:");
			Serial.print(_patern_load_bitstream[i],HEX);
			Serial.println("");
			}
			*/

			_sequenceMap.setBitstream(b,_patern_load_bitstream);
		}
		

	} else if (!_connect) {
		presetBitstream _presetBitstream;
		for (b = BANK_START_IDX; b < SEQUENCE_BANK_LENGTH; b++) {
			_sequenceMap.setBitstream(b, _presetBitstream.patern_preset_bitstream);
		}
	}
	
}



/*
シークエンスマップの内容をビットストリームで取得する
引数  ：取得先ビットストリーム
戻り値：なし
*/
void	modeManager::getSequenceBitstream(unsigned char* _bitstream){
  /*
  char tempBitStream[SEQUENCE_ALLBYTE];
  memset(tempBitStream, 0x00, SEQUENCE_ALLBYTE);
  */
  //_sequenceMap->getBitstream(_bitstream);
}



/*
トラックマップをFRAMからロードして設定する(ロード出来なかったらプリセットを設定する)
戻り値：なし
*/
void	modeManager::presetTrack(){

	int t;
	unsigned char _track_load_bitstream[TRACK_ALLBYTE];
	uint16_t _memAddr;

	//FRAMへの接続を開始する
	bool _connect	=	_panelManager->connectFRAM();

	//シークエンスマップをFRAMからロードして設定する
	if	( _connect ) {

		for (t = TRACKMAP_START_IDX; t < TRACKMAP_TRACK_LENGTH; t++) {
			_memAddr = (TRACK_ALLBYTE * t) + TRACKMAP_START_ADDRESS;

			Serial.print("modeManager::presetTrack() _memAddr:");
			Serial.print(_memAddr,HEX);
			Serial.println("");
			
			_panelManager->loadFRAM(_track_load_bitstream, _memAddr, TRACK_ALLBYTE);
		
		    /*
			for (int i=0 ; i<TRACK_ALLBYTE ; i++){
			Serial.print("_track_load_bitstream[");
			Serial.print(i,HEX);
			Serial.print("]:");
			Serial.print(_track_load_bitstream[i],HEX);
			Serial.println("");
			}
			*/

			_trackMap.setBitstream(t,_track_load_bitstream);
		}
		

	} else if (!_connect) {
		presetTrackBitstream _presetTrackBitstream;
		for (t = TRACKMAP_START_IDX; t < TRACKMAP_TRACK_LENGTH; t++) {
			_trackMap.setBitstream(t, _presetTrackBitstream.track_preset_bitstream);
		}
	}
	
}



/*
トラックマップの内容をビットストリームで取得する
引数  ：取得先ビットストリーム
戻り値：なし
*/
void	modeManager::getTrackBitstream(unsigned char* _bitstream){

}

/*
ユーティリティーモードフラグをFRAMからロードして設定する(ロード出来なかったらプリセットを設定する)
戻り値：なし
*/
void	modeManager::presetUtility(){
	int t;
	unsigned char _utility_load_bitstream[UTILITY_ALLBYTE];
	uint16_t _memAddr;

	//FRAMへの接続を開始する
	bool _connect	=	_panelManager->connectFRAM();

	//シークエンスマップをFRAMからロードして設定する
	if	( _connect ) {

		_memAddr = UTILITY_START_ADDRESS;

		Serial.print("modeManager::presetUtility() _memAddr:");
		Serial.print(_memAddr,HEX);
		Serial.println("");
		
		_panelManager->loadFRAM(_utility_load_bitstream, _memAddr, UTILITY_ALLBYTE);
		
		for (int i=0 ; i<UTILITY_ALLBYTE ; i++){
		Serial.print("_utility_load_bitstream[");
		Serial.print(i,HEX);
		Serial.print("]:");
		Serial.print(_utility_load_bitstream[i],HEX);
		Serial.println("");
		}
		
		_syncPolarity	=	(0x01 & _utility_load_bitstream[0]);	//シンク信号極性   (false:立ち上がり, true:立下り)
		_syncTempo		=	(0x02 & _utility_load_bitstream[0]);	//テンポ同期ソース (false:シンク信号, true:MIDI IN)

	} else if (!_connect) {
		_syncPolarity	=	SYNC_TRIGER_POSITIVE;		//シンク信号極性   (false:立ち上がり, true:立下り)
		_syncTempo	=	SYNC_TEMPO_SYNC_SIGNAL;				//テンポ同期ソース (false:シンク信号, true:MIDI IN)
	}
}


/*
モード切替判定を行う
*/
void	modeManager::_changeMode() {

	bool _SwTrack		= false;
	bool _SwPatern	= false;
	bool _SwWrite		= false;


	//現状入力情報を取得
	//ボタン押下中変数と比較
	for ( int i=0 ; i<SW_INDEX_MAX ; i++){
		if ( _panelManager->getSwitch(i) != _currentSwtich[i] ){
			
			/*
			Serial.print("modeManager::_changeMode() change index:");
			Serial.print(i);
			Serial.print(" _currentSwtich[i]:");
			Serial.print(_currentSwtich[i]);
			Serial.print(" _panelManager->getSwitch(i):");
			Serial.print(_panelManager->getSwitch(i));
			Serial.println("");*/
 

			_SwTrack	= (( false == _currentSwtich[static_cast<int>(Switch::TRACK)]) && 
			               (true == _panelManager->getSwitch(static_cast<int>(Switch::TRACK))));
			_SwPatern	= (( false == _currentSwtich[static_cast<int>(Switch::PATTERN)]) && 
			               (true == _panelManager->getSwitch(static_cast<int>(Switch::PATTERN))));
			_SwWrite	= (( false == _currentSwtich[static_cast<int>(Switch::PLAY_WRITE)]) && 
			               (true == _panelManager->getSwitch(static_cast<int>(Switch::PLAY_WRITE))));

			
			Serial.print(" _SwTrack:");
			Serial.print(_SwTrack);
			Serial.print(" _SwPatern:");
			Serial.print(_SwPatern);
			Serial.print(" _SwWrite:");
			Serial.print(_SwWrite);
			Serial.println("");
			
			
			_currentSwtich[i] = _panelManager->getSwitch(i);
		}
	}



	//デバッグモードなら切替処理は行わない
	if (_debugMode) {
	return;
	}

	//ラン/ストップ状態がランなら切替処理は行わない
	if (RUN_STOP::RUN == _currentMode->getRunStop()) {
		return;
	}

	//現在のモード名を取得する
	MODE_NAME	mode = _currentMode->getModeName();

	//ボタン押下状況に応じたモード名を設定
	MODE_NAME changeMode = MODE_NAME::NONE;
	int _currentPatern = PATTERN_START_IDX;
	int _currentTrack  = TRACKMAP_START_IDX;
	bool	_track	=	false;
	bool	_patern	=	false;
	bool	_write	=	false;
	_bank	=	_currentMode->getCurrnetBank();

	if	(	MODE_NAME::PATERN_PLAY == mode ) {				//パターンプレイ
		_currentPatern	=	_currentMode->getCurrnetPattern();
		_track	=	false;
		_patern	=	true;
		_write	=	false;

		/*
		Serial.print("MODE_NAME::PATERN_PLAY _SwWrite:");
		Serial.print(_SwWrite);
		Serial.print(" _SwTrack:");
		Serial.print(_SwTrack);
		Serial.println("");
        */

		if	(	_SwWrite	)	{
			changeMode = MODE_NAME::PATERN_WRITE;
		} else if (	_SwTrack	)	{
			changeMode = MODE_NAME::TRACK_PLAY;
		}

	} else if	(	MODE_NAME::PATERN_WRITE == mode ) {		//パターンライト
		_track	=	false;
		_patern	=	true;
		_write	=	true;

        /*
		Serial.print("MODE_NAME::PATERN_WRITE _SwWrite:");
		Serial.print(_SwWrite);
		Serial.println("");
        */

		if	(	_SwWrite	)	{
			changeMode = MODE_NAME::PATERN_PLAY;
			_currentPatern = _currentMode->getCurrnetPattern();

			Serial.print("MODE_NAME::PATERN_WRITE->PATERN_PLAY");
			Serial.print(" _bank:");
			Serial.print(_bank);
			Serial.print(" _currentPatern:"); 
			Serial.print(_currentPatern);
			Serial.println("");

			//パターン配列からビットストリームに設定する
			unsigned char _current_patern_bitstream[PATTERN_ALLBYTE];
			_sequenceMap.getBitstream( _bank , _currentPatern , _current_patern_bitstream);

			int _startAddr = (_bank * SEQUENCE_ALLBYTE ) + (_currentPatern * PATTERN_ALLBYTE);
			_panelManager->saveFRAM( _startAddr, _current_patern_bitstream , PATTERN_ALLBYTE);
		}

	} else if	(	MODE_NAME::TRACK_PLAY == mode ) {		//トラックプレイ
		_currentTrack  = _currentMode->getCurrnetTrack();
		_track	=	true;
		_patern	=	false;
		_write	=	false;

		if	(	_SwWrite	)	{
			changeMode = MODE_NAME::TRACK_WRITE;
		} else if (	_SwPatern	)	{
			changeMode = MODE_NAME::PATERN_PLAY;
		}

	} else if	(	MODE_NAME::TRACK_WRITE == mode ) {		//トラックライト
		_track	=	true;
		_patern	=	false;
		_write	=	true;
				
		if	(	_SwWrite	)	{
			changeMode = MODE_NAME::TRACK_PLAY;
			_currentTrack  = _currentMode->getCurrnetTrack();

			Serial.print("MODE_NAME::TRACK_WRITE->TRACK_PLAY");
			Serial.print(" _currentTrack:");
			Serial.print(_currentTrack);
			Serial.println("");

			//パターン配列からビットストリームに設定する =>トラックライトのデバッグが終わったら書き込めるようにする
			unsigned char _current_track_bitstream[TRACK_ALLBYTE];
			_trackMap.getBitstream(_currentTrack , _current_track_bitstream);

			int _startAddr = (_currentTrack * TRACK_ALLBYTE) + TRACKMAP_START_ADDRESS;
			_panelManager->saveFRAM( _startAddr, _current_track_bitstream , TRACK_ALLBYTE);		
			
		}

	}


	//モード未選択なら切替なし
	if (MODE_NAME::NONE == changeMode) {
		return;

	//現在モードと切替時モードが一緒なら切替なし
	}
	else if (mode == changeMode) {
		return;
	}


	//異なるモードが選択されたらモードを切り替える
	delete	_currentMode;

	if (MODE_NAME::PATERN_PLAY == changeMode) {
		_currentMode = new paternPlay( _panelManager, _voltage, &_sequenceMap);
		_currentMode->setBank(_bank);
		_currentMode->setPattern(_currentPatern);
	}
	else if (MODE_NAME::PATERN_WRITE == changeMode) {
		_currentMode = new paternWrite( _panelManager, _voltage, &_sequenceMap, _currentPatern);
		_currentMode->setBank(_bank);

	}
	else if (MODE_NAME::TRACK_PLAY == changeMode) {

		Serial.print("MODE_NAME::TRACK_PLAY == changeMode _currentTrack:");
		Serial.print(_currentTrack);
		Serial.println("");

		_currentMode = new trackPlay( _panelManager, _voltage, &_sequenceMap, &_trackMap);
		_currentMode->setTrack(_currentTrack);
	}
	else if (MODE_NAME::TRACK_WRITE == changeMode) {
		_currentMode = new trackWrite( _panelManager, _voltage, &_sequenceMap, &_trackMap, _currentTrack);
		_currentMode->setTrack(_currentTrack);
	}

}



/*
開始ステップを取得する
戻り値：true=>開始ステップ, false=>通常ステップ
*/
bool 	modeManager::getFirstStep(){
	return _currentMode->getFirstStep();
}

/*
開始ステップを設定する
引数：true=>開始ステップ, false=>通常ステップ
*/
void modeManager::setFirstStep(bool value){
	_currentMode->setFirstStep(value);
}


/*
MIDI:スタート受信フラグを設定
引数：true=>受信済, false=>未受信
*/
void	modeManager::setMIDIStart(bool value){
	_currentMode->setMIDIStart(value);
}

/*
MIDI:ストップ受信フラグを設定
引数：true=>受信済, false=>未受信
*/
void	modeManager::setMIDIStop(bool value){
	_currentMode->setMIDIStop(value);
}

/*
シンク信号極性を取得
戻り値：シンク信号極性   (false:立ち上がり, true:立下り)
*/
bool	modeManager::getSyncPolarity(){
	return _syncPolarity;
}

/*
テンポ同期ソースを取得
戻り値：テンポ同期ソース (false:シンク信号, true:MIDI IN)
*/
bool	modeManager::getSyncTempo(){
	return _syncTempo;
}
