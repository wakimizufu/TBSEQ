#include "panelManager.h"

/*
コンストラクタ
start     :カウンタ開始値(デフォルト=0)
*/
panelManager::panelManager(unsigned int start = 0) :countTriger(THD_PANEL_MANAGER, start) {
    _sequenceList_index = 0;
    _sequence_up = false;
    _tempo_adc_value = 0;
    _connectFRAM = false;
}


/*
各種I/Oへの初期化処理を行う
戻り値：なし
*/
void panelManager::init(){

//GPIO
gpio_init(LED_BUILTIN); // set LED pin to OUTPUT
gpio_set_dir(LED_BUILTIN,GPIO_OUT);

gpio_init(PIN_GATE);  //GATE
gpio_set_dir(PIN_GATE,GPIO_OUT);

gpio_init(PIN_ACCENT); //ACCENT
gpio_set_dir(PIN_ACCENT,GPIO_OUT);

gpio_init(PIN_SLIDE); //SLIDE
gpio_set_dir(PIN_SLIDE,GPIO_OUT);


//ADC
//Raspberry Pi Pico W サーミスタで気温を計算する
//https://qiita.com/pipito-yukio/items/59f7a853c27bfbccd21e
adc_init();
adc_gpio_init(PIN_TEMPO_ADC);
adc_select_input(0);  //PIN26=A0 の電圧をADCする


//I2C:I2C0に GP0(SDA)とGP1(SCL)を設定します。
Wire.begin();     //I2C使用開始
Wire.setClock(I2C_CLOCK);
Wire.setSDA(I2C_WIRE0_SDA);   //I2C0はWireオブジェクトを使用します。
Wire.setSCL(I2C_WIRE0_SCL);

//PWM
//PWM信号とローパスフィルタを用いた簡易D/A：http://okawa-denshi.jp/blog/?th=2009072200
analogWriteFreq(10000); //fc=10kHz
analogWriteRange(256);  //解像度=8bit
pinMode(PIN_CV, OUTPUT);  //CV
analogWrite(PIN_CV,100);


//MCP23017 初期化 addr:0x20 マトリクススイッチ
//⇒IODIRA(FF) 全て入力ポート
Wire.beginTransmission(I2C_ADDR_SW);
Wire.write(0x00); 
Wire.write(0x00); 
Wire.endTransmission();

//⇒IODIRB(FF) 全て出力ポート
Wire.beginTransmission(I2C_ADDR_SW);
Wire.write(0x01); 
Wire.write(0xFF); 
Wire.endTransmission();


//MCP23017 初期化 addr:0x21 LEDマトリクス
//⇒IODIRA(00) 全て出力ポート
Wire.beginTransmission(I2C_ADDR_LED);
Wire.write(0x00); 
Wire.write(0x00); 
Wire.endTransmission();

//⇒IODIRB(00) 全て出力ポート
Wire.beginTransmission(I2C_ADDR_LED);
Wire.write(0x01); 
Wire.write(0x00); 
Wire.endTransmission();

//初期化完了時LED設定
gpio_put(LED_BUILTIN, false); // toggle the LED
setLEDRow(LED_ROW_0,0xFF);
setLEDRow(LED_ROW_1,0xFF);
setLEDRow(LED_ROW_2,0xFF);
setLEDRow(LED_ROW_3,0xFF);

delay(2000);

gpio_put(LED_BUILTIN, true); // toggle the LED
setLEDRow(LED_ROW_0,0x00);
setLEDRow(LED_ROW_1,0x00);
setLEDRow(LED_ROW_2,0x00);
setLEDRow(LED_ROW_3,0x00);

}



/*
[仮想関数]カウンタ閾値に達した⇒MIDIクロックがカウントアップをセット
*/
void panelManager::trigger() {

    //シーケンスインデックスが末尾まで進んだら先頭に戻す
    if ( _sequenceList_index >= PANEL_MANAGER_SEQ_LIST){
        _sequenceList_index = 0;
    }

    //シーケンスリストより実施シーケンスを取得
    PANEL_MANAGER_SEQ _sequence=_sequenceList[_sequenceList_index];

    /*
    Serial.print("panelManager::trigger. _sequenceList_index:");
    Serial.print(_sequenceList_index);
    Serial.print(" _sequence:");
    Serial.println(static_cast<int>(_sequence));
    */

    //スイッチ入力
    if ((_sequence == PANEL_MANAGER_SEQ::SW_1ST_ROW0) ||
        (_sequence == PANEL_MANAGER_SEQ::SW_1ST_ROW1) ||
        (_sequence == PANEL_MANAGER_SEQ::SW_1ST_ROW2) ||
        (_sequence == PANEL_MANAGER_SEQ::SW_1ST_ROW3) ||
        (_sequence == PANEL_MANAGER_SEQ::SW_2ST_ROW0) ||
        (_sequence == PANEL_MANAGER_SEQ::SW_2ST_ROW1) ||
        (_sequence == PANEL_MANAGER_SEQ::SW_2ST_ROW2) ||
        (_sequence == PANEL_MANAGER_SEQ::SW_2ST_ROW3)) {

        //I2C::スイッチ入力
        int _SW_Value = 0x00;
        int _SW_Row = 0;
        int _SW_Row_Addr = 0;

        if ((_sequence == PANEL_MANAGER_SEQ::SW_1ST_ROW0) ||
           (_sequence == PANEL_MANAGER_SEQ::SW_2ST_ROW0)) {
            _SW_Row = 0;
            _SW_Row_Addr = 0x01;
            }
        else if ((_sequence == PANEL_MANAGER_SEQ::SW_1ST_ROW1) ||
                (_sequence == PANEL_MANAGER_SEQ::SW_2ST_ROW1)) {
            _SW_Row = 1;
            _SW_Row_Addr = 0x02;
            }
        else if ((_sequence == PANEL_MANAGER_SEQ::SW_1ST_ROW2) ||
                (_sequence == PANEL_MANAGER_SEQ::SW_2ST_ROW2)) {
            _SW_Row = 2;
            _SW_Row_Addr = 0x04;
            }
        else if ((_sequence == PANEL_MANAGER_SEQ::SW_1ST_ROW3) ||
                (_sequence == PANEL_MANAGER_SEQ::SW_2ST_ROW3)) {
            _SW_Row = 3;
            _SW_Row_Addr = 0x08;
            }



        /*Serial.print("panelManager::trigger.");
        Serial.print(" _matrixSwitch.getScan():");
        Serial.print(_matrixSwitch.getScan());
        Serial.print(" _SW_Row:");
        Serial.print(_SW_Row,HEX);
        Serial.print(" _SW_Row_Addr:");
        Serial.print(_SW_Row_Addr,HEX);*/

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
    if ((_sequence == PANEL_MANAGER_SEQ::LED_ROW0) ||
        (_sequence == PANEL_MANAGER_SEQ::LED_ROW1) ||
        (_sequence == PANEL_MANAGER_SEQ::LED_ROW2) ||
        (_sequence == PANEL_MANAGER_SEQ::LED_ROW3)) {


        int _LED_Row = 0;
        int _LED_Row_value = 0;

        if (_sequence == PANEL_MANAGER_SEQ::LED_ROW0) {
            _LED_Row = 0;
            _LED_Row_value = 0x01;
            }
        else if (_sequence == PANEL_MANAGER_SEQ::LED_ROW1) {
            _LED_Row = 1;
            _LED_Row_value = 0x02;
            }
        else if (_sequence == PANEL_MANAGER_SEQ::LED_ROW2)  {
            _LED_Row = 2;
            _LED_Row_value = 0x04;
            }
        else if (_sequence == PANEL_MANAGER_SEQ::LED_ROW3) {
            _LED_Row = 3;
            _LED_Row_value = 0x08;
            }


        //I2C::LED出力 
        int _LED_Col_value = static_cast<int>(_matrixLED.getRow(_LED_Row));   
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

        //LED出力用アドレスCol値書き込み
        //⇒GPIOB(13) Bポート値 無選択状態に設定⇒これが無いと次の行に列の表示内容が薄く残る？対策
        Wire.beginTransmission(I2C_ADDR_LED);
        Wire.write(0x13); 
        Wire.write(0xff);
        Wire.endTransmission();
    }

    //次回スイッチスキャン回数を更新
    if ((_sequence == PANEL_MANAGER_SEQ::SW_1ST_ROW3) ||
        (_sequence == PANEL_MANAGER_SEQ::SW_2ST_ROW3)) {
        _matrixSwitch.nextScan();

        //SWスキャンインデックス：決定 ならスイッチ入力を確定する
        if (_matrixSwitch.getScan() == static_cast<int>(SwitchScan::Finalize)) {
            _matrixSwitch.finalize();
            _matrixSwitch.nextScan();

            /*
            int _scanrow;
            for (_scanrow=0; _scanrow < SW_ROW_MAX ; _scanrow++){
              if ( 0x00 != _matrixSwitch.getRow(_scanrow)){
                Serial.print("_matrixSwitch.getRow(");
                Serial.print(_scanrow);
                Serial.print("):");
                Serial.println(static_cast<int>(_matrixSwitch.getRow(_scanrow)),HEX);
              }
            }
            */

            gpio_put(LED_BUILTIN, !gpio_get(LED_BUILTIN)); // toggle the LED
            //Serial.println("_matrixSwitch.finalize");
        }
    }

    //テンポADC値読み取り
    if (_sequence == PANEL_MANAGER_SEQ::TEMPO_ADC_READ) {
        _tempo_adc_value=adc_read();
        //Serial.print("_tempo_adc_value=adc_read() >>4:");
        //Serial.println(_tempo_adc_value>>4,HEX);
    }

    //次回シークエンス&シークエンス1周完了フラグを設定
    if (_sequence == PANEL_MANAGER_SEQ::END) {
        _sequence_up = true;
    }

    _sequenceList_index++;

}


/*
FRAMへの接続を開始する
戻り値：bool true=>接続成功 ,false=>接続失敗
*/
bool panelManager::connectFRAM(){

    //FRAMからシークエンスマップをロードする
    int rv = fram.begin(I2C_ADDR_FRAM11);
    bool _ret = false;

    if (rv != 0)    {
        Serial.print("fram INIT ERROR: ");
        Serial.println(rv);
        _ret = false;

    } else {
        Serial.println("fram connected: ");
        _ret = true;
    }

    _connectFRAM = _ret;
    return _ret;
}

/*
FRAMからビットストリームをロードする
引数  ：設定先ビットストリーム, ロードするバイト数
戻り値：なし
*/
void panelManager::loadFRAM(unsigned char* _bitstream, int _loadSize ){
    if (_connectFRAM) {
        fram.read(0x000, _bitstream,_loadSize);
    } 
}

/*
FRAMへビットストリームをセーブする
引数  ：設定先ビットストリーム, ロードするバイト数
戻り値：なし
*/
void panelManager::saveFRAM(int _startAddres , unsigned char* _bitstream, int _loadSize){
    if (_connectFRAM) {

			Serial.print("panelManager::saveFRAM _startAddres:");
			Serial.print(_startAddres,HEX);
			Serial.print(" _loadSize:");
			Serial.print(_loadSize,HEX);
            Serial.println("");
        //fram.write(_startAddres, _bitstream, _loadSize);
    }     
}


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
