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
    */
		paternWrite(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap);

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

    private:
    	int	_pattern;	//指定パターン
    	int	_step;		//現在ステップ
};




#endif


