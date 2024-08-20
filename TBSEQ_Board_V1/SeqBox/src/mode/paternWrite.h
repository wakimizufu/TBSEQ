/*
モードクラス：パターンライト用クラスを定義
*/

#ifndef paternWrite_h
#define paternWrite_h

#include "mode.h"

class paternWrite: public mode{

    public:
     /*
    コンストラクタ
		ptPanelManager:panelManagerクラスポインタ
		ptVoltage     :voltageクラスポインタ
		sequenceMap   :sequenceMapクラスポインタ
		_initPattern  :編集対象パターン(1～16)
		*/
		paternWrite(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, int _initPattern);

		/*
		[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
		*/
		virtual void runSequence();


		/*
		[仮想関数]MIDIクロックカウント閾値達成時に実行されるアプリケーションを実施する
		*/
		virtual void runClock();


		/*
		ラン/ストップ切替チェック
		*/
		void	changeRunStop();

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

    private:
		int		_LEDCount;	//LED点滅カウント
		bool	_pushRunSW;	//ラン/ストップSW前回状態フラグ

		int	_next_pattern;	//次に演奏する指定パターン(1-8)

		bool _LEDtempo; //テンポカウント時LED点灯フラグ
		int	 _LEDstep;	//テンポカウント時ステップカウンタ
};




#endif


