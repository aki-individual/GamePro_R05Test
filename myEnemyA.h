/*
	myEnemyA.h
*/
#pragma once

#include "myEnemyBase.h"

//////////////////
// EnemyAAA
// ��������AEnemyBase�̔h���N���X���`���Ă���.
// EnemyBase�͒��ۃN���X�Ȃ̂ŁA���̂𐶐����邱�Ƃ͏o���Ȃ�.
// EnemyBase����h�������N���X����́A��̓I�Ȏ��̂������Ƃ��ł���.

// EnemyBase�̔h���N���X�FEnemyAAA�i���i�j.
class EnemyAAA : public EnemyBase
{
public:
	// EnemyAAA�͐e�N���X�̃R���X�g���N�^�𖾎��I�Ɏw�肵�Ă���.
	// EnemyBase(pTex)�������ƁAEnemyBase()���Ăяo�����ƂɂȂ邪.
	// ���������̃R���X�g���N�^EnemyBase::EnemyBase()�͑��݂��Ȃ��̂�.
	// �R���p�C���G���[���o��.
	EnemyAAA(MyTexture* pTex) : EnemyBase(pTex)
	{
		//_tprintf(_T("�GAAA���� addr=%p\n"), this);
	}
	~EnemyAAA() override
	{
		//_tprintf(_T("�GAAA���� addr=%p\n"), this);
	}
	bool Update(float time) override;
	void Fire() override;
	char Getch() override
	{
		return 'A';
	}
	int Score() override
	{
		return 10;
	}
};

