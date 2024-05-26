/*
モードクラス：トラックプレイ用クラスを定義
*/

#ifndef trackPlay_h
#define trackPlay_h

#include "mode.h"

class trackPlay: public mode{

    public:
     /*
    コンストラクタ
		ptPanelManager:panelManagerクラスポインタ
		ptVoltage     :voltageクラスポインタ
    */
	trackPlay(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap);

	/*
	[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
	*/
	virtual void runSequence();

	/*
	[仮想関数]MIDIクロックカウント閾値達成時に実行されるアプリケーションを実施する
	*/
	virtual void runClock();

    private:
};




#endif


