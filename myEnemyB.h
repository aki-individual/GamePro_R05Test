/*
	myEnemyB.h
*/
#pragma once

#include "myEnemyBase.h"

//////////////////
// EnemyBBB
// EnemyBase�̔h���N���X�FEnemyBBB�i�ǐՁj.
class EnemyBBB : public EnemyBase
{
public:
	EnemyBBB(MyTexture* pTex) : EnemyBase(pTex)
	{
		//_tprintf(_T("�GBBB���� addr=%p\n"), this);
	}
	~EnemyBBB() override
	{
		//_tprintf(_T("�GBBB���� addr=%p\n"), this);
	}
	bool Update(float time) override;
	void Fire() override;
	char Getch() override
	{
		return 'B';
	}
	int Score() override
	{
		return 15;
	}
};
