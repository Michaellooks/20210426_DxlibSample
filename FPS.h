#pragma once

//ヘッダファイルの読み込み
#include "game.h"

//マクロ定義
#define GAME_FPS		60	//FPSの標準
#define GAME_FPS_MAX	240	//FPSの最大値
#define GAME_FPS_MIN	60	//FPS最小値

//FPS構造体
struct FPS
{
	BOOL IsInitFlag = FALSE;	//最初の測定開始フラグ
	LONGLONG StartTime = 0;		//測定開始時刻
	LONGLONG NowTime = 0;		//現在の時刻
	LONGLONG OldTime = 0;		//以前の時刻

	int Value = GAME_FPS;		//FPS値

	float DeltaTime = 0.000001; //０が５個　経過時間
	int Count = 1;				//現在のフレーム数
	float DrawValue = 0.0f;		//計算結果を描画
	int SampleRate = GAME_FPS;	//平均をとるサンプル値
};

//グローバル変数の外部参照
extern FPS fps;

//プロトタイプ宣言
VOID FPSUpdate(VOID);	//FPS値を更新する
VOID FPSDraw(VOID);		//FPS値を描画
VOID FPSWait(VOID);		//FPS値を待つ