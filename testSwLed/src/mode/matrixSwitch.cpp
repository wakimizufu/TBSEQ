#include "matrixSwitch.h"

/*
コンストラクタ
*/
matrixSwitch::matrixSwitch() {
    _scan = static_cast<int>(SwitchScan::Scan1st);

    for (int i = 0;i < SW_SCAN_MAX;i++) {
        for (int j = 0;j < SW_INDEX_MAX;j++) {
            _currentSwtich[i][j] = false;
        }
    }
}

/*
現在のスキャン状態を取得
*/
int matrixSwitch::getScan() {
    return _scan;
}

/*
次のスキャン状態を設定＆取得
*/
void matrixSwitch::nextScan() {
    if (_scan == static_cast<int>(SwitchScan::Finalize)) {
        _scan = static_cast<int>(SwitchScan::Scan1st);
    }
    else {
        _scan++;
    }
}


/*
指定スキャン,行のスイッチ状態列を設定する
*/
void matrixSwitch::setRow(int scan, int row, char value) {

    if (_scan == static_cast<int>(SwitchScan::Finalize)) {
        return;
    }

    int _rowIndex = row * SW_COL_MAX;
    char _maskbit;

    for (int i = 0;i < SW_COL_MAX;i++) {
        _maskbit = 1 << i;
        _currentSwtich[scan][_rowIndex + i] = _maskbit & value;
    }
}

/*
スキャン回数より決定を設定する
*/
void matrixSwitch::finalize() {

    if (_scan != static_cast<int>(SwitchScan::Finalize)) {
        return;
    }

    for (int i = 0;i < SW_INDEX_MAX;i++) {
        _currentSwtich[static_cast<int>(SwitchScan::Finalize)][i] =
            _currentSwtich[static_cast<int>(SwitchScan::Scan1st)][i] &&
            _currentSwtich[static_cast<int>(SwitchScan::Scan2nd)][i];
    }
}


/*
スキャン:決定 より指定行のスイッチ状態列を取得する
*/
char matrixSwitch::getRow(int row) {
    char _ret = 0x00;
    int _rowIndex = row * SW_COL_MAX;
    int _scanFinal = static_cast<int>(SwitchScan::Finalize);

    //bit7-1を_retに設定
    for (int i = SW_COL_MAX - 1;1 <= i;i--) {
        _ret = _ret + (char)_currentSwtich[_scanFinal][_rowIndex + i];
        _ret = _ret << 1;
    }

    //bit0を_retに設定
    _ret = _ret + (char)_currentSwtich[_scanFinal][_rowIndex];

    return _ret;
}

/*
スキャン:決定 より指定インデックスのスイッチ状態を取得する
*/
bool matrixSwitch::get(int index) {
    return _currentSwtich[static_cast<int>(SwitchScan::Finalize)][index];
}