#include "tempo.h"

/*
コンストラクタ
start     :カウンタ開始値(デフォルト=0)
*/
tempo::tempo(unsigned int start = 0) :countTriger(THD_TEMPO, start) {
	_adc_value = 0;
}

/*
[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
*/
void tempo::trigger() {
	//_adc_value	=	analogRead(0);
	_adc_value = 100;
};

/*
テンポ値を取得する
戻り値:テンポ値(30～285)
*/
int	tempo::getTempo() {
	return	convertArray[_adc_value][TEMPO_COL_TEMPO];
}

/*
テンポに対する500nsカウント閾値を取得する
戻り値:500nsカウント閾値
*/
int	tempo::getCountThd() {
	return	convertArray[_adc_value][TEMPO_COL_COUNT];
}