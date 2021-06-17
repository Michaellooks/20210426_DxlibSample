
//ヘッダファイルの読み込み
#include "game.h"

#include "keyboard.h"		//キーボードの処理

#include "FPS.h"			//FPSの処理

//構造体の定義


//画像の構造体
struct IMAGE
{
	int handle = -1;	//画像のハンドル（管理番号）
	char path[255];		//画像の場所（パス）

	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ

	BOOL IsDraw = FALSE;//画像が描画できる？
};

//キャラクタの構造体
struct CHARACTOR
{
	IMAGE img;			//画像構造体

	int speed = 1;		//移動速度

	RECT coll;			//当たり判定の領域(四角)

	
};

//動画の構造体
struct MOVIE
{
	int handle = -1;	//動画のハンドル
	char path[255];		//動画のパス

	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ

	int Volume = 255;	//ボリューム(最小)0〜255(最大)
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;	//音楽のハンドル
	char path[255];		//音楽のパス

	int Volume = -1;	//ボリューム（MIN　０〜２５５　MAX）
	int playType = -1;	//BGM or SE
};

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//	現在のゲームのシーン
GAME_SCENE OldGameScene;	//	前回のゲームのシーン
GAME_SCENE NextGameScene;	//	次のゲームのシーン

//プレイ背景の動画
MOVIE playMovie;

//プレイヤー
CHARACTOR player;

//ゴール
CHARACTOR goal;

//敵
CHARACTOR enemy;

//画像を読み込む
IMAGE TitleLogo;			//タイトルロゴ
IMAGE TitleEnter;			//エンターキーを押す
IMAGE EndGameOver;			//ゲームオーバーロゴ
IMAGE EndClear;				//クリアロゴ
IMAGE EndClearUnderline;	//クリアロゴの下線

//音楽
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;
AUDIO EndOverBGM;

//効果音
AUDIO PlayerSE;

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

//PushEnterの点滅
int PushEnterCnt = 0;				//カウンタ
//int PushEnterCntMAX = 60;			//カウンタMAX値
const int PushEnterCntMAX = 100;		//カウンタMAX値
BOOL PushEnterBrink = FALSE;		//点滅しているか

//プロトタイプ宣言
VOID Title(VOID);							//	タイトル画面
VOID TitleProc(VOID);						//	タイトル画面（処理）
VOID TitleDraw(VOID);						//	タイトル画面（描画）

VOID Play(VOID);							//	プレイ画面
VOID PlayProc(VOID);						//	プレイ画面（処理）
VOID PlayDraw(VOID);						//	プレイ画面（描画）

VOID End(VOID);								//	エンド画面
VOID EndProc(VOID);							//	エンド画面（処理）
VOID EndDraw(VOID);							//	エンド画面（描画）

VOID EndOver(VOID);							//ゲームオーバー画面
VOID EndOverProc(VOID);						//ゲームオーバー画面（処理）
VOID EndOverDraw(VOID);						//ゲームオーバー画面（描画）

VOID Change(VOID);							//	切り替え画面
VOID ChangeProc(VOID);						//	切り替え画面（処理）
VOID ChangeDraw(VOID);						//	切り替え画面（描画）

VOID ChangeScene(GAME_SCENE scene);			//シーン切り替え

VOID CollUpdatePlayer(CHARACTOR* chara);	//当たり判定の領域を更新

VOID CollUpdate(CHARACTOR* chara);			//当たり判定

BOOL OnCollRect(RECT A, RECT B);			//矩形と矩形の当たり判定

BOOL GameLoad(VOID);						//ゲームのデータの読み込み

VOID GameInit(VOID);						//ゲームデータの初期化

BOOL LoadImageMem(IMAGE* image, const char* path);								//画像の読み込み

BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//ゲームの音楽を読み込み



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

	//ゲーム読み込み
	if (!GameLoad())
	{

		//データの読み込みに失敗したとき

		DxLib_End();	//DxLib終了
		return -1;		//異常終了

	}

	//ゲームの初期化
	GameInit();

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
		case GAME_SCENE_ENDOVER:
			EndOver();			//ゲームオーバー画面
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
	DeleteGraph(playMovie.handle);	//動画をメモリ上から削除
	DeleteGraph(player.img.handle);		//画像をメモリ上から削除
	DeleteGraph(goal.img.handle);		//画像をメモリ上から削除
	DeleteGraph(enemy.img.handle);		//画像をメモリ上から削除
	
	DeleteGraph(TitleLogo.handle);				//画像をメモリ上から削除
	DeleteGraph(TitleEnter.handle);				//画像をメモリ上から削除
	DeleteGraph(EndClear.handle);				//画像をメモリ上から削除
	DeleteGraph(EndClearUnderline.handle);		//画像をメモリ上から削除
	DeleteGraph(EndGameOver.handle);		//画像をメモリ上から削除

	DeleteSoundMem(TitleBGM.handle);	//音楽をメモリ上からっ削除
	DeleteSoundMem(PlayBGM.handle);		//音楽をメモリ上からっ削除
	DeleteSoundMem(EndBGM.handle);		//音楽をメモリ上からっ削除
	DeleteSoundMem(EndOverBGM.handle);		//音楽をメモリ上からっ削除
	
	DeleteSoundMem(PlayerSE.handle);	//音楽をメモリ上からっ削除

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					//	ソフトの終了 
}

/// <summary>
/// ゲームのデータを読み込む
/// </summary>
/// <returns>読み込めたら TRUE / 読み込めたら FALSE</returns>
BOOL GameLoad(VOID)
{
	//プレイ動画の背景を読み込み
	strcpyDx(playMovie.path, ".\\Movie\\PlayMovie.mp4");	//パスのコピー
	playMovie.handle = LoadGraph(playMovie.path);		//画像の読み込み

	//動画が読み込め勝った時は、エラー(ー１)が入る
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインウィンドウハンドル
			playMovie.path,			//メッセージ本文
			"動画読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//画像の幅と高さを取得
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//動画のボリューム
	playMovie.Volume = 255;

	//画像の読み込み
	if (!LoadImageMem(&player.img, ".\\Image\\player.png")) { return FALSE; }
	if (!LoadImageMem(&goal.img, ".\\Image\\goal.png")) { return FALSE; }
	
	//敵の読み込み
	if (!LoadImageMem(&enemy.img, ".\\Image\\enemy.png")) { return FALSE; }
	
	//ロゴを読み込み
	if (!LoadImageMem(&TitleLogo, ".\\Image\\titlelogo.png")) { return FALSE; }
	if (!LoadImageMem(&TitleEnter, ".\\Image\\pushenter.png")) { return FALSE; }
	if (!LoadImageMem(&EndClear, ".\\Image\\gameclear.png")) { return FALSE; }
	if (!LoadImageMem(&EndClearUnderline, ".\\Image\\gameclearUnderline.png")) { return FALSE; }
	if (!LoadImageMem(&EndGameOver, ".\\Image\\gameover.png")) { return FALSE; }

	//音楽の読み込み
	if (!LoadAudio(&TitleBGM, ".\\AUDIO\\game_title.wav", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PlayBGM, ".\\AUDIO\\game_play.wav", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&EndBGM, ".\\AUDIO\\game_end.wav", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }

	if (!LoadAudio(&EndOverBGM, ".\\AUDIO\\game_over.wav", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	
	//SEの読み込み
	if (!LoadAudio(&PlayerSE, ".\\AUDIO\\SE_DOG.mp3", 255, DX_PLAYTYPE_BACK)) { return FALSE; }

	return TRUE;	//すべて読み込めた
}

/// <summary>
/// ゲームデータの初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//プレイヤーを初期化
	player.img.x = 0;											//中央寄せの場合（GAME_WIDTH / 2 - player.img.width / 2）
	player.img.y = GAME_HEIGHT / 2 - player.img.height / 2;		//中央寄せ
	player.speed = 500;
	player.img.IsDraw = TRUE;

	//当たり判定を更新する
	CollUpdatePlayer(&player);	//プレイヤーの当たり判定のアドレス


	//ゴールを初期化
	goal.img.x = GAME_WIDTH - goal.img.width;
	goal.img.y = 150;
	goal.speed = 500;
	goal.img.IsDraw = TRUE;	//描画できる

	//当たり判定を更新する
	CollUpdate(&goal);	//プレイヤーの当たり判定のアドレス

	//敵を初期化
	enemy.img.x = GAME_WIDTH / 2  - enemy.img.width / 2;
	enemy.img.y = 150;
	enemy.speed = 500;
	enemy.img.IsDraw = TRUE;	//描画できる

	//当たり判定を更新する
	CollUpdate(&enemy);	//敵の当たり判定のアドレス

	//タイトルロゴの位置を決める
	TitleLogo.x = GAME_WIDTH / 2 - TitleLogo.width / 2;			//中央揃え
	TitleLogo.y = 1;

	//PushEnterの位置を決める
	TitleEnter.x = GAME_WIDTH / 2 - TitleEnter.width / 2;		//中央揃え
	TitleEnter.y = GAME_HEIGHT - TitleEnter.height - 50;

	//ClearLogoの位置を決める
	EndClear.x = GAME_WIDTH / 2 - EndClear.width / 2;			//中央揃え
	EndClear.y = GAME_HEIGHT / 2 - EndClear.height / 2;			//中央揃え
	
	//ClearLogoUnderlinの位置を決める
	EndClearUnderline.x = GAME_WIDTH / 2 - EndClearUnderline.width / 2;				//中央揃え
	EndClearUnderline.y = GAME_HEIGHT / 2 - EndClearUnderline.height / 2;			//中央揃え

	//GameOverの位置を決める
	EndGameOver.x = GAME_WIDTH / 2 - EndGameOver.width / 2;				//中央揃え
	EndGameOver.y = GAME_HEIGHT / 2 - EndGameOver.height / 2;			//中央揃え

	//PushEnterの点滅
	PushEnterCnt = 0;
	//PushEnterCntMAX = 60; //初期化をしなくても良い？
	PushEnterBrink = FALSE;

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
	//BGMが流れていないとき
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
	}
	
	//プレイシーンへ切り替える
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMを止める
		StopSoundMem(TitleBGM.handle);
		
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//ゲームの初期化
		GameInit();

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
	//タイトルロゴの描画
	//画像を描画
	DrawGraph(TitleLogo.x, TitleLogo.y, TitleLogo.handle, TRUE);

	//MAX値まで待つ
	if (PushEnterCnt < PushEnterCntMAX) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	//カウンタの初期化
	} 

	if (PushEnterBrink == TRUE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMAX) * 255);

		//PushEnterの描画
		DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (PushEnterBrink == FALSE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);
		 
		//PushEnterの描画
		DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

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
	/*
	//エンドシーンへ切り替える
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}
	*/

	//BGMが流れていないとき
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	//プレイヤーの操作
	if (KeyDown(KEY_INPUT_UP))
	{
		player.img.y -= player.speed * fps.DeltaTime;

		//SEが流れていないとき
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			//BGMを流す
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_DOWN))
	{
		player.img.y += player.speed * fps.DeltaTime;

		//SEが流れていないとき
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			//BGMを流す
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_LEFT))
	{
		player.img.x -= player.speed * fps.DeltaTime;

		//SEが流れていないとき
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			//BGMを流す
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_RIGHT))
	{
		player.img.x += player.speed * fps.DeltaTime;

		//SEが流れていないとき
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			//BGMを流す
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	//当たり判定を更新する
	CollUpdatePlayer(&player);

	//ゴールの当たり判定を更新する
	CollUpdate(&goal);

	//当たり判定を更新する
	CollUpdate(&enemy);

	//プレイヤーがゴールに当たったときは　※更新をした後に書く
	if (OnCollRect(player.coll, goal.coll) == TRUE)
	{
		//BGMを止める
		StopSoundMem(PlayBGM.handle);

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
		return;				//処理を強制終了
	}

	//プレイヤーが敵に当たったときは　※更新をした後に書く
	if (OnCollRect(player.coll, enemy.coll) == TRUE)
	{
		//BGMを止める
		StopSoundMem(PlayBGM.handle);

		//GameOver画面に切り替え
		ChangeScene(GAME_SCENE_ENDOVER);
		return;				//処理を強制終了
	}


	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	//背景動画を描画
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//再生する
		SeekMovieToGraph(playMovie.handle, 0);	//シークバーを最初に戻す
		PlayMovieToGraph(playMovie.handle);		//動画を再生
	}
	
	//動画を描画する(画像を引き伸ばす)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	//プレイヤーを描画
	if (player.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		DrawString(0, 0, "プレイ画面を表示", GetColor(0, 0, 0));

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	if (goal.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(goal.img.x, goal.img.y, goal.img.handle, TRUE);

		DrawString(0, 0, "プレイ画面を表示", GetColor(0, 0, 0));

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(goal.coll.left, goal.coll.top, goal.coll.right, goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	if (enemy.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(enemy.img.x, enemy.img.y, enemy.img.handle, TRUE);

		DrawString(0, 0, "プレイ画面を表示", GetColor(0, 0, 0));

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(enemy.coll.left, enemy.coll.top, enemy.coll.right, enemy.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
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

	//BGMが流れていないとき
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(EndBGM.handle, EndBGM.playType);
	}

	//タイトルシーンへ切り替える
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMを止める
		StopSoundMem(EndBGM.handle);

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
	//PushEnterの描画
	DrawGraph(EndClear.x, EndClear.y, EndClear.handle, TRUE);

	//MAX値まで待つ
	if (PushEnterCnt < PushEnterCntMAX) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	//カウンタの初期化
	}

	if (PushEnterBrink == TRUE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMAX) * 255);

		//PushEnterの描画
		DrawGraph(EndClearUnderline.x, EndClearUnderline.y, EndClearUnderline.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (PushEnterBrink == FALSE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);

		//PushEnterの描画
		DrawGraph(EndClearUnderline.x, EndClearUnderline.y, EndClearUnderline.handle, TRUE);

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	

	return;
}

/// <summary>
/// GameOve画面
/// </summary>
VOID EndOver(VOID)
{
	EndOverProc();		//処理
	EndOverDraw();		//描画

	return;
}

/// <summary>
/// GameOve画面の処理
/// </summary>
VOID EndOverProc(VOID)
{
	//BGMが流れていないとき
	if (CheckSoundMem(EndOverBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(EndOverBGM.handle, EndOverBGM.playType);
	}

	//タイトルシーンへ切り替える
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMを止める
		StopSoundMem(EndOverBGM.handle);

		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}

/// <summary>
/// GameOver画面の描画
/// </summary>
VOID EndOverDraw(VOID)
{
	//GameOverの描画
	DrawGraph(EndGameOver.x, EndGameOver.y, EndGameOver.handle, TRUE);

	DrawString(0, 0, "GameOver画面", GetColor(255, 0, 0));

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
		TitleDraw();			//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();				//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();				//エンド画面の描画
		break;
	case GAME_SCENE_ENDOVER:	//ゲームオーバー画面の描画
		EndOverDraw();
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
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(255, 255, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(255, 0, 0));
	
	return;
}

/// <summary>
/// プレイヤー当たり判定の領域更新
/// </summary>
/// <param name="Coll"></param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 25;						//当たり判定を微調整
	chara->coll.top = chara->img.y + 15;						//当たり判定を微調整
	chara->coll.right = chara->img.x + chara->img.width - 15;		//当たり判定を微調整
	chara->coll.bottom = chara->img.y + chara->img.height - 5;		//当たり判定を微調整

	return;
}

/// <summary>
/// ゴール当たり判定の更新
/// </summary>
/// <param name="chara"></param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;


	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の処理
/// </summary>
/// <param name="A">矩形(くけい)A</param>
/// <param name="B">矩形(くけい)B</param>
/// <returns>当たったか当たってないか</returns>
BOOL OnCollRect(RECT A, RECT B)
{
	if ( 
		 A.left < B.right &&	//左辺のX座標＜右辺のX座標   かつ
		 A.right > B.left &&	//右辺のX座標＞左辺のX座標   かつ
		 A.top < B.bottom &&	//上辺のＹ座標＜下辺のＹ座標 かつ
		 A.bottom > B.top		//下辺のＹ座標＞上辺のＹ座標
		)
	{

		return TRUE;
	
	}
	else
	{
	
		return FALSE;
	
	}
}

BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	//音楽の読み込み
	strcpyDx(audio->path, path);					//パスのコピー
	audio->handle = LoadSoundMem(audio->path);		//音楽の読み込み

	//音楽が読み込めなかったときは、エラー（ー1）が入る
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			audio->path,			//メッセージ本文
			"音楽読み込みエラー",	//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;		//読み込み失敗
	}

	//その他の設定
	audio->Volume = volume;
	audio->playType = playType;

	return TRUE;
}

BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//ゴールの画像を読み込み
	strcpyDx(image->path, path);	//パスのコピー
	image->handle = LoadGraph(image->path);	//画像の読み込み

	//画像が読み込めなかったときは、エラー(-1)が入る
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			image->path,			//メッセージ本文
			"画像読み込みエラー！",		//メッセージタイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//画像の幅と高さを取得
	GetGraphSize(image->handle, &image->width, &image->height);

	//読み込めた
	return TRUE;
}
