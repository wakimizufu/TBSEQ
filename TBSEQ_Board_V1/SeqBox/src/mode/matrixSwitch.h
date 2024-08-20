/*
マトリクススイッチの状態を管理する
*/

#ifndef matrixSwitch_h
#define matrixSwitch_h


//#include <Arduino.h>

//マトリクススイッチ要素数
#define SW_INDEX_MAX 34

//マトリクススイッチ行長
#define SW_ROW_MAX 3

//マトリクススイッチ列長
#define SW_COL_MAX 13

//SWスキャンカウント回数
#define SW_SCAN_MAX 3

//マトリクススイッチ行インデックス
#define SW_ROW_0 0
#define SW_ROW_1 1
#define SW_ROW_2 2
#define SW_ROW_3 3


//SWスキャンインデックス
enum class SwitchScan
{
		Scan1st,		//スキャン1回目
		Scan2nd,		//スキャン2回目
		Finalize,		//決定
};

//SW名⇒インデックス
enum class Switch
{
    C,          //row0,col0
    CSHARP,     //row0,col1    
    D,          //row0,col2
    DSHARP,     //row0,col3
    E,          //row0,col4
    F,          //row0,col5
    FSHARP,     //row0,col6
    G,          //row0,col7
    GSHARP,     //row0,col8
    A,          //row0,col9
    ASHARP,     //row0,col10
    B,          //row0,col11
    C2,         //row0,col12
    UP,         //row1,col0
    DOWN,       //row1,col1
    NOTE,       //row1,col2
    SLIDE,      //row1,col3
    ACC,        //row1,col4
    NEXT,       //row1,col5
    BACK,       //row1,col6
    LENMAX,     //row1,col7
    RUN_STOP,   //row1,col8
    FUNCTION,   //row1,col9
    TRACK,      //row1,col10
    PATTERN,    //row1,col11
    PLAY_WRITE, //row1,col12
    BANK_A,     //row2,col0
    BANK_B,     //row2,col1
    BANK_C,     //row2,col2
    BANK_D,     //row2,col3
    SW31,       //row2,col4
    SW32,       //row2,col5
    SW33,       //row2,col6
    SW34,       //row2,col7
};

class matrixSwitch
{
public:
    /*
    コンストラクタ
    */
    matrixSwitch();

    /*
    現在のスキャン状態を取得
    */
    int getScan();

    /*
    次のスキャン状態を設定＆取得
    */
    void nextScan();


    /*
    指定スキャン,行のスイッチ状態列を設定する
    */
    void setRow(int scan, int row, char value);

    /*
    スキャン回数より決定を設定する
    */
    void finalize();


    /*
    スキャン:決定 より指定行のスイッチ状態列を取得する
    */
    char getRow(int row);

    /*
    スキャン:決定 より指定インデックスのスイッチ状態を取得する
    */
    bool get(int index);

    /*
    指定スキャン,指定インデックスのスイッチ状態を設定する
    */
    void set(int scan, int index , bool value);

private:
	int	_scan;
    bool	_currentSwtich[SW_SCAN_MAX][SW_INDEX_MAX];

};
#endif
