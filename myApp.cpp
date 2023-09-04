/*
	myApp.cpp
	Kobayashi Atsuki.
	�z�z�R�[�h�����̂܂܎g�p.

	�K�v�ȗv�f���w�b�_�[�t�@�C���ɋL�q���A�������C���N���[�h���邱�ƂŁA
	�����̂��炢�񂵂��Ȃ����A�R�[�h�̉ǐ������߂�B
*/
#include "stdafx.h"
#include "myApp.h"
#include "myTexture.h"
#include "myGameStage.h"
#include "myBulletBuffer.h"
#include "myEnemyManager.h"
#include "myBulletBuffer.h"
#include "myTimer.h"
#include "myEnemyA.h"
#include "myEnemyB.h"
#include "myEnemyC.h"
#include "myEnemyD.h"
#include "myEnemyE.h"
#include "myEnemyF.h"

//#define FULLSCREEN

// �E�B���h�E�^�C�g���o�[�ɕ\�������o�[�W������.
#define WDATE(x, y) _T(x y)

// �v���C���[�e�N�X�`���̕\���̈�̂��߂̔z��.
static const RECT g_PlayerImageArea[] = {
	{ 0,   0, 31,  32 },
	{ 0,  32, 31,  64 },
	{ 0,  64, 31,  96 },
	{ 0,  96, 31, 128 },
};

// �C�x���g�����R�[���o�b�N�i�E�B���h�E�v���V�[�W���j.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �C�x���g�̎�ނɉ����āAswitch���ɂď�����؂蕪����.
	switch (uMsg)
	{
	case WM_KEYDOWN: {
		// ESC�L�[���������ꂽ��I��.
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		break;
	}

	case WM_CLOSE: {
		// �I���ʒm(CLOSE�{�^���������ꂽ�ꍇ�Ȃ�)���͂����ꍇ.
		// �v���O�������I�������邽�߁A�C�x���g���[�v��0��ʒm����.
		// ���̌��ʁAGetMessage�̖߂�l��0�ɂȂ�.
		PostQuitMessage(0);
		break;
	}

	default:
		break;
	}

	// �f�t�H���g�̃E�B���h�E�C�x���g����.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// �G���[��������HRESULT�𕶎���ɕϊ����邽�߂̕⏕�֐�.
static const TCHAR* D3DErrStr(HRESULT res)
{
	switch (res)
	{
	case D3D_OK: return _T("D3D_OK");
	case D3DERR_DEVICELOST: return _T("D3DERR_DEVICELOST");
	case D3DERR_DRIVERINTERNALERROR: return _T("D3DERR_DRIVERINTERNALERROR");
	case D3DERR_INVALIDCALL: return _T("D3DERR_INVALIDCALL");
	case D3DERR_OUTOFVIDEOMEMORY: return _T("D3DERR_OUTOFVIDEOMEMORY");
	case D3DERR_DEVICENOTRESET: return _T("D3DERR_DEVICENOTRESET");
	case D3DERR_NOTAVAILABLE: return _T("D3DERR_NOTAVAILABLE");
	case D3DXERR_INVALIDDATA: return _T("D3DXERR_INVALIDDATA");
	case MMSYSERR_NODRIVER: return _T("MMSYSERR_NODRIVER");
	case MMSYSERR_INVALPARAM: return _T("MMSYSERR_INVALPARAM");
	case MMSYSERR_BADDEVICEID: return _T("MMSYSERR_BADDEVICEID");
	case JOYERR_UNPLUGGED: return _T("JOYERR_UNPLUGGED");
	case JOYERR_PARMS: return _T("JOYERR_PARMS");
	case E_OUTOFMEMORY: return _T("E_OUTOFMEMORY");
	}
	return _T("unknown error");
}

static const TCHAR* MMErrStr(MMRESULT res)
{
	switch (res)
	{
	case JOYERR_NOERROR: return _T("NO ERROR");
	case MMSYSERR_NODRIVER: return _T("�Q�[���R���g���[���̃h���C�o������܂���");
	case MMSYSERR_INVALPARAM: return _T("�s���ȃp�����[�^�w��ł�");
	case JOYERR_UNPLUGGED: return _T("�Q�[���R���g���[�����ڑ�����Ă��܂���");
	case JOYERR_PARMS: return _T("�Q�[���R���g���[���̎w�肪�s���ł�");
	default: return _T("��G���[");
	}
}

////////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����f�[�^����.
App App::sm_appData;

// �A�v���P�[�V����������������.
bool App::InitApp()
{
	// ���̃v���O���������s�����Ƃ��̃C���X�^���X�n���h�����擾.
	hInstance = GetModuleHandle(NULL);

	HRESULT hr = E_FAIL;

	// �L���ȃW���C�X�e�B�b�N��T��.
	JOYINFO ji;
	MMRESULT mr = joyGetPos(JOYSTICKID1, &ji);
	if (mr == JOYERR_NOERROR)
	{
		joyID = JOYSTICKID1;
		_tprintf(_T("JOY1\n"));
	}
	else
	{
		mr = joyGetPos(JOYSTICKID2, &ji);
		if (mr == JOYERR_NOERROR)
		{
			joyID = JOYSTICKID2;
			_tprintf(_T("JOY2\n"));
		}
		else
		{
			MessageBox(NULL, _T("�W���C�X�e�B�b�N���q�����Ă��܂���\n�L�[�{�[�h�ő��삵�܂�"), _T("JOYSTICK"), MB_OK);
			joyID = -1;
		}
	}

	hWnd = InitWindow();			// �E�B���h�E���쐬����.

	// Direct3D������������.
	hr = GetAppInst()->InitDirect3D();
	if (FAILED(hr))
	{
		MessageBox(NULL, D3DErrStr(hr), _T("Direct3D���������s"), MB_OK);
		return false;
	}

	// �X�v���C�g�̍쐬.
	hr = D3DXCreateSprite(pDevice, &(pSprite));
	if (FAILED(hr))
	{
		MessageBox(NULL, D3DErrStr(hr), _T("�X�v���C�g�쐬���s"), MB_OK);
		return false;
	}

	// �Ƃ肠���������l�����Ă���.
	playerX = (WIDTH / 2);
	playerY = (HEIGHT - 100);
	playerPower = 100;
	gameState = GAME_STATE_LOAD;

	// �e�ۊǗ��N���X�̃I�u�W�F�N�g�𐶐� & ������.
	pPlayerBullet = new BulletBuffer(50);
	if (pPlayerBullet == NULL || !pPlayerBullet->Init())
	{
		return false;
	}

	// �e�ۊǗ��N���X�i�G�p�j�̃I�u�W�F�N�g�𐶐�&������.
	pEnemyBullet = new BulletBuffer(100);
	if (pEnemyBullet == NULL || !pEnemyBullet->Init())
	{
		return false;
	}

	// �������e�N�X�`���ŕ\�����邽�߂̃I�u�W�F�N�g�𐶐�.
	pScoreDisp = new NumberDisp();
	if (pScoreDisp == NULL)
	{
		return false;
	}

	// �e�N�X�`����ǂݍ��ށB�G���[���ɂ�LoadTexture�͗�O�𔭐�������.
	// LoadTexture���Ƃ�if���������͖̂ʓ|�Ȃ̂ŁAtry�`throw�`catch���g��.
	try
	{
		pFont = MyBitmapFont::LoadFont(pDevice, _T("data/image/fontBitmap.png"));
		pPlayerTex = MyTexture::LoadTexture(pDevice, _T("data/image/player_Taki.png"));
		pBulletTex = MyTexture::LoadTexture(pDevice, _T("data/image/bullet.png"));
		pEnemyTexA = MyTexture::LoadTexture(pDevice, _T("data/image/enemyAAA.png"));
		pEnemyTexB = MyTexture::LoadTexture(pDevice, _T("data/image/enemyBBB.png"));
		pEnemyTexC = MyTexture::LoadTexture(pDevice, _T("data/image/enemyCCC.png"));
		pEnemyTexD = MyTexture::LoadTexture(pDevice, _T("data/image/enemyDDD.png"));
		pEnemyTexE = MyTexture::LoadTexture(pDevice, _T("data/image/enemyEEE.png"));
		pEnemyTexF = MyTexture::LoadTexture(pDevice, _T("data/image/enemyFFF.png"));
		pEnemyBltTex = MyTexture::LoadTexture(pDevice, _T("data/image/enemyBullet.png"));
		pStageTex = MyTexture::LoadTexture(pDevice, _T("data/image/keikakudoori.png"));
		pTitleBGTex = MyTexture::LoadTexture(pDevice, _T("data/image/jyounouchi.png"));

		pReadyTex = MyTexture::LoadTexture(pDevice, _T("data/image/ready.png"));
		pGameOverTex = MyTexture::LoadTexture(pDevice, _T("data/image/gameover.png"));
		pTitleTex = MyTexture::LoadTexture(pDevice, _T("data/image/title.png"));
		pClearTex = MyTexture::LoadTexture(pDevice, _T("data/image/clear.png"));
		pStage1Tex = MyTexture::LoadTexture(pDevice, _T("data/image/stage1.png"));
		pStage2Tex = MyTexture::LoadTexture(pDevice, _T("data/image/stage2.png"));
		pStage3Tex = MyTexture::LoadTexture(pDevice, _T("data/image/stage3.png"));

		// ���l�\���p�e�N�X�`���̓ǂݍ���.
		MyTexture* pTex = MyTexture::LoadTexture(pDevice, _T("data/image/numbers.png"));
		// �X�R�A�\���Ɏg���e�N�X�`���̏���ݒ�.
		pScoreDisp->Init(pTex, 64, 64);// 64:�����P���̃e�N�X�`���T�C�Y.
	}
	// catch���try�̒���ɋL�q����.
	catch (HRESULT /*hr*/)
	{
		return false;
	}

	// �G�Ǘ��N���X�̃I�u�W�F�N�g�𐶐�.
	pEnemyMgr = new EnemyManager(70);
	if (pEnemyMgr == NULL || !pEnemyMgr->Init())
	{
		return false;
	}



	// �T�E���h�֘A����.
	{
		// DirectSound�̏�����.
		hr = MySound::InitDirectSound(hWnd);
		if (FAILED(hr))
		{
			MessageBox(NULL, D3DErrStr(hr), _T("�_�C���N�g�T�E���h�쐬���s"), MB_OK);
			return false;
		}

		// BGM�̓ǂݍ���.
		static const TCHAR* pcszBgmFileName[BGM_COUNT] = {
			_T("data/bgm/launch.wav"),		// BGM0.
			_T("data/bgm/C1_0838AB.WAV"),	// BGM1.
			_T("data/bgm/C1_0523AB.WAV"),	// BGM2.
		};
		for (int i = BGM_BEGIN; i < BGM_COUNT; i++)
		{
			pBgm[i] = MySound::LoadSound(pcszBgmFileName[i]);
			if (pBgm[i] == NULL) {
				return false;
			}
		}

		// SE�̓ǂݍ���.
		static const TCHAR* pcszSeFileName[SE_COUNT] = {
			_T("data/se/Space Gun 01.wav"), // ���ˉ�.
			_T("data/se/bakuha.WAV"),		// ���ĉ�.
			_T("data/se/ganx.WAV"),			// ��e��.
			_T("data/se/daibakuhatsu.wav"),	// ���S��.
			_T("data/se/S02_AO02.WAV"),		// �J�n��.
			_T("data/se/ReadyStart.WAV"),	// �v���C�X�^�[�g��.
			_T("data/se/BulletSwitch.WAV")	// �e�ۃ��[�h�X�C�b�`��.
		};
		for (int i = SE_BEGIN; i < SE_COUNT; i++) {
			pSe[i] = MySound::LoadSound(pcszSeFileName[i]);
			if (pSe[i] == NULL) {
				return false;
			}
		}
	}

	CTimer::GetTimer()->InitTimer();
	return true;
}

// ���C�����[�v.
void App::MainLoop()
{
	ShowWindow(hWnd, SW_SHOWNORMAL);	// �쐬�����E�B���h�E��\������.
	CTimer::GetTimer()->StartTimer();

	// BGM���Đ�.
	pBgm[BGM_0]->Play(true);

	// �C�x���g���[�v.
	// �u���b�N�^�֐�GetMessage�ł͂Ȃ��m���u���b�N�^�֐���PeekMessage���g��.
	MSG msg;
	bool flag = 1;
	while (flag)
	{
		// ���b�Z�[�W�����邩�ǂ����m�F����.
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			// ���b�Z�[�W������̂ŏ�������.
			if (GetMessage(&msg, NULL, 0, 0) == 1)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				flag = 0;
			}
		}
		else
		{
			UpdateData();	// �ʒu�̍Čv�Z.
			DrawData();		// �`��.
		}
		// Sleep�ȂǍs���Ă��Ȃ����ADrawData�֐�����pDevice->Present��.
		// ��ʍX�V�҂����s���̂ŁA���ʓI��VSync���ƂɃC�x���g���[�v�����.
	}
}

// ���I�Ɋm�ۂ������������������.
void App::ReleaseDataSet()
{
	// �T�E���h�̊J��.
	MySound::ReleaseAll();

	// �e�N�X�`���̊J��.
	MyTexture::ReleaseAll();

	// ���I�I�u�W�F�N�g�̊J��.
	if (pStage) { delete pStage; }
	if (pScoreDisp) { delete pScoreDisp; }
	if (pEnemyBullet) { delete pEnemyBullet; }
	if (pEnemyMgr) { delete pEnemyMgr; }
	if (pPlayerBullet) { delete pPlayerBullet; }
	if (pFont) { delete pFont; }

	// DirectX�f�o�C�X�̊J��.
	if (pSprite) { pSprite->Release(); }
	if (pDevice) { pDevice->Release(); }
	if (pD3D) { pD3D->Release(); }
}

// �SBGM��~.
void App::StopAllBGM()
{
	for (int i = 0; i < _countof(pBgm); i++)
	{
		if (pBgm[i] != NULL)
		{
			pBgm[i]->Stop();
		}
	}
}

// �X�e�[�W�ς��Ȃǂ̎��ɕK�v�ȃf�[�^���������Z�b�g����.
void App::ResetGameData()
{
	pEnemyMgr->ResetAll();
	pEnemyBullet->ResetAll();
	pPlayerBullet->ResetAll();

	// �v���C���[�̏����l
	playerX = WIDTH / 2;
	playerY = HEIGHT - 100;
	playerPower = 100;
	nextFireFrame = 0;
	stateTimer = 0;
}

// Window���쐬����.
// return �E�B���h�E�n���h��.
HWND App::InitWindow()
{
	// �܂��E�B���h�E�N���X��o�^����.
	// ����̓E�B���h�E������̏����̎d����Windows�ɋ����邽�߂ł���.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));		// �ϐ�wc���[���N���A����.
	wc.cbSize = sizeof(WNDCLASSEX);				// ���̍\���̂̑傫����^����.
	wc.lpfnWndProc = (WNDPROC)WindowProc;		// �E�B���h�E�v���V�[�W���o�^.
	wc.hInstance = hInstance;			// �C���X�^���X�n���h����ݒ�.
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);	// �}�E�X�J�[�\���̓o�^.
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);// �w�i��GRAY��.
	wc.lpszClassName = _T("window01");			// �N���X���ACreateWindow�ƈ�v������.
	RegisterClassEx(&wc);						// �o�^.

	RECT rc = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_tprintf(_T("rc: %d, %d -- %d, %d\n"), rc.left, rc.top, rc.right, rc.bottom);

	// �E�B���h�E���쐬����.
	static const TCHAR* pVersion = WDATE(__DATE__, __TIME__);
	hWnd = CreateWindow(_T("window01"), pVersion, WS_OVERLAPPEDWINDOW, 0, 0, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

	return hWnd;
}

// Direct3D������������.
// return ���������G���[�܂���D3D_OK.
HRESULT App::InitDirect3D()
{
	HRESULT hr;

	// Direct3D�C���X�^���X�I�u�W�F�N�g�𐶐�����.
	// D3D_SDK_VERSION�ƁA�����^�C���o�[�W�����ԍ����K�؂łȂ��ƁANULL���Ԃ�.
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	// NULL�Ȃ烉���^�C�����s�K��.
	if (pD3D == NULL) return E_FAIL;

	// PRESENT�p�����[�^���[���N���A���A�K�؂ɏ�����.
	ZeroMemory(&d3dpp, sizeof(d3dpp));
#ifdef FULLSCREEN
	d3dpp.Windowed = FALSE;
	//int fx = GetSystemMetrics(SM_CXSCREEN);
	//int fy = GetSystemMetrics(SM_CYSCREEN);
	int fx = WIDTH;
	int fy = HEIGHT;
	_tprintf(_T("�t���X�N���[�� %d x %d\n"), fx, fy);
	d3dpp.BackBufferHeight = fy;
	d3dpp.BackBufferWidth = fx;
#else
	d3dpp.Windowed = TRUE;
#endif
	d3dpp.BackBufferHeight = HEIGHT;
	d3dpp.BackBufferWidth = WIDTH;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// �o�b�N�o�b�t�@��RGB���ꂼ��W�r�b�g��.
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	// Present���ɐ��������ɍ��킹��.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BHAL&HARD.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("HAL & HARD\n"));
		return hr;
	}
	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BHAL&SOFT.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("HAL & SOFT\n"));
		return hr;
	}
	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BREF&HARD.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("REF & HARD\n"));
		return hr;
	}
	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BREF&SOFT.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("REF & SOFT\n"));
		return hr;
	}
	return hr;
}

// �f�[�^���A�b�v�f�[�g����.
void App::UpdateData()
{
	gameTimer++;
	stateTimer++;

	buttons = 0;
	JOYINFOEX ji = { 0 };
	if (joyID != -1)
	{
		// �W���C�X�e�B�b�N�̏��𓾂�.
		ji.dwSize = sizeof(ji);
		ji.dwFlags = JOY_RETURNALL;
		joyGetPosEx(joyID, &ji);
		buttons = ji.dwButtons;
	}

	BYTE keyBuff[256];
	if (!GetKeyboardState(keyBuff))
	{
		return;
	}

	if (keyBuff[VK_SPACE] & 0x80)
	{
		buttons |= JOY_BUTTON1;
	}
	if (keyBuff[VK_CONTROL] & 0x80)
	{
		buttons |= JOY_BUTTON2;
	}

	// �Q�[����Ԃɉ����������ɕύX.
	switch (gameState)
	{
	case GAME_STATE_LOAD:
	{
		GetAppInst()->ResetGameData();

		// �����̓ǂݍ��ݍς݂̃X�e�[�W�f�[�^������Ή��.
		if (pStage != NULL)
		{
			delete pStage;
		}

		stageCount++;

		pStage = new GameStage;

		// ��Ԃ�J��.
		//	�X�e�[�W�ԍ��ɉ����ăX�e�[�W�f�[�^��ǂݍ���.
		if (stageCount == 1) {
			pStage->ReadStage(_T("data/stage/stage1.txt"));
		}
		else if (stageCount == 2) {
			pStage->ReadStage(_T("data/stage/stage2.txt"));
		}
		else if (stageCount == 3) {
			pStage->ReadStage(_T("data/stage/stage3.txt"));
		}
		else if (stageCount == 4) {
			gameState = GAME_STATE_CLEAR;
			break;
		}

		gameState = GAME_STATE_READY;
		break;
	}

	case GAME_STATE_READY:
	{
		// B�{�^��(CTRL-Key)�����������ꂽ��Q�[���J�n.
		if (buttons == JOY_BUTTON2)
		{
			gameState = GAME_STATE_PLAYING;
			stateTimer = 0;
			gameTimer = 0;
			pSe[SE_READYSTART]->Play();// �v���C�J�nSE��炷.
		}
		break;
	}

	case GAME_STATE_PLAYING:
	{
		UpdatePlaying(ji, keyBuff, true);

		// �v���C���[�̃p���[���[���ɂȂ�����A�Q�[���I�[�o�[.
		if (playerPower <= 0)
		{
			GetAppInst()->StopAllBGM();
			pSe[SE_DEAD]->Play();
			gameState = GAME_STATE_DEAD;
			stateTimer = 0;
		}

		break;
	}

	case GAME_STATE_DEAD:
	{
		UpdatePlaying(ji, keyBuff, false);
		if (stateTimer > 300)
		{
			GetAppInst()->ResetGameData();
			gameState = GAME_STATE_READY;
			stateTimer = 0;
			pStage->Rewind();
		}
		break;
	}
	}
}

// GAME_PLAYING��Ԃ̎���Update
// playable��true�ł���Ύ��@����
void App::UpdatePlaying(const JOYINFOEX& ji, const BYTE keyBuff[256], bool playable)
{
	if (playable)
	{
		if (joyID != -1)
		{
			// wXpos �̒l�͈̔͂�0�`65535�܂ŁB�����l��32500�t�߁i�덷����j
			int jx = (ji.dwXpos >> 8) - 127;
			int jy = (ji.dwYpos >> 8) - 127;
			int jz = (ji.dwZpos >> 8) - 127;

			// jx�i���E�j�͕ω����Ă��邩�H
			// �덷���z�����邽�߁A���l���傫����(��������)�𔻒肷��B
			if (abs(jx) > 10)
			{
				playerX += jx / 15;
			}
			if (abs(jy) > 10)
			{
				playerY += jy / 15;
			}
		}

		if (keyBuff[VK_LEFT] & 0x80)
		{
			playerRoll = 1; // ���X��.
			playerX -= 5;
		}
		if (keyBuff[VK_RIGHT] & 0x80)
		{

			playerRoll = 2; // �E�X��.
			playerX += 5;
		}
		if (keyBuff[VK_UP] & 0x80)
		{
			playerY -= 5;
		}
		if (keyBuff[VK_DOWN] & 0x80)
		{
			playerY += 5;
		}


		// ��񉟂��œ��삷��.
		// LeftCtrl�L�[��������Ă���
		if (keyBuff[VK_LCONTROL]) {
			// �L�[�����������ꂽ��.
			if (oneShotInput == 1) {
				oneShotInput = 0;
				isBulletSwitch = !isBulletSwitch; // �e�ۃ��[�h���X�C�b�`.
				pSe[SE_PLAYER_BULLET_SWITCH]->Play();// �v���C�J�nSE��炷.
			}
		}
		else {
			oneShotInput = 1;// ���Z�b�g.
		}

		// �{�^����������Ă���΁A�e�۔���.
		if ((buttons & JOY_BUTTON1) && (nextFireFrame <= 0))
		{
				if (isBulletSwitch) {
					// �e�ې���.
					BulletOne* pBlt = pPlayerBullet->Next();
					pBlt->Init(playerX, playerY, 0.0f, -20.0f);
					// �ˏo��AN�J�E���g(1�J�E���g��1/60sec)�͔��˕s�\��.
					nextFireFrame = 5;
				}
				else {
					// �U�e����
					for (int i = 0; i < 3; i++) {
						BulletOne* pBlt = pPlayerBullet->Next();
						pBlt->Init(playerX, playerY, (i - 1) * 2.0f, -20.0f);
					}

					// �ˏo��AN�J�E���g(1�J�E���g��1/60sec)�͔��˕s�\��.
					nextFireFrame = 10;
				}
				pSe[SE_BEAM]->Play();// ���ˉ���炷.
		}
		// ���ˉ\�ɂȂ�܂ł̃^�C�����O���c���Ă��邩�ǂ���.
		if (nextFireFrame > 0)
		{
			nextFireFrame--;
		}
	}

	// �X�e�[�W�f�[�^��i�߂�i15�t���[�����ɃX�e�[�W�f�[�^���`�F�b�N����j.
	if (gameTimer % 15 == 0)
	{
		if (!pStage->Update())
		{
			// BGM���~.
			GetAppInst()->StopAllBGM();

			// �Ō�ɓ��B�����̂ŃX�e�[�W��ύX����.
			gameState = GAME_STATE_LOAD;
		}
	}

	// �G����̍U��.
	pEnemyMgr->Fire();

	// �e�ۂƓG���X�V�A���������蔲���΍�̂��ߎ���������.
	for (int i = 0; i < 10; i++)
	{
		pPlayerBullet->Update(1.0f / 10);
		pEnemyBullet->Update(1.0f / 10);
		pEnemyMgr->Update(1.0f / 10);

		// �e�ۂƓG�̏Փ˂𔻒�.
		pPlayerBullet->Collision();

		// �G�̒e�ۂƃv���C���[�̏Փ�.
		if (pEnemyBullet->CollisionPlayer(playerX, playerY, 100))
		{
			if (playerPower > 0)
			{
				playerPower--;			// �ϋv�͌���.
				pSe[SE_HIDAN]->Play();	// HIDAN SE��炷.
			}
		}
	}
}

// �X�v���C�g��`��.
// return ���������G���[.
HRESULT App::DrawData()
{
	D3DXVECTOR3 cnt(0, 0, 0);
	D3DXVECTOR3 pos(0, 0, 0);

	D3DCOLOR rgb = D3DCOLOR_XRGB(10, 10, 80);
	// ��ʑS�̂�����.
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0);

	// �`����J�n�i�V�[���`��̊J�n�j.
	pDevice->BeginScene();

	// �X�v���C�g�̕`����J�n.
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// �w�i�`��.
	cnt.x = 0; cnt.y = 0;
	pos.x = 0;
	pos.y = stageTexPosY1;
	pSprite->Draw(pStageTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);

	// �w�i�`��.
	pos.y = stageTexPosY2;
	pSprite->Draw(pStageTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);

	// �Q�[���̏�Ԃɂ��`�敪��.
	switch (gameState)
	{
	break;
	case GAME_STATE_READY:
	{
		cnt.x = 0; cnt.y = 0;
		pos.x = 0; pos.y = 0;

		pSprite->Draw(pTitleBGTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);

		cnt.x = 128; cnt.y = 32;
		pos.x = WIDTH / 2; pos.y = HEIGHT / 2;

		if (stageCount == 0) {
			if (stateTimer % 30 < 20)
			{
				pSprite->Draw(pTitleTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
			}
		}
		else {
			if (stateTimer % 30 < 20)
			{
				pSprite->Draw(pReadyTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
			}
		}
	}
	break;

	case GAME_STATE_PLAYING:
	{
		if (stateTimer < 90 && stateTimer % 5 < 3)
		{
			// STAGE1...
			cnt.x = 128; cnt.y = 64;
			pos.x = WIDTH / 2; pos.y = HEIGHT / 2;
			if (stageCount == 1) {
				pSprite->Draw(pStage1Tex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
			}
			else if (stageCount == 2) {
				pSprite->Draw(pStage2Tex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
			}
			else if (stageCount == 3) {
				pSprite->Draw(pStage3Tex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
			}

		}

		// �G�ƒe�ۂ̕\��.
		DrawDataPlaying();

		// �v���C���[�@�i��e�o�[�W�����j�̕`��.
		cnt.x = 16; cnt.y = 16;
		pos.x = playerX;
		pos.y = playerY;
		pSprite->Draw(pPlayerTex->GetTexture(), &g_PlayerImageArea[playerRoll], &cnt, &pos, 0xFFFFFFFF);

		playerRoll = 0; // ���@�̌X�������Z�b�g.

		stageTexPosY1++;
		stageTexPosY2++;
		if (stageTexPosY1 > HEIGHT) {
			stageTexPosY1 = -720;
		}

		if (stageTexPosY2 > HEIGHT) {
			stageTexPosY2 = -720;
		}
	}
	break;

	case GAME_STATE_DEAD:
	{
		// �G�ƒe�ۂ̕\��.
		DrawDataPlaying();

		// �v���C���[�@�i��e�o�[�W�����j�̕`��.
		cnt.x = 16; cnt.y = 16;
		pos.x = playerX;
		pos.y = playerY;
		pSprite->Draw(pPlayerTex->GetTexture(), &g_PlayerImageArea[3], &cnt, &pos, 0xFFFFFFFF);

		// GAME OVER...
		if (stateTimer < 120 && stateTimer % 30 < 20)
		{
			cnt.x = 128; cnt.y = 64;
			pos.x = WIDTH / 2; pos.y = HEIGHT / 2;
			pSprite->Draw(pGameOverTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
		}
	}
	break;
	
	case GAME_STATE_CLEAR:
	{
		cnt.x = 128; cnt.y = 32;
		pos.x = WIDTH / 2; pos.y = HEIGHT / 2;
		if (stateTimer % 30 < 20)
		{
			pSprite->Draw(pClearTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
		}
	}
	break;
	}

	// �X�R�A�̕\��.
	float x = 0;
	float y = 10.0f;
	pScoreDisp->Draw(pSprite, score, 1000, x, y);

	// �c��p���[�̕\��.
	x += WIDTH - (64 * 3);
	pScoreDisp->Draw(pSprite, playerPower, 100, x, y);

	// �X�v���C�g�̕`����I���A�V�[���̕`����I��.
	pSprite->End();

	// �f�o�b�O�p�̏��\��.
#if defined(_DEBUG)

	LONGLONG ms;
	UINT fps;
	CTimer::GetTimer()->GetPassTime(&ms, &fps);
	CTimer::GetTimer()->StartTimer();

	char txt[256] = { 0 };

	sprintf_s(txt, "microSeconds: %llu, FPS:%d\n", ms, fps);
	pFont->DrawBitmapText(txt, 20, 660, 8, 0xFFFFFFFF);

	sprintf_s(txt, "game: %04d state: %04d pad %d:(%04.1f, %03.1f) btn:%04X  line=%03d  power=%d",
		gameTimer, stateTimer, joyID, playerX, playerY,
		buttons, pStage->GetCurrent(), playerPower);

	pFont->DrawBitmapText(txt, 20, 680, 8, 0xFFFFFFFF);

	sprintf_s(txt, "bullet:%s", pPlayerBullet->GetText());
	pFont->DrawBitmapText(txt, 20, 700, 8, 0xFFFFFFFF);

	sprintf_s(txt, "enemies:%s", pEnemyMgr->GetText());
	pFont->DrawBitmapText(txt, 20, 720, 8, 0xFFFFFFFF);

#endif

	pDevice->EndScene();

	// ���ۂɉ�ʂɕ\���B�o�b�N�o�b�t�@����t�����g�o�b�t�@�ւ̓]��.
	// �f�o�C�X�������̃t���O�w��ɂ��A������VSYNC��҂�.
	pDevice->Present(NULL, NULL, NULL, NULL);

	return D3D_OK;
}

// �G�ƒe�ۂ̕\��.
void App::DrawDataPlaying()
{
	pEnemyMgr->Show();
	pPlayerBullet->Draw(pBulletTex);
	pEnemyBullet->Draw(pEnemyBltTex);
}

// ���_�����Z.
void App::AddScore(int add)
{
	score += add;
}

void App::PlayBGM(SoundBGM bgm, bool loop)
{
	assert(BGM_BEGIN <= bgm && bgm < BGM_COUNT);
	if (BGM_BEGIN <= bgm && bgm < BGM_COUNT) {
		assert(pBgm[bgm] != NULL);
		if (pBgm[bgm] != NULL) {
			pBgm[bgm]->Play(loop);
		}
	}
}

void App::PlaySE(SoundSE se)
{
	assert(SE_BEGIN <= se && se < SE_COUNT);
	if (SE_BEGIN <= se && se < SE_COUNT) {
		pSe[se]->Play();
	}
}

// �w��̃X�v���C�g��`��.
void App::DrawSprite(MyTexture* pTex, D3DXVECTOR3* pCenter, D3DXVECTOR3* pPosition)
{
	pSprite->Draw(pTex->GetTexture(), NULL, pCenter, pPosition, 0xFFFFFFFF);
}

// �w��̃X�v���C�g��`��iUV���W�w��t�j.
void App::DrawSprite(MyTexture* pTex, const RECT* pcRect, D3DXVECTOR3* pCenter, D3DXVECTOR3* pPosition)
{
	pSprite->Draw(pTex->GetTexture(), pcRect, pCenter, pPosition, 0xFFFFFFFF);
}

// �G�e�ۂ̋󂢂Ă���Ƃ���𓾂�.
BulletOne* App::GetNextEnemyBlt()
{
	assert(pEnemyBullet != NULL);
	if (pEnemyBullet != NULL) {
		return pEnemyBullet->Next();
	}
	return NULL;
}

void App::CreateEnemyA(float x)
{
	EnemyAAA* pEnemyA = new EnemyAAA(pEnemyTexA);
	pEnemyA->Init(x, 0.0f, 0.0f, 5.0f);
	pEnemyMgr->Add(pEnemyA);
}

void App::CreateEnemyB(float x)
{
	EnemyBBB* pEnemyB = new EnemyBBB(pEnemyTexB);
	pEnemyB->Init(x, 0.0f, 0.0f, 0.0f);
	pEnemyMgr->Add(pEnemyB);
}

void App::CreateEnemyC(float x)
{
	EnemyCCC* pEnemyC = new EnemyCCC(pEnemyTexC, x, g_ptn2);
	pEnemyC->Init(x, 0.0f, 0.0f, 0.0f);
	pEnemyMgr->Add(pEnemyC);
}

void App::CreateEnemyD(float x)
{
	EnemyDDD* pEnemyD = new EnemyDDD(pEnemyTexD, x) ;
	pEnemyD->Init(x, 0.0f, 0.0f, 6.0f);
	pEnemyMgr->Add(pEnemyD);
}

void App::CreateEnemyE(float x)
{
	EnemyEEE* pEnemyE = new EnemyEEE(pEnemyTexE, x, g_ptn3);
	pEnemyE->Init(x, 0.0f, 0.0f, 0.0f);
	pEnemyMgr->Add(pEnemyE);
}

void App::CreateEnemyF(float x)
{
	EnemyFFF* pEnemyF = new EnemyFFF(pEnemyTexF, x, g_ptn4);
	pEnemyF->Init(x, 0.0f, 0.0f, 0.0f);
	pEnemyMgr->Add(pEnemyF);
}