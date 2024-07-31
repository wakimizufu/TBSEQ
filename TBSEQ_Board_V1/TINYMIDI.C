//-------------------------------------------------
//ファイル名:MidiModl.C
//機能:Midiモニタと出力インターフェイスの制御を行う。
//
//
//--------------------------------------------------
#include <16F628.h>

#use delay(clock = 20000000)
#fuses HS, NOWDT, NOMCLR, NOPUT, NOPROTECT, NOBROWNOUT, NOLVP,NOCPD


// I/O port使う為の宣言?
#use fast_io(A)
#use fast_io(B)

//USARTの宣言
#use rs232(baud = 31250, xmit = PIN_B2, rcv = PIN_B1, PARITY = N)

//各ポートの入出力ピンの設定                     7 6 5 4 3 2 1 0
#define PIN_MAP_A 0x00    //port A initial mode  - - O O O O O O 
#define PIN_MAP_B 0x02    //port B initial mode  O O O O O O I O 

#byte CMCON =0x1F	//レジストファイル CMCONへのポインタ
#byte OPTION_REG =0x81	//レジストファイル OPTION_REGへのポインタ
#byte TXSTA =0x98	//レジストファイル TXSTAへのポインタ


//MIDI INから来たメッセージを保存するFIFOバッファの設定
#define IN_BUFFER_SIZE 48

static int g_aInBuffer[IN_BUFFER_SIZE];	// FIFOバッファ配列
static int g_iInBufWrite;		// 書き込み位置(配列のindex)
static int g_iInBufRead;		// 読み込み位置(配列のindex)
static int g_iRecvByte;			// バッファより取得した内容

//MIDIメッセージ関連
#include <MIDIMsg.C>


//-------------------------------------------------
//関数名:recvMIDIIN()
//機能  :SPI受信割り込みを受信＝MIDI INからメッセージを受信 
//       の際の処理を行う
//引数  :なし
//戻り値:なし
//--------------------------------------------------
#INT_RDA
void recvMIDIIN()
	{
        int iCurrentRecv;

	//1バイト取得する
	iCurrentRecv = getch();

	//システムメッセージ/リアルタイムメッセージは無視する
	if ( iCurrentRecv >=	0xF0 )
		{
		return;
		}
	//書き込み位置を指定する。
	g_iInBufWrite	=	( g_iInBufWrite + 1 ) % IN_BUFFER_SIZE;

	//バッファのサイズをオーバーしたら受信したのを切り捨てる
	if	( g_iInBufRead == g_iInBufWrite )	
		{
		g_iInBufWrite--;
		}
	else		//バッファに空きが有れば受信データを保存する
		{	
		g_aInBuffer[g_iInBufWrite]	=	iCurrentRecv;
		}
	}

//-------------------------------------------------
//関数名:getInBufChar()
//機能  :MIDI IN FIFOバッファより1Byte取得する
//引数  :なし
//戻り値:TRUE->データ取得成功 , FALSE->バッファにデータ無し
//--------------------------------------------------
#separate
int	getInBufChar( )
	{
	//受信データが有れば1Byte取得する
	if	(	g_iInBufRead	!=	g_iInBufWrite	)
		{
		//読み込み位置を指定する。
		g_iInBufRead	=	( g_iInBufRead + 1 ) % IN_BUFFER_SIZE;

		g_iRecvByte	=	g_aInBuffer[g_iInBufRead];
		return	TRUE;
		}
	else		//受信データが無ければFALSEを返す
		{
		return	FALSE;
		}
	}




//-------------------------------------------------
//関数名:initIO()
//機能  :I/O関連の初期化処理を行う。
//引数  :なし
//戻り値:なし
//--------------------------------------------------
#separate
void	initIO()
	{
	//各ポートの入出力ピンを設定する
	set_tris_a ( PIN_MAP_A );
	set_tris_b ( PIN_MAP_B );

	//CMCONは全てデジタルポートで動作させる
	CMCON =0x07;

	//RA4をデジタルポートにする
	OPTION_REG	=	0xDF;

	//FIFOバッファのポインタをリセット 配列の初期化は省略
	g_iInBufWrite	=	0;
	g_iInBufRead	=	0;

	//ステータスバイトを初期化
	g_iStatus	=	0x00;


	}

//////////////////////////////////////////////
// Main Routin
//
main()
	{
	//I/O関連の初期化処理を行う
	initIO();

	//MIDIメッセージ関連の初期化を行う
	initMidiMessage();

	//MIDI受信チャンネルを設定する
	setRecvChannel();

	//割り込みを許可する
	enable_interrupts ( INT_RDA );
	enable_interrupts ( GLOBAL );

	//メインルーチン
	while(1)
		{
		//MIDI受信後、バッファに溜まっているデータを逐次処理する
		if ( getInBufChar ( ) )
			{
 			execMIDIRecv();
			}
		}


	}
