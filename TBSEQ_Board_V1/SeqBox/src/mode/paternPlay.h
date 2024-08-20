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
		ラン/ストップフラグ:ストップ キーボード機能
		戻り値:true->キーボード機能実行中. false->通常処理へ遷移する
		*/
		bool	execKeyborad();


		/*
		指定パターンを取得する
		戻り値:指定パターン(1-8)
		*/
		int	getCurrnetPattern();

		
  private:
		int		_LEDCount;	//LED点滅カウント
		bool	_pushRunSW;	//ラン/ストップSW前回状態フラグ

		int	_next_pattern;	//次に演奏する指定パターン(1-8)

		bool _LEDtempo; //テンポカウント時LED点灯フラグ
		int	 _LEDstep;	//テンポカウント時ステップカウンタ

		/*
		16音符毎MIDIクロックカウントが後半クロックになったらゲートをオフする
		*/
		void _gate_on_16note();

		/*
		16音符毎MIDIクロックカウントが後半クロックになったらゲートをオフする
		*/
		void _gate_off_16note();

		/*
		16音符毎MIDIクロックカウントが最終クロックになったら次に演奏するステップを決定する
		*/
		void _next_step_16note();

};




#endif


