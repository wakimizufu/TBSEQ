/*
各種モード用クラスの基底クラス
*/

#ifndef mode_h
#define mode_h

#include "voltage.h"
#include "panelManager.h"
#include "sequenceMap.h"


//16音符毎MIDIクロックカウント 16分音符:6クロック 開始クロック
#define MIDICLOCK_START_16NOTE	1

//16音符毎MIDIクロックカウント 16分音符:6クロック ゲートオフクロック
#define MIDICLOCK_GATEOFF_16NOTE	4

//16音符毎MIDIクロックカウント 16分音符:6クロック 最終クロック
#define MIDICLOCK_STOP_16NOTE	6


//modeモード名
enum class MODE_NAME
{
PATERN_PLAY,		//パターンプレイ
PATERN_WRITE,		//パターンライト
TRACK_PLAY,			//トラックプレイ
TRACK_WRITE,		//トラックライト
NONE,						//(変化なし)
};


//ラン/ストップフラグ
enum class RUN_STOP
{
	RUN,		//ラン
	STOP,		//ストップ
};


class mode{

    public:

    /*
    コンストラクタ
    modename      :モード名(Class MODE_NAME)
		ptPanelManager:panelManagerクラスポインタ
		ptVoltage     :voltageクラスポインタ
		ptSequenceMap :sequenceMapクラスポインタ
    */
    explicit mode(MODE_NAME modename	,	panelManager * ptPanelManager , voltage * ptVoltage	,	sequenceMap * ptSequenceMap	){
			_modeName	=	modename;
			_panelManager	=	ptPanelManager;		//panelManagerクラスポインタ
			_voltage	=	ptVoltage;						//【コンストラクタで設定】voltageクラスポインタ
			_sequenceMap	=	ptSequenceMap;		//【コンストラクタで設定】sequenceMapクラスポインタ

			_run_stop	=	RUN_STOP::STOP;
			_midiclock_16note=MIDICLOCK_START_16NOTE;
    }

		/*
		[純粋仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
		*/
    virtual void runSequence()=0;

		/*
		[純粋仮想関数]MIDIクロックカウント閾値達成時に実行されるアプリケーションを実施する
		*/
    virtual void runClock()=0;

    /*
    モード名を取得する
    戻り値：モード名(Class MODE_NAME)
    */
    MODE_NAME	getModeName(){
			return	_modeName;
		}

    /*
    ラン/ストップフラグを取得する
    戻り値：ラン/ストップフラグ(Class RUN_STOP)
    */
    RUN_STOP	getRunStop(){
			return	_run_stop;
		}
    
    protected:
    	MODE_NAME	_modeName;	//モード名
			RUN_STOP	_run_stop;		//ラン/ストップフラグ
			
			panelManager * _panelManager;	//【コンストラクタで設定】panelManagerクラスポインタ
			voltage * _voltage;						//【コンストラクタで設定】voltageクラスポインタ
			sequenceMap * _sequenceMap;		//【コンストラクタで設定】sequenceMapクラスポインタ

			int	_midiclock_16note;	//16音符毎MIDIクロックカウント 16分音符:6クロック⇒1～6
};


#endif


