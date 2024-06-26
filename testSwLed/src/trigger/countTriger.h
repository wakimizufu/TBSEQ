/*
カウンタ値を加算後、閾値を比較して一致したらトリガーを発火して処理を行う為の基底クラス
*/

#ifndef countTriger_h
#define countTriger_h

class countTriger
{
  public:

		/*
		コンストラクタ
		thredhold :カウンタ閾値
		start     :カウンタ開始値(デフォルト=0)
		*/
	  countTriger(unsigned int thredhold, unsigned int start);

		/*
		[仮想関数]デストラクタ
		*/
		virtual ~countTriger();

		/*
		カウンタをインクリメントする
		⇒カウンタをインクリメントした結果閾値に達したらカウンタを0に設定する
		戻り値：true=>カウンタ＝閾値, false=>カウンタ＜閾値
		*/
		bool countUp();

		/*
		[純粋仮想関数]カウンタ閾値に達した際にトリガー発火する
		*/
    virtual void trigger()=0;

  protected:
    unsigned int _count;			//現在のカウンタ値
    unsigned int _thredhold;	//カウンタ閾値
};

#endif