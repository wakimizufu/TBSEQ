#include "countTriger.h"

/*
コンストラクタ
thredhold :カウンタ閾値
start     :カウンタ開始値(デフォルト=0)
*/
countTriger::countTriger(unsigned int thredhold, unsigned int start = 0) {
	_thredhold = thredhold;
	_count = start;
}

/*
[仮想関数]デストラクタ
*/
countTriger::~countTriger() {
}

/*
カウンタをインクリメントする
⇒カウンタをインクリメントした結果閾値に達したらカウンタを0に設定する
戻り値：true=>カウンタ＝閾値, false=>カウンタ＜閾値
*/
bool countTriger::countUp() {
	_count++;

	if (_thredhold == _count) {
		trigger();
		_count = 0;
		return	true;
	}

	return	false;
}