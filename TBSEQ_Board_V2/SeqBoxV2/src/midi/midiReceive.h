/*
MIDI受信処理を行う
⇒使用UART:UART0(Serial1)
*/

#ifndef midiReceive_h
#define midiReceive_h

#include <arduino.h>
#include "midiConst.h"
#include "pico/stdlib.h"

class midiReceive
{
  public:

		/*
		コンストラクタ
		*/
	  	midiReceive();

		/*
		受信MIDIメッセージをFIFOが空になるまで受信する
		戻り値:なし
		*/
		void	receiveMidiMessage();

		/*
		receiveMidiMessage()実行後にタイミングクロック受信チェック
		戻り値:true=>受信済, false=>未受信
		*/
		bool	isTimmingClock();

		/*
		receiveMidiMessage()実行後にスタート受信チェック
		戻り値:true=>受信済, false=>未受信
		*/
		bool	isStart();

		/*
		receiveMidiMessage()実行後にストップ受信チェック
		戻り値:true=>受信済, false=>未受信
		*/
		bool	isStop();

		/*
		受信開始/停止フラグを設定
		引数:true=>受信開始, false=>受信停止
		*/
		void	setReceiveEnable(bool value);

		/*
		受信開始/停止フラグチェック
		戻り値:true=>受信開始, false=>受信停止
		*/
		bool	isEnable();

  private:
	bool	_Enable;	//受信開始/停止フラグ

	//システムリアルタイムメッセージ受信フラグ
	bool	_TimmingClock;	//タイミングクロック
	bool	_Start;					//スタート
	bool	_Stop;					//ストップ

  
};

#endif