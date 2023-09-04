/*
	myEnemyC.h
*/
#pragma once

#include "myEnemyBase.h"

// EnemyCCC //////////////////////////////////////
// EnemyBaseの子クラス：EnemyCCC（移動パターンを持つ）.
class EnemyCCC : public EnemyBase
{
private:
	const PatternMove* m_pPat;	// 移動パターン.
	int m_patIndex;				// 移動パターン配列のどこまで参照したか.
	float m_offsetX;			// 開始位置.
public:
	EnemyCCC(MyTexture* pTex, float x, const PatternMove* pPat)
		: EnemyBase(pTex), m_offsetX(x), m_pPat(pPat), m_patIndex(0)
	{
		//_tprintf(_T("敵CCC生成 addr=%p\n"), this);
	}
	~EnemyCCC() override
	{
		//_tprintf(_T("敵CCC消滅 addr=%p\n"), this);
	}

	void Init(float x, float y, float vx, float vy) override;
	void Show() override;
	bool Update(float time) override;
	void Fire() override;
	char Getch() override
	{
		return 'C';
	}
	int Score() override
	{
		return 20;
	}
};

