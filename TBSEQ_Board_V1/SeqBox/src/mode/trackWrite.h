/*
モードクラス：トラックライト用クラスを定義
*/

#ifndef trackWrite_h
#define trackWrite_h

#include "mode.h"

class trackWrite: public mode{

    public:
		/*
		コンストラクタ
		ptPanelManager:panelManagerクラスポインタ
		ptVoltage     :voltageクラスポインタ
		ptSequenceMap :sequenceMapクラスポインタ
		ptTrackMap    :trackMapクラスポインタ
		*/
		trackWrite(panelManager* ptPanelManager, voltage* ptVoltage, sequenceMap* ptSequenceMap, trackMap* ptTrackMap);

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


