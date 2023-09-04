#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <assert.h>
#include "mySound.h"
#include "myFile.h"

// エラーの有無を確認し、エラーがある場合はメッセージを表示してプログラムを終了する.
// hr	DirectSound関数の戻り値.
// msg	表示したい文字列.
static void DS_Err(HRESULT hr, LPCTSTR msg)
{
	if (FAILED(hr))
	{
		MessageBox(NULL, msg, _T("DirectSound"), MB_OK);
		exit(1);
	}
}

////////////////////////// MySound //////////////////////////
// staticなメンバ変数はクラス外で実体の定義が必要.
// staticなメンバ変数はオブジェクト間で共有される.
IDirectSound* MySound::sm_pDS = NULL;		// クラス変数の実体を定義.
MySound* MySound::sm_pStack[MAX_SOUND] = { NULL };
int MySound::sm_maxSound = 0;

MySound* MySound::LoadSound(const TCHAR* pSndFile)
{
	if (sm_pDS == NULL)
	{
		return NULL;
	}

	if (sm_maxSound >= MAX_SOUND) {
		return NULL;// バッファオーバーで再生不可.
	}

	// waveファイルからデータを読み込む.
	WAVEFORMATEX fmt;
	int dataSize;
	void* pWaveData = WaveRead(pSndFile, &dataSize, &fmt);

	// nullでなければ読み込み成功、DirectSoundBufferを作成する.
	if (pWaveData)
	{
		MySound* pObj = new MySound;
		sm_pStack[sm_maxSound++] = pObj;
		pObj->m_pSnd = MakeDSB(&fmt, pWaveData, dataSize);
		WaveFree(pWaveData);
		return pObj;
	}

	// 読み込み失敗.
	return NULL;
}

MySound::~MySound()
{
	if (m_pSnd)
	{
		m_pSnd->Release();
	}
}

// 再生。現在再生中の音は止めて、先頭から再生しなおす.
// loopフラグをtrueにすると、ループ再生となる.
void MySound::Play(bool loop)
{
	if (m_pSnd)
	{
		m_pSnd->Stop();
		m_pSnd->SetCurrentPosition(0);
		m_pSnd->Play(0, 0, (loop ? DSBPLAY_LOOPING : 0));
	}
}

// 停止。強制的に停止する.
void MySound::Stop()
{
	m_pSnd->Stop();
}

void MySound::Fadeout()
{
	LONG volume;
	m_pSnd->GetVolume(&volume);

	m_targetVol = DSBVOLUME_MIN;
//	m_fadeCnt = 30;// 30F(0.5秒)でボリュームを0にする.
	m_fadeCnt = 60;// 30Fだとフェードアウトが分かりにくいので
	m_changeVol = (m_targetVol - volume) / m_fadeCnt;
}

void MySound::UpdateSound()
{
	if (m_fadeCnt > 0) {
		m_fadeCnt--;
		if (m_fadeCnt == 0) {
			m_currentVol = m_targetVol;
		}
		else {
			m_currentVol += m_changeVol;
		}

		DWORD dwStatus;
		HRESULT ret = m_pSnd->GetStatus(&dwStatus);
		assert(ret == DS_OK);
		if (dwStatus & DSBSTATUS_PLAYING) {
			m_pSnd->SetVolume((INT)m_currentVol);
		}

		if (m_currentVol <= DSBVOLUME_MIN) {
			m_pSnd->Stop();

			// 次の再生に備えるためにMAX音量に戻す.
			m_currentVol = DSBVOLUME_MAX;
			m_pSnd->SetVolume((INT)m_currentVol);
		}
		printf("m_currentVol = %f\n", m_currentVol);
	}
}

// 全MySoundオブジェクトを破棄する.
void MySound::ReleaseAll()
{
	for (int i = 0; i < sm_maxSound; i++)
	{
		if (sm_pStack[i])
		{
			delete sm_pStack[i];
			sm_pStack[i] = NULL;
		}
	}
	sm_maxSound = 0;
	sm_pDS->Release();
	sm_pDS = NULL;
}

// DirectSoundを初期化する.
// hWnd		ウィンドウハンドル。協調レベルの設定に必須.
// return	DirectSoundオブジェクト.
HRESULT MySound::InitDirectSound(HWND hWnd)
{
	HRESULT hr = S_OK;
	if (sm_pDS == NULL) {
		// DirectSoundの生成。デフォルトサウンドデバイスを指定する(第一引数NULL）.
		hr = DirectSoundCreate(NULL, &sm_pDS, NULL);
		DS_Err(hr, _T("DirectSoundの作成に失敗"));

		// 協調レベルを設定する。DSSCL_PRIORITYが推奨されている.
		hr = sm_pDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
		DS_Err(hr, _T("SetCooperativeLevelに失敗。他のアプリが占有しています"));
	}
	return hr;
}

// DirectSoundBufferを作成する.
// pDS		DirectSoundオブジェクト.
// fmt		WAVEフォーマット.
// data		DirectSoundBufferに転送する音声データ。PCM WAVE形式.
// dataSize	dataのバイト数.
// return	DirectSoundBufferオブジェクト.
IDirectSoundBuffer* MySound::MakeDSB(WAVEFORMATEX* pFmt, void* pData, int dataSize)
{
	HRESULT hr = S_OK;

	// セカンダリバッファ（waveデータを保持しておくバッファ）の作成.
	// descはバッファについて説明する変数.
	// なお、プライマリバッファは音声を再生するためのバッファであり.
	// DirectSoundオブジェクト作成時に自動的に作成されている.
	DSBUFFERDESC desc;
	ZeroMemory(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = 0;
	desc.dwFlags |= DSBCAPS_GETCURRENTPOSITION2;// 再生位置取得可能.
	desc.dwFlags |= DSBCAPS_CTRLVOLUME;		// 音量変更可能.
	desc.dwFlags |= DSBCAPS_CTRLPAN;		// パン変更可能.
	desc.dwFlags |= DSBCAPS_GLOBALFOCUS;	// 常に再生可能.
	desc.dwFlags |= DSBCAPS_LOCDEFER;		// バッファをリソースに割り当てる.
	desc.dwBufferBytes = dataSize;			// データサイズ（バイト）.
	desc.lpwfxFormat = pFmt;				// waveフォーマット.

	// サウンドバッファを作成する。作成したバッファは pDSB に格納される.
	IDirectSoundBuffer* pDSB = NULL;
	hr = sm_pDS->CreateSoundBuffer(&desc, &pDSB, NULL);
	DS_Err(hr, _T("サウンドバッファの作成に失敗"));

	// waveデータの転送.
	// サウンドバッファに対して、読み込んだデータをコピーする.
	LPVOID ptr1, ptr2;
	DWORD bytes1, bytes2;

	// まずバッファをロックし、メモリアドレスを取得する.
	// ここでコピー先のメモリアドレスが２つになる場合もある（らしい）.
	hr = pDSB->Lock(0, dataSize, &ptr1, &bytes1, &ptr2, &bytes2, 0);
	DS_Err(hr, _T("サウンドバッファのロックに失敗"));

	// ptr1とptr2は別のメモリ領域なので、それぞれにデータをコピーする.
	if (ptr1) CopyMemory(ptr1, pData, bytes1);
	// ptr1にコピーしきれない量をptr2にコピーする.
	if (ptr2) CopyMemory(ptr2, (BYTE*)pData + bytes1, bytes2);

	// アンロックする.
	// サウンドバッファのロック～アンロックは短時間で行う必要がある.
	pDSB->Unlock(ptr1, bytes1, ptr2, bytes2);

	// 作成したDSBを返す.
	return pDSB;
}

// waveファイル読み込みルーチン.
typedef struct
{
	char id[4];
	unsigned long size;
} CHK_HD;

typedef struct
{
	char id[4];
} WAVE_HD;

void* MySound::WaveRead(LPCTSTR pFname, int* pDataSize, WAVEFORMATEX* pFmt)
{
	FILE* fp = OpenFile(pFname, _T("rb"));
	if (fp == NULL)
	{
		MessageBox(NULL, _T("WAVEファイルが開けません"), pFname, MB_OK);
		return NULL;
	}

	// ヘッダチャンクの読みとり。ヘッダチャンクは常に'RIFF'.
	CHK_HD hd;
	fread(&hd, sizeof(hd), 1, fp);
	if (strncmp(hd.id, "RIFF", 4))
	{
		fprintf(stderr, "not RIFF format.\n");
		return NULL;
	}

	// WAVEチャンクの読みとり.
	WAVE_HD whd;
	fread(&whd, sizeof(whd), 1, fp);
	if (strncmp(whd.id, "WAVE", 4))
	{
		fprintf(stderr, "not WAVE format.\n");
		return NULL;
	}

	char* pData = NULL;
	*pDataSize = 0;

	// 各チャンクの読みとり.
	bool bLoop = true;
	while (bLoop)
	{
		// ヘッダチャンクを読みとる.
		size_t readLen = fread(&hd, sizeof(hd), 1, fp);
		if (readLen == 0)
		{
			bLoop = false;
			continue;
		}
		if (!strncmp(hd.id, "data", 4))
		{
			// dataチャンク、hd.sizeにはdataチャンクのバイト数が入っている.
			pData = new char[hd.size];
			*pDataSize = hd.size;
			fread(pData, 1, hd.size, fp);
		}
		else if (!strncmp(hd.id, "fmt ", 4))
		{
			// fmtチャンク.
			char* pChank = new char[hd.size];
			fread(pChank, hd.size, 1, fp);
			printf("sizeof(fmt) = %zu, hd.size=%u\n", sizeof(WAVEFORMATEX), hd.size);

			size_t cpySize = (hd.size < sizeof(WAVEFORMATEX) ? hd.size : sizeof(WAVEFORMATEX));
			memcpy(pFmt, pChank, cpySize);

			printf("cbsize = %d\n", pFmt->cbSize);
			delete[] pChank;
		}
		else
		{
			 // これら以外のチャンクは読み飛ばす.
			fseek(fp, hd.size, SEEK_CUR);
		}
	}
	fclose(fp);
	return pData;// 配列を返す.
}

void MySound::WaveFree(void* pWaveData)
{
	// WaveRead呼び出しで受け取ったポインタを開放.
	if (pWaveData)
	{
		delete[] pWaveData;// WaveReadでは配列を確保しているので delete[] で開放.
	}
}
