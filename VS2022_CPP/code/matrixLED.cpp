#include "matrixLED.h"


matrixLED::matrixLED(){
        for (int i = 0;i < LED_INDEX_MAX;i++) {
            _currentLED[i] = false;
        }
}

void matrixLED::setRow(int row, char value) {
    int _rowIndex = row * LED_COL_MAX;
    char _maskbit;

    for (int i = 0;i < LED_COL_MAX;i++) {
        _maskbit = 1 << i;
        _currentLED[_rowIndex + i] = _maskbit & value;
    }
}

char matrixLED::getRow(int row) {
    char _ret = 0x00;
    int _rowIndex = row * LED_COL_MAX;

    //bit7-1‚ð_ret‚ÉÝ’è
    for (int i = LED_COL_MAX - 1;1 <= i;i--) {
        _ret = _ret + (char)_currentLED[_rowIndex + i];
        _ret = _ret << 1;
    }

    //bit0‚ð_ret‚ÉÝ’è
    _ret = _ret + (char)_currentLED[_rowIndex];

    return _ret;
}

void matrixLED::set(int index, bool value) {
    _currentLED[index] = value;
}

bool matrixLED::get(int index) {
    return _currentLED[index];
}
