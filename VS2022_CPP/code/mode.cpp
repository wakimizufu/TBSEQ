#include "mode.h"

/*
�R���X�g���N�^
modename      :���[�h��(Class MODE_NAME)
	ptPanelManager:panelManager�N���X�|�C���^
	ptVoltage     :voltage�N���X�|�C���^
	ptSequenceMap :sequenceMap�N���X�|�C���^
*/
mode::mode(MODE_NAME modename, panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap) {
	_modeName = modename;
	_panelManager = ptPanelManager;		//panelManager�N���X�|�C���^
	_voltage = ptVoltage;						//�y�R���X�g���N�^�Őݒ�zvoltage�N���X�|�C���^
	_sequenceMap = ptSequenceMap;		//�y�R���X�g���N�^�Őݒ�zsequenceMap�N���X�|�C���^

	_run_stop = RUN_STOP::STOP;
	_midiclock_16note = MIDICLOCK_START_16NOTE;
}

/*
���[�h�����擾����
�߂�l�F���[�h��(Class MODE_NAME)
*/
MODE_NAME	mode::getModeName() {
	return	_modeName;
}

/*
����/�X�g�b�v�t���O���擾����
�߂�l�F����/�X�g�b�v�t���O(Class RUN_STOP)
*/
RUN_STOP	mode::getRunStop() {
	return	_run_stop;
}