#include "paternWrite.h"

paternWrite::paternWrite(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) :mode(MODE_NAME::PATERN_WRITE, ptPanelManager, ptVoltage, ptSequenceMap) {

	//�e��Ԃ������l�ɕύX����
	_run_stop = RUN_STOP::STOP;		//����/�X�g�b�v�t���O
	_pattern = 0;	//�w��p�^�[��
	_step = 0;	//���݃X�e�b�v
}

/*
[���z�֐�]�J�E���g臒l�B�����Ɏ��s�����A�v���P�[�V���������{����
*/
void paternWrite::runSequence() {

	//����/�X�g�b�v�t���O:����
	if (_run_stop == RUN_STOP::RUN) {
		execRunSequence();

		//����/�X�g�b�v�t���O:�X�g�b�v
	}
	else	if (_run_stop == RUN_STOP::STOP) {
		execStopSequence();
	}
}


/*
[���z�֐�]MIDI�N���b�N�J�E���g臒l�B�����Ɏ��s�����A�v���P�[�V���������{����
*/
void paternWrite::runClock() {

	//����/�X�g�b�v�t���O:����
	if (_run_stop == RUN_STOP::RUN) {
		execRunClock();

		//����/�X�g�b�v�t���O:�X�g�b�v
	}
	else	if (_run_stop == RUN_STOP::STOP) {
		execStopClock();
	}
}


/*
����/�X�g�b�v�t���O:���� �V�[�N�G���X���������s
*/
void	paternWrite::execRunSequence() {
}

/*
����/�X�g�b�v�t���O:�X�g�b�v �V�[�N�G���X���������s
*/
void	paternWrite::execStopSequence() {
}

/*
����/�X�g�b�v�t���O:���� MIDI�N���b�N�J�E���g���������s
*/
void	paternWrite::execRunClock() {
}

/*
����/�X�g�b�v�t���O:�X�g�b�v MIDI�N���b�N�J�E���g���������s
*/
void	paternWrite::execStopClock() {
}
