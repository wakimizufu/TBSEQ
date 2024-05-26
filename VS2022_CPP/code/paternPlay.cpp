#include "paternPlay.h"

/*
�R���X�g���N�^
	ptPanelManager:panelManager�N���X�|�C���^
	ptVoltage     :voltage�N���X�|�C���^
*/
paternPlay::paternPlay(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) :mode(MODE_NAME::PATERN_PLAY, ptPanelManager, ptVoltage, ptSequenceMap) {

	//�e��Ԃ������l�ɕύX����
	_pattern = PATTERN_START_IDX;						//�w��p�^�[��
	_step = STEP_START_IDX;								//���݃X�e�b�v
	_LEDCount = 0;														//LED�_�ŃJ�E���g
	_pushRunSW = false;												//����/�X�g�b�v�O���ԃt���O
	_keyborad = false;												//�L�[�{�[�h�t���O
	_midiclock_16note = MIDICLOCK_START_16NOTE;	//16������MIDI�N���b�N�J�E���g

	//����/�X�g�b�v�t���O���X�g�b�v
	_run_stop = RUN_STOP::STOP;

	//voltage�N���X �o�͓��e�����Z�b�g
	_voltage->reset();

	//LED�o�͂��N���A
	_panelManager->setLEDRow(LED_ROW_0, 0x00);
	_panelManager->setLEDRow(LED_ROW_1, 0x00);
	_panelManager->setLEDRow(LED_ROW_2, 0x60);
	_panelManager->setLEDRow(LED_ROW_3, 0x00);
}

/*
[���z�֐�]�J�E���g臒l�B�����Ɏ��s�����A�v���P�[�V���������{����
*/
void paternPlay::runSequence() {

	//����/�X�g�b�v�ؑփ`�F�b�N
	changeRunStop();

	//�w��p�^�[���ɑΉ�����LED�����Z�b�g
	_panelManager->setLEDRow(LED_ROW_0, 0x00);
	_panelManager->setLEDRow(LED_ROW_1, 0x00);

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
void paternPlay::runClock() {

	//����/�X�g�b�v�t���O:����
	if (_run_stop == RUN_STOP::RUN) {
		execRunClock();

		//����/�X�g�b�v�t���O:�X�g�b�v
	}
	else	if (_run_stop == RUN_STOP::STOP) {
		execStopClock();
	}

	//16������MIDI�N���b�N�J�E���g���X�V
	_midiclock_16note++;

	//16������MIDI�N���b�N�J�E���g��7�J�E���g�ڂȂ�1�J�E���g�ɖ߂�
	if (_midiclock_16note > MIDICLOCK_STOP_16NOTE) {
		_midiclock_16note = MIDICLOCK_START_16NOTE;

		//16������MIDI�N���b�N�A�b�v�����猻�݃X�e�b�v���C���N�������g
		_step++;

		//���݃X�e�b�v���ŏI�J�E���g�𒴂�����J�n�X�e�b�v�ɖ߂�
		if (_step >= PATERN_STEP_LENGTH) {
			_step = PATTERN_START_IDX;
		}

	}
}


/*
����/�X�g�b�v�t���O:���� �V�[�N�G���X���������s
*/
void	paternPlay::execRunSequence() {
	_panelManager->setLED(_get_pattrn_LED(_pattern), true);
}

/*
����/�X�g�b�v�t���O:�X�g�b�v �V�[�N�G���X���������s
*/
void	paternPlay::execStopSequence() {

	//�p�^�[���I�����`�F�b�N
	if (_panelManager->getSwitch(static_cast<int>(Switch::C))) {
		_pattern = 0;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::D))) {
		_pattern = 1;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::E))) {
		_pattern = 2;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::F))) {
		_pattern = 3;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::G))) {
		_pattern = 4;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::A))) {
		_pattern = 5;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::B))) {
		_pattern = 6;

	}
	else if (_panelManager->getSwitch(static_cast<int>(Switch::C2))) {
		_pattern = 7;
	}

	//�p�^�[���I��LED��_��
	_panelManager->setLED(_get_pattrn_LED(_pattern), true);

}

/*
����/�X�g�b�v�t���O:���� MIDI�N���b�N�J�E���g���������s
*/
void	paternPlay::execRunClock() {
}

/*
����/�X�g�b�v�t���O:�X�g�b�v MIDI�N���b�N�J�E���g���������s
*/
void	paternPlay::execStopClock() {
}


/*
����/�X�g�b�v�ؑփ`�F�b�N
*/
void	paternPlay::changeRunStop() {

	//���݂̃���/�X�g�b�vSW���
	bool	nowRunSW = _panelManager->getSwitch(static_cast<int>(Switch::RUN));

	//�O����=OFF,���ݏ��=ON �Ȃ烂�[�h�ؑւ��s��
	if ((!_pushRunSW) && (nowRunSW)) {

		//����/�X�g�b�v�t���O:����
		if (_run_stop == RUN_STOP::STOP) {
			_run_stop = RUN_STOP::RUN;
			_panelManager->setLED(static_cast<int>(LED::RUN), true);
			_midiclock_16note = MIDICLOCK_START_16NOTE;

			//����/�X�g�b�v�t���O:�X�g�b�v
		}
		else	if (_run_stop == RUN_STOP::RUN) {
			_run_stop = RUN_STOP::STOP;
			_panelManager->setLED(static_cast<int>(LED::RUN), false);
			_midiclock_16note = MIDICLOCK_START_16NOTE;
		}
	}

	//�O���ԁ����ݏ��
	_pushRunSW = nowRunSW;

}


	  /*
	  16������MIDI�N���b�N�J�E���g���㔼�N���b�N�ɂȂ�����Q�[�g���I�t����
	  */
	  void paternPlay::_gate_off_16note() {

		  bool	_sSlide = _sequenceMap->paterns[_pattern].steps[_step].slide;

		  if ((_midiclock_16note == MIDICLOCK_GATEOFF_16NOTE) && (!_sSlide)) {
			  _voltage->gate(false);
		  }
	  }

	  /*
	  �w��p�^�[���ɉ�����class:LED�l���擾
	  */
	  int	paternPlay::_get_pattrn_LED(int pattern) {
		  int	result = static_cast<int>(LED::C);

		  switch (pattern) {
		  case 0:
			  result = static_cast<int>(LED::C);
			  break;
		  case 1:
			  result = static_cast<int>(LED::D);
			  break;
		  case 2:
			  result = static_cast<int>(LED::E);
			  break;
		  case 3:
			  result = static_cast<int>(LED::F);
			  break;
		  case 4:
			  result = static_cast<int>(LED::G);
			  break;
		  case 5:
			  result = static_cast<int>(LED::A);
			  break;
		  case 6:
			  result = static_cast<int>(LED::B);
			  break;
		  case 7:
			  result = static_cast<int>(LED::C2);
			  break;
		  default:
			  break;
		  }

		  return	result;
	  }