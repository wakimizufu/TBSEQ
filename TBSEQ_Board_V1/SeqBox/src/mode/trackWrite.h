/*
モードクラス：トラックライト用クラスを定義
*/

#ifndef trackWrite_h
#define trackWrite_h

#include "mode.h"

class trackWrite: public mode{

    public:
		/*
		コンストラクタ
		ptPanelManager:panelManagerクラスポインタ
		ptVoltage     :voltageクラスポインタ
		ptSequenceMap :sequenceMapクラスポインタ
		ptTrackMap    :trackMapクラスポインタ
		*/
		trackWrite(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, trackMap* ptTrackMap, int _initTrack);

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

		/*
		ラン/ストップフラグ:ストップ 転調設定を実行
		*/
		bool	execTransport();

    private:
		int		_LEDCount;	//LED点滅カウント
		bool	_pushRunSW;	//ラン/ストップSW前回状態フラグ

		int	_next_pattern;	//次に演奏する指定パターン(1-8)

		bool _LEDtempo; //テンポカウント時LED点灯フラグ
		int	 _LEDstep;	//テンポカウント時ステップカウンタ


};




#endif


