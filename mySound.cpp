#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <assert.h>
#include "mySound.h"
#include "myFile.h"

// �G���[�̗L�����m�F���A�G���[������ꍇ�̓��b�Z�[�W��\�����ăv���O�������I������.
// hr	DirectSound�֐��̖߂�l.
// msg	�\��������������.
static void DS_Err(HRESULT hr, LPCTSTR msg)
{
	if (FAILED(hr))
	{
		MessageBox(NULL, msg, _T("DirectSound"), MB_OK);
		exit(1);
	}
}

////////////////////////// MySound //////////////////////////
// static�ȃ����o�ϐ��̓N���X�O�Ŏ��̂̒�`���K�v.
// static�ȃ����o�ϐ��̓I�u�W�F�N�g�Ԃŋ��L�����.
IDirectSound* MySound::sm_pDS = NULL;		// �N���X�ϐ��̎��̂��`.
MySound* MySound::sm_pStack[MAX_SOUND] = { NULL };
int MySound::sm_maxSound = 0;

MySound* MySound::LoadSound(const TCHAR* pSndFile)
{
	if (sm_pDS == NULL)
	{
		return NULL;
	}

	if (sm_maxSound >= MAX_SOUND) {
		return NULL;// �o�b�t�@�I�[�o�[�ōĐ��s��.
	}

	// wave�t�@�C������f�[�^��ǂݍ���.
	WAVEFORMATEX fmt;
	int dataSize;
	void* pWaveData = WaveRead(pSndFile, &dataSize, &fmt);

	// null�łȂ���Γǂݍ��ݐ����ADirectSoundBuffer���쐬����.
	if (pWaveData)
	{
		MySound* pObj = new MySound;
		sm_pStack[sm_maxSound++] = pObj;
		pObj->m_pSnd = MakeDSB(&fmt, pWaveData, dataSize);
		WaveFree(pWaveData);
		return pObj;
	}

	// �ǂݍ��ݎ��s.
	return NULL;
}

MySound::~MySound()
{
	if (m_pSnd)
	{
		m_pSnd->Release();
	}
}

// �Đ��B���ݍĐ����̉��͎~�߂āA�擪����Đ����Ȃ���.
// loop�t���O��true�ɂ���ƁA���[�v�Đ��ƂȂ�.
void MySound::Play(bool loop)
{
	if (m_pSnd)
	{
		m_pSnd->Stop();
		m_pSnd->SetCurrentPosition(0);
		m_pSnd->Play(0, 0, (loop ? DSBPLAY_LOOPING : 0));
	}
}

// ��~�B�����I�ɒ�~����.
void MySound::Stop()
{
	m_pSnd->Stop();
}

void MySound::Fadeout()
{
	LONG volume;
	m_pSnd->GetVolume(&volume);

	m_targetVol = DSBVOLUME_MIN;
//	m_fadeCnt = 30;// 30F(0.5�b)�Ń{�����[����0�ɂ���.
	m_fadeCnt = 60;// 30F���ƃt�F�[�h�A�E�g��������ɂ����̂�
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

			// ���̍Đ��ɔ����邽�߂�MAX���ʂɖ߂�.
			m_currentVol = DSBVOLUME_MAX;
			m_pSnd->SetVolume((INT)m_currentVol);
		}
		printf("m_currentVol = %f\n", m_currentVol);
	}
}

// �SMySound�I�u�W�F�N�g��j������.
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

// DirectSound������������.
// hWnd		�E�B���h�E�n���h���B�������x���̐ݒ�ɕK�{.
// return	DirectSound�I�u�W�F�N�g.
HRESULT MySound::InitDirectSound(HWND hWnd)
{
	HRESULT hr = S_OK;
	if (sm_pDS == NULL) {
		// DirectSound�̐����B�f�t�H���g�T�E���h�f�o�C�X���w�肷��(������NULL�j.
		hr = DirectSoundCreate(NULL, &sm_pDS, NULL);
		DS_Err(hr, _T("DirectSound�̍쐬�Ɏ��s"));

		// �������x����ݒ肷��BDSSCL_PRIORITY����������Ă���.
		hr = sm_pDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
		DS_Err(hr, _T("SetCooperativeLevel�Ɏ��s�B���̃A�v������L���Ă��܂�"));
	}
	return hr;
}

// DirectSoundBuffer���쐬����.
// pDS		DirectSound�I�u�W�F�N�g.
// fmt		WAVE�t�H�[�}�b�g.
// data		DirectSoundBuffer�ɓ]�����鉹���f�[�^�BPCM WAVE�`��.
// dataSize	data�̃o�C�g��.
// return	DirectSoundBuffer�I�u�W�F�N�g.
IDirectSoundBuffer* MySound::MakeDSB(WAVEFORMATEX* pFmt, void* pData, int dataSize)
{
	HRESULT hr = S_OK;

	// �Z�J���_���o�b�t�@�iwave�f�[�^��ێ����Ă����o�b�t�@�j�̍쐬.
	// desc�̓o�b�t�@�ɂ��Đ�������ϐ�.
	// �Ȃ��A�v���C�}���o�b�t�@�͉������Đ����邽�߂̃o�b�t�@�ł���.
	// DirectSound�I�u�W�F�N�g�쐬���Ɏ����I�ɍ쐬����Ă���.
	DSBUFFERDESC desc;
	ZeroMemory(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = 0;
	desc.dwFlags |= DSBCAPS_GETCURRENTPOSITION2;// �Đ��ʒu�擾�\.
	desc.dwFlags |= DSBCAPS_CTRLVOLUME;		// ���ʕύX�\.
	desc.dwFlags |= DSBCAPS_CTRLPAN;		// �p���ύX�\.
	desc.dwFlags |= DSBCAPS_GLOBALFOCUS;	// ��ɍĐ��\.
	desc.dwFlags |= DSBCAPS_LOCDEFER;		// �o�b�t�@�����\�[�X�Ɋ��蓖�Ă�.
	desc.dwBufferBytes = dataSize;			// �f�[�^�T�C�Y�i�o�C�g�j.
	desc.lpwfxFormat = pFmt;				// wave�t�H�[�}�b�g.

	// �T�E���h�o�b�t�@���쐬����B�쐬�����o�b�t�@�� pDSB �Ɋi�[�����.
	IDirectSoundBuffer* pDSB = NULL;
	hr = sm_pDS->CreateSoundBuffer(&desc, &pDSB, NULL);
	DS_Err(hr, _T("�T�E���h�o�b�t�@�̍쐬�Ɏ��s"));

	// wave�f�[�^�̓]��.
	// �T�E���h�o�b�t�@�ɑ΂��āA�ǂݍ��񂾃f�[�^���R�s�[����.
	LPVOID ptr1, ptr2;
	DWORD bytes1, bytes2;

	// �܂��o�b�t�@�����b�N���A�������A�h���X���擾����.
	// �����ŃR�s�[��̃������A�h���X���Q�ɂȂ�ꍇ������i�炵���j.
	hr = pDSB->Lock(0, dataSize, &ptr1, &bytes1, &ptr2, &bytes2, 0);
	DS_Err(hr, _T("�T�E���h�o�b�t�@�̃��b�N�Ɏ��s"));

	// ptr1��ptr2�͕ʂ̃������̈�Ȃ̂ŁA���ꂼ��Ƀf�[�^���R�s�[����.
	if (ptr1) CopyMemory(ptr1, pData, bytes1);
	// ptr1�ɃR�s�[������Ȃ��ʂ�ptr2�ɃR�s�[����.
	if (ptr2) CopyMemory(ptr2, (BYTE*)pData + bytes1, bytes2);

	// �A�����b�N����.
	// �T�E���h�o�b�t�@�̃��b�N�`�A�����b�N�͒Z���Ԃōs���K�v������.
	pDSB->Unlock(ptr1, bytes1, ptr2, bytes2);

	// �쐬����DSB��Ԃ�.
	return pDSB;
}

// wave�t�@�C���ǂݍ��݃��[�`��.
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
		MessageBox(NULL, _T("WAVE�t�@�C�����J���܂���"), pFname, MB_OK);
		return NULL;
	}

	// �w�b�_�`�����N�̓ǂ݂Ƃ�B�w�b�_�`�����N�͏��'RIFF'.
	CHK_HD hd;
	fread(&hd, sizeof(hd), 1, fp);
	if (strncmp(hd.id, "RIFF", 4))
	{
		fprintf(stderr, "not RIFF format.\n");
		return NULL;
	}

	// WAVE�`�����N�̓ǂ݂Ƃ�.
	WAVE_HD whd;
	fread(&whd, sizeof(whd), 1, fp);
	if (strncmp(whd.id, "WAVE", 4))
	{
		fprintf(stderr, "not WAVE format.\n");
		return NULL;
	}

	char* pData = NULL;
	*pDataSize = 0;

	// �e�`�����N�̓ǂ݂Ƃ�.
	bool bLoop = true;
	while (bLoop)
	{
		// �w�b�_�`�����N��ǂ݂Ƃ�.
		size_t readLen = fread(&hd, sizeof(hd), 1, fp);
		if (readLen == 0)
		{
			bLoop = false;
			continue;
		}
		if (!strncmp(hd.id, "data", 4))
		{
			// data�`�����N�Ahd.size�ɂ�data�`�����N�̃o�C�g���������Ă���.
			pData = new char[hd.size];
			*pDataSize = hd.size;
			fread(pData, 1, hd.size, fp);
		}
		else if (!strncmp(hd.id, "fmt ", 4))
		{
			// fmt�`�����N.
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
			 // �����ȊO�̃`�����N�͓ǂݔ�΂�.
			fseek(fp, hd.size, SEEK_CUR);
		}
	}
	fclose(fp);
	return pData;// �z���Ԃ�.
}

void MySound::WaveFree(void* pWaveData)
{
	// WaveRead�Ăяo���Ŏ󂯎�����|�C���^���J��.
	if (pWaveData)
	{
		delete[] pWaveData;// WaveRead�ł͔z����m�ۂ��Ă���̂� delete[] �ŊJ��.
	}
}
