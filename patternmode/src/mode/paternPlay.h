/*
モードクラス：パターンプレイ用クラスを定義
*/

#ifndef paternPlay_h
#define paternPlay_h

#include "mode.h"

class paternPlay: public mode{

    public:
     /*
    コンストラクタ
		ptPanelManager:panelManagerクラスポインタ
		ptVoltage     :voltageクラスポインタ
    */
		paternPlay(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap);

		/*
		[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
		*/
		virtual void runSequence();

		/*
		[仮想関数]MIDIクロックカウント閾値達成時に実行されるアプリケーションを実施する
		*/
		virtual void runClock();


		/*
		ラン/ストップフラグ:ラン シークエンス処理を実行
		*/
		void	execRunSequence();

		/*
		ラン/ストップフラグ:ストップ シークエンス処理を実行
		*/
		void	execStopSequence();

		/*
		ラン/ストップフラグ:ラン MIDIクロックカウント処理を実行
		*/
		void	execRunClock();

		/*
		ラン/ストップフラグ:ストップ MIDIクロックカウント処理を実行
		*/
		void	execStopClock();


		/*
		ラン/ストップ切替チェック
		*/
		void	changeRunStop();

		/*
		ラン/ストップ切替チェック
		*/
		void	changeRunStop();
		
  private:
  	int		_pattern;	//指定パターン(1-8)
  	int		_step;		//現在ステップ(1-16)
  	int		_LEDCount;	//LED点滅カウント
  	bool	_pushRunSW;	//ラン/ストップSW前回状態フラグ
  	bool	_keyborad;	//キーボードフラグ
	bool _currentSwtich[SW_INDEX_MAX];	//ボタン押下中

	//パターン番号：スイッチ対応配列
	const int _scanPatternSwich[SEQUENCE_PATTERN_LENGTH] {
		static_cast<int>(Switch::C),
		static_cast<int>(Switch::D),
		static_cast<int>(Switch::E),
		static_cast<int>(Switch::F),
		static_cast<int>(Switch::G),
		static_cast<int>(Switch::A),
		static_cast<int>(Switch::B),
		static_cast<int>(Switch::C2)
	};

	//パターン番号：LED対応配列
	const int _scanPatternLED[SEQUENCE_PATTERN_LENGTH] {
		static_cast<int>(LED::C),
		static_cast<int>(LED::D),
		static_cast<int>(LED::E),
		static_cast<int>(LED::F),
		static_cast<int>(LED::G),
		static_cast<int>(LED::A),
		static_cast<int>(LED::B),
		static_cast<int>(LED::C2)
	}


		/*
		16音符毎MIDIクロックカウントが後半クロックになったらゲートをオフする
		*/
	void _gate_off_16note();

};




#endif


