#include "countTriger.h"

/*
�R���X�g���N�^
thredhold :�J�E���^臒l
start     :�J�E���^�J�n�l(�f�t�H���g=0)
*/
countTriger::countTriger(unsigned int thredhold, unsigned int start = 0) {
	_thredhold = thredhold;
	_count = start;
}

/*
[���z�֐�]�f�X�g���N�^
*/
countTriger::~countTriger() {
}

/*
�J�E���^���C���N�������g����
�˃J�E���^���C���N�������g��������臒l�ɒB������J�E���^��0�ɐݒ肷��
�߂�l�Ftrue=>�J�E���^��臒l, false=>�J�E���^��臒l
*/
bool countTriger::countUp() {
	_count++;

	if (_thredhold == _count) {
		trigger();
		_count = 0;
		return	true;
	}

	return	false;
}