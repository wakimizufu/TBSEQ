/*
モードクラス：ユーティリティーモード用クラスを定義
*/

#ifndef utilityMode_h
#define utilityMode_h

#include "mode.h"

class utilityMode: public mode{

    public:
		/*
		コンストラクタ
		ptPanelManager:panelManagerクラスポインタ
		ptVoltage     :voltageクラスポインタ
		ptSequenceMap :sequenceMapクラスポインタ
		ptTrackMap    :trackMapクラスポインタ
		*/
		utilityMode(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, trackMap* ptTrackMap);

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
			bool _syncPolarity;		//シンク信号極性   (false:立ち上がり, true:立下り)
			bool _syncTempo;			//テンポ同期ソース (false:シンク信号, true:MIDI IN)

};




#endif


