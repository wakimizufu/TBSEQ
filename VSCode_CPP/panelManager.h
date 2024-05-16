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
    explicit panelManager(unsigned int start = 0):countTriger(	THD_PANEL_MANAGER	,	start)	{
        _sequence=static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW0);
        _sequence_up=false;
		}

		/*
		[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
		*/
    virtual void trigger(){
            int _seq_value = _sequence / 3;
            int _seq_mod   = _sequence % 3;

            //スイッチ入力
            if ( ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW0)) ||
                 ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW1)) ||
                 ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW2)) ||
                 ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW3)) ||
                 ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW0)) ||
                 ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW1)) ||
                 ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW2)) ||
                 ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW3)) ){

                    //I2C::スイッチ入力
                    char _SW_Value;
                    /*
                    Wire.requestFrom(2, 6);    // Request 6 bytes from slave device number two
                    while(Wire.available()) {
                        _SW_Value = Wire.read();    // Receive a byte as character
                    }
                    */

                    int _SW_Row = ((_seq_value * 2) + _seq_mod) % 4;

                    std::cout << "panelManager::_matrixSwitch.setRow() scan:" << _matrixSwitch.getScan() << " _SW_Row:" << _SW_Row << " _SW_Value:" << _SW_Value << std::endl;
                    _matrixSwitch.setRow ( _matrixSwitch.getScan() , _SW_Row , _SW_Value );
                 }


            //LED出力
            if ( ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW0)) ||
                 ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW1)) ||
                 ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW2)) ||
                 ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW3)) ){

                    //I2C::LED出力 
                    char _LED_Row_value;
                    std::cout << "panelManager::_matrixLED.setRow row:" << _seq_value << " _LED_Row_value:" << _LED_Row_value << std::endl;
                    _matrixLED.setRow(_seq_value , _LED_Row_value );
                    //Wire.beginTransmission(44);  // Transmit to device number 44 (0x2C)
                    //Wire.write(_LED_Row_value);  // Sends value byte
                    //Wire.endTransmission();      // Stop transmitting
                 }

            //次回スイッチスキャン回数を更新
            if ( ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW3)) ||
                 ( _sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW3)) ){     
                    _matrixSwitch.nextScan();  

                    //SWスキャンインデックス：決定 ならスイッチ入力を確定する
                    if( _matrixSwitch.getScan() == static_cast<int>(SwitchScan::Finalize) ){
                        _matrixSwitch.finalize();
                        _matrixSwitch.nextScan();  
                    }   
                 }

            //次回シークエンス&シークエンス1周完了フラグを設定
            if( _sequence==static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW3)){
                _sequence=static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW0);
                _sequence_up=true;
            } else {
                _sequence++;
            }
		};

    /*
    指定LEDインデックスへの設定を行う
    index：enum class LED
    value：true=>点灯 , false=>消灯
    戻り値：なし
    */
    void setLED ( int index , bool value)    {
        _matrixLED.set(index , value);
    }    

    /*
    指定LED行の値(bit7～0)を取得する
    index：指定LED行
    value：char 指定LED行の値(bit7～0)
    戻り値：なし
    */
    void setLEDRow(int row,char value){
        _matrixLED.setRow(row,value);
    }


    /*
    指定LEDインデックスの値を取得する
    index：enum class LED
    戻り値：bool true=>点灯 , false=>消灯
    */
    bool getLED ( int index)    {
        return _matrixLED.get(index);
    }

    /*
    指定LED行の値(bit7～0)を取得する
    index：指定LED行
    戻り値：char 指定LED行の値(bit7～0)
    */
    char getLEDRow(int row){
        return _matrixLED.getRow(row);
    }

    /*
    指定スイッチインデックスの値を取得する
    index：enum class Switch
    戻り値：bool true=>押下 , false=>開放
    */
    bool getSwitch ( int index)    {
        return _matrixSwitch.get(index);
    }

    /*
    指定スイッチ行の値(bit7～0)を取得する
    index：指定スイッチ行
    戻り値：char 指定スイッチ行の値(bit7～0)
    */
    char getSwitchRow(int row){
        return _matrixSwitch.getRow(row);
    }

    /*
    シークエンス1周完了フラグを完了待ちに変更
    */
	void clear(){
		_sequence_up	=	false;
		}

    /*
    シークエンス1周完了フラグを取得
    */
   	bool getSequenceUp(){
		return _sequence_up;
	}

  protected:
    int _sequence;      //シークエンスカウント
    bool _sequence_up;	//シークエンス1周完了フラグ true->シークエンス1周完了, false->完了待ち

    matrixLED _matrixLED;       
    matrixSwitch _matrixSwitch;
};

#endif