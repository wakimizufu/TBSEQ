/*
MIDI関連マクロを定義
*/

#ifndef midiConst_h
#define midiConst_h

//-----------------【MIDIメッセージ:ステータスバイト】-----------------

//ノートオン
#define MIDI_STATUS_NOTEON	0x80

//コントロールチェンジ
#define MIDI_STATUS_CONTROL	0xB0

//MIDIクロック
#define MIDI_STATUS_CLOCK	0xF8

//スタート
#define MIDI_STATUS_START	0xFA

//コンティニュー
#define MIDI_STATUS_CONTINUE	0xFB

//ストップ
#define MIDI_STATUS_STOP	0xFC

//-----------------【MIDIメッセージ:コントロールチェンジ】-----------------

//ポルタメントOn/Off(63以下:オフ 64以上:オン)
#define MIDI_CONTROL_PORTAMENT	0x41

//スライド(ポルタメント)フラグを(63以下:オフ 64以上:オン)にするので左シフトする
#define PORTAMENT_LEFT_SHIFT 6

//-----------------【midiSender/midiRecever関連】-----------------

//ノート送信フラグ
#define NOTE_ON true
#define NOTE_OFF false

//アクセントフラグ
#define ACCENT_ON true
#define ACCENT_OFF false

//スライド(ポルタメント)フラグ
#define SLIDE_ON true
#define SLIDE_OFF false

//ベロシティ ノートオン時固定値
#define VEROCITY_ON	0x7F

//ベロシティ ノートオフ時固定値
#define VEROCITY_OFF	0x00

#endif
