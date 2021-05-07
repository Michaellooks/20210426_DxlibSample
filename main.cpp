//ヘッダーファイルの読み込み
#include "DxLib.h"	//DxLibを使う時は必要

//マクロ定義
#define GAME_TITLE "ゲームタイトル"		//ゲームタイトル

#define GAME_WIDTH  1280				//ゲーム画面の幅(ウィドス)
#define GAME_HEIGHT 720					//ゲーム画面の高さ(ウィドス)
#define GAME_COLOR	32					//ゲームの色域

#define GAME_ICON_ID   333				//ゲームのアイコンのID

#define GAME_WINDOW_BAR 0				//ウィンドウバーの種類	

// プログラムは WinMain から始まります
//Windowsのプログラミング方法 = (WinAPI)で動いている！
//DxLibは、DirectXという、ゲームプログラミングをかんたんに使える仕組み

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	SetOutApplicationLogValidFlag(FALSE);				//tag.txtを出力しない

	ChangeWindowMode(TRUE);								//ウィンドウモードに設定

	SetMainWindowText(GAME_TITLE);						//ウィンドウのタイトル文字

	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定

	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定

	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色

	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読み込み

	SetWindowStyleMode(GAME_WINDOW_BAR);				//ウィンドウバーの状態

	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期を有効にする

	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする



	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	//DrawPixel(320, 240, GetColor(255, 255, 255));	// 点を打つ

	//WaitKey();				// キー入力待ち

	//四角の位置を決める
	int X = GAME_WIDTH / 2;					
	int Y = GAME_HEIGHT / 2;

	//四角の大きさを決める
	int width = 32;				//幅
	int height = 32;			//高さ

	//円の半径を決める
	int radius = 100;

	//速度
	int spead = 1;

	int Xspead = spead;
	int Yspead = spead;

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//無限ループ
	while (1)
	{
		
		//何かしらのキーが押されたとき
		if (CheckHitKeyAll() != 0)
		{
			break;	//無限ループを抜ける
		}

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0)	//‐1のとき、エラーやウィンドウが閉じられた
		{
			break;	//無限ループを抜ける
		}


		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }

		//四角を描画する
		//drawbox(
		//	x, y, x + width, y + height,
		//	getcolor(255, 0, 0),	//色を取得
		//	true					//塗りつぶし
		//);

		//円を描画
		DrawCircle(
			X, Y, radius,
			GetColor(0, 255, 0),
			FALSE, 5
		);

		X += Xspead;						//四角の位置を右にずらす
		Y += Yspead;

		if (Y - radius < 0 || Y + radius > GAME_HEIGHT)
		{
			//移動速度の符号を反転させる
			//+1なら-1にする  /	 -1ならば+1にする
			Yspead = -Yspead;	//移動する向きを反転

			//壁に当たったら加速
			if (Xspead > 0) { Xspead += 2; }
			else if (Xspead < 0) { Xspead -= 2; }

		}

		if (X - radius < 0 || X + radius > GAME_WIDTH)
		{
			//移動速度の符号を反転させる
			//+1なら-1にする  /	 -1ならば+1にする
			Xspead = -Xspead;	//移動する向きを反転

			//壁に当たったら加速
			if (Yspead > 0) { Yspead += 2; }
			else if (Yspead < 0) { Yspead -= 2; }
		}


		ScreenFlip();				//ダブルバッファリングした画面を描画

	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了 
}