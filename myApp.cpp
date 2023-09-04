/*
	myApp.cpp
	Kobayashi Atsuki.
	配布コードをそのまま使用.

	必要な要素をヘッダーファイルに記述し、それらをインクルードすることで、
	引数のたらい回しをなくし、コードの可読性を高める。
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

// ウィンドウタイトルバーに表示されるバージョン名.
#define WDATE(x, y) _T(x y)

// プレイヤーテクスチャの表示領域のための配列.
static const RECT g_PlayerImageArea[] = {
	{ 0,   0, 31,  32 },
	{ 0,  32, 31,  64 },
	{ 0,  64, 31,  96 },
	{ 0,  96, 31, 128 },
};

// イベント処理コールバック（ウィンドウプロシージャ）.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// イベントの種類に応じて、switch文にて処理を切り分ける.
	switch (uMsg)
	{
	case WM_KEYDOWN: {
		// ESCキーが押下されたら終了.
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		break;
	}

	case WM_CLOSE: {
		// 終了通知(CLOSEボタンが押された場合など)が届いた場合.
		// プログラムを終了させるため、イベントループに0を通知する.
		// この結果、GetMessageの戻り値は0になる.
		PostQuitMessage(0);
		break;
	}

	default:
		break;
	}

	// デフォルトのウィンドウイベント処理.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// エラー発生時のHRESULTを文字列に変換するための補助関数.
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
	case MMSYSERR_NODRIVER: return _T("ゲームコントローラのドライバがありません");
	case MMSYSERR_INVALPARAM: return _T("不正なパラメータ指定です");
	case JOYERR_UNPLUGGED: return _T("ゲームコントローラが接続されていません");
	case JOYERR_PARMS: return _T("ゲームコントローラの指定が不正です");
	default: return _T("謎エラー");
	}
}

////////////////////////////////////////////////////////////////////////////////
// アプリケーションデータ実体.
App App::sm_appData;

// アプリケーションを初期化する.
bool App::InitApp()
{
	// このプログラムが実行されるときのインスタンスハンドルを取得.
	hInstance = GetModuleHandle(NULL);

	HRESULT hr = E_FAIL;

	// 有効なジョイスティックを探す.
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
			MessageBox(NULL, _T("ジョイスティックが繋がっていません\nキーボードで操作します"), _T("JOYSTICK"), MB_OK);
			joyID = -1;
		}
	}

	hWnd = InitWindow();			// ウィンドウを作成する.

	// Direct3Dを初期化する.
	hr = GetAppInst()->InitDirect3D();
	if (FAILED(hr))
	{
		MessageBox(NULL, D3DErrStr(hr), _T("Direct3D初期化失敗"), MB_OK);
		return false;
	}

	// スプライトの作成.
	hr = D3DXCreateSprite(pDevice, &(pSprite));
	if (FAILED(hr))
	{
		MessageBox(NULL, D3DErrStr(hr), _T("スプライト作成失敗"), MB_OK);
		return false;
	}

	// とりあえず初期値を入れておく.
	playerX = (WIDTH / 2);
	playerY = (HEIGHT - 100);
	playerPower = 100;
	gameState = GAME_STATE_LOAD;

	// 弾丸管理クラスのオブジェクトを生成 & 初期化.
	pPlayerBullet = new BulletBuffer(50);
	if (pPlayerBullet == NULL || !pPlayerBullet->Init())
	{
		return false;
	}

	// 弾丸管理クラス（敵用）のオブジェクトを生成&初期化.
	pEnemyBullet = new BulletBuffer(100);
	if (pEnemyBullet == NULL || !pEnemyBullet->Init())
	{
		return false;
	}

	// 数字をテクスチャで表示するためのオブジェクトを生成.
	pScoreDisp = new NumberDisp();
	if (pScoreDisp == NULL)
	{
		return false;
	}

	// テクスチャを読み込む。エラー時にはLoadTextureは例外を発生させる.
	// LoadTextureごとにif判定をするのは面倒なので、try～throw～catchを使う.
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

		// 数値表示用テクスチャの読み込み.
		MyTexture* pTex = MyTexture::LoadTexture(pDevice, _T("data/image/numbers.png"));
		// スコア表示に使うテクスチャの情報を設定.
		pScoreDisp->Init(pTex, 64, 64);// 64:数字１つ分のテクスチャサイズ.
	}
	// catch句はtryの直後に記述する.
	catch (HRESULT /*hr*/)
	{
		return false;
	}

	// 敵管理クラスのオブジェクトを生成.
	pEnemyMgr = new EnemyManager(70);
	if (pEnemyMgr == NULL || !pEnemyMgr->Init())
	{
		return false;
	}



	// サウンド関連準備.
	{
		// DirectSoundの初期化.
		hr = MySound::InitDirectSound(hWnd);
		if (FAILED(hr))
		{
			MessageBox(NULL, D3DErrStr(hr), _T("ダイレクトサウンド作成失敗"), MB_OK);
			return false;
		}

		// BGMの読み込み.
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

		// SEの読み込み.
		static const TCHAR* pcszSeFileName[SE_COUNT] = {
			_T("data/se/Space Gun 01.wav"), // 発射音.
			_T("data/se/bakuha.WAV"),		// 撃墜音.
			_T("data/se/ganx.WAV"),			// 被弾音.
			_T("data/se/daibakuhatsu.wav"),	// 死亡音.
			_T("data/se/S02_AO02.WAV"),		// 開始音.
			_T("data/se/ReadyStart.WAV"),	// プレイスタート音.
			_T("data/se/BulletSwitch.WAV")	// 弾丸モードスイッチ音.
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

// メインループ.
void App::MainLoop()
{
	ShowWindow(hWnd, SW_SHOWNORMAL);	// 作成したウィンドウを表示する.
	CTimer::GetTimer()->StartTimer();

	// BGMを再生.
	pBgm[BGM_0]->Play(true);

	// イベントループ.
	// ブロック型関数GetMessageではなくノンブロック型関数のPeekMessageを使う.
	MSG msg;
	bool flag = 1;
	while (flag)
	{
		// メッセージがあるかどうか確認する.
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			// メッセージがあるので処理する.
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
			UpdateData();	// 位置の再計算.
			DrawData();		// 描画.
		}
		// Sleepなど行っていないが、DrawData関数内のpDevice->Presentが.
		// 画面更新待ちを行うので、結果的にVSyncごとにイベントループが回る.
	}
}

// 動的に確保したメモリを解放する.
void App::ReleaseDataSet()
{
	// サウンドの開放.
	MySound::ReleaseAll();

	// テクスチャの開放.
	MyTexture::ReleaseAll();

	// 動的オブジェクトの開放.
	if (pStage) { delete pStage; }
	if (pScoreDisp) { delete pScoreDisp; }
	if (pEnemyBullet) { delete pEnemyBullet; }
	if (pEnemyMgr) { delete pEnemyMgr; }
	if (pPlayerBullet) { delete pPlayerBullet; }
	if (pFont) { delete pFont; }

	// DirectXデバイスの開放.
	if (pSprite) { pSprite->Release(); }
	if (pDevice) { pDevice->Release(); }
	if (pD3D) { pD3D->Release(); }
}

// 全BGM停止.
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

// ステージ変わりなどの時に必要なデータだけをリセットする.
void App::ResetGameData()
{
	pEnemyMgr->ResetAll();
	pEnemyBullet->ResetAll();
	pPlayerBullet->ResetAll();

	// プレイヤーの初期値
	playerX = WIDTH / 2;
	playerY = HEIGHT - 100;
	playerPower = 100;
	nextFireFrame = 0;
	stateTimer = 0;
}

// Windowを作成する.
// return ウィンドウハンドル.
HWND App::InitWindow()
{
	// まずウィンドウクラスを登録する.
	// これはウィンドウ生成後の処理の仕方をWindowsに教えるためである.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));		// 変数wcをゼロクリアする.
	wc.cbSize = sizeof(WNDCLASSEX);				// この構造体の大きさを与える.
	wc.lpfnWndProc = (WNDPROC)WindowProc;		// ウィンドウプロシージャ登録.
	wc.hInstance = hInstance;			// インスタンスハンドルを設定.
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);	// マウスカーソルの登録.
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);// 背景をGRAYに.
	wc.lpszClassName = _T("window01");			// クラス名、CreateWindowと一致させる.
	RegisterClassEx(&wc);						// 登録.

	RECT rc = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_tprintf(_T("rc: %d, %d -- %d, %d\n"), rc.left, rc.top, rc.right, rc.bottom);

	// ウィンドウを作成する.
	static const TCHAR* pVersion = WDATE(__DATE__, __TIME__);
	hWnd = CreateWindow(_T("window01"), pVersion, WS_OVERLAPPEDWINDOW, 0, 0, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

	return hWnd;
}

// Direct3Dを初期化する.
// return 発生したエラーまたはD3D_OK.
HRESULT App::InitDirect3D()
{
	HRESULT hr;

	// Direct3Dインスタンスオブジェクトを生成する.
	// D3D_SDK_VERSIONと、ランタイムバージョン番号が適切でないと、NULLが返る.
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	// NULLならランタイムが不適切.
	if (pD3D == NULL) return E_FAIL;

	// PRESENTパラメータをゼロクリアし、適切に初期化.
	ZeroMemory(&d3dpp, sizeof(d3dpp));
#ifdef FULLSCREEN
	d3dpp.Windowed = FALSE;
	//int fx = GetSystemMetrics(SM_CXSCREEN);
	//int fy = GetSystemMetrics(SM_CYSCREEN);
	int fx = WIDTH;
	int fy = HEIGHT;
	_tprintf(_T("フルスクリーン %d x %d\n"), fx, fy);
	d3dpp.BackBufferHeight = fy;
	d3dpp.BackBufferWidth = fx;
#else
	d3dpp.Windowed = TRUE;
#endif
	d3dpp.BackBufferHeight = HEIGHT;
	d3dpp.BackBufferWidth = WIDTH;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// バックバッファはRGBそれぞれ８ビットで.
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	// Present時に垂直同期に合わせる.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// D3Dデバイスオブジェクトの作成。HAL&HARD.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("HAL & HARD\n"));
		return hr;
	}
	// D3Dデバイスオブジェクトの作成。HAL&SOFT.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("HAL & SOFT\n"));
		return hr;
	}
	// D3Dデバイスオブジェクトの作成。REF&HARD.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("REF & HARD\n"));
		return hr;
	}
	// D3Dデバイスオブジェクトの作成。REF&SOFT.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("REF & SOFT\n"));
		return hr;
	}
	return hr;
}

// データをアップデートする.
void App::UpdateData()
{
	gameTimer++;
	stateTimer++;

	buttons = 0;
	JOYINFOEX ji = { 0 };
	if (joyID != -1)
	{
		// ジョイスティックの情報を得る.
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

	// ゲーム状態に応じた処理に変更.
	switch (gameState)
	{
	case GAME_STATE_LOAD:
	{
		GetAppInst()->ResetGameData();

		// 既存の読み込み済みのステージデータがあれば解放.
		if (pStage != NULL)
		{
			delete pStage;
		}

		stageCount++;

		pStage = new GameStage;

		// 状態を遷移.
		//	ステージ番号に応じてステージデータを読み込む.
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
		// Bボタン(CTRL-Key)だけが押されたらゲーム開始.
		if (buttons == JOY_BUTTON2)
		{
			gameState = GAME_STATE_PLAYING;
			stateTimer = 0;
			gameTimer = 0;
			pSe[SE_READYSTART]->Play();// プレイ開始SEを鳴らす.
		}
		break;
	}

	case GAME_STATE_PLAYING:
	{
		UpdatePlaying(ji, keyBuff, true);

		// プレイヤーのパワーがゼロになったら、ゲームオーバー.
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

// GAME_PLAYING状態の時のUpdate
// playableがtrueであれば自機操作
void App::UpdatePlaying(const JOYINFOEX& ji, const BYTE keyBuff[256], bool playable)
{
	if (playable)
	{
		if (joyID != -1)
		{
			// wXpos の値の範囲は0～65535まで。中央値が32500付近（誤差あり）
			int jx = (ji.dwXpos >> 8) - 127;
			int jy = (ji.dwYpos >> 8) - 127;
			int jz = (ji.dwZpos >> 8) - 127;

			// jx（左右）は変化しているか？
			// 誤差を吸収するため、一定値より大きいか(小さいか)を判定する。
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
			playerRoll = 1; // 左傾き.
			playerX -= 5;
		}
		if (keyBuff[VK_RIGHT] & 0x80)
		{

			playerRoll = 2; // 右傾き.
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


		// 二回押しで動作する.
		// LeftCtrlキーが押されている
		if (keyBuff[VK_LCONTROL]) {
			// キー押しが離されたか.
			if (oneShotInput == 1) {
				oneShotInput = 0;
				isBulletSwitch = !isBulletSwitch; // 弾丸モードをスイッチ.
				pSe[SE_PLAYER_BULLET_SWITCH]->Play();// プレイ開始SEを鳴らす.
			}
		}
		else {
			oneShotInput = 1;// リセット.
		}

		// ボタンが押されていれば、弾丸発射.
		if ((buttons & JOY_BUTTON1) && (nextFireFrame <= 0))
		{
				if (isBulletSwitch) {
					// 弾丸生成.
					BulletOne* pBlt = pPlayerBullet->Next();
					pBlt->Init(playerX, playerY, 0.0f, -20.0f);
					// 射出後、Nカウント(1カウント＝1/60sec)は発射不可能に.
					nextFireFrame = 5;
				}
				else {
					// 散弾生成
					for (int i = 0; i < 3; i++) {
						BulletOne* pBlt = pPlayerBullet->Next();
						pBlt->Init(playerX, playerY, (i - 1) * 2.0f, -20.0f);
					}

					// 射出後、Nカウント(1カウント＝1/60sec)は発射不可能に.
					nextFireFrame = 10;
				}
				pSe[SE_BEAM]->Play();// 発射音を鳴らす.
		}
		// 発射可能になるまでのタイムラグが残っているかどうか.
		if (nextFireFrame > 0)
		{
			nextFireFrame--;
		}
	}

	// ステージデータを進める（15フレーム毎にステージデータをチェックする）.
	if (gameTimer % 15 == 0)
	{
		if (!pStage->Update())
		{
			// BGMを停止.
			GetAppInst()->StopAllBGM();

			// 最後に到達したのでステージを変更する.
			gameState = GAME_STATE_LOAD;
		}
	}

	// 敵からの攻撃.
	pEnemyMgr->Fire();

	// 弾丸と敵を更新、ただしすり抜け対策のため時分割する.
	for (int i = 0; i < 10; i++)
	{
		pPlayerBullet->Update(1.0f / 10);
		pEnemyBullet->Update(1.0f / 10);
		pEnemyMgr->Update(1.0f / 10);

		// 弾丸と敵の衝突を判定.
		pPlayerBullet->Collision();

		// 敵の弾丸とプレイヤーの衝突.
		if (pEnemyBullet->CollisionPlayer(playerX, playerY, 100))
		{
			if (playerPower > 0)
			{
				playerPower--;			// 耐久力減少.
				pSe[SE_HIDAN]->Play();	// HIDAN SEを鳴らす.
			}
		}
	}
}

// スプライトを描画.
// return 発生したエラー.
HRESULT App::DrawData()
{
	D3DXVECTOR3 cnt(0, 0, 0);
	D3DXVECTOR3 pos(0, 0, 0);

	D3DCOLOR rgb = D3DCOLOR_XRGB(10, 10, 80);
	// 画面全体を消去.
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0);

	// 描画を開始（シーン描画の開始）.
	pDevice->BeginScene();

	// スプライトの描画を開始.
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// 背景描画.
	cnt.x = 0; cnt.y = 0;
	pos.x = 0;
	pos.y = stageTexPosY1;
	pSprite->Draw(pStageTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);

	// 背景描画.
	pos.y = stageTexPosY2;
	pSprite->Draw(pStageTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);

	// ゲームの状態による描画分け.
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

		// 敵と弾丸の表示.
		DrawDataPlaying();

		// プレイヤー機（被弾バージョン）の描画.
		cnt.x = 16; cnt.y = 16;
		pos.x = playerX;
		pos.y = playerY;
		pSprite->Draw(pPlayerTex->GetTexture(), &g_PlayerImageArea[playerRoll], &cnt, &pos, 0xFFFFFFFF);

		playerRoll = 0; // 自機の傾きをリセット.

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
		// 敵と弾丸の表示.
		DrawDataPlaying();

		// プレイヤー機（被弾バージョン）の描画.
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

	// スコアの表示.
	float x = 0;
	float y = 10.0f;
	pScoreDisp->Draw(pSprite, score, 1000, x, y);

	// 残りパワーの表示.
	x += WIDTH - (64 * 3);
	pScoreDisp->Draw(pSprite, playerPower, 100, x, y);

	// スプライトの描画を終了、シーンの描画を終了.
	pSprite->End();

	// デバッグ用の情報表示.
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

	// 実際に画面に表示。バックバッファからフロントバッファへの転送.
	// デバイス生成時のフラグ指定により、ここでVSYNCを待つ.
	pDevice->Present(NULL, NULL, NULL, NULL);

	return D3D_OK;
}

// 敵と弾丸の表示.
void App::DrawDataPlaying()
{
	pEnemyMgr->Show();
	pPlayerBullet->Draw(pBulletTex);
	pEnemyBullet->Draw(pEnemyBltTex);
}

// 得点を加算.
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

// 指定のスプライトを描画.
void App::DrawSprite(MyTexture* pTex, D3DXVECTOR3* pCenter, D3DXVECTOR3* pPosition)
{
	pSprite->Draw(pTex->GetTexture(), NULL, pCenter, pPosition, 0xFFFFFFFF);
}

// 指定のスプライトを描画（UV座標指定付）.
void App::DrawSprite(MyTexture* pTex, const RECT* pcRect, D3DXVECTOR3* pCenter, D3DXVECTOR3* pPosition)
{
	pSprite->Draw(pTex->GetTexture(), pcRect, pCenter, pPosition, 0xFFFFFFFF);
}

// 敵弾丸の空いているところを得る.
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