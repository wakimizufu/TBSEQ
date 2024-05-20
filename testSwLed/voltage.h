/*
CV/GATE等の電圧出力を行う
*/

#ifndef voltage_h
#define voltage_h

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
		explicit voltage(){
			_note2PWM[0]	=		17;	//F0
			_note2PWM[1]	=		21;	//F#0
			_note2PWM[2]	=		26;	//G0
			_note2PWM[3]	=		30;	//G#0
			_note2PWM[4]	=		35;	//A0
			_note2PWM[5]	=		40;	//A#0
			_note2PWM[6]	=		45;	//B0
			_note2PWM[7]	=		51;	//C1
			_note2PWM[8]	=		54;	//C#1
			_note2PWM[9]	=		57;	//D1
			_note2PWM[10]	=		61;	//D#1
			_note2PWM[11]	=		65;	//E1
			_note2PWM[12]	=		68;	//F1
			_note2PWM[13]	=		72;	//F#1
			_note2PWM[14]	=		77;	//G1
			_note2PWM[15]	=		81;	//G#1
			_note2PWM[16]	=		86;	//A1
			_note2PWM[17]	=		91;	//A#1
			_note2PWM[18]	=		96;	//B1
			_note2PWM[19]	=		102;	//C2
			_note2PWM[20]	=		105;	//C#2
			_note2PWM[21]	=		109;	//D2
			_note2PWM[22]	=		112;	//D#2
			_note2PWM[23]	=		116;	//E2
			_note2PWM[24]	=		119;	//F2
			_note2PWM[25]	=		123;	//F#2
			_note2PWM[26]	=		128;	//G2
			_note2PWM[27]	=		133;	//G#2
			_note2PWM[28]	=		137;	//A2
			_note2PWM[29]	=		142;	//A#2
			_note2PWM[30]	=		147;	//B2
			_note2PWM[31]	=		154;	//C3
			_note2PWM[32]	=		156;	//C#3
			_note2PWM[33]	=		160;	//D3
			_note2PWM[34]	=		163;	//D#3
			_note2PWM[35]	=		167;	//E3
			_note2PWM[36]	=		170;	//F3
			_note2PWM[37]	=		175;	//F#3
			_note2PWM[38]	=		179;	//G3
			_note2PWM[39]	=		184;	//G#3
			_note2PWM[40]	=		188;	//A3
			_note2PWM[41]	=		194;	//A#3
			_note2PWM[42]	=		199;	//B3
			_note2PWM[43]	=		205;	//C4
			_note2PWM[44]	=		207;	//C#4
			_note2PWM[45]	=		211;	//D4
			_note2PWM[46]	=		215;	//D#4
			_note2PWM[47]	=		218; //E4	

			//出力内容をリセット
			reset();
		}

		/*
		出力内容をリセット
		*/
		void	reset(){
			cv(0);
			gate(GATE_OFF);
			accent(ACCENT_OFF);
			slide(SLIDE_OFF);
		}

		/*
		CVを設定する
		*/
		void	cv(int note){
			//Timer2 PMWを0～255の範囲で設定する
			if	(	note < SIZE_NOTE2PWM ) {
			//std::cout << "voltage::cv() : note:" << note << " PWM:" << _note2PWM[note] << std::endl;
			//analogWrite(_note2PWM[note]);
			}
			
		}
		
		/*
		GATEを設定する
		*/
		void	gate(int value){
			//DigitalWrite(value);
		}
		
		/*
		ACCENTを設定する
		*/
		void	accent(int value){
			//DigitalWrite(value);
		}
		
		/*
		SLIDEを設定する
		*/
		void	slide(int value){
			//DigitalWrite(value);
		}
		
		
  private:
	//配列:PWM値インデックス⇒MIDIノート値
	int _note2PWM[SIZE_NOTE2PWM];
};

#endif