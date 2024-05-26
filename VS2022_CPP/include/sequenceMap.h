/*
スイッチ入力/LED出力を定期間隔で更新する
*/

#ifndef sequenceMap_h
#define sequenceMap_h


/*
step ステップの各種情報を定義
*/
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

class sequenceMap {
    public:

    //パターン配列
    patern paterns[SEQUENCE_PATTERN_LENGTH];

    /*
    コンストラクタ
     */
    explicit sequenceMap()	{
	}

    private:
};


#endif
