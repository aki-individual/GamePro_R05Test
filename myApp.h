/*
	myApp.h
*/
#pragma once

#include "myBitmapFont.h"
#include "myBulletBuffer.h"
#include "myBulletOne.h"
#include "myEnemyManager.h"
#include "myGameStage.h"
#include "mySound.h"


// �Q�[���S�̂̏�ԑJ��.
enum GameState
{
	GAME_STATE_NONE = -1,// �����������.

	GAME_STATE_LOAD, 	// �X�e�[�W�ǂݍ���.
	GAME_STATE_READY, 	// �Q�[���J�n�҂�.
	GAME_STATE_PLAYING, // �Q�[���i�s��.
	GAME_STATE_CLEAR, 	// �X�e�[�W�N���A�I.
	GAME_STATE_DEAD, 	// �v���C���[�����j�����.
};

// DataSet�\���̂��V���O���g�����f����App�N���X�Ƃ��Ē�`.
//class DataSet��App�ɖ��̕ύX.
class App
{
private:
	static App sm_appData;	// ���̃N���X�̃I�u�W�F�N�g����.
public:
	static App* GetInstance() { return &sm_appData; }

public:
	bool InitApp();
	void MainLoop();
	void ReleaseDataSet();
	void StopAllBGM();

private:
	void ResetGameData();
	HWND InitWindow();
	HRESULT InitDirect3D();
	void UpdateData();
	void UpdatePlaying(const JOYINFOEX&, const BYTE keyBuff[256], bool playable);
	HRESULT DrawData();
	void DrawDataPlaying();

private:
	// �R���X�g���N�^��private�̈�ɔz�u���ď���ȃI�u�W�F�N�g�������o���Ȃ��悤�ɂ���.
	App()						// �R���X�g���N�^.
		: hInstance(NULL)			// �C���X�^���X�n���h��.
		, hWnd(NULL)				// �\���E�B���h�E.
		, pD3D(NULL)				// Direct3D�C���X�^���X�I�u�W�F�N�g.
		, pDevice(NULL)				// Direct3D�f�o�C�X�I�u�W�F�N�g.
		, pSprite(NULL)				// �X�v���C�g.
		, pFont(NULL)				// �r�b�g�}�b�v�t�H���g.
		, pPlayerTex(NULL)			// ���@.
		, pBulletTex(NULL)			// ���@�e��.
		, playerX(0.0f)				// ���@�̌���x�ʒu.
		, playerY(0.0f)				// ���@�̌���y�ʒu.
		, nextFireFrame(0)			// ���@�����ɒe�����Ă�܂ł̎��ԁi�t���[���j.
		, playerRoll(0)				// ���@�̌X��.
		, oneShotInput(0)			// ���������Ă����v�Ȃ悤��.
		, isBulletSwitch(false)		// ���@�̒e�ۂ̃��[�h���X�C�b�`����Ă��邩.
		, buttons(0)				// �{�^���̏��.
		, pPlayerBullet(NULL)		// ���@�̒e��.
		, pEnemyMgr(NULL)			// �G�̔z��.
		, pEnemyTexA(NULL)			// �GA�̃e�N�X�`��.
		, pEnemyTexB(NULL)			// �GB�̃e�N�X�`��.
		, pEnemyTexC(NULL)			// �GC�̃e�N�X�`��.
		, pEnemyTexD(NULL)			// �GD�̃e�N�X�`��.
		, pEnemyTexE(NULL)			// �GE�̃e�N�X�`��.
		, pEnemyTexF(NULL)			// �GF�̃e�N�X�`��.
		, pScoreDisp(NULL)			// �X�R�A�\��.
		, score(0)					// �X�R�A.
		, playerPower(0)			// ���@�̑ϋv��.
		, pEnemyBltTex(NULL)		// �G�e�ۃe�N�X�`��.
		, pEnemyBullet(NULL)		// �G�̒e��.
		, joyID(-1)					// �Q�[���R���g���[����ID.
		, gameTimer(0)				// �S�̂̃^�C�}�[.
		, pStage(NULL)				// �Q�[���X�e�[�W.
		, gameState(GAME_STATE_NONE)// �Q�[���S�̂̏��.
		, stateTimer(0)				// ��ԑJ�ڗp�^�C�}�[.
		, stageTexPosY1(0)			// 1���ڂ̃X�e�[�W�w�i��y���W.
		, stageTexPosY2(-720)		// 2���ڂ̃X�e�[�W�w�i��y���W.
		, stageCount(0)
		, pReadyTex(NULL)			// READY.
		, pStage1Tex(NULL)			// STAGE1.
		, pGameOverTex(NULL)		// GAMEOVER.
		, pStageTex(NULL)			// �X�e�[�W�̔w�i�e�N�X�`��.
		, pTitleTex(NULL)			// �^�C�g���̃e�L�X�g�e�N�X�`��.
		, pTitleBGTex(NULL)			// �^�C�g���̔w�i�e�N�X�`��.
	{
		memset(&d3dpp, 0, sizeof(d3dpp));	// D3D�f�o�C�X�쐬���̃p�����[�^.
		memset(pBgm, NULL, sizeof(pBgm));	// BGM.
		memset(pSe, NULL, sizeof(pSe));		// SE.
	}

public:
	EnemyManager* GetEnemyMgr() { return pEnemyMgr; }
	void GetPlayerPos(float& x, float& y) { x = playerX; y = playerY; }
	GameState GetGameState() { return gameState; }

	void AddScore(int add);	// ���_�����Z.
	void PlayBGM(SoundBGM, bool bLoop = false);
	void PlaySE(SoundSE);
	void DrawSprite(MyTexture*, D3DXVECTOR3*, D3DXVECTOR3*);
	void DrawSprite(MyTexture*, const RECT*, D3DXVECTOR3*, D3DXVECTOR3*);
	BulletOne* GetNextEnemyBlt();
	void CreateEnemyA(float x);
	void CreateEnemyB(float x);
	void CreateEnemyC(float x);
	void CreateEnemyD(float x);
	void CreateEnemyE(float x);
	void CreateEnemyF(float x);


private:
	HINSTANCE hInstance;			// �C���X�^���X�n���h��.
	HWND hWnd;						// �\���E�B���h�E.

	IDirect3D9* pD3D;				// Direct3D�C���X�^���X�I�u�W�F�N�g.
	D3DPRESENT_PARAMETERS d3dpp;	// �f�o�C�X�쐬���̃p�����[�^.

	IDirect3DDevice9* pDevice;		// Direct3D�f�o�C�X�I�u�W�F�N�g.
	ID3DXSprite* pSprite;			// �X�v���C�g.
	MyBitmapFont* pFont;			// �r�b�g�}�b�v�t�H���g.

	MyTexture* pPlayerTex;			// ���@.
	MyTexture* pBulletTex;			// ���@�e��.

	float playerX, playerY;			// ���@�̌��݈ʒu.
	int nextFireFrame;				// ���@�����ɒe�����Ă�܂ł̎��ԁi�t���[���j.
	int playerRoll;					// ���@�̌X��.(0:�X���Ȃ��A1:���A2:�E).
	int oneShotInput;				// �L�[���������Ă����v�Ȃ悤��.
	bool isBulletSwitch;			// ���@�e�ۂ̃��[�h���X�C�b�`����Ă��邩.
	UINT buttons;					// �{�^���̏��.
	BulletBuffer* pPlayerBullet;	// ���@�̒e��.

	EnemyManager* pEnemyMgr;		// �G�̔z��.
	MyTexture* pEnemyTexA;			// �GA�̃e�N�X�`��.
	MyTexture* pEnemyTexB;			// �GB�̃e�N�X�`��.
	MyTexture* pEnemyTexC;			// �GC�̃e�N�X�`��.
	MyTexture* pEnemyTexD;			// �GD�̃e�N�X�`��.
	MyTexture* pEnemyTexE;			// �GE�̃e�N�X�`��.
	MyTexture* pEnemyTexF;			// �GF�̃e�N�X�`��.

	NumberDisp* pScoreDisp;			// �X�R�A�\��.
	int score;						// �X�R�A.
	int playerPower;				// ���@�̑ϋv��.

	MyTexture* pEnemyBltTex;		// �G�e�ۃe�N�X�`��.
	BulletBuffer* pEnemyBullet;		// �G�̒e��.

	UINT joyID;						// �Q�[���R���g���[����ID.
	int gameTimer;					// �S�̂̃^�C�}�[.

	MySound* pBgm[BGM_COUNT];		// BGM.
	MySound* pSe[SE_COUNT];			// SE.

	GameStage* pStage;				// �Q�[���X�e�[�W.
	GameState gameState;			// �Q�[���S�̂̏��.
	int stateTimer;					// ��ԑJ�ڗp�^�C�}�[.
	int stageCount;					// �ǂݍ��ރX�e�[�W�̔ԍ�.
	float stageTexPosY1;			// �w�i��y���W1.
	float stageTexPosY2;			// �w�i��y���W2.
	MyTexture* pReadyTex;			// READY.
	MyTexture* pStage1Tex;			// STAGE1.
	MyTexture* pStage2Tex;			// STAGE2.
	MyTexture* pStage3Tex;			// STAGE3.
	MyTexture* pGameOverTex;		// GAMEOVER.
	MyTexture* pTitleTex;			// TITLE.
	MyTexture* pTitleBGTex;			// TITLE�̔w�i.
	MyTexture* pClearTex;			// CLEAR.
	MyTexture* pStageTex;			// �X�e�[�W�̔w�i�e�N�X�`��.

};

inline App* GetAppInst() { return App::GetInstance(); }
