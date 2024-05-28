#include "modeManager.h"

/*
�R���X�g���N�^
ptPanelManager:panelManager�N���X�|�C���^
ptVoltage     :voltage�N���X�|�C���^
noteThredhold :MIDI�N���b�N�J�E���^臒l
start         :�J�E���^�J�n�l(�f�t�H���g=0)
*/
modeManager::modeManager(panelManager* ptPanelManager, voltage* ptVoltage, unsigned int noteThredhold, unsigned int start = 0) :countTriger(THD_PANEL_MANAGER, start) {

	_panelManager = ptPanelManager;		//panelManager�N���X�|�C���^
	_voltage = ptVoltage;						//�y�R���X�g���N�^�Őݒ�zvoltage�N���X�|�C���^

	//���݂̃��[�h�N���X�̏����l��ݒ肷��
	_currentMode = new paternWrite( _panelManager, _voltage, &_sequenceMap);

	_clockCount = 0;							//���݂�MIDI�N���b�N�J�E���^�l
	_noteThredhold = noteThredhold;	//MIDI�N���b�N�J�E���^臒l
}

/*
[���z�֐�]�J�E���^臒l�ɒB������MIDI�N���b�N���J�E���g�A�b�v���Z�b�g
*/
void modeManager::trigger() {
	bool _track = _panelManager->getSwitch(static_cast<int>(Switch::TRACK));
	bool _patern = _panelManager->getSwitch(static_cast<int>(Switch::PATTERN));
	bool _write = _panelManager->getSwitch(static_cast<int>(Switch::WRITE));

	//���[�h�ؑ֔�����s��
	_changeMode(_track, _patern, _write);

	//���[�h�ɑ΂��ăJ�E���g臒l�B�����Ɏ��s�����A�v���P�[�V���������{����
	_currentMode->runSequence();
}

/*
MIDI�N���b�N�J�E���^���C���N�������g����
��MIDI�N���b�N�J�E���^���C���N�������g��������臒l�ɒB������MIDI�N���b�N�J�E���^��0�ɐݒ肷��
�߂�l�Ftrue=>MIDI�N���b�N�J�E���^��臒l, false=>MIDI�N���b�N�J�E���^��臒l
*/
bool modeManager::clockCountUp() {
	_clockCount++;

	if (_noteThredhold == _clockCount) {
		_currentMode->runClock();
		_clockCount = 0;
		return	true;
	}

	return	false;
}


/*
MIDI�N���b�N�J�E���^臒l��ݒ肷��
noteThredhold :MIDI�N���b�N�J�E���^臒l
�߂�l�F�Ȃ�
*/
void	modeManager::setNoteThredhold(unsigned int noteThredhold) {
	_noteThredhold = noteThredhold;

	if (_noteThredhold <= _clockCount) {
		_currentMode->runClock();
		_clockCount = 0;
	}
}

/*
�������ꂽ�{�^���󋵂𔽉f����(true->������,false->������)
�߂�l:mode���[�h��
*/
MODE_NAME modeManager::getModeName() {
	return	_currentMode->getModeName();
}


	  /*
	  ���[�h�ؑ֔�����s��
	  bool	_track;		���f���F�g���b�N�{�^�����
	  bool	_patern;	���f���F�p�^�[���{�^�����
	  bool	_write;		���f���F���C�g�{�^�����
	  */
	  void	modeManager::_changeMode(bool _track, bool _patern, bool _write) {

		  //����/�X�g�b�v��Ԃ������Ȃ�ؑ֏����͍s��Ȃ�
		  if (RUN_STOP::STOP == _currentMode->getRunStop()) {
			  return;
		  }

		  //���݂̃��[�h�����擾����
		  MODE_NAME	mode = _currentMode->getModeName();

		  //�{�^�������󋵂ɉ��������[�h����ݒ�
		  MODE_NAME changeMode = MODE_NAME::NONE;

		  if (_patern && !_write) {
			  changeMode = MODE_NAME::PATERN_PLAY;

		  }
		  else if (_patern && _write) {
			  changeMode = MODE_NAME::PATERN_WRITE;

		  }
		  else if (_track && _write) {
			  changeMode = MODE_NAME::TRACK_PLAY;

		  }
		  else if (_track && _write) {
			  changeMode = MODE_NAME::TRACK_WRITE;
		  }

		  //���[�h���I���Ȃ�ؑւȂ�
		  if (MODE_NAME::NONE == changeMode) {
			  return;

			  //���݃��[�h�Ɛؑ֎����[�h���ꏏ�Ȃ�ؑւȂ�
		  }
		  else if (mode == changeMode) {
			  return;
		  }


		  //�قȂ郂�[�h���I�����ꂽ�烂�[�h��؂�ւ���
		  delete	_currentMode;

		  if (MODE_NAME::PATERN_PLAY == changeMode) {
			  _currentMode = new paternPlay( _panelManager, _voltage, &_sequenceMap);

		  }
		  else if (MODE_NAME::PATERN_WRITE == changeMode) {
			  _currentMode = new paternWrite( _panelManager, _voltage, &_sequenceMap);

		  }
		  else if (MODE_NAME::TRACK_PLAY == changeMode) {
			  //_currentMode = new trackPlay(	_currentMode	,	_panelManager	,	_voltage	,	&_sequenceMap	);

		  }
		  else if (MODE_NAME::TRACK_WRITE == changeMode) {
			  //_currentMode = new trackWrite(	_currentMode	,	_panelManager	,	_voltage	,	&_sequenceMap	);

		  }

	  }
