/*
スイッチ入力/LED出力を定期間隔で更新する
*/

#ifndef panelManager_h
#define panelManager_h

#include <iostream>

/* #include "Arduino.h" */
/* #include "Wire.h" */
#include "countTriger.h"
#include "matrixLED.h"
#include "matrixSwitch.h"

//読み取り間隔カウンタ閾値 5msec (500ns * 10000カウント)
#define THD_PANEL_MANAGER 10000

//panelManagerシークエンスインデックス
enum class PANEL_MANAGER_SEQ
{
SW_1ST_ROW0,
SW_1ST_ROW1,
LED_ROW0,
SW_1ST_ROW2,
SW_1ST_ROW3,
LED_ROW1,
SW_2ST_ROW0,
SW_2ST_ROW1,
LED_ROW2,
SW_2ST_ROW2,
SW_2ST_ROW3,
LED_ROW3,
};


class panelManager: public countTriger
{
  public:

		/*
		コンストラクタ
		start     :カウンタ開始値(デフォルト=0)
		*/
      panelManager(unsigned int start);

		/*
		[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
		*/
      virtual void trigger();

    /*
    指定LEDインデックスへの設定を行う
    index：enum class LED
    value：true=>点灯 , false=>消灯
    戻り値：なし
    */
      void setLED(int index, bool value);

    /*
    指定LED行の値(bit7～0)を取得する
    index：指定LED行
    value：char 指定LED行の値(bit7～0)
    戻り値：なし
    */
      void setLEDRow(int row, char value);


    /*
    指定LEDインデックスの値を取得する
    index：enum class LED
    戻り値：bool true=>点灯 , false=>消灯
    */
      bool getLED(int index);

    /*
    指定LED行の値(bit7～0)を取得する
    index：指定LED行
    戻り値：char 指定LED行の値(bit7～0)
    */
      char getLEDRow(int row);

    /*
    指定スイッチインデックスの値を取得する
    index：enum class Switch
    戻り値：bool true=>押下 , false=>開放
    */
      bool getSwitch(int index);

    /*
    指定スイッチ行の値(bit7～0)を取得する
    index：指定スイッチ行
    戻り値：char 指定スイッチ行の値(bit7～0)
    */
      char getSwitchRow(int row);

    /*
    シークエンス1周完了フラグを完了待ちに変更
    */
      void clear();

    /*
    シークエンス1周完了フラグを取得
    */
      bool getSequenceUp();

  protected:
    int _sequence;      //シークエンスカウント
    bool _sequence_up;	//シークエンス1周完了フラグ true->シークエンス1周完了, false->完了待ち

    matrixLED _matrixLED;       
    matrixSwitch _matrixSwitch;
};

#endif