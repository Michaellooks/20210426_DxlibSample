#pragma once

//�w�b�_�t�@�C���̓ǂݍ���
#include "DxLib.h"

//�}�N����`
#define GAME_FPS		60	//FPS�̕W��
#define GAME_FPS_MAX	240	//FPS�̍ő�l
#define GAME_FPS_MIN	60	//FPS�ŏ��l

//FPS�\����
struct FPS
{
	BOOL IsInitFlag = FALSE;	//�ŏ��̑���J�n�t���O
	LONGLONG StartTime = 0;		//����J�n����
	LONGLONG NowTime = 0;		//���݂̎���
	LONGLONG OldTime = 0;		//�ȑO�̎���

	int Value = GAME_FPS;		//FPS�l

	float DeltaTime = 0.000001; //�O���T�@�o�ߎ���
	int Count = 1;				//���݂̃t���[����
	float DrawValue = 0.0f;		//�v�Z���ʂ�`��
	int SampleRate = GAME_FPS;	//���ς��Ƃ�T���v���l
};

//�v���g�^�C�v�錾
VOID FPSUpdate(VOID);	//FPS�l���X�V����
VOID FPSDraw(VOID);		//FPS�l��`��
VOID FPSWait(VOID);		//FPS�l��҂�