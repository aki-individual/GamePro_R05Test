/*
	myEnemyBase.h
*/
#pragma once

#include "myTexture.h"

//////////////////////////////////
// EnemyBase
// �G�P�̂��Ǘ������{�N���X.
class EnemyBase
{
//private:
protected:
	// �ȉ��̕ϐ��́u�h���N���X�v����p�ɂɃA�N�Z�X����邽��.
	// private�ł͂Ȃ�protected�Ɏw�肵�Ă���.
	MyTexture* m_pTex;		// �\������e�N�X�`��.
	float m_posX, m_posY;	// ���݈ʒu.
	float m_velX, m_velY;	// XY�����ւ̑��x����.
	float m_timer;			// �������Ă���̎���.

	// ���̃N���X�͒��ۃN���X�Ȃ̂ŁA�h���N���X�ȊO�����new�ł��Ȃ��悤�ɂ���.
	// ���̂��߁A�R���X�g���N�^��protected�G���A�ɋL�q����.
	EnemyBase(MyTexture* pTex)
		: m_pTex(pTex)
		, m_posX(0.0f)
		, m_posY(0.0f)
		, m_velX(0.0f)
		, m_velY(0.0f)
		, m_timer(0.0f)
	{
		//_tprintf(_T("calll EnemyBase constractor\n"));
	}

public:
	// �f�X�g���N�^�́A�q�ȊO������A�N�Z�X�����\�������邽�߁upublic�v.
	// �h�����Ƃ��Đ݌v���Ă���̂Ńf�X�g���N�^��virtual�w�肵�Ă���.
	virtual ~EnemyBase()
	{}

	// �G�L�����̓���.
	virtual bool Update(float time);

	// �f�o�b�O�p�ɕ�����Ԃ�.
	virtual char Getch()
	{
		return '.';
	}

	// �G���ʒu(posX, posY)�A�ړ����x(vx, vy)�ŏ��������o��������.
	virtual void Init(float posX, float posY, float vx, float vy);
	// �L�����N�^��\������.
	virtual void Show();
	// �ʒu(cx,cy)�Ƃ̋�����limit2�ȉ����ǂ����𔻒�.
	virtual bool Collision(float cx, float cy, float limit2);

	// �X�R�A��Ԃ��B�����o�֐��̎��̂��`�����������z�֐��Ƃ���.
	virtual int Score() = 0;// �h���N���X�ł͕K����`���K�v�ƂȂ�.
	// �U������B��{�N���X�ł̍U�����@�͒�`�����������z�֐��Ƃ���.
	virtual void Fire() = 0;
};

// �p�^�[�����[�u�̂��߂̍\���́B����(frame)�ɂ�����ʒu(posX, posY)������.
// �L�[�t���[���Ŏw�肷��̂ŁA�L�[�Ԃ͕�Ԃ��Ȃ���΂Ȃ�Ȃ�.
struct PatternMove
{
	float frame;		// �����i�t���[���j.
	float posX, posY;	// �ʒu.
};

extern const RECT g_EnemyImageArea[];
extern const PatternMove g_ptn1[];
extern const PatternMove g_ptn2[];
extern const PatternMove g_ptn3[];
extern const PatternMove g_ptn4[];