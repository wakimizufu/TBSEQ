#include "trackPlay.h"

/*
�R���X�g���N�^
	ptPanelManager:panelManager�N���X�|�C���^
	ptVoltage     :voltage�N���X�|�C���^
*/
trackPlay::trackPlay(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) :mode(MODE_NAME::TRACK_PLAY, ptPanelManager, ptVoltage, ptSequenceMap) {
}

/*
[���z�֐�]�J�E���g臒l�B�����Ɏ��s�����A�v���P�[�V���������{����
*/
void trackPlay::runSequence() {
}

/*
[���z�֐�]MIDI�N���b�N�J�E���g臒l�B�����Ɏ��s�����A�v���P�[�V���������{����
*/
void trackPlay::runClock() {
}
