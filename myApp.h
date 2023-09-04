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


// ゲーム全体の状態遷移.
enum GameState
{
	GAME_STATE_NONE = -1,// 未初期化状態.

	GAME_STATE_LOAD, 	// ステージ読み込み.
	GAME_STATE_READY, 	// ゲーム開始待ち.
	GAME_STATE_PLAYING, // ゲーム進行中.
	GAME_STATE_CLEAR, 	// ステージクリア！.
	GAME_STATE_DEAD, 	// プレイヤーが撃破される.
};

// DataSet構造体をシングルトンモデルのAppクラスとして定義.
//class DataSet→Appに名称変更.
class App
{
private:
	static App sm_appData;	// このクラスのオブジェクト実体.
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
	// コンストラクタをprivate領域に配置して勝手なオブジェクト生成が出来ないようにする.
	App()						// コンストラクタ.
		: hInstance(NULL)			// インスタンスハンドル.
		, hWnd(NULL)				// 表示ウィンドウ.
		, pD3D(NULL)				// Direct3Dインスタンスオブジェクト.
		, pDevice(NULL)				// Direct3Dデバイスオブジェクト.
		, pSprite(NULL)				// スプライト.
		, pFont(NULL)				// ビットマップフォント.
		, pPlayerTex(NULL)			// 自機.
		, pBulletTex(NULL)			// 自機弾丸.
		, playerX(0.0f)				// 自機の現在x位置.
		, playerY(0.0f)				// 自機の現在y位置.
		, nextFireFrame(0)			// 自機が次に弾を撃てるまでの時間（フレーム）.
		, playerRoll(0)				// 自機の傾き.
		, oneShotInput(0)			// 長押ししても大丈夫なように.
		, isBulletSwitch(false)		// 自機の弾丸のモードがスイッチされているか.
		, buttons(0)				// ボタンの情報.
		, pPlayerBullet(NULL)		// 自機の弾丸.
		, pEnemyMgr(NULL)			// 敵の配列.
		, pEnemyTexA(NULL)			// 敵Aのテクスチャ.
		, pEnemyTexB(NULL)			// 敵Bのテクスチャ.
		, pEnemyTexC(NULL)			// 敵Cのテクスチャ.
		, pEnemyTexD(NULL)			// 敵Dのテクスチャ.
		, pEnemyTexE(NULL)			// 敵Eのテクスチャ.
		, pEnemyTexF(NULL)			// 敵Fのテクスチャ.
		, pScoreDisp(NULL)			// スコア表示.
		, score(0)					// スコア.
		, playerPower(0)			// 自機の耐久力.
		, pEnemyBltTex(NULL)		// 敵弾丸テクスチャ.
		, pEnemyBullet(NULL)		// 敵の弾丸.
		, joyID(-1)					// ゲームコントローラのID.
		, gameTimer(0)				// 全体のタイマー.
		, pStage(NULL)				// ゲームステージ.
		, gameState(GAME_STATE_NONE)// ゲーム全体の状態.
		, stateTimer(0)				// 状態遷移用タイマー.
		, stageTexPosY1(0)			// 1枚目のステージ背景のy座標.
		, stageTexPosY2(-720)		// 2枚目のステージ背景のy座標.
		, stageCount(0)
		, pReadyTex(NULL)			// READY.
		, pStage1Tex(NULL)			// STAGE1.
		, pGameOverTex(NULL)		// GAMEOVER.
		, pStageTex(NULL)			// ステージの背景テクスチャ.
		, pTitleTex(NULL)			// タイトルのテキストテクスチャ.
		, pTitleBGTex(NULL)			// タイトルの背景テクスチャ.
	{
		memset(&d3dpp, 0, sizeof(d3dpp));	// D3Dデバイス作成時のパラメータ.
		memset(pBgm, NULL, sizeof(pBgm));	// BGM.
		memset(pSe, NULL, sizeof(pSe));		// SE.
	}

public:
	EnemyManager* GetEnemyMgr() { return pEnemyMgr; }
	void GetPlayerPos(float& x, float& y) { x = playerX; y = playerY; }
	GameState GetGameState() { return gameState; }

	void AddScore(int add);	// 得点を加算.
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
	HINSTANCE hInstance;			// インスタンスハンドル.
	HWND hWnd;						// 表示ウィンドウ.

	IDirect3D9* pD3D;				// Direct3Dインスタンスオブジェクト.
	D3DPRESENT_PARAMETERS d3dpp;	// デバイス作成時のパラメータ.

	IDirect3DDevice9* pDevice;		// Direct3Dデバイスオブジェクト.
	ID3DXSprite* pSprite;			// スプライト.
	MyBitmapFont* pFont;			// ビットマップフォント.

	MyTexture* pPlayerTex;			// 自機.
	MyTexture* pBulletTex;			// 自機弾丸.

	float playerX, playerY;			// 自機の現在位置.
	int nextFireFrame;				// 自機が次に弾を撃てるまでの時間（フレーム）.
	int playerRoll;					// 自機の傾き.(0:傾きなし、1:左、2:右).
	int oneShotInput;				// キー長押ししても大丈夫なように.
	bool isBulletSwitch;			// 自機弾丸のモードがスイッチされているか.
	UINT buttons;					// ボタンの情報.
	BulletBuffer* pPlayerBullet;	// 自機の弾丸.

	EnemyManager* pEnemyMgr;		// 敵の配列.
	MyTexture* pEnemyTexA;			// 敵Aのテクスチャ.
	MyTexture* pEnemyTexB;			// 敵Bのテクスチャ.
	MyTexture* pEnemyTexC;			// 敵Cのテクスチャ.
	MyTexture* pEnemyTexD;			// 敵Dのテクスチャ.
	MyTexture* pEnemyTexE;			// 敵Eのテクスチャ.
	MyTexture* pEnemyTexF;			// 敵Fのテクスチャ.

	NumberDisp* pScoreDisp;			// スコア表示.
	int score;						// スコア.
	int playerPower;				// 自機の耐久力.

	MyTexture* pEnemyBltTex;		// 敵弾丸テクスチャ.
	BulletBuffer* pEnemyBullet;		// 敵の弾丸.

	UINT joyID;						// ゲームコントローラのID.
	int gameTimer;					// 全体のタイマー.

	MySound* pBgm[BGM_COUNT];		// BGM.
	MySound* pSe[SE_COUNT];			// SE.

	GameStage* pStage;				// ゲームステージ.
	GameState gameState;			// ゲーム全体の状態.
	int stateTimer;					// 状態遷移用タイマー.
	int stageCount;					// 読み込むステージの番号.
	float stageTexPosY1;			// 背景のy座標1.
	float stageTexPosY2;			// 背景のy座標2.
	MyTexture* pReadyTex;			// READY.
	MyTexture* pStage1Tex;			// STAGE1.
	MyTexture* pStage2Tex;			// STAGE2.
	MyTexture* pStage3Tex;			// STAGE3.
	MyTexture* pGameOverTex;		// GAMEOVER.
	MyTexture* pTitleTex;			// TITLE.
	MyTexture* pTitleBGTex;			// TITLEの背景.
	MyTexture* pClearTex;			// CLEAR.
	MyTexture* pStageTex;			// ステージの背景テクスチャ.

};

inline App* GetAppInst() { return App::GetInstance(); }
