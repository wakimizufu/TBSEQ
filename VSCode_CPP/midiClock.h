/*
MIDIクロックをカウントする
*/

#ifndef midiClock_h
#define midiClock_h

//#include <Arduino.h>
#include "countTriger.h"


class midiClock: public countTriger
{
  public:

		/*
		コンストラクタ
		thredhold :カウンタ閾値
		start     :カウンタ開始値(デフォルト=0)
		*/
    explicit midiClock(unsigned int thredhold , unsigned int start = 0):countTriger(	thredhold	,	start)	{
			_count_up	=	false;
		}

		/*
		[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
		*/
    virtual void trigger(){
			_count_up	=	true;
		};

		/*
		カウントアップフラグをアップ待ちに変更&カウンタを0クリア
		*/
		void	clear(){
			_count_up	=	false;
			_count	=	0;
			}
		

		/*
		カウントアップフラグを取得する
		戻り値:カウントアップフラグ true->カウントアップ済, false->アップ待ち
		*/
		bool getCountUp(){
			return	_count_up;
		}
		

		/*
		テンポに応じたMIDIクロック:カウンタ閾値を変更する
		value:変更するMIDIクロック:カウンタ閾値
		*/
		void	setTempo2Threshold	(	int	value	){
			_thredhold	=	value;

			//カウント値が閾値を超えたらカウントアップとする
			if	(	_count >=	_thredhold	)	{
				_count_up	=	true;
				_count	=	0;
			}
		}



  protected:
    bool _count_up;	//カウントアップフラグ true->カウントアップ済, false->アップ待ち
};

#endif