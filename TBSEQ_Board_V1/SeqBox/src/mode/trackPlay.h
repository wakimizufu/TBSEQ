/*
モードクラス：トラックプレイ用クラスを定義
*/

#ifndef trackPlay_h
#define trackPlay_h

#include "mode.h"

class trackPlay: public mode{

	public:
	/*
	コンストラクタ
	ptPanelManager:panelManagerクラスポインタ
	ptVoltage     :voltageクラスポインタ
	ptSequenceMap :sequenceMapクラスポインタ
	ptTrackMap    :trackMapクラスポインタ
	*/
	trackPlay(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, trackMap* ptTrackMap);

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
		指定トラックを取得する
		戻り値:指定トラック(1-13)
		*/
		int	getCurrnetTrack();

		/*
		指定トラックから演奏するバンク/パターンを取得する
		引数:指定トラック(1-13)
			指定トラックステップ(1-16)
		    【ポインタ】演奏バンク(1-4)
		    【ポインタ】演奏パターン(1-8)
		    【ポインタ】転調(1-12)
		    【ポインタ】最終ステップ(true/false)
		*/
		void	getTrack2Pattern(int track, 
								int trackStep,  
								unsigned char* ptBank , 
								unsigned char* ptPattern , 
								unsigned char* ptTransport , 
								bool* ptLastStep);

    private:
			int		_LEDCount;	//LED点滅カウント
			bool	_pushRunSW;	//ラン/ストップSW前回状態フラグ

			int	_next_track;	//次に演奏する指定トラック(1-13)

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


