/*
	myTimer.h
*/
#pragma once

#include <windows.h>

/////////////////////////////////////////////////////////////
// ���^�C�}�[�N���X.

class CTimer
{
public:
	static CTimer sm_Timer;

private:
	LARGE_INTEGER StartingTime;
	LARGE_INTEGER Frequency;

	CTimer()
	{
		StartingTime.QuadPart = 0;
		Frequency.QuadPart = 0;
	}

public:
	~CTimer()
	{
	};

	// �^�C�}�[������.
	void InitTimer()
	{
		QueryPerformanceFrequency(&Frequency);
	}
	// �^�C�}�[�J�n.
	void StartTimer()
	{
		QueryPerformanceCounter(&StartingTime);
	}
	// �J�n����̌o�ߎ��Ԃ𓾂�.
	void GetPassTime(LONGLONG*, UINT*);

	static CTimer* GetTimer()
	{
		return &sm_Timer;
	}
};

// ���^�C�}�[�N���X.
/////////////////////////////////////////////////////////////
