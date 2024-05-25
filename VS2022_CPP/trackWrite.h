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
    */
    explicit trackWrite(	panelManager * ptPanelManager , voltage * ptVoltage	):mode(MODE_NAME::TRACK_WRITE	,	ptPanelManager	,	ptVoltage	){
    }

		/*
		[仮想関数]カウント閾値達成時に実行されるアプリケーションを実施する
		*/
    virtual void runSequence(){
		}

		/*
		[仮想関数]MIDIクロックカウント閾値達成時に実行されるアプリケーションを実施する
		*/
    virtual void runClock(){
		}
		
    private:
};




#endif


