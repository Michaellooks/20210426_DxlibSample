
//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"

#include "keyboard.h"		//�L�[�{�[�h�̏���

#include "FPS.h"			//FPS�̏���

//�\���̂̒�`

//�L�����N�^�̍\����
struct CHARACTOR
{
	int handle = -1;	//�摜�̃n���h��(�Ǘ��ԍ�)
	char path[255];		//�摜�̏ꏊ(�p�X)

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	int speed = 1;		//�ړ����x

	RECT coll;			//�����蔻��̗̈�(�l�p)

	BOOL IsDraw = FALSE;//�摜���`��ł���
};

//����̍\����
struct MOVIE
{
	int handle = -1;	//����̃n���h��
	char path[255];		//����̃p�X

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	int Volume = 255;	//�{�����[��(�ŏ�)0�`255(�ő�)
};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//	���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//	�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;	//	���̃Q�[���̃V�[��

//�v���C�w�i�̓���
MOVIE playMovie;

//�v���C���[
CHARACTOR player;

//�S�[��
CHARACTOR goal;

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;		//	�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;		//	�t�F�[�h�C��

int fadeTimeMill = 2000;						//	�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;		//	�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCntMax = fadeTimeMax;		//�t�F�[�h�A�E�g�̃J�E���^MAX

//�v���g�^�C�v�錾
VOID Title(VOID);							//	�^�C�g�����
VOID TitleProc(VOID);						//	�^�C�g����ʁi�����j
VOID TitleDraw(VOID);						//	�^�C�g����ʁi�`��j

VOID Play(VOID);							//	�v���C���
VOID PlayProc(VOID);						//	�v���C��ʁi�����j
VOID PlayDraw(VOID);						//	�v���C��ʁi�`��j

VOID End(VOID);								//	�G���h���
VOID EndProc(VOID);							//	�G���h��ʁi�����j
VOID EndDraw(VOID);							//	�G���h��ʁi�`��j

VOID Change(VOID);							//	�؂�ւ����
VOID ChangeProc(VOID);						//	�؂�ւ���ʁi�����j
VOID ChangeDraw(VOID);						//	�؂�ւ���ʁi�`��j

VOID ChangeScene(GAME_SCENE scene);			//�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V

VOID CollUpdate(CHARACTOR* chara);			//�����蔻��

BOOL OnCollRect(RECT A, RECT B);			//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);						//�Q�[���̃f�[�^�̓ǂݍ���

VOID GameInit(VOID);						//�Q�[���f�[�^�̏�����

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O���@ = (WinAPI)�œ����Ă���I
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O�����񂽂�Ɏg����d�g��

int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{

	SetOutApplicationLogValidFlag(FALSE);				//	tag.txt���o�͂��Ȃ�

	ChangeWindowMode(TRUE);								//	�E�B���h�E���[�h�ɐݒ�

	SetMainWindowText(GAME_TITLE);						//	�E�B���h�E�̃^�C�g������

	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//	�E�B���h�E�̉𑜓x��ݒ�

	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//	�E�B���h�E�̑傫����ݒ�

	SetBackgroundColor(255, 255, 255);					//	�f�t�H���g�̔w�i�̐F

	SetWindowIconID(GAME_ICON_ID);						//	�A�C�R���t�@�C����ǂݍ���

	SetWindowStyleMode(GAME_WINDOW_BAR);				//	�E�B���h�E�o�[�̏��

	SetWaitVSyncFlag(TRUE);								//	�f�B�X�v���C�̐���������L���ɂ��� ���d�v

	SetAlwaysRunFlag(TRUE);								//	�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	//DrawPixel(320, 240, GetColor(255, 255, 255));	// �_��ł�

	//WaitKey();				// �L�[���͑҂�

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���ǂݍ���
	if (!GameLoad())
	{

		//�f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�

		DxLib_End();	//DxLib�I��
		return -1;		//�ُ�I��

	}

	//�Q�[���̏�����
	GameInit();

	//�������[�v
	while (1)
	{

		if (ProcessMessage() != 0)	{ break; }	//���b�Z�[�W���󂯎�葱����

		if (ClearDrawScreen() != 0) { break; }  //��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();
		
		ScreenFlip();			//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�I���Ƃ��̏���
	DeleteGraph(playMovie.handle);	//������������ォ��폜
	DeleteGraph(player.handle);		//�摜���������ォ��폜
	DeleteGraph(goal.handle);		//�摜���������ォ��폜

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					//	�\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂��� TRUE / �ǂݍ��߂��� FALSE</returns>
BOOL GameLoad(VOID)
{
	//�v���C����̔w�i��ǂݍ���
	strcpyDx(playMovie.path, ".\\Movie\\PlayMovie.mp4");	//�p�X�̃R�s�[
	playMovie.handle = LoadGraph(playMovie.path);		//�摜�̓ǂݍ���

	//���悪�ǂݍ��ߏ��������́A�G���[(�[�P)������
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���E�B���h�E�n���h��
			playMovie.path,			//���b�Z�[�W�{��
			"����ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//����̃{�����[��
	playMovie.Volume = 255;

	//�v���C���[�̉摜�̓ǂݍ���
	strcpyDx(player.path, ".\\image\\player.png");	//�p�X�̃R�s�[
	player.handle = LoadGraph(player.path);			//�摜�̓ǂݍ���

	//�摜���ǂݍ��ߏ��������́A�G���[(�[�P)������
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���E�B���h�E�n���h��
			player.path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(player.handle, &player.width, &player.height);

	/*
	//�����蔻����X�V����
	CollUpdatePlayer(&player);	//�v���C���[�̓����蔻��̃A�h���X ���ʒu��ς��Ă��瓖���蔻���ς���
	*/

	

	//�S�[���̉摜�̓ǂݍ���
	strcpyDx(goal.path, ".\\image\\goal.png");	//�p�X�̃R�s�[
	goal.handle = LoadGraph(goal.path);			//�摜�̓ǂݍ���

	//�摜���ǂݍ��ߏ��������́A�G���[(�[�P)������
	if (goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���E�B���h�E�n���h��
			goal.path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(goal.handle, &goal.width, &goal.height);


	return TRUE;	//���ׂēǂݍ��߂�
}

/// <summary>
/// �Q�[���f�[�^�̏�����
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//�v���C���[��������
	player.x = 0;										//�����񂹂̏ꍇ�iGAME_WIDTH / 2 - player.width / 2�j
	player.y = GAME_HEIGHT / 2 - player.height / 2;		//������
	player.speed = 500;
	player.IsDraw = TRUE;

	//�����蔻����X�V����
	CollUpdatePlayer(&player);	//�v���C���[�̓����蔻��̃A�h���X


	//�S�[����������
	goal.x = GAME_WIDTH - goal.width;
	goal.y = 150;
	goal.speed = 500;
	goal.IsDraw = TRUE;	//�`��ł���

	//�����蔻����X�V����
	CollUpdate(&goal);	//�v���C���[�̓����蔻��̃A�h���X
}

/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scerne">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;		//�V�[����؂�ւ�
	IsFadeIn = FALSE;		//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;		//�t�F�[�h�A�E�g���Ȃ�

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();		//����
	TitleDraw();		//�`��
	
	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{
	//�v���C�V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�Q�[���̏�����
		GameInit();

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);
	}


	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "�^�C�g����ʂ�\��", GetColor(0, 0, 0));
	
	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();		//����
	PlayDraw();		//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	/*
	//�G���h�V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}
	*/

	//�v���C���[�̑���
	if (KeyDown(KEY_INPUT_UP))
	{
		player.y -= player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_DOWN))
	{
		player.y += player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_LEFT))
	{
		player.x -= player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_RIGHT))
	{
		player.x += player.speed * fps.DeltaTime;
	}

	//�����蔻����X�V����
	CollUpdatePlayer(&player);

	//�S�[���̓����蔻����X�V����
	CollUpdate(&goal);

	//�v���C���[���S�[���ɓ��������Ƃ��́@���X�V��������ɏ���
	if (OnCollRect(player.coll, goal.coll) == TRUE)
	{
		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
		return;				//�����������I��
	}

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	//�w�i�����`��
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//�Đ�����
		SeekMovieToGraph(playMovie.handle, 0);	//�V�[�N�o�[���ŏ��ɖ߂�
		PlayMovieToGraph(playMovie.handle);		//������Đ�
	}
	
	//�����`�悷��(�摜�������L�΂�)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	//�v���C���[��`��
	if (player.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(player.x, player.y, player.handle, TRUE);

		DrawString(0, 0, "�v���C��ʂ�\��", GetColor(0, 0, 0));

		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	if (goal.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(goal.x, goal.y, goal.handle, TRUE);

		DrawString(0, 0, "�v���C��ʂ�\��", GetColor(0, 0, 0));

		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(goal.coll.left, goal.coll.top, goal.coll.right, goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}


	

	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();		//����
	EndDraw();		//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	//�^�C�g���V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h��ʂ�\��", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();		//����
	ChangeDraw();		//�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���^�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����

			fadeInCnt = fadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������I��
		}
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���^�𑝂₷
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����

			fadeOutCnt = fadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;				//�t�F�[�h�A�E�g�����I��
		}
	}

	//�؂�ւ������I��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃Q�[���V�[���X�V
	}

	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(255, 255, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(255, 0, 0));
	
	return;
}

/// <summary>
/// �v���C���[�����蔻��̗̈�X�V
/// </summary>
/// <param name="Coll"></param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->x + 25;						//�����蔻��������
	chara->coll.top = chara->y + 15;						//�����蔻��������
	chara->coll.right = chara->x + chara->width - 15;		//�����蔻��������
	chara->coll.bottom = chara->y + chara->height - 5;		//�����蔻��������

	return;
}

/// <summary>
/// �S�[�������蔻��̍X�V
/// </summary>
/// <param name="chara"></param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;


	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
/// �����蔻��̏���
/// </summary>
/// <param name="A">��`(������)A</param>
/// <param name="B">��`(������)B</param>
/// <returns>�����������������ĂȂ���</returns>
BOOL OnCollRect(RECT A, RECT B)
{
	if ( 
		 A.left < B.right &&	//���ӂ�X���W���E�ӂ�X���W   ����
		 A.right > B.left &&	//�E�ӂ�X���W�����ӂ�X���W   ����
		 A.top < B.bottom &&	//��ӂ̂x���W�����ӂ̂x���W ����
		 A.bottom > B.top		//���ӂ̂x���W����ӂ̂x���W
		)
	{

		return TRUE;
	
	}
	else
	{
	
		return FALSE;
	
	}
}