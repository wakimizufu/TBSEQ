/*
スイッチ入力/LED出力を定期間隔で更新する
*/

#ifndef sequenceMap_h
#define sequenceMap_h


/*
step ステップの各種情報を定義
*/


//step:1ステップの総バイト数
#define STEP_ALLBYTE 2

class step {
    public:
        int    note;   //ノート( NOTE_PWM_INDEX::NOTE_C2～NOTE_PWM_INDEX::NOTE_C3 を設定)
        bool    note_on; //ノートオンオフ true:ノートオン/false:ノートオフ

        //オクターブUPとDOWNはどちらもtrueにはならない
        bool    up;     //オクターブUP true:C3/false:C2
        bool    down;   //オクターブDOWN true:C1/false:C2

        bool    acc;    //アクセント true:ON/false:OFF
        bool    slide;  //スライド true:ON/false:OFF

   		/*
		コンストラクタ
		*/
        explicit step(){
            note=static_cast<int>(NOTE_PWM_INDEX::NOTE_C2);
            up=false;
            down=false;
            note_on=false;
            acc=false;
            slide=false;
		}
    
    private:
};

/*
patern 1パターン内の全ステップを管理する
*/

//step:ステップ開始インデックス
#define STEP_START_IDX 0

//step:1パターン内の全ステップ数
#define PATERN_STEP_LENGTH 16


class patern {
    public:
        step steps[PATERN_STEP_LENGTH];

        /*
        コンストラクタ
        */
        explicit patern()	{
        }
    
    private:

};


/*
sequenceMap パターンを管理する
*/

//sequenceMap:パターン開始インデックス
#define PATTERN_START_IDX 0

//sequenceMap:パターン全数
#define SEQUENCE_PATTERN_LENGTH 8

//sequenceMap:パターン全数
const int SEQUENCE_ALLBYTE = SEQUENCE_PATTERN_LENGTH * PATERN_STEP_LENGTH * STEP_ALLBYTE;

class sequenceMap {
    public:

    //パターン配列
    patern paterns[SEQUENCE_PATTERN_LENGTH];

    /*
    コンストラクタ
     */
    explicit sequenceMap()	{
	}

	/*
ビットストリームからパターン配列を設定する
引数:ビットストリーム
*/
	void setBitstream(char* _bitstream) {

		int _bitInd = 0;
		char _byte = 0;

		for (int p = 0; p < SEQUENCE_PATTERN_LENGTH; p++) {
			for (int s = 0; s < PATERN_STEP_LENGTH; s++) {

				_byte = *(_bitstream + _bitInd);
				paterns[p].steps[s].slide = (0x10 == (0x10 & _byte)); //bit:4 スライド true:ON/false:OFF
				paterns[p].steps[s].acc = (0x08 == (0x08 & _byte)); //bit:3 アクセント true:ON/false:OFF
				paterns[p].steps[s].down = (0x04 == (0x04 & _byte)); //bit:2 オクターブDOWN true:C1/false:C2
				paterns[p].steps[s].up = (0x02 == (0x02 & _byte)); //bit:1 オクターブUP true:ON/false:OFF
				paterns[p].steps[s].note_on = (0x01 == (0x01 & _byte)); //bit:0 ノートオンオフ true:ノートオン/false:ノートオフ
				_bitInd++;

				_byte = *(_bitstream + _bitInd);
				paterns[p].steps[s].note = static_cast<int>(_byte);	//ノート( NOTE_PWM_INDEX::NOTE_C2～NOTE_PWM_INDEX::NOTE_C3 を設定)
				_bitInd++;
			}
		}



	}

	/*
	パターン配列からビットストリームに設定する
	引数:ビットストリーム
	*/
	void getBitstream(char* _bitstream) {

		int _bitInd = 0;
		char _byte = 0;

		for (int p = 0; p < SEQUENCE_PATTERN_LENGTH; p++) {
			for (int s = 0; s < PATERN_STEP_LENGTH; s++) {

				_byte = 0x00;
				if (paterns[p].steps[s].slide) { _byte = _byte ^ 0x10; }	//bit:4 スライド true:ON/false:OFF
				if (paterns[p].steps[s].acc) { _byte = _byte ^ 0x08; }		//bit:3 アクセント true:ON/false:OFF
				if (paterns[p].steps[s].down) { _byte = _byte ^ 0x04; }		//bit:2 オクターブDOWN true:C1/false:C2
				if (paterns[p].steps[s].up) { _byte = _byte ^ 0x02; }		//bit:1 オクターブUP true:ON/false:OFF
				if (paterns[p].steps[s].note_on) { _byte = _byte ^ 0x01; }	//bit:0 ノートオンオフ true:ノートオン/false:ノートオフ
				*(_bitstream + _bitInd) = _byte;
				_bitInd++;

				_byte = static_cast<char>(paterns[p].steps[s].note);	//ノート( NOTE_PWM_INDEX::NOTE_C2～NOTE_PWM_INDEX::NOTE_C3 を設定)
				*(_bitstream + _bitInd) = _byte;
				_bitInd++;
			}
		}
	}

    private:
};


/*
 presetBitstream パターンを管理する
*/
class presetBitstream {
public:
	/*
	パターン情報 ビットストリーム
	☆総バイト数  :0x00FF=256Byte
		0x0000:sequenceMap開始
			0x0000:パターン1
			0x0020:パターン2
			0x0040:パターン3
			0x0060:パターン4
			0x0080:パターン5
			0x00A0:パターン6
			0x00C0:パターン7
			0x00E0:パターン8

		0x0000:パターン
			0x0000:ステップ1
			0x0002:ステップ2
			0x0004:ステップ3
			0x0006:ステップ4
			0x0008:ステップ5
			0x000A:ステップ6
			0x000C:ステップ7
			0x000E:ステップ8
			0x0010:ステップ9
			0x0012:ステップ10
			0x0014:ステップ11
			0x0016:ステップ12
			0x0018:ステップ13
			0x001A:ステップ14
			0x001C:ステップ15
			0x001E:ステップ16

		[ステップ]
		1Byte:
			bit7-5:(未使用:全て0)
			bit  4:スライド(オン:1,オフ:0)
			bit  3:アクセント(オン:1,オフ:0)
			bit  2:オクターブDown(オン:1,オフ:0)
			bit  1:オクターブUp(オン:1,オフ:0)
			bit  0:ノートオンオフ(オン:1,オフ:0)

		1Byte:
			ノート(8ビット  NOTE_PWM_INDEX::NOTE_C2～NOTE_PWM_INDEX::NOTE_C3 を設定)
	*/
	char patern_preset_bitstream[SEQUENCE_ALLBYTE]{
	0x01,19, 0x01,20, 0x01,21, 0x01,22, 0x01,23, 0x01,24, 0x01,25, 0x01,26, 0x01,27, 0x01,28, 0x01,29, 0x01,30, 0x01,29, 0x01,28, 0x01,27, 0x01,26,	//パターン1
	0x03,19, 0x03,20, 0x03,21, 0x03,22, 0x03,23, 0x03,24, 0x03,25, 0x03,26, 0x03,27, 0x03,28, 0x03,29, 0x03,30, 0x03,29, 0x03,28, 0x03,27, 0x03,26,	//パターン2
	0x05,19, 0x05,20, 0x05,21, 0x05,22, 0x05,23, 0x05,24, 0x05,25, 0x05,26, 0x05,27, 0x05,28, 0x05,29, 0x05,30, 0x05,29, 0x05,28, 0x05,27, 0x05,26,	//パターン3
	0x01,19, 0x00,20, 0x01,21, 0x01,22, 0x00,23, 0x01,24, 0x19,25, 0x01,26, 0x01,27, 0x00,28, 0x01,29, 0x11,30, 0x01,29, 0x00,28, 0x1D,27, 0x01,26,	//パターン4
	0x01,19, 0x01,20, 0x01,21, 0x01,22, 0x01,23, 0x01,24, 0x01,25, 0x01,26, 0x01,27, 0x01,28, 0x01,29, 0x01,30, 0x01,29, 0x01,28, 0x01,27, 0x01,26,	//パターン5
	0x01,19, 0x01,20, 0x01,21, 0x01,22, 0x01,23, 0x01,24, 0x01,25, 0x01,26, 0x01,27, 0x01,28, 0x01,29, 0x01,30, 0x01,29, 0x01,28, 0x01,27, 0x01,26,	//パターン6
	0x01,19, 0x01,20, 0x01,21, 0x01,22, 0x01,23, 0x01,24, 0x01,25, 0x01,26, 0x01,27, 0x01,28, 0x01,29, 0x01,30, 0x01,29, 0x01,28, 0x01,27, 0x01,26,	//パターン7
	0x01,19, 0x01,20, 0x01,21, 0x01,22, 0x01,23, 0x01,24, 0x01,25, 0x01,26, 0x01,27, 0x01,28, 0x01,29, 0x01,30, 0x01,29, 0x01,28, 0x01,27, 0x01,26	//パターン8
	};


	/*
	コンストラクタ
	*/
	explicit presetBitstream() {
	}



private:
};


#endif
