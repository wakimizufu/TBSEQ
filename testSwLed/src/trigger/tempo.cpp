#include "tempo.h"

/*
�R���X�g���N�^
start     :�J�E���^�J�n�l(�f�t�H���g=0)
*/
tempo::tempo(unsigned int start = 0) :countTriger(THD_TEMPO, start) {
	_adc_value = 0;
}

/*
[���z�֐�]�J�E���^臒l�ɒB������MIDI�N���b�N���J�E���g�A�b�v���Z�b�g
*/
void tempo::trigger() {
	//_adc_value	=	analogRead(0);
	_adc_value = 100;
};

/*
�e���|�l���擾����
�߂�l:�e���|�l(30�`285)
*/
int	tempo::getTempo() {
	return	convertArray[_adc_value][TEMPO_COL_TEMPO];
}

/*
�e���|�ɑ΂���500ns�J�E���g臒l���擾����
�߂�l:500ns�J�E���g臒l
*/
int	tempo::getCountThd() {
	return	convertArray[_adc_value][TEMPO_COL_COUNT];
}