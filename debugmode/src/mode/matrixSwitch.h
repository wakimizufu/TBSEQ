/*
マトリクススイッチの状態を管理する
*/

#ifndef matrixSwitch_h
#define matrixSwitch_h


//#include <Arduino.h>

//マトリクススイッチ要素数
#define SW_INDEX_MAX 32

//マトリクススイッチ行長
#define SW_ROW_MAX 4

//マトリクススイッチ列長
#define SW_COL_MAX 8

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
    GSHARP,     //row1,col0
    A,          //row1,col1
    ASHARP,     //row1,col2
    B,          //row1,col3
    C2,         //row1,col4
    UP,         //row1,col5
    DOWN,       //row1,col6
    AC,         //row1,col7
    SLIDE,      //row2,col0
    NOTE,       //row2,col1
    STEP,       //row2,col2
    BACK,       //row2,col3
    PITCH,      //row2,col4
    CLEAR,      //row2,col5
    RUN,        //row2,col6
    SW24,       //row2,col7
    TRACK,      //row3,col0
    PATTERN,    //row3,col1
    PLAY,       //row3,col2
    WRITE,      //row3,col3
    FUNCTION,   //row3,col4
    SW30,       //row3,col5
    SW31,       //row3,col6
    SW32,       //row3,col7
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

private:
	int	_scan;
    bool	_currentSwtich[SW_SCAN_MAX][SW_INDEX_MAX];

};
#endif
