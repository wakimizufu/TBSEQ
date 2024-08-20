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

uint8_t led;

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

//HT16K33
// initialize everything, 0x00 is the i2c address for the first chip (0x70 is added in the class).
_HT16K33.begin(I2C_ADDR_HT16K33);

// flash the LEDs, first turn them on
for (led=0; led<LED_INDEX_MAX; led++) {
    _HT16K33.setLedNow(led);
    delay(10);
} // for led

//Next clear them
for (led=0; led<LED_INDEX_MAX; led++) {
    _HT16K33.clearLedNow(led);
    delay(10);
} // for led


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
    if ((_sequence == PANEL_MANAGER_SEQ::SW_READ) ) {

        int swIdx;
        uint16_t maskbit;
        bool swValue;

        int scan = _matrixSwitch.getScan();
        HT16K33::KEYDATA keydata;
        _HT16K33.readKeyRaw(keydata,true);

        for (uint8_t swrow=0 ;swrow<SW_ROW_MAX ;swrow++){
            for (uint8_t swcol=0 ;swrow<SW_COL_MAX ;swcol++){
                swIdx = (swrow*SW_ROW_MAX)+swcol;
                maskbit = 1 << swcol;
                swValue = keydata[swrow] & maskbit;

                _matrixSwitch.set(scan,swIdx,swValue);
            }
        }
    }


    //LED出力
    if ((_sequence == PANEL_MANAGER_SEQ::LED_WRITE)) {

        //I2C::LED出力 
        uint8_t _LED_Col_value = _matrixLED.getRow(_LED_Row);   

        for ( int led=0 ; led < LED_INDEX_MAX ; led++){
            if (_matrixLED.get(led)) {
                _HT16K33.setLed(led);
            } else {
                _HT16K33.clearLed(led);
            }
        }

        _HT16K33.sendLed();
    }

    //次回スイッチスキャン回数を更新
    if ((_sequence == PANEL_MANAGER_SEQ::SW_READ) ) {
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
引数  ：記録開始アドレス, 設定元ビットストリーム, セーブするバイト数
戻り値：なし
*/
void panelManager::saveFRAM(int _startAddres , unsigned char* _bitstream, int _loadSize){
    if (_connectFRAM) {

        Serial.print("panelManager::saveFRAM _startAddres:");
        Serial.print(_startAddres,HEX);
        Serial.print(" _loadSize:");
        Serial.print(_loadSize,HEX);
        Serial.println("");
        fram.write(_startAddres, _bitstream, _loadSize);
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
