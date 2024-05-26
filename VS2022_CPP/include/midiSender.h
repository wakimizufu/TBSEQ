/*
MIDI送信処理を行う
*/

#ifndef midiSender_h
#define midiSender_h

//#include <Arduino.h>
#include "midiConst.h"


class midiSender
{
  public:

		/*
		コンストラクタ
		*/
	  midiSender();

		/*
		システムリアルタイムメッセージ：MIDIクロックを送信
		*/
	  void	clock();
		

		/*
		システムリアルタイムメッセージ：スタートを送信
		*/
	  void	srt_start();

		/*
		システムリアルタイムメッセージ：コンティニューを送信
		*/
	  void	srt_continue();

		/*
		システムリアルタイムメッセージ：ストップを送信
		*/
	  void	srt_stop();
		

		/*
		チャンネルボイスメッセージ：ノートオンを送信
		*/
	  void	cb_noteOn(int	senderChannel, int note, bool	accent, bool	slide);

		/*
		チャンネルボイスメッセージ：ノートオフを送信
		*/
	  void	cb_noteOff();

  private:
		/*
		MIDIメッセージ送信
		*/
	  void	_sendMessage(int	status, int	dataOne, int	dateTwo);

		//_sendMessage() 送信無しの場合の識別子
		const int EMPTY_DATA = -1;
		
		//現在送信チャネル
		int	_senderChannel;

		//ノート送信フラグ
		bool	_NoteOn;
		
		//現在ノート値
		int	_note;
		
		//現在アクセントフラグ
		bool	_accent;
		
		//現在ポルタメントフラグ
		bool	_portament;
		

};

#endif