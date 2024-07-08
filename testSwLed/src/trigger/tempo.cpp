#include "tempo.h"

/*
コンストラクタ
start     :カウンタ開始値(デフォルト=0)
*/
tempo::tempo(unsigned int start = 0) :countTriger(THD_TEMPO, start) {
	_adc_value = 90;   //初期値はBMP:120とする
	_tempo_up = false;
}

/*
[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
*/
void tempo::trigger() {
	_tempo_up=true;
};

/*
テンポ値を取得する
戻り値:テンポ値(30～285)
*/
int	tempo::getTempo() {
	return	convertArray[_adc_value][TEMPO_COL_TEMPO];
}

/*
テンポに対する32usカウント閾値を取得する
戻り値:32usカウント閾値
*/
int	tempo::getCountThd() {
	return	convertArray[_adc_value][TEMPO_COL_COUNT];
}

/*
ADC値(12bit)を指定して32usカウント閾値を設定する
value:ADC値(12bit)
*/
void tempo::setTempo(int value){
	_adc_value	=	value << 4; //12bit⇒8bitへ変換

	if (_adc_value >= TEMPO_CNV_ROW_MAX) {
		_adc_value = 0;
	}
}

/*
テンポカウント済フラグをカウント中に変更
*/
void tempo::clear(){
	_tempo_up=false;
}

/*
テンポカウント済フラグを取得
戻り値:true->テンポカウント済, false->カウント中
*/
bool tempo::getTempoUp() {
	return	_tempo_up;
}
