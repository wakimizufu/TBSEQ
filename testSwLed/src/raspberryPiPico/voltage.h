/*
CV/GATE等の電圧出力を行う
*/

#ifndef voltage_h
#define voltage_h

#include <Wire.h>



//GATE設定 (GATE_ON:5V ,GATE_OFF:0V)
#define GATE_ON 1
#define GATE_OFF 0

//ACCENT設定 (ACCENT_ON:5V ,ACCENT_OFF:0V)
#define ACCENT_ON 1
#define ACCENT_OFF 0

//SLIDE設定 (ACCENT_ON:5V ,ACCENT_OFF:0V)
#define SLIDE_ON 1
#define SLIDE_OFF 0

//配列:PWM値インデックス⇒MIDIノート値 配列サイズ
#define SIZE_NOTE2PWM 48

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
		void	gate(int value);
		
		/*
		ACCENTを設定する
		*/
		void	accent(int value);
		
		/*
		SLIDEを設定する
		*/
		void	slide(int value);
		
		
  private:
	//配列:PWM値インデックス⇒MIDIノート値
	int _note2PWM[SIZE_NOTE2PWM];
};

#endif