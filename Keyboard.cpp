//キーボードのソースファイル

//ヘッダファイル読み込み
#include "keyboard.h"

//グローバル変数
KEYBOARD keyboard;

//関数

/// <summary>
/// キーの入力状態を更新する
/// </summary>
VOID AllKeyUpdate(VOID)
{
	//直前のキー入力を取っておく
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		
		keyboard.OldAllKeyState[i] = keyboard.AllKeyState[i];

	}

	//すべてのキーの入力状態を得る
	GetHitKeyStateAll(keyboard.TempKeyState);

	//押されているキーの時間を更新する
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{

		//キーコードを押しているとき
		if (keyboard.TempKeyState[i] != 0)
		{
			keyboard.AllKeyState[i]++;		//押している時間を計算
		}
		else
		{
			keyboard.AllKeyState[i] = 0;	//押している時間を0に戻す
		}

	}

	return;
}

/// <summary>
/// キーを押しているか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT_">キーコード</param>
/// <returns>押していたらTRUE</returns>
BOOL KeyDown(int KEY_INPUT_)
{
	
	if (keyboard.AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

/// <summary>
/// キーを上げているか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT_">キーコード</param>
/// <returns>上げていたらTRUE</returns>
BOOL KeyUp(int KEY_INPUT_)
{
	
	if (
		keyboard.OldAllKeyState[KEY_INPUT_] != 0	//以前は押していた
		&& keyboard.AllKeyState[KEY_INPUT_] == 0		//現在は押していない
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

/// <summary>
/// キーをクリックしたか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT_">キーコード</param>
/// <returns>クリックしたらTRUE</returns>
BOOL KeyClick(int KEY_INPUT_)
{
	
	if (
		keyboard.OldAllKeyState[KEY_INPUT_] != 0	//以前は押していた
		&& keyboard.AllKeyState[KEY_INPUT_] == 0		//現在は押していない
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}


/// <summary>
/// キーを押し続けているか、キーコードで判断する
/// </summary>
/// <param name="KEY_INPUT_">キーコード</param>
/// <param name="MilliTime">キーを押し続けているミリ秒</param>
/// <returns></returns>
BOOL keyDownKeep(int KEY_INPUT_, int MilliTime)
{

	//i秒は1000ミリ秒
	float MilliSce = 1000.0f;

	//押し続けている時間は、ミリ秒数×FPS値
	//例)1500ミリ秒押す / 100ミリ　→　1.5秒×60FPS = 90
	int UpdateTime = (MilliTime / MilliSce) * 60;

	if (keyboard.AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;	//押し続けていない
	}

}