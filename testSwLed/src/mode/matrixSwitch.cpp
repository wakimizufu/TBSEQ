#include "matrixSwitch.h"

/*
�R���X�g���N�^
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
���݂̃X�L������Ԃ��擾
*/
int matrixSwitch::getScan() {
    return _scan;
}

/*
���̃X�L������Ԃ�ݒ聕�擾
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
�w��X�L����,�s�̃X�C�b�`��ԗ��ݒ肷��
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
�X�L�����񐔂�茈���ݒ肷��
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
�X�L����:���� ���w��s�̃X�C�b�`��ԗ���擾����
*/
char matrixSwitch::getRow(int row) {
    char _ret = 0x00;
    int _rowIndex = row * SW_COL_MAX;
    int _scanFinal = static_cast<int>(SwitchScan::Finalize);

    //bit7-1��_ret�ɐݒ�
    for (int i = SW_COL_MAX - 1;1 <= i;i--) {
        _ret = _ret + (char)_currentSwtich[_scanFinal][_rowIndex + i];
        _ret = _ret << 1;
    }

    //bit0��_ret�ɐݒ�
    _ret = _ret + (char)_currentSwtich[_scanFinal][_rowIndex];

    return _ret;
}

/*
�X�L����:���� ���w��C���f�b�N�X�̃X�C�b�`��Ԃ��擾����
*/
bool matrixSwitch::get(int index) {
    return _currentSwtich[static_cast<int>(SwitchScan::Finalize)][index];
}