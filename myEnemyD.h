/*
*	myEnemyD.h
*	ステージの上半分を通過したらx軸をランダムな位置にワープさせる.
*/
#pragma once

#include "myEnemyBase.h"

// EnemyDDD //////////////////////////////////////
// EnemyBaseの子クラス：EnemyDDD.
class EnemyDDD : public EnemyBase
{
private:
	float m_offsetX;	// 開始位置.
	bool isWarp;		// ワープしたか.
public:
	EnemyDDD(MyTexture* pTex, float x)
		: EnemyBase(pTex), m_offsetX(x), isWarp(false)
	{
		//_tprintf(_T("敵DDD生成 addr=%p\n"), this);
	}
	~EnemyDDD() override
	{
		//_tprintf(_T("敵DDD消滅 addr=%p\n"), this);
	}

	bool Update(float time) override;
	void Fire() override;
	char Getch() override
	{
		return 'D';
	}
	int Score() override
	{
		return 50;
	}
};

