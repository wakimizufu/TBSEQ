/*
CV/GATE等の電圧出力を行う
*/

#ifndef voltage_h
#define voltage_h


#include <arduino.h>
#include "pico/stdlib.h"
#include "boradConst.h"

//GATE設定 (GATE_ON:5V ,GATE_OFF:0V)
#define GATE_ON true
#define GATE_OFF false

//ACCENT設定 (ACCENT_ON:0V ,ACCENT_OFF:5V)
#define ACCENT_ON false
#define ACCENT_OFF true

//SLIDE設定 (SLIDE_ON:5V ,SLIDE_OFF:0V)
#define SLIDE_ON true
#define SLIDE_OFF false

//配列:PWM値インデックス⇒MIDIノート値 配列サイズ
#define SIZE_NOTE2PWM 48

//シンク極性
//立ち上がり(正論理)
#define SYNC_TRIGER_POSITIVE	false
//立ち下がり(負論理)
#define SYNC_TRIGER_NEGATIVE	true     

//テンポ同期ソース
//シンク信号同期
#define SYNC_TEMPO_SYNC_SIGNAL	false
//MIDI IN同期
#define SYNC_TEMPO_MIDI_IN	true     


//音程名⇒PWM値インデックス
enum class NOTE_PWM_INDEX
{
NOTE_F0,
NOTE_FS0,
NOTE_G0,
NOTE_GS0,
NOTE_A0,
NOTE_AS0,
NOTE_B0,
NOTE_C1,
NOTE_CS1,
NOTE_D1,
NOTE_DS1,
NOTE_E1,
NOTE_F1,
NOTE_FS1,
NOTE_G1,
NOTE_GS1,
NOTE_A1,
NOTE_AS1,
NOTE_B1,
NOTE_C2,
NOTE_CS2,
NOTE_D2,
NOTE_DS2,
NOTE_E2,
NOTE_F2,
NOTE_FS2,
NOTE_G2,
NOTE_GS2,
NOTE_A2,
NOTE_AS2,
NOTE_B2,
NOTE_C3,
NOTE_CS3,
NOTE_D3,
NOTE_DS3,
NOTE_E3,
NOTE_F3,
NOTE_FS3,
NOTE_G3,
NOTE_GS3,
NOTE_A3,
NOTE_AS3,
NOTE_B3,
NOTE_C4,
NOTE_CS4,
NOTE_D4,
NOTE_DS4,
NOTE_E4,
};


class voltage
{
  public:

		/*
		コンストラクタ
		*/
		//explicit voltage();
		voltage();
	
		/*
		出力内容をリセット
		*/
		void	reset();

		/*
		CVを設定する
		*/
		void	cv(int note);
		
		/*
		GATEを設定する
		*/
		void	gate(bool value);
		
		/*
		ACCENTを設定する
		*/
		void	accent(bool value);
		
		/*
		SLIDEを設定する
		*/
		void	slide(bool value);
		
		/*
		シンク:極性を設定する
		*/
		void	syncPolarity(bool value);		
		
		/*
		シンク:初期状態にリセットする
		*/
		void	syncReset();

		/*
		シンク:トリガーONに設定する
		*/
		void	syncOn();

		/*
		シンク:現在のシンクレベルを反転して設定する
		*/
		void	syncFlip();

		/*
		シンク:現在のシンクRun/StopレベルをRunにする
		*/
		void	syncOutRun();

		/*
		シンク:現在のシンクRun/StopレベルをRunにする
		*/
		void	syncOutStop();

  private:
	//配列:PWM値インデックス⇒MIDIノート値
	int _note2PWM[SIZE_NOTE2PWM];

	//シンク極性
	bool _sync_polarity;

	//シンクレベル
	bool _sync_Level;
};

#endif