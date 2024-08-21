/*
マトリクスLEDの状態を管理する
*/

#ifndef matrixLED_h
#define matrixLED_h

#include <stdint.h> 

//マトリクスLED要素数
#define LED_INDEX_MAX 32

//マトリクスLED行長
#define LED_ROW_MAX 2

//マトリクスLED列長
#define LED_COL_MAX 16

//マトリクスLED行インデックス
#define LED_ROW_0 0
#define LED_ROW_1 1
#define LED_ROW_2 2
#define LED_ROW_3 3


//LED名⇒インデックス
enum class LED
{
    C,          //col0,LED0
    CSHARP,     //col0,LED1    
    D,          //col0,LED2
    DSHARP,     //col0,LED3
    E,          //col0,LED4
    F,          //col0,LED5
    FSHARP,     //col0,LED6
    G,          //col0,LED7
    GSHARP,     //col0,LED8
    A,          //col0,LED9
    ASHARP,     //col0,LED10
    B,          //col0,LED11
    C2,         //col0,LED12
    UP,         //col0,LED13
    DOWN,       //col0,LED14
    NOTE_ON,    //col0,LED15
    NOTE_TIE,   //col1,LED0
    SLIDE,      //col1,LED1
    ACC,        //col1,LED2
    LENMAX,     //col1,LED3
    RUN_STOP,   //col1,LED4
    TRACK,      //col1,LED5
    PATTERN,    //col1,LED6
    PLAY_WRITE, //col1,LED7
    BANK_A,     //col1,LED8
    BANK_B,     //col1,LED9
    BANK_C,     //col1,LED10
    BANK_D,     //col1,LED11
    LED29,      //col1,LED12
    LED30,      //col1,LED13
    LED31,      //col1,LED14
    LED32,      //col1,LED15
};

class matrixLED
{
public:
    /*
    コンストラクタ
    */
    matrixLED();

    void setRow(int row, uint16_t value);

    uint16_t getRow(int row);
  

    void set(int index, bool value);

    bool get(int index);

private:
    bool _currentLED[LED_INDEX_MAX];

};

#endif