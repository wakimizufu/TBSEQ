#include "panelManager.h"

/*
コンストラクタ
start     :カウンタ開始値(デフォルト=0)
*/
panelManager::panelManager(unsigned int start = 0) :countTriger(THD_PANEL_MANAGER, start) {
    _sequence = static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW0);
    _sequence_up = false;
    _tempo_adc_value = 0;
}

/*
[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
*/
void panelManager::trigger() {
    int _seq_value = _sequence / 3;
    int _seq_mod = _sequence % 3;

    /*
    Serial.print("panelManager::trigger. _sequence:");
    Serial.print(_sequence);
    Serial.print(" _seq_value:");
    Serial.print(_seq_value);
    Serial.print(" _seq_mod:");
    Serial.println(_seq_mod);
    */

    //スイッチ入力
    if ((_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW0)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW1)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW2)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW3)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW0)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW1)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW2)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW3))) {

        //I2C::スイッチ入力
        int _SW_Value = 0x00;
        int _SW_Value2 = 0x00;
        
        int _SW_Row = ((_seq_value * 2) + _seq_mod) % 4;
        int _SW_Row_Addr = 1<<_SW_Row;

        /*Serial.print("panelManager::trigger.");
        Serial.print(" _matrixSwitch.getScan():");
        Serial.print(_matrixSwitch.getScan());
        Serial.print(" _SW_Row:");
        Serial.print(_SW_Row,HEX);
        Serial.print(" _SW_Row_Addr:");
        Serial.print(_SW_Row_Addr,HEX);*/

        if ( 0==_matrixSwitch.getScan()){
            _SW_Value = 0xFF;
        } else if ( 1==_matrixSwitch.getScan()){
            _SW_Value = 0xD5;
        }


        //スイッチ入力用アドレスRow選択書き込み
        //⇒GPIOA(12) Aポート値 (0x01, 0x02, 0x04, 0x08)
        Wire.beginTransmission(I2C_ADDR_SW);
        Wire.write(0x12); 
        Wire.write(_SW_Row_Addr);
        Wire.endTransmission();
        
        //スイッチ入力用アドレスCol読込み
        //⇒GPIOB(13) Bポート値
        Wire.beginTransmission(I2C_ADDR_SW);
        Wire.write(0x13); 
        Wire.endTransmission();

        Wire.requestFrom(I2C_ADDR_SW,1);   //スイッチ入力用アドレスから1バイト分のデータを要求する
        //while(Wire.available()){
            _SW_Value=Wire.read();//データを読み込む
        //}
        
        //スイッチ入力用アドレスRow選択書き込み
        //⇒GPIOA(12) Aポート値 無選択状態に設定⇒これが無いとAポート切り替え時に誤動作する
        Wire.beginTransmission(I2C_ADDR_SW);
        Wire.write(0x12); 
        Wire.write(0x00);
        Wire.endTransmission();

        /*Serial.print(" _SW_Value:");
        Serial.print(_SW_Value,HEX);
        Serial.print(" _SW_Value2:");
        Serial.println(_SW_Value2,HEX);*/

        _matrixSwitch.setRow(_matrixSwitch.getScan(), _SW_Row, static_cast<char>(_SW_Value));
        
    }


    //LED出力
    if ((_sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW0)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW1)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW2)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW3))) {

        /*Serial.print("panelManager::trigger. _seq_value:");
        Serial.print(_seq_value);*/

        //I2C::LED出力 
        int  _LED_Row_value = 1<<_seq_value;
        /*Serial.print(" _LED_Row_value:");
        Serial.print(_LED_Row_value,HEX);*/

        //char _LED_Col_value = _matrixLED.getRow(_seq_value);   
        int _LED_Col_value = static_cast<int>(_matrixLED.getRow(_seq_value));   
        /*Serial.print(" _LED_Col_value:");
        Serial.print(_LED_Col_value,HEX);
        Serial.print(" invert:");
        Serial.println(0xff - _LED_Col_value,HEX);*/

        //LED出力用アドレスRow選択書き込み
        //⇒GPIOA(12) Aポート値 (0x01, 0x02, 0x04, 0x08)
        Wire.beginTransmission(I2C_ADDR_LED);
        Wire.write(0x12); 
        Wire.write(_LED_Row_value); 
        Wire.endTransmission();
        
        //LED出力用アドレスCol値書き込み
        //⇒GPIOB(13) Bポート値 0⇒点灯 1⇒消灯
        Wire.beginTransmission(I2C_ADDR_LED);
        Wire.write(0x13); 
        Wire.write(0xff-_LED_Col_value); 
        Wire.endTransmission();
    }

    //次回スイッチスキャン回数を更新
    if ((_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW3)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW3))) {
        _matrixSwitch.nextScan();

        //SWスキャンインデックス：決定 ならスイッチ入力を確定する
        if (_matrixSwitch.getScan() == static_cast<int>(SwitchScan::Finalize)) {
            _matrixSwitch.finalize();
            _matrixSwitch.nextScan();

            int _scanrow;
            for (_scanrow=0; _scanrow < SW_ROW_MAX ; _scanrow++){
              /*Serial.print("_matrixSwitch.getRow(");
              Serial.print(_scanrow);
              Serial.print("):");
              Serial.println(static_cast<int>(_matrixSwitch.getRow(_scanrow)),HEX);*/
            }

            gpio_put(LED_BUILTIN, !gpio_get(LED_BUILTIN)); // toggle the LED
            //Serial.println("_matrixSwitch.finalize");
        }
    }

    //テンポADC値読み取り
    if (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::TEMPO_ADC_READ)) {
        _tempo_adc_value=adc_read();
        //Serial.print("_tempo_adc_value=adc_read() >>4:");
        //Serial.println(_tempo_adc_value>>4,HEX);
    }

    //次回シークエンス&シークエンス1周完了フラグを設定
    if (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::TEMPO_ADC_READ)) {
        _sequence = static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW0);
        _sequence_up = true;
    }
    else {
        _sequence++;
    }
};

/*
指定LEDインデックスへの設定を行う
index：enum class LED
value：true=>点灯 , false=>消灯
戻り値：なし
*/
void panelManager::setLED(int index, bool value) {
    _matrixLED.set(index, value);
}

/*
指定LED行の値(bit7～0)を取得する
index：指定LED行
value：char 指定LED行の値(bit7～0)
戻り値：なし
*/
void panelManager::setLEDRow(int row, char value) {
    _matrixLED.setRow(row, value);
}


/*
指定LEDインデックスの値を取得する
index：enum class LED
戻り値：bool true=>点灯 , false=>消灯
*/
bool panelManager::getLED(int index) {
    return _matrixLED.get(index);
}

/*
指定LED行の値(bit7～0)を取得する
index：指定LED行
戻り値：char 指定LED行の値(bit7～0)
*/
char panelManager::getLEDRow(int row) {
    return _matrixLED.getRow(row);
}

/*
指定スイッチインデックスの値を取得する
index：enum class Switch
戻り値：bool true=>押下 , false=>開放
*/
bool panelManager::getSwitch(int index) {
    return _matrixSwitch.get(index);
}

/*
指定スイッチ行の値(bit7～0)を取得する
index：指定スイッチ行
戻り値：char 指定スイッチ行の値(bit7～0)
*/
char panelManager::getSwitchRow(int row) {
    return _matrixSwitch.getRow(row);
}

/*
テンポADC値を取得する
戻り値：int テンポADC値(12bit)
*/
int panelManager::getTempoADC(){
    return _tempo_adc_value;
}


/*
シークエンス1周完了フラグを完了待ちに変更
*/
void panelManager::clear() {
    _sequence_up = false;
}

/*
シークエンス1周完了フラグを取得
*/
bool panelManager::getSequenceUp() {
    return _sequence_up;
}
