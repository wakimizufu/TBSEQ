/*
各種モード切替/管理する
*/

#ifndef modeManager_h
#define modeManager_h

//arduino ideでコンパイルする際は以下をコメント解除する
/*
#define DEBUG_ARDUINO 0

#if defined(DEBUG_ARDUINO)
#else
	#include <iostream>
#endif
*/

/* #include "Arduino.h" */
/* #include "Wire.h" */
#include "mode.h"
#include "debugMode.h"
#include "paternPlay.h"
#include "paternWrite.h"
#include "trackPlay.h"
#include "trackWrite.h"

//読み取り間隔カウンタ閾値 10.24msec (32us * 320カウント)
#define THD_MODE_MANAGER 160



class modeManager: public countTriger
{
  public:

		/*
		コンストラクタ
		ptPanelManager:panelManagerクラスポインタ
		ptVoltage     :voltageクラスポインタ
		noteThredhold :MIDIクロックカウンタ閾値
		start         :カウンタ開始値(デフォルト=0)
		*/
	  modeManager(panelManager* ptPanelManager, voltage* ptVoltage, int noteThredhold, int start);

		/*
		[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
		*/
	  virtual void trigger();

		/*
		MIDIクロックカウンタをインクリメントする
		⇒MIDIクロックカウンタをインクリメントした結果閾値に達したらMIDIクロックカウンタを0に設定する
		戻り値：true=>MIDIクロックカウンタ＝閾値, false=>MIDIクロックカウンタ＜閾値
		*/
	  bool clockCountUp();


		/*
		MIDIクロックカウンタ閾値を設定する
		noteThredhold :MIDIクロックカウンタ閾値
		戻り値：なし
		*/
	  void	setNoteThredhold(int noteThredhold);

		/*
		押下されたボタン状況を反映する(true->押下中,false->未押下)
		戻り値:modeモード名
		*/
	  MODE_NAME getModeName();
		
		/*
		デバッグモードに変更する
		=>デバッグモードから通常モードには戻らない仕様
		戻り値：なし
		*/
	  void	changeDebugMode();

		/*
		シークエンスマップをclass presetBitstreamで定義したプリセットで設定する
		戻り値：なし
		*/
	  void	presetSequence();

	   /*
		シークエンスマップの内容をビットストリームで取得する
		引数  ：取得先ビットストリーム
		戻り値：なし
		*/
	  void	getSequenceBitstream(char* _bitstream);


  private:
	mode * _currentMode;			//現在のモードクラス
	panelManager * _panelManager;	//【コンストラクタで設定】panelManagerクラスポインタ
	voltage * _voltage;				//【コンストラクタで設定】voltageクラスポインタ
	sequenceMap _sequenceMap;		//シークエンスマップ
	bool _debugMode;				//デバッグフラグ (true->デバッグモード ,false->通常モード)

    int _clockCount;			//現在のMIDIクロックカウンタ値
    int _noteThredhold;	//音符カウンタ閾値

		/*
		モード切替判定を行う
		bool	_track;		判断時：トラックボタン状態
		bool	_patern;	判断時：パターンボタン状態
		bool	_write;		判断時：ライトボタン状態
		*/
	void	_changeMode(bool _track, bool _patern, bool _write);
};

#endif