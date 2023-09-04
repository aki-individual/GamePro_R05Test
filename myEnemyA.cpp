/*
	myEnemyA.cpp
*/
#include "stdafx.h"
#include "myEnemyA.h"
#include "myBulletOne.h"
#include "myBulletBuffer.h"
#include "myApp.h"

// �ړ�.
bool EnemyAAA::Update(float time)
{
	EnemyBase::Update(time);		// �e��Update���Ăяo��.

	// �ʒu���X�V.
	m_posX += m_velX * time;
	m_posY += m_velY * time;
	if (m_timer > 10.0f)
	{
		// ��莞�Ԍ�ɁA��ʊO�i���j�Ɉړ�����Ə���.
		if (m_posY > HEIGHT + 32.0f)
		{
			return false;
		}
		// ��ʏ���Ɉړ�����\��͂Ȃ����ی��ŉ�ʏ���Ɉړ������ꍇ�����ł�����.
		if ( m_posY < 0.0f - 32.0f) {
			return false;			
		}
	}
	return true;
}

// �v���C���[�����ɔ���.
void EnemyAAA::Fire()
{
	if ((rand() % 1000 < 10) && (m_timer < 60))
	{
		App* pApp = GetAppInst();

		float playerX, playerY;
		pApp->GetPlayerPos(playerX, playerY);

		// �v���C���[�ւ̃x�N�g�����Z�o
		float dx = playerX - m_posX;
		float dy = playerY - m_posY;
		float dd = (float)sqrt(dx * dx + dy * dy);
		// �e�̑��x���o��
		dx /= dd; dy /= dd;
		dx *= 5;  dy *= 5;

		BulletOne* pBlt = pApp->GetNextEnemyBlt();
		if (pBlt != NULL) {
			pBlt->Init(m_posX, m_posY, dx, dy);
		}
	}
}
