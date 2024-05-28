#include "midiClock.h"

/*
�R���X�g���N�^
thredhold :�J�E���^臒l
start     :�J�E���^�J�n�l(�f�t�H���g=0)
*/
midiClock::midiClock(unsigned int thredhold, unsigned int start = 0) :countTriger(thredhold, start) {
	_count_up = false;
}

/*
[���z�֐�]�J�E���^臒l�ɒB������MIDI�N���b�N���J�E���g�A�b�v���Z�b�g
*/
void  midiClock::trigger() {
	_count_up = true;
};

/*
�J�E���g�A�b�v�t���O���A�b�v�҂��ɕύX&�J�E���^��0�N���A
*/
void	midiClock::clear() {
	_count_up = false;
	_count = 0;
}


/*
�J�E���g�A�b�v�t���O���擾����
�߂�l:�J�E���g�A�b�v�t���O true->�J�E���g�A�b�v��, false->�A�b�v�҂�
*/
bool midiClock::getCountUp() {
	return	_count_up;
}


/*
�e���|�ɉ�����MIDI�N���b�N:�J�E���^臒l��ύX����
value:�ύX����MIDI�N���b�N:�J�E���^臒l
*/
void	midiClock::setTempo2Threshold(int	value) {
	_thredhold = value;

	//�J�E���g�l��臒l�𒴂�����J�E���g�A�b�v�Ƃ���
	if (_count >= _thredhold) {
		_count_up = true;
		_count = 0;
	}
}
