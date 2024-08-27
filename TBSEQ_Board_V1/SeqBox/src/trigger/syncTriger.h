/*
シンク開始時のトリガーカウンタを定期的に更新/値を返す
*/

#ifndef synctriger_h
#define synctriger_h

#include "../trigger/countTriger.h"

//読み取り間隔カウンタ閾値 15008us=15.008ms (32us * 469カウント)
#define THD_SYNC_TRIGER 469


class syncTriger: public countTriger
{
  public:

		/*
		コンストラクタ
		start     :カウンタ開始値(デフォルト=0)
		*/
	  syncTriger(unsigned int start);

		/*
		[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
		*/
	  virtual void trigger();

		/*
		テンポカウント済フラグをカウント中に変更
		*/
	  void clear();

		/*
		テンポカウント済フラグを取得
		戻り値:true->テンポカウント済, false->カウント中
		*/
      bool getSyncUp();

  protected:
	 	bool _syncTriger_up;	//トリガーカウント済フラグ true->トリガーカウント済, false->カウント中

};

#endif