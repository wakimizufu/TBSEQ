#include "midiReceive.h"

/*
MIDI受信処理を行う
⇒使用UART:UART0(Serial1)
*/

midiReceive::midiReceive() {
	_Enable			=	false;	//受信開始/停止フラグ
	_TimmingClock	=	false;	//タイミングクロック
	_Start			=	false;	//スタート
	_Stop			=	false;	//ストップ
}


/*
受信MIDIメッセージをFIFOが空になるまで受信する
戻り値:なし
*/
void	midiReceive::receiveMidiMessage(){

	int incomingByte = 0; // for incoming serial data
		
		
	_TimmingClock	=	false;	//タイミングクロック
	_Start				=	false;	//スタート
	_Stop					=	false;	//ストップ

	while (Serial1.available() > 0)	{

		// read the incoming byte:
		incomingByte = Serial1.read();

		if	(	incomingByte	==	MIDI_STATUS_CLOCK	)	{
			_TimmingClock	=	true;
			Serial.println("MIDI recv:MIDI_STATUS_CLOCK");
		}

		if	(	incomingByte	==	MIDI_STATUS_START	)	{
			_Start	=	true;
			Serial.println("MIDI recv:MIDI_STATUS_START");
		}

		if	(	incomingByte	==	MIDI_STATUS_STOP	)	{
			_Stop	=	true;
			Serial.println("MIDI recv:MIDI_STATUS_STOP");
		}

		// say what you got:
		//Serial.print("MIDI recv:");
		//Serial.print(incomingByte, HEX);
		//Serial.println("");
	}

}

/*
receiveMidiMessage()実行後にタイミングクロック受信チェック
戻り値:true=>受信済, false=>未受信
*/
bool	midiReceive::isTimmingClock(){
	return	_TimmingClock;
}


/*
receiveMidiMessage()実行後にスタート受信チェック
戻り値:true=>受信済, false=>未受信
*/
bool	midiReceive::isStart(){
	return	_Start;
}

/*
receiveMidiMessage()実行後にストップ受信チェック
戻り値:true=>受信済, false=>未受信
*/
bool	midiReceive::isStop(){
	return	_Stop;
}


/*
receiveMidiMessage()実行後にストップ受信チェック
引数:true=>受信開始, false=>受信停止
*/
void	midiReceive::setReceiveEnable(bool value){
	_Enable	=	value;
}

/*
受信開始/停止フラグチェック
戻り値:true=>受信開始, false=>受信停止
*/
bool	midiReceive::isEnable(){
	return	_Enable;
}
