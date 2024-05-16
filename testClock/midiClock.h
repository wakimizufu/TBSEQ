/*
MIDIクロックをカウントする
*/

#ifndef midiClock_h
#define midiClock_h

#include <Arduino.h>
#include <countTriger.h>

//MIDIクロック カウンタ閾値992us (32us * 31カウント)
#define THD_MIDI_CLOCK 31

class midiClock: public countTriger
{
  public:

		/*
		コンストラクタ
		start     :カウンタ開始値(デフォルト=0)
		*/
    explicit midiClock(unsigned int start = 0):countTriger(	THD_MIDI_CLOCK	,	start)	{
			_count_up	=	false;
		}

		/*
		[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
		*/
    virtual void trigger(){
			_count_up	=	true;
		};

		/*
		MIDIクロックがカウントアップしたか確認する
		⇒カウントアップ済ならクリアする
		戻り値:true->カウントアップ済, false->アップ待ち
		*/
		bool	countUp(){
			bool ret_value	=	_count_up;
			_count_up	=	false;
			return	ret_value;
		}

  protected:
    bool _count_up;	//カウントアップフラグ true->カウントアップ済, false->アップ待ち
};

#endif