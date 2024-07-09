/*
マトリクスLEDの状態を管理する
*/

#ifndef matrixLED_h
#define matrixLED_h


//マトリクスLED要素数
#define LED_INDEX_MAX 32

//マトリクスLED行長
#define LED_ROW_MAX 4

//マトリクスLED列長
#define LED_COL_MAX 8

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
    GSHARP,     //col1,LED0
    A,          //col1,LED1
    ASHARP,     //col1,LED2
    B,          //col1,LED3
    C2,         //col1,LED4
    UP,         //col1,LED5
    DOWN,       //col1,LED6
    AC,         //col1,LED7
    SLIDE,      //col2,LED0
    NOTE,       //col2,LED1
    PITCH,      //col2,LED2
    RUN,        //col2,LED3
    TRACK,      //col2,LED4
    PATTERN,    //col2,LED5
    PLAY,       //col2,LED6
    WRITE,      //col2,LED7
    LED25,      //col3,LED0
    LED26,      //col3,LED1
    LED27,      //col3,LED2
    LED28,      //col3,LED3
    LED29,      //col3,LED4
    LED30,      //col3,LED5
    LED31,      //col3,LED6
    LED32,      //col3,LED7
};

class matrixLED
{
public:
    /*
    コンストラクタ
    */
    matrixLED();

    void setRow(int row, char value);

    char getRow(int row);
  

    void set(int index, bool value);

    bool get(int index);

private:
    bool _currentLED[LED_INDEX_MAX];

};

#endif