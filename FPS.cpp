//FPSのソースファイル

//ヘッダファイルの読み込み
#include "FPS.h"

//グローバル変数
FPS fps;		//FPSの管理

//関数

/// <summary>
/// FPS値を計算し、値を更新する
/// </summary>
/// <param name=""></param>
VOID FPSUpdate(VOID)
{
	//一番最初の処理
	if (FALSE == fps.IsInitFlag)
	{
		//測定開始時刻をマイクロ秒単位で取得
		fps.StartTime = GetNowHiPerformanceCount();	//windowsが起動してから経過した時間(マイクロ秒)

		fps.IsInitFlag = TRUE;	//フラグを立てる
	}

	//現在の時刻をマイクロ秒単位で取得
	fps.NowTime = GetNowHiPerformanceCount();

	//前回取得した時間からの経過時間を秒(少数)に変換してからセット
	fps.DeltaTime = (fps.NowTime - fps.OldTime) / 1000000.0f;

	//今回取得した時間を保存
	fps.OldTime = fps.NowTime;

	//1フレーム目～FPS設定値までは、カウントアップ
	if (fps.Count < fps.SampleRate)
	{
		//カウンタを増やす
		fps.Count++;
	}
	else
	{
		//FPS設定値のフレームで、平均FPSを計算

		//現在の時刻から、０フレーム目の時間を引き、FPSの数値で割る
		//現在の平均FPS値が出る
		fps.DrawValue = 1000000.f / ((fps.NowTime - fps.StartTime) / (float)fps.SampleRate);

		//測定開始時刻をマイクロ秒単位で取得
		fps.StartTime = GetNowHiPerformanceCount();	//Windowsが起動してから経過した時間(マイクロ秒)

		//カウンタ初期化
		fps.Count = 1;
	}

	return;

}


/// <summary>
/// 平均FPS値を描画する
/// </summary>
/// <param name=""></param>
VOID FPSDraw(VOID)
{
	if (GAME_DEBUG == TRUE)
	{
		//文字列を描画
		DrawFormatString(0, GAME_HEIGHT - 20, GetColor(0, 0, 0), "FPS:%.1f", fps.DrawValue);
	}

	return;
}

/// <summary>
/// FPSで処理を待つ
/// </summary>
/// <param name=""></param>
VOID FPSWait(VOID)
{
	//現在の時刻－最初の時刻で、現在かかっている時刻を取得する
	LONGLONG resultTime = fps.NowTime - fps.StartTime;

	//待つべきミリ秒数(1秒 / FPS値 * 現在のフレーム数)から、現在かかっている時刻を引く
	int waitTiem = 1000000.0f / fps.Value * fps.Count - resultTime;

	//マイクロ秒からミリ秒に変換
	waitTiem /= 1000.0f;

	//処理が早かったら、処理を待つ
	if (waitTiem > 0)
	{
		WaitTimer(waitTiem);	//引数ミリ秒
	}

	//垂直同期をONにしているか？
	if (FALSE == GetWaitVSyncFlag())
	{
		//FPS最大値ではないとき
		if (fps.Value < GAME_FPS_MAX)
		{
			//1秒毎のFPS値よりも、待つ時間が小さい時は、もっとFPS値を上げてもいい
			//待つ時間　１０ミリ　＜＝　1秒/６０FPS　=　16.6666ミリ　もう少し早くできる
			if (waitTiem > 0
				&& waitTiem <= 1000.0f / fps.Value)
			{
				fps.Count++;
			}
			else
			{
				//FPS値ガ追いついておらず、遅いときは、FPS値を下げる
				if (fps.Value > GAME_FPS_MIN)
				{
					fps.Value--;
				}
			}
		}
	}
}