#include "syncTriger.h"

/*
コンストラクタ
start     :カウンタ開始値(デフォルト=0)
*/
syncTriger::syncTriger(unsigned int start = 0) :countTriger(THD_SYNC_TRIGER, start) {
	_syncTriger_up = false;
}

/*
[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
*/
void syncTriger::trigger() {
	_syncTriger_up=true;
};


/*
テンポカウント済フラグをカウント中に変更
*/
void syncTriger::clear(){
	_syncTriger_up=false;
}

/*
テンポカウント済フラグを取得
戻り値:true->テンポカウント済, false->カウント中
*/
bool syncTriger::getSyncUp() {
	return	_syncTriger_up;
}
