#include "midiSender.h"

midiSender::midiSender() {
	_NoteOn = NOTE_OFF;		//�m�[�g���M�t���O
	_note = 0x40;				//���݃m�[�g�l
	_accent = ACCENT_OFF;	//���݃A�N�Z���g�t���O
	_portament = SLIDE_OFF;	//���݃|���^�����g�t���O
}

/*
�V�X�e�����A���^�C�����b�Z�[�W�FMIDI�N���b�N�𑗐M
*/
void	midiSender::clock() {
	_sendMessage(MIDI_STATUS_CLOCK, EMPTY_DATA, EMPTY_DATA);
}


/*
�V�X�e�����A���^�C�����b�Z�[�W�F�X�^�[�g�𑗐M
*/
void	midiSender::srt_start() {
	_sendMessage(MIDI_STATUS_START, EMPTY_DATA, EMPTY_DATA);
}

/*
�V�X�e�����A���^�C�����b�Z�[�W�F�R���e�B�j���[�𑗐M
*/
void	midiSender::srt_continue() {
	_sendMessage(MIDI_STATUS_CONTINUE, EMPTY_DATA, EMPTY_DATA);
}

/*
�V�X�e�����A���^�C�����b�Z�[�W�F�X�g�b�v�𑗐M
*/
void	midiSender::srt_stop() {
	_sendMessage(MIDI_STATUS_STOP, EMPTY_DATA, EMPTY_DATA);
}


/*
�`�����l���{�C�X���b�Z�[�W�F�m�[�g�I���𑗐M
*/
void	midiSender::cb_noteOn(int	senderChannel, int note, bool	accent, bool	slide) {
	_NoteOn = NOTE_ON;			//�m�[�g���M�t���O

	//�m�[�g�I��
	_sendMessage(MIDI_STATUS_NOTEON || _senderChannel, note, VEROCITY_ON);
	_note = note;					//���݃m�[�g�l

	//�|���^�����g�t���O
	if (_portament != slide) {
		_sendMessage(MIDI_STATUS_CONTROL || _senderChannel, slide << PORTAMENT_LEFT_SHIFT, EMPTY_DATA);
		_portament = slide;					//���݃|���^�����g�t���O
	}

	_accent = accent;				//���݃A�N�Z���g�t���O
	_senderChannel = senderChannel;	//���ݑ��M�`���l��

}

/*
�`�����l���{�C�X���b�Z�[�W�F�m�[�g�I�t�𑗐M
*/
void	midiSender::cb_noteOff() {
	_NoteOn = NOTE_OFF;			//�m�[�g���M�t���O

	//�m�[�g�I�t(���m�ɂ̓x���V�e�B��0�Ńm�[�g�I������)
	_sendMessage(MIDI_STATUS_NOTEON || _senderChannel, _note, VEROCITY_OFF);
}

  /*
  MIDI���b�Z�[�W���M
  */
	  void	midiSender::_sendMessage(int	status, int	dataOne, int	dateTwo) {

	  }