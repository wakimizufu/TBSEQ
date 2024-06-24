#include "paternPlay.h"

/*
コンストラクタ
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
*/
paternPlay::paternPlay(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) :mode(MODE_NAME::PATERN_PLAY, ptPanelManager, ptVoltage, ptSequenceMap) {

	//各状態を初期値に変更する
	_pattern = PATTERN_START_IDX;						//指定パターン
	_step = STEP_START_IDX;								//現在ステップ
	_LEDCount = 0;														//LED点滅カウント
	_pushRunSW = false;												//ラン/ストップ前回状態フラグ
	_keyborad = false;												//キーボードフラグ
	_midiclock_16note = MIDICLOCK_START_16NOTE;	//16音符毎MIDIクロックカウント

	//ラン/ストップフラグ←ストップ
	_run_stop = RUN_STOP::STOP;

	//voltageクラス 出力内容をリセット
	_voltage->reset();

	//LED出力をクリア
	_panelManager->setLEDRow(LED_ROW_0, 0x00);
	_panelManager->setLEDRow(LED_ROW_1, 0x00);
	_panelManager->setLEDRow(LED_ROW_2, 0x60);
	_panelManager->setLEDRow(LED_ROW_3, 0x00);
}

/*
[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
*/
void paternPlay::runSequence() {

	//ラン/ストップ切替チェック
	changeRunStop();

	//指定パターンに対応したLEDをリセット
	_panelManager->setLEDRow(LED_ROW_0, 0x00);
	_panelManager->setLEDRow(LED_ROW_1, 0x00);

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
void paternPlay::runClock() {

	//ラン/ストップフラグ:ラン
	if (_run_stop == RUN_STOP::RUN) {
		execRunClock();

		//ラン/ストップフラグ:ストップ
	}
	else	if (_run_stop == RUN_STOP::STOP) {
		execStopClock();
	}

	//16音符毎MIDIクロックカウントを更新
	_midiclock_16note++;

	//16音符毎MIDIクロックカウントが7カウント目なら1カウントに戻す
	if (_midiclock_16note > MIDICLOCK_STOP_16NOTE) {
		_midiclock_16note = MIDICLOCK_START_16NOTE;

		//16音符毎MIDIクロックアップしたら現在ステップをインクリメント
		_step++;

		//現在ステップが最終カウントを超えたら開始ステップに戻す
		if (_step >= PATERN_STEP_LENGTH) {
			_step = PATTERN_START_IDX;
		}

	}
}


/*
ラン/ストップフラグ:ラン シークエンス処理を実行
*/
void	paternPlay::execRunSequence() {
	_panelManager->setLED(_get_pattrn_LED(_pattern), true);
}

/*
ラン/ストップフラグ:ストップ シークエンス処理を実行
*/
void	paternPlay::execStopSequence() {

	//パターン選択をチェック
	if (_panelManager->getSwitch(static_cast<int>(Switch::C))) {
		_pattern = 0;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::D))) {
		_pattern = 1;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::E))) {
		_pattern = 2;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::F))) {
		_pattern = 3;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::G))) {
		_pattern = 4;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::A))) {
		_pattern = 5;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::B))) {
		_pattern = 6;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::C2))) {
		_pattern = 7;
	}

	//パターン選択LEDを点灯
	_panelManager->setLED(_get_pattrn_LED(_pattern), true);

}

/*
ラン/ストップフラグ:ラン MIDIクロックカウント処理を実行
*/
void	paternPlay::execRunClock() {
}

/*
ラン/ストップフラグ:ストップ MIDIクロックカウント処理を実行
*/
void	paternPlay::execStopClock() {
}


/*
ラン/ストップ切替チェック
*/
void	paternPlay::changeRunStop() {

	//現在のラン/ストップSW状態
	bool	nowRunSW = _panelManager->getSwitch(static_cast<int>(Switch::RUN));

	//前回状態=OFF,現在状態=ON ならモード切替を行う
	if ((!_pushRunSW) && (nowRunSW)) {

		//ラン/ストップフラグ:ラン
		if (_run_stop == RUN_STOP::STOP) {
			_run_stop = RUN_STOP::RUN;
			_panelManager->setLED(static_cast<int>(LED::RUN), true);
			_midiclock_16note = MIDICLOCK_START_16NOTE;

			//ラン/ストップフラグ:ストップ
		}
		else	if (_run_stop == RUN_STOP::RUN) {
			_run_stop = RUN_STOP::STOP;
			_panelManager->setLED(static_cast<int>(LED::RUN), false);
			_midiclock_16note = MIDICLOCK_START_16NOTE;
		}
	}

	//前回状態←現在状態
	_pushRunSW = nowRunSW;

}


	  /*
	  16音符毎MIDIクロックカウントが後半クロックになったらゲートをオフする
	  */
	  void paternPlay::_gate_off_16note() {

		  bool	_sSlide = _sequenceMap->paterns[_pattern].steps[_step].slide;

		  if ((_midiclock_16note == MIDICLOCK_GATEOFF_16NOTE) && (!_sSlide)) {
			  _voltage->gate(false);
		  }
	  }

	  /*
	  指定パターンに応じたclass:LED値を取得
	  */
	  int	paternPlay::_get_pattrn_LED(int pattern) {
		  int	result = static_cast<int>(LED::C);

		  switch (pattern) {
		  case 0:
			  result = static_cast<int>(LED::C);
			  break;
		  case 1:
			  result = static_cast<int>(LED::D);
			  break;
		  case 2:
			  result = static_cast<int>(LED::E);
			  break;
		  case 3:
			  result = static_cast<int>(LED::F);
			  break;
		  case 4:
			  result = static_cast<int>(LED::G);
			  break;
		  case 5:
			  result = static_cast<int>(LED::A);
			  break;
		  case 6:
			  result = static_cast<int>(LED::B);
			  break;
		  case 7:
			  result = static_cast<int>(LED::C2);
			  break;
		  default:
			  break;
		  }

		  return	result;
	  }