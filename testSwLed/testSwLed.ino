/*
テストモード：スイッチ&LEDパネル
入力：マトリクススイッチ
出力：マトリクスLED
*/


#include <Wire.h>

#include "matrixLED.h"


//I2Cアドレス
const char I2C_ADDR_SW  = 0x20; //MCP27017:マトリクススイッチ
const char I2C_ADDR_LED = 0x21; //MCP27017:LEDスイッチ
const char I2C_ADDR_EEPROM = 0x50; //24LC256

//マトリクススイッチ
//サンプリング回数 (0~設定値-1)
#define SW_CNT_SAMPLE 3
//スキャン行数 (0~設定値-1)
#define SW_MAX_ROW 8

char sampleSwitch; //現在のサンプリング回数(0~設定値-1)
char scanSwitch[SW_CNT_SAMPLE][SW_MAX_ROW]; //サンプリング結果
char valueSwitch[SW_MAX_ROW]; //最新の値

//マトリクスLED
//LED行数 (0~設定値-1)
//#define LED_MAX_ROW 4
//char valueLED[LED_MAX_ROW]; //最新の値

//マトリクスLED
matrixLED cMatrixLed;

bool toggle_panelWR(void *) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle the LED
  return true; // repeat? true
}


/*
マトリクススイッチ,マトリクスLED関連配列を初期化
*/
/*
void init_PanelArray(){

  char sampleSwitch = 0;

  for (int i=0;i<SW_CNT_SAMPLE;i++){
    for (int j=0;j<SW_MAX_ROW;j++){
      scanSwitch[i][j]  = 0x00;
    }
  }

  for (int k=0;k<SW_MAX_ROW;k++){
    valueSwitch[k]  = 0x00;
  }

  for (int l=0;l<SW_MAX_ROW;l++){
    valueLED[l]  = 0xFF;
  }
}
*/



void setup() {

  pinMode(LED_BUILTIN, OUTPUT); // set LED pin to OUTPUT
  Wire.begin(); //I2C使用開始

}

void loop() {

}
