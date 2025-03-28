/*
各種モード用クラスの基底クラス
*/

#ifndef mode_h
#define mode_h

 #include "../raspberryPiPico/voltage.h"
 #include "../raspberryPiPico/panelManager.h"
 #include "sequenceMap.h"


//16音符毎MIDIクロックカウント 16分音符:6クロック 開始クロック
#define MIDICLOCK_START_16NOTE	1

//16音符毎MIDIクロックカウント 16分音符:6クロック ゲートオフクロック
#define MIDICLOCK_GATEOFF_16NOTE	4

//16音符毎MIDIクロックカウント 16分音符:6クロック 最終クロック
#define MIDICLOCK_STOP_16NOTE	6

//16音符毎MIDIクロックカウント 最大クロック数
#define MIDICLOCK_MAX	96

//16音符毎MIDIクロックカウント 開始クロック
#define MIDICLOCK_START	1


//16音符カウント 16分音符:最大ステップ数
#define MIDI_STEP_MAX	16

//16音符カウント 16分音符:開始ステップ数
#define MIDI_STEP_START	1


//ノートオン スイッチ&LED総数
#define NOTEON_SWLED_MAX	13


//modeモード名
enum class MODE_NAME
{
DEBUG,				//デバッグモード
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
	mode(MODE_NAME modename, panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap);

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
	MODE_NAME	getModeName();

    /*
    ラン/ストップフラグを取得する
    戻り値：ラン/ストップフラグ(Class RUN_STOP)
    */
	RUN_STOP	getRunStop();

    /*
    現在の指定パターンを取得する
    戻り値：指定パターン(1-8)
    */
	int	getCurrnetPattern();

    /*
    指定スイッチが押下したタイミングか取得する
    引数  ：class enum Switch値(intに変換して設定する)
    戻り値：true:押下したタイミング false:その他の状態
    */
	bool	isSwitchOnClick(int _SwIndex);

    /*
    指定スイッチが押下⇒離れたタイミングか取得する
    引数  ：class enum Switch値(intに変換して設定する)
    戻り値：true:押下⇒離れたタイミング false:その他の状態
    */
	bool	isSwitchOffClick(int _SwIndex);

    protected:
    	MODE_NAME	_modeName;	//モード名
		RUN_STOP	_run_stop;		//ラン/ストップフラグ
			
		panelManager * _panelManager;	//【コンストラクタで設定】panelManagerクラスポインタ
		voltage * _voltage;						//【コンストラクタで設定】voltageクラスポインタ
		sequenceMap * _sequenceMap;		//【コンストラクタで設定】sequenceMapクラスポインタ

		int	_midiclock_16note;	//16音符毎MIDIクロックカウント 16分音符:6クロック⇒1～6

		int	_pattern;	//指定パターン(1-8)
		int	_step;		//現在ステップ(1-16)
		bool _currentSwtich[SW_INDEX_MAX];	//ボタン押下中
		bool _onClickSwtich[SW_INDEX_MAX];	//ボタン押下タイミング
		bool _offClickSwtich[SW_INDEX_MAX];	//ボタン押下⇒離したタイミング

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
		};

		//note_on：スイッチ対応配列
		const int _noteOnSwich[NOTEON_SWLED_MAX] {
			static_cast<int>(Switch::C),
			static_cast<int>(Switch::CSHARP),
			static_cast<int>(Switch::D),
			static_cast<int>(Switch::DSHARP),
			static_cast<int>(Switch::E),
			static_cast<int>(Switch::F),
			static_cast<int>(Switch::FSHARP),
			static_cast<int>(Switch::G),
			static_cast<int>(Switch::GSHARP),
			static_cast<int>(Switch::A),
			static_cast<int>(Switch::ASHARP),
			static_cast<int>(Switch::B),
			static_cast<int>(Switch::C2)
		};

		//note_on：LED対応配列
		const int _noteOnLED[NOTEON_SWLED_MAX] {
			static_cast<int>(LED::C),
			static_cast<int>(LED::CSHARP),
			static_cast<int>(LED::D),
			static_cast<int>(LED::DSHARP),
			static_cast<int>(LED::E),
			static_cast<int>(LED::F),
			static_cast<int>(LED::FSHARP),
			static_cast<int>(LED::G),
			static_cast<int>(LED::GSHARP),
			static_cast<int>(LED::A),
			static_cast<int>(LED::ASHARP),
			static_cast<int>(LED::B),
			static_cast<int>(LED::C2)
		};


};


#endif


