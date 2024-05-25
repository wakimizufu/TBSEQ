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
    explicit paternWrite(	panelManager * ptPanelManager , voltage * ptVoltage	):mode(MODE_NAME::PATERN_WRITE	,	ptPanelManager	,	ptVoltage	){

			//各状態を初期値に変更する
			_run_stop	=	RUN_STOP::STOP;		//ラン/ストップフラグ
    	_pattern	=	0;	//指定パターン
    	_step			=	0;	//現在ステップ
    }

		/*
		[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
		*/
    virtual void runSequence(){

			//ラン/ストップフラグ:ラン
			if	(	_run_stop	==	RUN_STOP::RUN	)	{
				execRunSequence();

			//ラン/ストップフラグ:ストップ
			}	else	if	(	_run_stop	==	RUN_STOP::STOP	)	{
				execStopSequence();
			}
		}


		/*
		[仮想関数]MIDIクロックカウント閾値達成時に実行されるアプリケーションを実施する
		*/
    virtual void runClock(){

			//ラン/ストップフラグ:ラン
			if	(	_run_stop	==	RUN_STOP::RUN	)	{
				execRunClock();

			//ラン/ストップフラグ:ストップ
			}	else	if	(	_run_stop	==	RUN_STOP::STOP	)	{
				execStopClock();
			}
		}


		/*
		ラン/ストップフラグ:ラン シークエンス処理を実行
		*/
		void	execRunSequence(){
		}

		/*
		ラン/ストップフラグ:ストップ シークエンス処理を実行
		*/
		void	execStopSequence(){
		}

		/*
		ラン/ストップフラグ:ラン MIDIクロックカウント処理を実行
		*/
		void	execRunClock(){
		}

		/*
		ラン/ストップフラグ:ストップ MIDIクロックカウント処理を実行
		*/
		void	execStopClock(){
		}

    private:
    	int	_pattern;	//指定パターン
    	int	_step;		//現在ステップ
};




#endif


