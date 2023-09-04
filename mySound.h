/*
	mySound.h
*/
#pragma once

#include <dsound.h>

// 列挙型にSE使用時の名称を設定（配列管理なので数値だと分かりにくい）.
enum SoundSE
{
	SE_BEAM,		// 発射音.
	SE_TEKI_DEAD,	// 撃墜音.
	SE_HIDAN,		// 被弾音.
	SE_DEAD,		// 死亡音.
	SE_START,		// 開始音.

	SE_READYSTART,	// プレイスタート音.
	SE_PLAYER_BULLET_SWITCH, // 自機の弾丸モードスイッチ音.


	SE_COUNT,
	SE_BEGIN = 0,
};

enum SoundBGM
{
	BGM_0,			// レディ状態のBGM.
	BGM_1,			// プレイ中BGMその１.
	BGM_2,			// プレイ中BGMその２.

	BGM_COUNT,
	BGM_BEGIN = 0,
};

constexpr int MAX_SOUND = 256;

////////////////////////// MySound //////////////////////////
// サウンドを扱うためのクラス.
class MySound
{
private:
	IDirectSoundBuffer* m_pSnd;		// サウンドバッファ.
	float m_currentVol;				// 現在ボリューム.
	float m_targetVol;				// フェードイン／アウト用目標ボリューム.
	float m_changeVol;				// ボリューム変化量.
	INT m_fadeCnt;

	MySound()
	: m_pSnd(NULL)
	, m_currentVol(0)
	, m_targetVol(0)
	, m_changeVol(0)
	, m_fadeCnt(0)
	{}

	~MySound();

	static IDirectSound* sm_pDS;	// DirectSoundデバイス.
	static int sm_maxSound;			// 音の読み込み数.
	static MySound* sm_pStack[MAX_SOUND];

	static IDirectSoundBuffer* MakeDSB(WAVEFORMATEX* pFmt, void* pData, int dataSize);
	static void* WaveRead(LPCTSTR pFname, int* pDataSize, WAVEFORMATEX* pFmt);
	static void WaveFree(void* pData);

public:
	void Play(bool loop = false);
	void Stop();

	void Fadeout();
	void UpdateSound();

	// ダイレクトサウンドデバイスの準備.
	static HRESULT InitDirectSound(HWND hWnd);

	// wavファイルの読み込みはstaticメソッド経由で.
	static MySound* LoadSound(const TCHAR* fname);
	static void ReleaseAll();
};

