#include "trackWrite.h"

/*
�R���X�g���N�^
	ptPanelManager:panelManager�N���X�|�C���^
	ptVoltage     :voltage�N���X�|�C���^
*/
trackWrite::trackWrite(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) :mode(MODE_NAME::TRACK_WRITE, ptPanelManager, ptVoltage, ptSequenceMap) {
}

/*
[���z�֐�]�J�E���g臒l�B�����Ɏ��s�����A�v���P�[�V���������{����
*/
void trackWrite::runSequence() {
}

/*
[���z�֐�]MIDI�N���b�N�J�E���g臒l�B�����Ɏ��s�����A�v���P�[�V���������{����
*/
void trackWrite::runClock() {
}