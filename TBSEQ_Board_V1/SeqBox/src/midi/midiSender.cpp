#include "midiSender.h"

midiSender::midiSender() {
	_NoteOn = NOTE_OFF;		//ノート送信フラグ
	_note = 0x40;				//現在ノート値
	_accent = CC_ACCENT_OFF;	//現在アクセントフラグ
	_portament = CC_SLIDE_OFF;	//現在ポルタメントフラグ
}

/*
システムリアルタイムメッセージ：MIDIクロックを送信
*/
void	midiSender::clock() {
	_sendMessage(MIDI_STATUS_CLOCK, EMPTY_DATA, EMPTY_DATA);
}


/*
システムリアルタイムメッセージ：スタートを送信
*/
void	midiSender::srt_start() {
	_sendMessage(MIDI_STATUS_START, EMPTY_DATA, EMPTY_DATA);
}

/*
システムリアルタイムメッセージ：コンティニューを送信
*/
void	midiSender::srt_continue() {
	_sendMessage(MIDI_STATUS_CONTINUE, EMPTY_DATA, EMPTY_DATA);
}

/*
システムリアルタイムメッセージ：ストップを送信
*/
void	midiSender::srt_stop() {
	_sendMessage(MIDI_STATUS_STOP, EMPTY_DATA, EMPTY_DATA);
}


/*
チャンネルボイスメッセージ：ノートオンを送信
*/
void	midiSender::cb_noteOn(int	senderChannel, int note, bool	accent, bool	slide) {
	_NoteOn = NOTE_ON;			//ノート送信フラグ

	//ノートオン
	_sendMessage(MIDI_STATUS_NOTEON || _senderChannel, note, CC_VEROCITY_ON);
	_note = note;					//現在ノート値

	//ポルタメントフラグ
	if (_portament != slide) {
		_sendMessage(MIDI_STATUS_CONTROL || _senderChannel, slide << CC_PORTAMENT_LEFT_SHIFT, EMPTY_DATA);
		_portament = slide;					//現在ポルタメントフラグ
	}

	_accent = accent;				//現在アクセントフラグ
	_senderChannel = senderChannel;	//現在送信チャネル

}

/*
チャンネルボイスメッセージ：ノートオフを送信
*/
void	midiSender::cb_noteOff() {
	_NoteOn = NOTE_OFF;			//ノート送信フラグ

	//ノートオフ(正確にはベロシティを0でノートオンする)
	_sendMessage(MIDI_STATUS_NOTEON || _senderChannel, _note, CC_VEROCITY_OFF);
}

  /*
  MIDIメッセージ送信
  */
	  void	midiSender::_sendMessage(int	status, int	dataOne, int	dateTwo) {

	  }