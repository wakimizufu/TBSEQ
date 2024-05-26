#include "midiClock.h"

/*
コンストラクタ
thredhold :カウンタ閾値
start     :カウンタ開始値(デフォルト=0)
*/
midiClock::midiClock(unsigned int thredhold, unsigned int start = 0) :countTriger(thredhold, start) {
	_count_up = false;
}

/*
[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
*/
void  midiClock::trigger() {
	_count_up = true;
};

/*
カウントアップフラグをアップ待ちに変更&カウンタを0クリア
*/
void	midiClock::clear() {
	_count_up = false;
	_count = 0;
}


/*
カウントアップフラグを取得する
戻り値:カウントアップフラグ true->カウントアップ済, false->アップ待ち
*/
bool midiClock::getCountUp() {
	return	_count_up;
}


/*
テンポに応じたMIDIクロック:カウンタ閾値を変更する
value:変更するMIDIクロック:カウンタ閾値
*/
void	midiClock::setTempo2Threshold(int	value) {
	_thredhold = value;

	//カウント値が閾値を超えたらカウントアップとする
	if (_count >= _thredhold) {
		_count_up = true;
		_count = 0;
	}
}
