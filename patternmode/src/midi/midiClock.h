/*
MIDIクロックをカウントする
*/

#ifndef midiClock_h
#define midiClock_h

#include "../trigger/countTriger.h"


class midiClock: public countTriger
{
  public:

		/*
		コンストラクタ
		thredhold :カウンタ閾値
		start     :カウンタ開始値(デフォルト=0)
		*/
	  midiClock(unsigned int thredhold, unsigned int start);

		/*
		[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
		*/
	  virtual void trigger();

		/*
		カウントアップフラグをアップ待ちに変更&カウンタを0クリア
		*/
	  void	clear();
		

		/*
		カウントアップフラグを取得する
		戻り値:カウントアップフラグ true->カウントアップ済, false->アップ待ち
		*/
	  bool getCountUp();
		

		/*
		テンポに応じたMIDIクロック:カウンタ閾値を変更する
		value:変更するMIDIクロック:カウンタ閾値
		*/
	  void	setTempo2Threshold(int	value);


  protected:
    bool _count_up;	//カウントアップフラグ true->カウントアップ済, false->アップ待ち
};

#endif