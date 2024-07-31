//-------------------------------------------------
//ファイル名:MIDIMessage.C
//機能:MIDIメッセージに関する処理を行う
//
//
//--------------------------------------------------


static int g_iRecvCh;					//有効MIDIチャンネル
static int g_iStatus;					//現在のステータス
static int g_iDataByteCount;	//現在のデータバイト数
static int g_iNoteNumber;			//直前に受信したノート番号
static int g_iControlNumber;	//直前に受信したコントロール番号

static int g_iAssignCnt;			//優先割り当てVoiceカウンタ


//ステータスバイト
#define	ST_NOTE_OFF		0x80	//ノートoff
#define	ST_NOTE_ON		0x90	//ノートon
#define ST_POLY_AFTER	0xA0	//ポリフォニック・キー・プレッシャー(アフタータッチ)
#define ST_CONTROL		0xB0	//コントロールチェンジ/チャンネルモードメッセージ
#define ST_PRGM_CHNG	0xC0	//プログラムチェンジ
#define ST_CH_AFTER		0xD0	//チャンネル・プレッシャー(アフタータッチ)
#define ST_PITCH_BD		0xE0	//ピッチベンター

#define ST_DISABLE		0x00	//無効ステータス


//assignVoice()引数
#define	ASSIGN_NOTE_OFF		0	//ノートoff
#define	ASSIGN_NOTE_ON		1	//ノートon



//ボイスステータス
//[注意]プログラム上ではボイス(配列の要素)の発音判定は以下の通りとする
//      ベロシティ0x00       ＝ GATE信号OFF
//      ベロシティ0x01～0xFF ＝ GATE信号ON 

#define	MIDI_ACC	PIN_A3			//ACCSENT信号	(0Vでon +5Vでoff)
#define	MIDI_GLID	PIN_A1			//GLIDE信号		(+5Vでon 0Vでoff)


//-------------------------------------------------
//関数名:setRecvChannel()
//機能  :MIDI受信チャンネルを設定する
//引数  :なし
//戻り値:なし
//--------------------------------------------------
void	setRecvChannel( )
	{
	//1ch固定
	g_iRecvCh	=	0x00;
	}


//-------------------------------------------------
//関数名:setGate()
//機能  :指定のゲートにON/OFFの設定を行なう
//引数  :ベロシティ(0ならゲートをOFF,1以上ならゲートON) , 対象ゲート
//戻り値:なし
//--------------------------------------------------
void	setGate ( int iVelocity , int iIndex )
	{
	int	iMode;

	iMode	=	TRUE;
	if ( ASSIGN_NOTE_OFF == iVelocity )
		{
		iMode	=	FALSE;
		}

	output_bit	(	PIN_A2	,	iMode	);
	}


//-------------------------------------------------
//関数名:setContlrChng()
//機能  :コントロール番号別に処理を行なう。
//引数  :コントロール番号 , コントロール値
//戻り値:なし
//--------------------------------------------------
void	setContlrChng ( int iCntNumber , int iValue )
	{
	int	i;

	switch ( iCntNumber )
		{
		case	11:	//エクスプレッション(=アクセントOn/Off)
			if ( 0x7F == iValue )
				{
				//127ならOn
				output_low	( MIDI_ACC );	
				}
			else
				{
				//127以外はOff
				output_high	( MIDI_ACC  );
				}
			break;

		case	65:	//ポルタメント(=グライドOn/Off)
			if ( 0x7F == iValue )
				{
				//127ならOn
				output_high	( MIDI_GLID );	
				}
			else
				{
				//127以外はOff
				output_low	( MIDI_GLID  );
				}
			break;

		case	123:	//オールノートOFF
			if ( 0x00 == iValue )		//0以外は無効
				{
				//GATEを全て閉じる
				output_low	( PIN_A2  );
				output_low	( MIDI_GLID  );
				output_high	( MIDI_ACC  );
				}
			break;


		default:
			break;
		}
	}


//-------------------------------------------------
//関数名:setNote()
//機能  :指定のゲートに音程の設定を行なう
//引数  :ノート番号 , 対象ゲート
//戻り値:なし
//--------------------------------------------------
void	setNote ( int iNote , int iIndex )
	{
	int	iAddres;

	//DACのデータを設定
	iAddres	= READ_EEPROM( 0x7F & iNote );
	output_b ( iAddres  & 0xF9 );
	output_bit(PIN_A3,( iAddres & 0x02 ));
	output_bit(PIN_A4,( iAddres & 0x04 ));
	}


//-------------------------------------------------
//関数名:setStatusByte()
//機能  :ステータスバイトを受信した際に、
//       受信チャンネルの照合及びデータバイト数の設定を行う
//引数  :なし
//戻り値:有効なステータス->ステータスバイト(チャンネル指定抜き)
//       無効なステータス->0x00
//--------------------------------------------------
int	setStatusByte ( )
	{
	int iStatus;

	//システムステータスは無効	
	if ( 0xF0 <= g_iRecvByte ) return	ST_DISABLE;

	//当該チャンネル以外なら無効
	if ( g_iRecvCh != ( g_iRecvByte & 0x0F ) )	return ST_DISABLE;

	//有効ステータスを抽出
	iStatus	=	g_iRecvByte & 0xF0;
	switch ( iStatus )
		{
		case ST_NOTE_OFF:		//ノートoff
		case ST_NOTE_ON:		//ノートon
		case ST_CONTROL:		//コントロールチェンジ

			//以上のステータスは有効
			g_iDataByteCount	=	1;	//データバイト数を1にセットする
			return	iStatus;
			break;
		
		default:
			return	ST_DISABLE;	//その他のステータスも無効
			break;
		}
	}

//-------------------------------------------------
//関数名:execNoteOff()
//機能  :ノートOFFに関する処理を行う
//引数  :なし
//戻り値:なし
//--------------------------------------------------
void execNoteOff()
	{
	//データバイトのカウント毎の処理を行う。
	switch ( g_iDataByteCount )
		{
		case	1:	//ノート番号

			//受信したノート番号を記録する
			g_iNoteNumber	=	g_iRecvByte;
			g_iDataByteCount++;
			break;

		case	2:	//offベロシティ
			//対象とするノート番号のVoiceをoffする
			output_bit	(	PIN_A2	,	FALSE	);
			g_iDataByteCount	=	1;
			break;
			
			
		default:	//異常時
			g_iStatus	=	ST_DISABLE;
			g_iDataByteCount	=	1;
			break;

		}
	}


//-------------------------------------------------
//関数名:execNoteOn()
//機能  :ノートOnに関する処理を行う
//引数  :なし
//戻り値:なし
//--------------------------------------------------
void execNoteOn()
	{
	int	iAssignIndex;

	//データバイトのカウント毎の処理を行う。
	switch ( g_iDataByteCount )
		{
		case	1:	//ノート番号

			//受信したノート番号を記録する
			g_iNoteNumber	=	g_iRecvByte;
			g_iDataByteCount++;
			break;

		case	2:	//ベロシティを調べる
			g_iDataByteCount	=	1;

			//新規に発音したい時、発音管理配列のインデックスを取得する
			iAssignIndex	=	0;
			//音程を設定する
			setNote ( g_iNoteNumber , iAssignIndex );

			//ゲートをonする
			setGate ( g_iRecvByte , iAssignIndex );
			break;

		default:	//異常時
			g_iStatus	=	ST_DISABLE;
			g_iDataByteCount	=	1;
			break;
		}
	}


//-------------------------------------------------
//関数名:execContlrChng()
//機能  :プログラムチェンジ/チャンネルモードメッセージに関する処理を行う
//引数  :なし
//戻り値:なし
//--------------------------------------------------
void execContlrChng()
	{
	//データバイトのカウント毎の処理を行う。
	switch ( g_iDataByteCount )
		{
		case	1:	//ノート番号

			//受信したコントロール番号を記録する
			g_iControlNumber	=	g_iRecvByte;
			g_iDataByteCount++;
			break;

		case	2:	//offベロシティ

			//コントロール番号に沿った処理を行なう
			setContlrChng	(	g_iControlNumber	,	g_iRecvByte	);
			g_iDataByteCount	=	1;
			break;

		default:	//異常時
			g_iStatus	=	ST_DISABLE;
			g_iDataByteCount	=	1;
			break;
		}
	}


//-------------------------------------------------
//関数名:initMidiMessage()
//機能  :MIDI各種データの初期化を行う
//引数  :なし
//戻り値:なし
//--------------------------------------------------
void	initMidiMessage()
	{
	int	i;

	//ステータスバイトを無効ステータスに初期化
	g_iStatus	=	0x00;

	//ノート番号を初期化
	g_iNoteNumber	=	0x00;

	//データバイト数を1にセット
	g_iDataByteCount	=	1;

	//優先割り当てVoiceカウンタ
	g_iAssignCnt	=	0;


	//GATE,ACSSENT,GLIDEをすべてoffにする
	setGate( ASSIGN_NOTE_OFF , 0x00 );	//GATE -> PIN_A1

	output_high	( MIDI_ACC  );	//ACSSENT
	output_low	( MIDI_GLID );	//GLIDE
	}


//-------------------------------------------------
//関数名:execMIDIRecv()
//機能  :MIDIメッセージに関する処理を行う
//引数  :なし
//戻り値:なし
//--------------------------------------------------
void execMIDIRecv()
	{
	//ステータスバイトを受信したら、有効なステータスか調べる
	if ( 0x80 <= g_iRecvByte )	
		g_iStatus	=	setStatusByte();

	if ( 0x7F >= g_iRecvByte )	
		{ 
		//有効なステータスなら各種データバイトの対応を行う
		//ノートoff
		if	( ST_NOTE_OFF == g_iStatus )	
			execNoteOff();

		//ノートon
		if	( ST_NOTE_ON == g_iStatus )
			execNoteOn();

		//コントロールチェンジ/チャンネルモードメッセージ
		if	( ST_CONTROL == g_iStatus )
			execContlrChng();
		}
	}
