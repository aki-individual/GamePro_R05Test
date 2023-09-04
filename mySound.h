/*
	mySound.h
*/
#pragma once

#include <dsound.h>

// �񋓌^��SE�g�p���̖��̂�ݒ�i�z��Ǘ��Ȃ̂Ő��l���ƕ�����ɂ����j.
enum SoundSE
{
	SE_BEAM,		// ���ˉ�.
	SE_TEKI_DEAD,	// ���ĉ�.
	SE_HIDAN,		// ��e��.
	SE_DEAD,		// ���S��.
	SE_START,		// �J�n��.

	SE_READYSTART,	// �v���C�X�^�[�g��.
	SE_PLAYER_BULLET_SWITCH, // ���@�̒e�ۃ��[�h�X�C�b�`��.


	SE_COUNT,
	SE_BEGIN = 0,
};

enum SoundBGM
{
	BGM_0,			// ���f�B��Ԃ�BGM.
	BGM_1,			// �v���C��BGM���̂P.
	BGM_2,			// �v���C��BGM���̂Q.

	BGM_COUNT,
	BGM_BEGIN = 0,
};

constexpr int MAX_SOUND = 256;

////////////////////////// MySound //////////////////////////
// �T�E���h���������߂̃N���X.
class MySound
{
private:
	IDirectSoundBuffer* m_pSnd;		// �T�E���h�o�b�t�@.
	float m_currentVol;				// ���݃{�����[��.
	float m_targetVol;				// �t�F�[�h�C���^�A�E�g�p�ڕW�{�����[��.
	float m_changeVol;				// �{�����[���ω���.
	INT m_fadeCnt;

	MySound()
	: m_pSnd(NULL)
	, m_currentVol(0)
	, m_targetVol(0)
	, m_changeVol(0)
	, m_fadeCnt(0)
	{}

	~MySound();

	static IDirectSound* sm_pDS;	// DirectSound�f�o�C�X.
	static int sm_maxSound;			// ���̓ǂݍ��ݐ�.
	static MySound* sm_pStack[MAX_SOUND];

	static IDirectSoundBuffer* MakeDSB(WAVEFORMATEX* pFmt, void* pData, int dataSize);
	static void* WaveRead(LPCTSTR pFname, int* pDataSize, WAVEFORMATEX* pFmt);
	static void WaveFree(void* pData);

public:
	void Play(bool loop = false);
	void Stop();

	void Fadeout();
	void UpdateSound();

	// �_�C���N�g�T�E���h�f�o�C�X�̏���.
	static HRESULT InitDirectSound(HWND hWnd);

	// wav�t�@�C���̓ǂݍ��݂�static���\�b�h�o�R��.
	static MySound* LoadSound(const TCHAR* fname);
	static void ReleaseAll();
};

