/*
各種モード切替/管理する
*/

#ifndef modeManager_h
#define modeManager_h

#include <iostream>

/* #include "Arduino.h" */
/* #include "Wire.h" */
#include "mode.h"
#include "paternPlay.h"
#include "paternWrite.h"
#include "trackPlay.h"
#include "trackWrite.h"

//読み取り間隔カウンタ閾値 10msec (500ns * 20000カウント)
#define THD_MODE_MANAGER 20000



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
    explicit modeManager(	panelManager * ptPanelManager , voltage * ptVoltage , unsigned int noteThredhold ,unsigned int start = 0):countTriger(	THD_PANEL_MANAGER	,	start)	{

			_panelManager	=	ptPanelManager;		//panelManagerクラスポインタ
			_voltage	=	ptVoltage;						//【コンストラクタで設定】voltageクラスポインタ
			
			//現在のモードクラスの初期値を設定する
			_currentMode = new paternWrite(	_currentMode	,	_panelManager	,	_voltage	,	&_sequenceMap	);	

			_clockCount			=	0;							//現在のMIDIクロックカウンタ値
			_noteThredhold	=	noteThredhold;	//MIDIクロックカウンタ閾値
		}

		/*
		[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
		*/
    virtual void trigger(){
			bool _track	=	_panelManager->getSwitch(static_cast<int>(Switch::TRACK));
			bool _patern	=	_panelManager->getSwitch(static_cast<int>(Switch::PATTERN));
			bool _write	=	_panelManager->getSwitch(static_cast<int>(Switch::WRITE));
			
			//モード切替判定を行う
			_changeMode(_track , _patern, _write);
			
			//モードに対してカウント閾値達成時に実行されるアプリケーションを実施する
			_currentMode->runSequence();
		};

		/*
		MIDIクロックカウンタをインクリメントする
		⇒MIDIクロックカウンタをインクリメントした結果閾値に達したらMIDIクロックカウンタを0に設定する
		戻り値：true=>MIDIクロックカウンタ＝閾値, false=>MIDIクロックカウンタ＜閾値
		*/
    bool clockCountUp(){
			_clockCount++;
			
			if	(	_noteThredhold	==	_clockCount	){
				_currentMode->runClock();
				_clockCount	=	0;
				return	true;
			}
			
			return	false;
		}


		/*
		MIDIクロックカウンタ閾値を設定する
		noteThredhold :MIDIクロックカウンタ閾値
		戻り値：なし
		*/
		void	setNoteThredhold(	unsigned int noteThredhold	){
			_noteThredhold	=	noteThredhold;
			
			if	(	_noteThredhold	=<	_clockCount	){
				_currentMode->runClock();
				_clockCount	=	0;
			}
		}

		/*
		押下されたボタン状況を反映する(true->押下中,false->未押下)
		戻り値:modeモード名
		*/
		MODE_NAME getModeName (){
			return	_currentMode->getModeName();
		}
		

  private:
	  mode * _currentMode;					//現在のモードクラス
		panelManager * _panelManager;	//【コンストラクタで設定】panelManagerクラスポインタ
		voltage * _voltage;						//【コンストラクタで設定】voltageクラスポインタ
		sequenceMap _sequenceMap;			//シークエンスマップ

    unsigned int _clockCount;			//現在のMIDIクロックカウンタ値
    unsigned int _noteThredhold;	//音符カウンタ閾値

		/*
		モード切替判定を行う
		bool	_track;		判断時：トラックボタン状態
		bool	_patern;	判断時：パターンボタン状態
		bool	_write;		判断時：ライトボタン状態
		*/
		void	_changeMode( _track, _patern, _write){
			
			//ラン/ストップ状態がランなら切替処理は行わない
			if	(	RUN_STOP::STOP == _currentMode->getRunStop()	)	{
				return;
			}

			//現在のモード名を取得する
			MODE_NAME	mode	=	_currentMode->getModeName();

			//ボタン押下状況に応じたモード名を設定
			MODE_NAME changeMode	=	MODE_NAME::NONE;
			
			if	(	_patern	&&	!_write){
				changeMode	=	MODE_NAME::PATERN_PLAY;
				
			}	else if	(	_patern	&&	_write){
				changeMode	=	MODE_NAME::PATERN_WRITE;

			}	else if	(	_track	&&	_write){
				changeMode	=	MODE_NAME::TRACK_PLAY;

			}	else if	(	_track	&&	_write){
				changeMode	=	MODE_NAME::TRACK_WRITE;
			}

			//モード未選択なら切替なし
			if	(	MODE_NAME::NONE	==	changeMode	)	{
				return;

			//現在モードと切替時モードが一緒なら切替なし
			}	else if	(	mode	==	changeMode	)	{
				return;
			}

			
			//異なるモードが選択されたらモードを切り替える
			delete	_currentMode;

			if	(	MODE_NAME::PATERN_PLAY	==	changeMode	)	{
			  _currentMode = new paternPlay(	_currentMode	,	_panelManager	,	_voltage	,	&_sequenceMap	);

			} else if	(	MODE_NAME::PATERN_WRITE	==	changeMode	)	{
			  _currentMode = new paternWrite(	_currentMode	,	_panelManager	,	_voltage	,	&_sequenceMap	);

			} else if	(	MODE_NAME::TRACK_PLAY	==	changeMode	)	{
			  //_currentMode = new trackPlay(	_currentMode	,	_panelManager	,	_voltage	,	&_sequenceMap	);

			} else if	(	MODE_NAME::TRACK_WRITE	==	changeMode	)	{
			  //_currentMode = new trackWrite(	_currentMode	,	_panelManager	,	_voltage	,	&_sequenceMap	);

			}
				
		}
};

#endif