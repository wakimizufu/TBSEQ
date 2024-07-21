#include "paternWrite.h"

paternWrite::paternWrite(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, int _initPattern ) :mode(MODE_NAME::PATERN_WRITE, ptPanelManager, ptVoltage, ptSequenceMap) {

	//各状態を初期値に変更する
	_pattern = _initPattern;				//指定パターン
	_step = STEP_START_IDX;						//現在ステップ
	_LEDCount = 0;								//LED点滅カウント
	_pushRunSW = false;							//ラン/ストップ前回状態フラグ
	_midiclock_16note = MIDICLOCK_START_16NOTE;	//16音符毎MIDIクロックカウント
	_LEDtempo=true;                             //テンポカウント時LED点灯フラグ
	_LEDstep=0;									//テンポカウント時ステップカウンタ

	//ラン/ストップフラグ←ストップ
	_run_stop = RUN_STOP::STOP;

	//voltageクラス 出力内容をリセット
	_voltage->reset();

	//LED出力をクリア
	_panelManager->setLEDRow(LED_ROW_0, 0x00);
	_panelManager->setLEDRow(LED_ROW_1, 0x00);
	_panelManager->setLEDRow(LED_ROW_2, 0xE0);
	_panelManager->setLEDRow(LED_ROW_3, 0x00);
}

/*
[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
*/
void paternWrite::runSequence() {

	//現状入力情報を取得
	//ボタン押下中変数と比較
	for ( int i=0 ; i<SW_INDEX_MAX ; i++){
		if ( _panelManager->getSwitch(i) != _currentSwtich[i] ){
			/*
			Serial.print("paternwrite::runSequence() change index:");
			Serial.print(i);
			Serial.print(" _currentSwtich[i]:");
			Serial.print(_currentSwtich[i]);
			Serial.print(" _panelManager->getSwitch(i):");
			Serial.print(_panelManager->getSwitch(i));
			Serial.println("");*/

			_currentSwtich[i] = _panelManager->getSwitch(i);
		}
	}


	//ラン/ストップ切替チェック
	changeRunStop();

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
void paternWrite::runClock() {

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
ラン/ストップ切替チェック
*/
void	paternPlay::changeRunStop() {

	//現在のラン/ストップSW状態
	bool	nowRunSW = _panelManager->getSwitch(static_cast<int>(Switch::RUN_STOP));

	//前回状態=OFF,現在状態=ON ならモード切替を行う
	if ((!_pushRunSW) && (nowRunSW)) {
		bool _LEDRunStop = false;

		//ラン/ストップフラグ:ラン
		if (_run_stop == RUN_STOP::STOP) {
			_run_stop = RUN_STOP::RUN;
			_LEDRunStop	= true;

		//ラン/ストップフラグ:ストップ
		}
		else	if (_run_stop == RUN_STOP::RUN) {
			_run_stop = RUN_STOP::STOP;
			_LEDRunStop	= false;

		}

		////現在ステップを1に戻す
		_step=1;					

		//_midiclock_16note⇒初期化
		_midiclock_16note = MIDICLOCK_START_16NOTE;

		//LED::RUN_STOP⇒状態に合わせて設定
		_panelManager->setLED(static_cast<int>(LED::RUN_STOP), _LEDRunStop);
	}

	//前回状態←現在状態
	_pushRunSW = nowRunSW;

}



/*
ラン/ストップフラグ:ラン シークエンス処理を実行
*/
void	paternWrite::execRunSequence() {
}

/*
ラン/ストップフラグ:ストップ シークエンス処理を実行
*/
void	paternWrite::execStopSequence() {
	
	int i;

	//指定ステップ情報を取得
	bool _up		=	_sequenceMap->paterns[_pattern].steps[_step].up;
	bool _down		=	_sequenceMap->paterns[_pattern].steps[_step].down;
	bool _acc		=	_sequenceMap->paterns[_pattern].steps[_step].acc;
	bool _slide		=	_sequenceMap->paterns[_pattern].steps[_step].slide;
	bool _laststep 		=	_sequenceMap->paterns[_pattern].steps[_step].lastStep;
	unsigned char _note_on	=	_sequenceMap->paterns[_pattern].steps[_step].note_on;
	unsigned char _note_relative = _sequenceMap->paterns[_pattern].steps[_step].note - static_cast<unsigned char>(NOTE_PWM_INDEX::NOTE_C2);


	//LED 表示更新
	_panelManager->setLEDRow(LED_ROW_0, 0x00);
	_panelManager->setLEDRow(LED_ROW_1, 0x00);
	_panelManager->setLEDRow(LED_ROW_2, 0x00);
	_panelManager->setLEDRow(LED_ROW_3, 0x00);
	_panelManager->setLED(static_cast<int>(LED::PATTERN), true);
	_panelManager->setLED(static_cast<int>(LED::PLAY_WRITE), true);
	
	_panelManager->setLED(static_cast<int>(LED::UP), _up);
	_panelManager->setLED(static_cast<int>(LED::DOWN), _down);
	_panelManager->setLED(static_cast<int>(LED::SLIDE), _slide);
	_panelManager->setLED(static_cast<int>(LED::ACC), _acc);
	_panelManager->setLED(_noteOnLED[_note_relative], true);

	if	(	STEP_NOTE_ON_NORMAL	==	_note_on	)	{
		_panelManager->setLED(static_cast<int>(LED::NOTE), true);
	} else if (	STEP_NOTE_ON_TIE	==	_note_on	)	{
		_panelManager->setLED(static_cast<int>(LED::LED21), true);
	}



	//SW ノート:発音ノートを更新
	for (i=static_cast<int>(Switch::C2) ; i>=static_cast<int>(Switch::C) ; i--){
		if (_currentSwtich[i]) {
			_sequenceMap->paterns[_pattern].steps[_step].note = static_cast<unsigned char>(i) + static_cast<unsigned char>(NOTE_PWM_INDEX::NOTE_C2);
			break;
		}
	}

	//SW ノート:ノートオンを更新
	if	(	_currentSwtich[static_cast<int>(Switch::NOTE)]	)	{
		if	(	STEP_NOTE_ON_NORMAL	==	_note_on	)	{
			_sequenceMap->paterns[_pattern].steps[_step].note_on	=	STEP_NOTE_ON_TIE;
		} else if (	STEP_NOTE_ON_TIE	==	_note_on	)	{
			_sequenceMap->paterns[_pattern].steps[_step].note_on	=	STEP_NOTE_OFF;
		} else if (	STEP_NOTE_OFF	==	_note_on	)	{
			_sequenceMap->paterns[_pattern].steps[_step].note_on	=	STEP_NOTE_ON_NORMAL;
		}
	}

	//SW アクセントを更新
	if	(	_currentSwtich[static_cast<int>(Switch::ACC)]	)	{
		_sequenceMap->paterns[_pattern].steps[_step].acc	=	!_acc;
	}
	

	//SW スライドを更新
	if	(	_currentSwtich[static_cast<int>(Switch::SLIDE)]	)	{
		_sequenceMap->paterns[_pattern].steps[_step].slide	=	!_slide;
	}

	//SW UPを更新
	if	(	_currentSwtich[static_cast<int>(Switch::UP)]	)	{
		_sequenceMap->paterns[_pattern].steps[_step].up	=	!_up;
	}

	//SW DOWNを更新
	if	(	_currentSwtich[static_cast<int>(Switch::DOWN)]	)	{
		_sequenceMap->paterns[_pattern].steps[_step].down	=	!_down;
	}


	//SW NEXT 次のステップに移行
	if	(	_currentSwtich[static_cast<int>(Switch::NEXT)]	)	{
		if(_laststep)	{
			_step	=	STEP_START_IDX;
		} else if (_laststep) {
			_step++;
		}

	//SW BACK 前のステップに移行
	} else if (	_currentSwtich[static_cast<int>(Switch::BACK)]	)	{
		_step--;
		if( step < STEP_START_IDX)	{
			
			_step	=	STEP_START_IDX;

			for (i=STEP_START_IDX ; i<PATERN_STEP_LENGTH ; i++){
				if(_sequenceMap->paterns[_pattern].steps[_step].lastStep){
					_step	=	i;
					break;
				}
			}

		}
	}


	//SW NEXT/BACK 設定したノートを発音する
	if	(	(	_panelManager->getSwitch(static_cast<int>(Switch::NEXT))	)	||
				(	_panelManager->getSwitch(static_cast<int>(Switch::BACK))	) )	{
		
 		int _note_CV=_sequenceMap->paterns[_pattern].steps[_step].note -	static_cast<int>(NOTE_PWM_INDEX::NOTE_C2);
		
		if ( _sequenceMap->paterns[_pattern].steps[_step].up ) {
			_note_CV	=	_note_CV	+	static_cast<int>(NOTE_PWM_INDEX::NOTE_C3);
		} else if ( _sequenceMap->paterns[_pattern].steps[_step].down ) {
			_note_CV	=	_note_CV	+	static_cast<int>(NOTE_PWM_INDEX::NOTE_C1);
		} else{
			_note_CV	=	_note_CV	+	static_cast<int>(NOTE_PWM_INDEX::NOTE_C2);
		}

		_voltage->cv(_note_CV);  //CVを設定する
		_voltage->gate(true);	//gate
		_voltage->accent(_sequenceMap->paterns[_pattern].steps[_step].acc);	//acc
		_voltage->slide(_sequenceMap->paterns[_pattern].steps[_step].slide);//slide

	} else {
		_voltage->gate(false);	//gate
		_voltage->accent(false);	//acc
		_voltage->slide(false);//slide
	}

	
}

/*
ラン/ストップフラグ:ラン MIDIクロックカウント処理を実行
*/
void	paternWrite::execRunClock() {
}

/*
ラン/ストップフラグ:ストップ MIDIクロックカウント処理を実行
*/
void	paternWrite::execStopClock() {
}
