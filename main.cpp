
//ヘッダファイルの読み込み
#include "game.h"

#include "keyboard.h"		//キーボードの処理

#include "FPS.h"			//FPSの処理

//構造体の定義

//キャラクタの構造体
struct CHARACTOR
{
	int handle = -1;	//画像のハンドル(管理番号)
	char path[255];		//画像の場所(パス)
	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ

	int speed = 1;		//移動速度

	RECT coll;			//当たり判定の領域(四角)

	BOOL IsDraw = FALSE;//画像が描画できる
};

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//	現在のゲームのシーン
GAME_SCENE OldGameScene;	//	前回のゲームのシーン
GAME_SCENE NextGameScene;	//	次のゲームのシーン

//プレイヤー
CHARACTOR player;

//画面の切り替え
BOOL IsFadeOut = FALSE;		//	フェードアウト
BOOL IsFadeIn = FALSE;		//	フェードイン

int fadeTimeMill = 2000;						//	切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;		//	ミリ秒をフレーム秒に変換

//フェードアウト
int fadeOutCntInit = 0;				//初期値
int fadeOutCnt = fadeOutCntInit;	//フェードアウトのカウンタ
int fadeOutCntMax = fadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int fadeInCntInit = fadeTimeMax;	//初期値
int fadeInCnt = fadeInCntInit;		//フェードアウトのカウンタ
int fadeInCntMax = fadeTimeMax;		//フェードアウトのカウンタMAX

//プロトタイプ宣言
VOID Title(VOID);				//	タイトル画面
VOID TitleProc(VOID);			//	タイトル画面（処理）
VOID TitleDraw(VOID);			//	タイトル画面（描画）

VOID Play(VOID);				//	プレイ画面
VOID PlayProc(VOID);			//	プレイ画面（処理）
VOID PlayDraw(VOID);			//	プレイ画面（描画）

VOID End(VOID);					//	エンド画面
VOID EndProc(VOID);				//	エンド画面（処理）
VOID EndDraw(VOID);				//	エンド画面（描画）

VOID Change(VOID);				//	切り替え画面
VOID ChangeProc(VOID);			//	切り替え画面（処理）
VOID ChangeDraw(VOID);			//	切り替え画面（描画）

VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

VOID CollUpdate(CHARACTOR* chara);	//当たり判定の領域を更新


// プログラムは WinMain から始まります
//Windowsのプログラミング方法 = (WinAPI)で動いている！
//DxLibは、DirectXという、ゲームプログラミングをかんたんに使える仕組み

int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{

	SetOutApplicationLogValidFlag(FALSE);				//	tag.txtを出力しない

	ChangeWindowMode(TRUE);								//	ウィンドウモードに設定

	SetMainWindowText(GAME_TITLE);						//	ウィンドウのタイトル文字

	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//	ウィンドウの解像度を設定

	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//	ウィンドウの大きさを設定

	SetBackgroundColor(255, 255, 255);					//	デフォルトの背景の色

	SetWindowIconID(GAME_ICON_ID);						//	アイコンファイルを読み込み

	SetWindowStyleMode(GAME_WINDOW_BAR);				//	ウィンドウバーの状態

	SetWaitVSyncFlag(TRUE);								//	ディスプレイの垂直同期を有効にする ※重要

	SetAlwaysRunFlag(TRUE);								//	ウィンドウをずっとアクティブにする

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	//DrawPixel(320, 240, GetColor(255, 255, 255));	// 点を打つ

	//WaitKey();				// キー入力待ち

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化

	//プレイヤーの画像の読み込み
	strcpyDx(player.path, ".\\image\\player.png");	//パスのコピー
	player.handle = LoadGraph(player.path);			//画像の読み込み

	//画像が読み込め勝った時は、エラー(ー１)が入る
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインウィンドウハンドル
			player.path,			//メッセージ本文
			"画像読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);

		DxLib_End();	//強制終了
		return -1;		//エラー終了
	}

	//画像の幅と高さを取得
	GetGraphSize(player.handle, &player.width, &player.height);

	//当たり判定を更新する
	CollUpdate(&player);	//プレイヤーの当たり判定のアドレス

	//プレイヤーを初期化
	player.x = GAME_WIDTH / 2 - player.width / 2;		//中央寄せ
	player.y = GAME_HEIGHT / 2 - player.height / 2;		//中央寄せ
	player.speed = 5;
	player.IsDraw = TRUE;
	

	//無限ループ
	while (1)
	{

		if (ProcessMessage() != 0)	{ break; }	//メッセージを受け取り続ける

		if (ClearDrawScreen() != 0) { break; }  //画面を消去する

		//キーボード入力の更新
		AllKeyUpdate();

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();				//プレイ画面
			break;
		case GAME_SCENE_END:
			End();				//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();			//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		

		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWait();
		
		ScreenFlip();			//ダブルバッファリングした画面を描画
	}

	//終わるときの処理
	DeleteGraph(player.handle);	//画像をメモリ上から削除

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					//	ソフトの終了 
}


/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="scerne">シーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;		//シーンを切り替え
	IsFadeIn = FALSE;		//フェードインしない
	IsFadeOut = TRUE;		//フェードアウトしない

	return;
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();		//処理
	TitleDraw();		//描画
	
	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
VOID TitleProc(VOID)
{
	//プレイシーンへ切り替える
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);
	}


	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "タイトル画面を表示", GetColor(0, 0, 0));
	
	return;
}

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();		//処理
	PlayDraw();		//描画

	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
VOID PlayProc(VOID)
{
	//エンドシーンへ切り替える
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}

	//プレイヤーの操作
	if (KeyDown(KEY_INPUT_UP))
	{
		player.y -= player.speed;
	}

	if (KeyDown(KEY_INPUT_DOWN))
	{
		player.y += player.speed;
	}

	if (KeyDown(KEY_INPUT_LEFT))
	{
		player.x -= player.speed;
	}

	if (KeyDown(KEY_INPUT_RIGHT))
	{
		player.x += player.speed;
	}

	//当たり判定を更新する
	CollUpdate(&player);

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{

	if (player.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(player.x, player.y, player.handle, TRUE);
	}

	DrawString(0, 0, "プレイ画面を表示", GetColor(0, 0, 0));

	if (GAME_DEBUG == TRUE)
	{
		//四角を描画
		DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
			GetColor(255, 0, 0), FALSE);
	}

	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();		//処理
	EndDraw();		//描画

	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{
	//タイトルシーンへ切り替える
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面を表示", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();		//処理
	ChangeDraw();		//描画

	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//カウンタを減らす
		}
		else
		{
			//フェードイン処理が終わった

			fadeInCnt = fadeInCntInit;	//カウンタ初期化
			IsFadeIn = FALSE;			//フェードイン処理終了
		}
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//カウンタを増やす
		}
		else
		{
			//フェードアウト処理が終わった

			fadeOutCnt = fadeOutCntInit;	//カウンタ初期化
			IsFadeOut = FALSE;				//フェードアウト処理終了
		}
	}

	//切り替え処理終了
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインしていない、フェードアウトもしていないとき
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のゲームシーン更新
	}

	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (IsFadeOut)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 255), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(255, 0, 0));
	
	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="Coll"></param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x + 25;						//当たり判定を微調整
	chara->coll.top = chara->y + 80;						//当たり判定を微調整
	chara->coll.right = chara->x + chara->width - 30;		//当たり判定を微調整
	chara->coll.bottom = chara->y + chara->height - 50;		//当たり判定を微調整

	return;
}