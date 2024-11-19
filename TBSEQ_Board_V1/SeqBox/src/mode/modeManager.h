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
#include "utilityMode.h"

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
		ユーティリティーモードに変更する
		=>ユーティリティーモードから通常モードには戻らない仕様
		戻り値：なし
		*/
	  void	changeUtilityMode();

		/*
		シークエンスマップをFRAMからロードして設定する(ロード出来なかったらプリセットを設定する)
		戻り値：なし
		*/
	  void	presetSequence();

	   /*
		シークエンスマップの内容をビットストリームで取得する
		引数  ：取得先ビットストリーム
		戻り値：なし
		*/
	  void	getSequenceBitstream(unsigned char* _bitstream);

		/*
		開始ステップを取得する
		戻り値：true=>開始ステップ, false=>通常ステップ
		*/
	  bool getFirstStep();

		/*
		開始ステップを設定する
		引数：true=>開始ステップ, false=>通常ステップ
		*/
	  void setFirstStep(bool value);

		/*
		MIDI:スタート受信フラグを設定
		引数：true=>受信済, false=>未受信
		*/
		void	setMIDIStart(bool value);

		/*
		MIDI:ストップ受信フラグを設定
		引数：true=>受信済, false=>未受信
		*/
		void	setMIDIStop(bool value);

		/*
		トラックマップをFRAMからロードして設定する(ロード出来なかったらプリセットを設定する)
		戻り値：なし
		*/
	  void	presetTrack();

	   /*
		トラックマップの内容をビットストリームで取得する
		引数  ：取得先ビットストリーム
		戻り値：なし
		*/
	  void	getTrackBitstream(unsigned char* _bitstream);

		/*
		ユーティリティーモードフラグをFRAMからロードして設定する(ロード出来なかったらプリセットを設定する)
		戻り値：なし
		*/
	  void	presetUtility();

		/*
		シンク信号極性を取得
		戻り値：シンク信号極性   (false:立ち上がり, true:立下り)
		*/
		bool	getSyncPolarity();

		/*
		テンポ同期ソースを取得
		戻り値：テンポ同期ソース (false:シンク信号, true:MIDI IN)
		*/
		bool	getSyncTempo();

  private:
	mode * _currentMode;					//現在のモードクラス
	panelManager * _panelManager;	//【コンストラクタで設定】panelManagerクラスポインタ
	voltage * _voltage;						//【コンストラクタで設定】voltageクラスポインタ
	sequenceMap _sequenceMap;			//シークエンスマップ
	trackMap _trackMap;						//トラックマップ
	bool _debugMode;							//デバッグフラグ (true->デバッグモード ,false->通常モード)
	bool _utilityMode;							//ユーティリティーフラグ (true->ユーティリティーモード ,false->通常モード)
	int _bank;										//現在の指定バンク

	bool _currentSwtich[SW_INDEX_MAX];	//ボタン押下中

    int _clockCount;			//現在のMIDIクロックカウンタ値
    int _noteThredhold;	//音符カウンタ閾値

	bool _syncPolarity;		//シンク信号極性   (false:立ち上がり, true:立下り)
	bool _syncTempo;		//テンポ同期ソース (false:シンク信号, true:MIDI IN)

	/*
	モード切替判定を行う
	*/
	void	_changeMode();
};

#endif