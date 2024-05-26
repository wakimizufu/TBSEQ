#include "panelManager.h"

/*
�R���X�g���N�^
start     :�J�E���^�J�n�l(�f�t�H���g=0)
*/
panelManager::panelManager(unsigned int start = 0) :countTriger(THD_PANEL_MANAGER, start) {
    _sequence = static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW0);
    _sequence_up = false;
}

/*
[���z�֐�]�J�E���^臒l�ɒB������MIDI�N���b�N���J�E���g�A�b�v���Z�b�g
*/
void panelManager::trigger() {
    int _seq_value = _sequence / 3;
    int _seq_mod = _sequence % 3;

    //�X�C�b�`����
    if ((_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW0)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW1)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW2)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW3)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW0)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW1)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW2)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW3))) {

        //I2C::�X�C�b�`����
        char _SW_Value = 0x00;
        /*
        Wire.requestFrom(2, 6);    // Request 6 bytes from slave device number two
        while(Wire.available()) {
            _SW_Value = Wire.read();    // Receive a byte as character
        }
        */

        int _SW_Row = ((_seq_value * 2) + _seq_mod) % 4;

        std::cout << "panelManager::_matrixSwitch.setRow() scan:" << _matrixSwitch.getScan() << " _SW_Row:" << _SW_Row << " _SW_Value:" << _SW_Value << std::endl;
        _matrixSwitch.setRow(_matrixSwitch.getScan(), _SW_Row, _SW_Value);
    }


    //LED�o��
    if ((_sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW0)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW1)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW2)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW3))) {

        //I2C::LED�o�� 
        char _LED_Row_value = 0x00;
        std::cout << "panelManager::_matrixLED.setRow row:" << _seq_value << " _LED_Row_value:" << _LED_Row_value << std::endl;
        _matrixLED.setRow(_seq_value, _LED_Row_value);
        //Wire.beginTransmission(44);  // Transmit to device number 44 (0x2C)
        //Wire.write(_LED_Row_value);  // Sends value byte
        //Wire.endTransmission();      // Stop transmitting
    }

    //����X�C�b�`�X�L�����񐔂��X�V
    if ((_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW3)) ||
        (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::SW_2ST_ROW3))) {
        _matrixSwitch.nextScan();

        //SW�X�L�����C���f�b�N�X�F���� �Ȃ�X�C�b�`���͂��m�肷��
        if (_matrixSwitch.getScan() == static_cast<int>(SwitchScan::Finalize)) {
            _matrixSwitch.finalize();
            _matrixSwitch.nextScan();
        }
    }

    //����V�[�N�G���X&�V�[�N�G���X1�������t���O��ݒ�
    if (_sequence == static_cast<int>(PANEL_MANAGER_SEQ::LED_ROW3)) {
        _sequence = static_cast<int>(PANEL_MANAGER_SEQ::SW_1ST_ROW0);
        _sequence_up = true;
    }
    else {
        _sequence++;
    }
};

/*
�w��LED�C���f�b�N�X�ւ̐ݒ���s��
index�Fenum class LED
value�Ftrue=>�_�� , false=>����
�߂�l�F�Ȃ�
*/
void panelManager::setLED(int index, bool value) {
    _matrixLED.set(index, value);
}

/*
�w��LED�s�̒l(bit7�`0)���擾����
index�F�w��LED�s
value�Fchar �w��LED�s�̒l(bit7�`0)
�߂�l�F�Ȃ�
*/
void panelManager::setLEDRow(int row, char value) {
    _matrixLED.setRow(row, value);
}


/*
�w��LED�C���f�b�N�X�̒l���擾����
index�Fenum class LED
�߂�l�Fbool true=>�_�� , false=>����
*/
bool panelManager::getLED(int index) {
    return _matrixLED.get(index);
}

/*
�w��LED�s�̒l(bit7�`0)���擾����
index�F�w��LED�s
�߂�l�Fchar �w��LED�s�̒l(bit7�`0)
*/
char panelManager::getLEDRow(int row) {
    return _matrixLED.getRow(row);
}

/*
�w��X�C�b�`�C���f�b�N�X�̒l���擾����
index�Fenum class Switch
�߂�l�Fbool true=>���� , false=>�J��
*/
bool panelManager::getSwitch(int index) {
    return _matrixSwitch.get(index);
}

/*
�w��X�C�b�`�s�̒l(bit7�`0)���擾����
index�F�w��X�C�b�`�s
�߂�l�Fchar �w��X�C�b�`�s�̒l(bit7�`0)
*/
char panelManager::getSwitchRow(int row) {
    return _matrixSwitch.getRow(row);
}

/*
�V�[�N�G���X1�������t���O�������҂��ɕύX
*/
void panelManager::clear() {
    _sequence_up = false;
}

/*
�V�[�N�G���X1�������t���O���擾
*/
bool panelManager::getSequenceUp() {
    return _sequence_up;
}
