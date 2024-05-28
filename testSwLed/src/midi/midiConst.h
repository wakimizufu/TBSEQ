/*
MIDI�֘A�}�N�����`
*/

#ifndef midiConst_h
#define midiConst_h

//-----------------�yMIDI���b�Z�[�W:�X�e�[�^�X�o�C�g�z-----------------

//�m�[�g�I��
#define MIDI_STATUS_NOTEON	0x80

//�R���g���[���`�F���W
#define MIDI_STATUS_CONTROL	0xB0

//MIDI�N���b�N
#define MIDI_STATUS_CLOCK	0xF8

//�X�^�[�g
#define MIDI_STATUS_START	0xFA

//�R���e�B�j���[
#define MIDI_STATUS_CONTINUE	0xFB

//�X�g�b�v
#define MIDI_STATUS_STOP	0xFC

//-----------------�yMIDI���b�Z�[�W:�R���g���[���`�F���W�z-----------------

//�|���^�����gOn/Off(63�ȉ�:�I�t 64�ȏ�:�I��)
#define MIDI_CONTROL_PORTAMENT	0x41

//�X���C�h(�|���^�����g)�t���O��(63�ȉ�:�I�t 64�ȏ�:�I��)�ɂ���̂ō��V�t�g����
#define PORTAMENT_LEFT_SHIFT 6

//-----------------�ymidiSender/midiRecever�֘A�z-----------------

//�m�[�g���M�t���O
#define NOTE_ON true
#define NOTE_OFF false

//�A�N�Z���g�t���O
#define ACCENT_ON true
#define ACCENT_OFF false

//�X���C�h(�|���^�����g)�t���O
#define SLIDE_ON true
#define SLIDE_OFF false

//�x���V�e�B �m�[�g�I�����Œ�l
#define VEROCITY_ON	0x7F

//�x���V�e�B �m�[�g�I�t���Œ�l
#define VEROCITY_OFF	0x00

#endif
