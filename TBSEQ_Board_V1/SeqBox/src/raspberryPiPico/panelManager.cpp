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

//matrixLED 全クリア
setLEDRow(LED_ROW_0, 0x0000);
setLEDRow(LED_ROW_1, 0x0000);

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
    if ( _sequenceList_index >= (sizeof(_sequenceList)/sizeof(PANEL_MANAGER_SEQ))){
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

        //Serial.println(" PANEL_MANAGER_SEQ::SW_READ");

        int scan = _matrixSwitch.getScan();
        HT16K33::KEYDATA keydata;
        _HT16K33.readKeyRaw(keydata,true);

        /*
        Serial.print(" scan:");
        Serial.print(scan);
        Serial.println("");                             
        */

        //Serial.println(" _HT16K33.readKeyRaw");

        for (uint8_t swrow=0 ;swrow<SW_ROW_MAX ;swrow++){

            /*
            if (0x0000!=keydata[swrow]){
                Serial.print("keydata[");
                Serial.print(swrow);
                Serial.print("] :");
                Serial.print(keydata[swrow],HEX);
                Serial.println("");                             
            }
            */
            

            for (uint8_t swcol=0 ;swcol<SW_COL_MAX ;swcol++){

                swIdx = (swrow*SW_COL_MAX)+swcol;
                maskbit = 1 << swcol;
                swValue = keydata[swrow] & maskbit;

                /*
                Serial.print(" swrow:");
                Serial.print(swrow);
                Serial.print(" swcol:");
                Serial.print(swcol);
                Serial.print("swIdx:");
                Serial.print(swIdx);
                Serial.print("maskbit:");
                Serial.print(maskbit,HEX);
                Serial.print("swValue:");
                Serial.print(swValue,HEX);
                Serial.println("");

                if(swValue){
                    Serial.print(" scan:");
                    Serial.print(scan);
                    Serial.print(" swrow:");
                    Serial.print(swrow);
                    Serial.print(" swcol:");
                    Serial.print(swcol);
                    Serial.print(" swIdx:");
                    Serial.print(swIdx);
                    Serial.print(" maskbit:");
                    Serial.print(maskbit,HEX);
                    Serial.println("");
                }
                */

                _matrixSwitch.set(scan,swIdx,swValue);
            }
        }




        //SWスキャンインデックス：決定 ならスイッチ入力を確定する
        if (_matrixSwitch.getScan() == static_cast<int>(SwitchScan::Finalize)) {
            _matrixSwitch.finalize();

            /*
            for (int _scanidx=0; _scanidx < SW_INDEX_MAX ; _scanidx++){
              if ( _matrixSwitch.get(_scanidx)){
                Serial.print("Finalize:");
                Serial.print(_scanidx);
                Serial.println("");
              }
            }
            */

            gpio_put(LED_BUILTIN, !gpio_get(LED_BUILTIN)); // toggle the LED
            //Serial.println("_matrixSwitch.finalize");
        }

        _matrixSwitch.nextScan();
    }


    //LED出力
    if ((_sequence == PANEL_MANAGER_SEQ::LED_WRITE)) {

        //I2C::LED出力 
        for ( int led=0 ; led < LED_INDEX_MAX ; led++){
            if (_matrixLED.get(led)) {
                _HT16K33.setLed(led);
            } else {
                _HT16K33.clearLed(led);
            }
        }

        _HT16K33.sendLed();
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

    //Serial.println("panelManager::trigger end");

    _sequenceList_index++;

}


/*
FRAMへの接続を開始する
戻り値：bool true=>接続成功 ,false=>接続失敗
*/
bool panelManager::connectFRAM(){

  Wire.beginTransmission(I2C_ADDR_FRAM11);

  int ret = Wire.endTransmission();
  _connectFRAM = (ret == 0);

   Serial.print("panelManager::connectFRAM ret:");
   Serial.print(ret);
   Serial.print(" _connectFRAM:");
   Serial.print(_connectFRAM);
   Serial.println("");

   return _connectFRAM;
}

/*
FRAMからビットストリームをロードする
引数  ：設定先ビットストリーム, ロードするバイト数
戻り値：なし
*/
void panelManager::loadFRAM(unsigned char* _bitstream, int _loadSize ){
    if (_connectFRAM) {
        //fram.read(0x000, _bitstream,_loadSize);
        //  void FRAM::read(uint16_t memAddr, uint8_t * obj, uint16_t size)

        uint16_t memAddr = 0x0000;
        uint8_t * obj = _bitstream;
        uint16_t size = _loadSize;

        const uint8_t blocksize = 24;
        uint8_t * p = obj;
        while (size >= blocksize)
        {
            _readBlock(memAddr, p, blocksize);
            memAddr += blocksize;
            p += blocksize;
            size -= blocksize;
        }
        //  remainder
        if (size > 0)
        {
            _readBlock(memAddr, p, size);
        }
    } 
}

void panelManager::_readBlock(uint16_t memAddr, uint8_t * obj, uint8_t size)
{
  //  Device uses Address Pages
  uint8_t DeviceAddrWithPageBits = I2C_ADDR_FRAM11 | ((memAddr & 0x0700) >> 8);
  Wire.beginTransmission(DeviceAddrWithPageBits);
  Wire.write((uint8_t) (memAddr & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(DeviceAddrWithPageBits, size);

  uint8_t * p = obj;
  for (uint8_t i = size; i > 0; i--)
  {
    *p++ = Wire.read();
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

        //fram.write(_startAddres, _bitstream, _loadSize);
        //  void FRAM::write(uint16_t memAddr, uint8_t * obj, uint16_t size)

        uint16_t memAddr = _startAddres;
        uint8_t * obj = _bitstream;
        uint16_t size = _loadSize;

        const int blocksize = 24;
        uint8_t * p = obj;
        while (size >= blocksize)
        {
            _writeBlock(memAddr, p, blocksize);
            memAddr += blocksize;
            p += blocksize;
            size -= blocksize;
        }
        //  remaining
        if (size > 0)
        {
            _writeBlock(memAddr, p, size);
        }

        Serial.println("panelManager::saveFRAM Finish");
    }

}     

void panelManager::_writeBlock(uint16_t memAddr, uint8_t * obj, uint8_t size)
{
  //  Device uses Address Pages
  uint8_t  DeviceAddrWithPageBits = I2C_ADDR_FRAM11 | ((memAddr & 0x0700) >> 8);
  Wire.beginTransmission(DeviceAddrWithPageBits);
  Wire.write((uint8_t) (memAddr & 0xFF));

  uint8_t * p = obj;
  for (uint8_t i = size; i > 0; i--)
  {
    Wire.write(*p++);
  }
  Wire.endTransmission();
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
