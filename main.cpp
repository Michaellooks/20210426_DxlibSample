//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"	//DxLib���g�����͕K�v

//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"		//�Q�[���^�C�g��

#define GAME_WIDTH  1280				//�Q�[����ʂ̕�(�E�B�h�X)
#define GAME_HEIGHT 720					//�Q�[����ʂ̍���(�E�B�h�X)
#define GAME_COLOR	32					//�Q�[���̐F��

#define GAME_ICON_ID   333				//�Q�[���̃A�C�R����ID

#define GAME_WINDOW_BAR 0				//�E�B���h�E�o�[�̎��	

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O���@ = (WinAPI)�œ����Ă���I
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O�����񂽂�Ɏg����d�g��

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	SetOutApplicationLogValidFlag(FALSE);				//tag.txt���o�͂��Ȃ�

	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�

	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g������

	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�

	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�

	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F

	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����ǂݍ���

	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏��

	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���

	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���



	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	//DrawPixel(320, 240, GetColor(255, 255, 255));	// �_��ł�

	//WaitKey();				// �L�[���͑҂�

	//�l�p�̈ʒu�����߂�
	int X = GAME_WIDTH / 2;					
	int Y = GAME_HEIGHT / 2;

	//�l�p�̑傫�������߂�
	int width = 32;				//��
	int height = 32;			//����

	//�~�̔��a�����߂�
	int radius = 100;

	//���x
	int spead = 1;

	int Xspead = spead;
	int Yspead = spead;

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�������[�v
	while (1)
	{
		
		//��������̃L�[�������ꂽ�Ƃ�
		if (CheckHitKeyAll() != 0)
		{
			break;	//�������[�v�𔲂���
		}

		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0)	//�]1�̂Ƃ��A�G���[��E�B���h�E������ꂽ
		{
			break;	//�������[�v�𔲂���
		}


		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }

		//�l�p��`�悷��
		//drawbox(
		//	x, y, x + width, y + height,
		//	getcolor(255, 0, 0),	//�F���擾
		//	true					//�h��Ԃ�
		//);

		//�~��`��
		DrawCircle(
			X, Y, radius,
			GetColor(0, 255, 0),
			FALSE, 5
		);

		X += Xspead;						//�l�p�̈ʒu���E�ɂ��炷
		Y += Yspead;

		if (Y - radius < 0 || Y + radius > GAME_HEIGHT)
		{
			//�ړ����x�̕����𔽓]������
			//+1�Ȃ�-1�ɂ���  /	 -1�Ȃ��+1�ɂ���
			Yspead = -Yspead;	//�ړ���������𔽓]

			//�ǂɓ������������
			if (Xspead > 0) { Xspead += 2; }
			else if (Xspead < 0) { Xspead -= 2; }

		}

		if (X - radius < 0 || X + radius > GAME_WIDTH)
		{
			//�ړ����x�̕����𔽓]������
			//+1�Ȃ�-1�ɂ���  /	 -1�Ȃ��+1�ɂ���
			Xspead = -Xspead;	//�ړ���������𔽓]

			//�ǂɓ������������
			if (Yspead > 0) { Yspead += 2; }
			else if (Yspead < 0) { Yspead -= 2; }
		}


		ScreenFlip();				//�_�u���o�b�t�@�����O������ʂ�`��

	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					// �\�t�g�̏I�� 
}