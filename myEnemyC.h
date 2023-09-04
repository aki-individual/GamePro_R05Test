/*
	myEnemyC.h
*/
#pragma once

#include "myEnemyBase.h"

// EnemyCCC //////////////////////////////////////
// EnemyBase�̎q�N���X�FEnemyCCC�i�ړ��p�^�[�������j.
class EnemyCCC : public EnemyBase
{
private:
	const PatternMove* m_pPat;	// �ړ��p�^�[��.
	int m_patIndex;				// �ړ��p�^�[���z��̂ǂ��܂ŎQ�Ƃ�����.
	float m_offsetX;			// �J�n�ʒu.
public:
	EnemyCCC(MyTexture* pTex, float x, const PatternMove* pPat)
		: EnemyBase(pTex), m_offsetX(x), m_pPat(pPat), m_patIndex(0)
	{
		//_tprintf(_T("�GCCC���� addr=%p\n"), this);
	}
	~EnemyCCC() override
	{
		//_tprintf(_T("�GCCC���� addr=%p\n"), this);
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

