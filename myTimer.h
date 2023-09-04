/*
	myTimer.h
*/
#pragma once

#include <windows.h>

/////////////////////////////////////////////////////////////
// ↓タイマークラス.

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

	// タイマー初期化.
	void InitTimer()
	{
		QueryPerformanceFrequency(&Frequency);
	}
	// タイマー開始.
	void StartTimer()
	{
		QueryPerformanceCounter(&StartingTime);
	}
	// 開始からの経過時間を得る.
	void GetPassTime(LONGLONG*, UINT*);

	static CTimer* GetTimer()
	{
		return &sm_Timer;
	}
};

// ↑タイマークラス.
/////////////////////////////////////////////////////////////
