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
		explicit midiSender(){
			_NoteOn			=	NOTE_OFF;		//ノート送信フラグ
			_note				=	0x40;				//現在ノート値
			_accent			=	ACCENT_OFF;	//現在アクセントフラグ
			_portament	=	SLIDE_OFF;	//現在ポルタメントフラグ
		}

		/*
		システムリアルタイムメッセージ：MIDIクロックを送信
		*/
		void	clock(){
			_sendMessage(	MIDI_STATUS_CLOCK	,	null	,	null	);
		}
		

		/*
		システムリアルタイムメッセージ：スタートを送信
		*/
		void	srt_start(){
			_sendMessage(	MIDI_STATUS_START	,	null	,	null	);
		}

		/*
		システムリアルタイムメッセージ：コンティニューを送信
		*/
		void	srt_continue(){
			_sendMessage(	MIDI_STATUS_START	,	null	,	null	);
		}

		/*
		システムリアルタイムメッセージ：ストップを送信
		*/
		void	srt_stop(){
			_sendMessage(	MIDI_STATUS_START	,	null	,	null	);
		}
		

		/*
		チャンネルボイスメッセージ：ノートオンを送信
		*/
		void	cb_noteOn	(	int	senderChannel	,	int note	,	bool	accent	,	bool	slide	){
			_NoteOn					=	NOTE_ON;			//ノート送信フラグ
			
			//ノートオン
			_sendMessage(	MIDI_STATUS_NOTEON || _senderChannel	,	note	,	VEROCITY_ON	);
			_note						=	note;					//現在ノート値

			//ポルタメントフラグ
			if	(	_portament	!=	slide	)	{
			_sendMessage(	MIDI_STATUS_CONTROL || _senderChannel	,	slide << PORTAMENT_LEFT_SHIFT	,	null	);
			_portament	=	slide;					//現在ポルタメントフラグ
			}

			_accent					=	accent;				//現在アクセントフラグ
			_senderChannel	=	senderChannel	//現在送信チャネル
			
		}

		/*
		チャンネルボイスメッセージ：ノートオフを送信
		*/
		void	cb_noteOff(){
			_NoteOn	=	NOTE_OFF;			//ノート送信フラグ

			//ノートオフ(正確にはベロシティを0でノートオンする)
			_sendMessage(	MIDI_STATUS_NOTEON || _senderChannel	,	_note	,	VEROCITY_OFF	);
		}

  private:
		/*
		MIDIメッセージ送信
		*/
		void	_sendMessage(	int	status	,	int	dataOne	,	int	dateTwo	){
			
		}

		
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