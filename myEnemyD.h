/*
*	myEnemyD.h
*	�X�e�[�W�̏㔼����ʉ߂�����x���������_���Ȉʒu�Ƀ��[�v������.
*/
#pragma once

#include "myEnemyBase.h"

// EnemyDDD //////////////////////////////////////
// EnemyBase�̎q�N���X�FEnemyDDD.
class EnemyDDD : public EnemyBase
{
private:
	float m_offsetX;	// �J�n�ʒu.
	bool isWarp;		// ���[�v������.
public:
	EnemyDDD(MyTexture* pTex, float x)
		: EnemyBase(pTex), m_offsetX(x), isWarp(false)
	{
		//_tprintf(_T("�GDDD���� addr=%p\n"), this);
	}
	~EnemyDDD() override
	{
		//_tprintf(_T("�GDDD���� addr=%p\n"), this);
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

