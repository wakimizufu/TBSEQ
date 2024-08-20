#include "matrixLED.h"


matrixLED::matrixLED(){
        for (int i = 0;i < LED_INDEX_MAX;i++) {
            _currentLED[i] = false;
        }
}

void matrixLED::setRow(int row, uint8_t value) {
    uint8_t _maskbit;
    int _rowIndex = row * LED_COL_MAX;
    if (_rowIndex >= LED_INDEX_MAX) {
        _rowIndex = 0;
    }

    for (int i = 0;i < LED_COL_MAX;i++) {
        _maskbit = 1 << i;
        _currentLED[_rowIndex + i] = _maskbit & value;
    }
}

uint8_t matrixLED::getRow(int row) {
    uint8_t _ret = 0x00;
    int _rowIndex = row * LED_COL_MAX;

    if (_rowIndex >= LED_INDEX_MAX) {
        _rowIndex = 0;
    }

    //bit16-1を_retに設定
    for (int i = LED_COL_MAX - 1;1 <= i;i--) {
        _ret = _ret + (uint8_t)_currentLED[_rowIndex + i];
        _ret = _ret << 1;
    }

    //bit0を_retに設定
    _ret = _ret + (uint8_t)_currentLED[_rowIndex];

    return _ret;
}

void matrixLED::set(int index, bool value) {
    _currentLED[index] = value;
}

bool matrixLED::get(int index) {
    return _currentLED[index];
}
