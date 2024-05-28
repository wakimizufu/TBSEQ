/*
CV/GATE等の電圧出力を行う
*/
#include "voltage.h"

/*
コンストラクタ
*/
voltage::voltage(){
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
void	voltage::reset(){
	cv(0);
	gate(GATE_OFF);
	accent(ACCENT_OFF);
	slide(SLIDE_OFF);
}

/*
CVを設定する
*/
void	voltage::cv(int note){
	//Timer2 PMWを0～255の範囲で設定する
	if	(	note < SIZE_NOTE2PWM ) {
		//analogWrite(_note2PWM[note]);
	}
	
}

		
/*
GATEを設定する
*/
void	voltage::gate(int value){
	//digitalWrite(value);
}

		
/*
ACCENTを設定する
*/
void	voltage::accent(int value){
	//digitalWrite(value);
}


/*
SLIDEを設定する
*/
void	voltage::slide(int value){
	//digitalWrite(value);
}
		
