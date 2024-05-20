/*
マトリクススイッチの状態を管理する
*/

#ifndef matrixSwitch_h
#define matrixSwitch_h

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
    explicit matrixSwitch(){
        _scan	=	static_cast<int>(SwitchScan::Scan1st);

        for ( int i = 0;i<SW_SCAN_MAX;i++){
            for ( int j = 0;j<SW_INDEX_MAX;j++){
                _currentSwtich[i][j]=false;
            }
        }
    }

    /*
    現在のスキャン状態を取得
    */
    int getScan(){
        return _scan;
    }

    /*
    次のスキャン状態を設定＆取得
    */
    void nextScan(){
        if	(	_scan == static_cast<int>(SwitchScan::Finalize) )	{
            _scan	=	static_cast<int>(SwitchScan::Scan1st);
        }	else	{
            _scan++;
        }
    }


    /*
    指定スキャン,行のスイッチ状態列を設定する
    */
    void setRow(int scan, int row, char value){

        if	( _scan == static_cast<int>(SwitchScan::Finalize) )	{
            return;
        }

        int _rowIndex=row*SW_COL_MAX;
        char _maskbit;
 
        for ( int i=0;i<SW_COL_MAX;i++){
            _maskbit=1<<i;
            _currentSwtich[scan][_rowIndex+i]= _maskbit & value;
        }
    }

    /*
    スキャン回数より決定を設定する
    */
    void finalize(){

        if	(	_scan != static_cast<int>(SwitchScan::Finalize) )	{
            return;
        }

        for ( int i=0;i<SW_INDEX_MAX;i++){
			_currentSwtich[static_cast<int>(SwitchScan::Finalize)][i]=
				_currentSwtich[static_cast<int>(SwitchScan::Scan1st)][i] &
				_currentSwtich[static_cast<int>(SwitchScan::Scan2nd)][i];
		}
	}


    /*
    スキャン:決定 より指定行のスイッチ状態列を取得する
    */
    char getRow(int row){
        char _ret = 0x00;
        int _rowIndex=row*SW_COL_MAX;
        int _scanFinal=static_cast<int>(SwitchScan::Finalize);

        //bit7-1を_retに設定
        for ( int i=SW_COL_MAX-1;1<=i;i--){
            _ret=_ret + (char)_currentSwtich[_scanFinal][_rowIndex+i];
            _ret=_ret<<1;
        }

        //bit0を_retに設定
        _ret=_ret + (char)_currentSwtich[_scanFinal][_rowIndex];

        return _ret;
    }

    /*
    スキャン:決定 より指定インデックスのスイッチ状態を取得する
    */
    bool get(int index){
        return _currentSwtich[static_cast<int>(SwitchScan::Finalize)][index];
    }

private:
	int	_scan;
    bool	_currentSwtich[SW_SCAN_MAX][SW_INDEX_MAX];

};
#endif
