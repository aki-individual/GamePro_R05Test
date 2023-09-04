/*
	myEnemyB.cpp
*/
#include "stdafx.h"
#include "myEnemyB.h"
#include "myBulletOne.h"
#include "myBulletBuffer.h"
#include "myApp.h"

// �v���C���[��ǐ�.
bool EnemyBBB::Update(float time)
{
	App* pApp = GetAppInst();
	EnemyBase::Update(time);		// �e��Update���Ăяo��.

	if (m_timer < 50.0f)
	{
		float playerX, playerY;
		pApp->GetPlayerPos(playerX, playerY);

		// �v���C���[�@�Ɍ����đ��x�x�N�g���𒲐�.
		m_velX = playerX - m_posX;
		m_velY = playerY - m_posY;
		// ���x�x�N�g������.
		float v = (float)sqrt(m_velX * m_velX + m_velY * m_velY);
		m_velX /= (v / 10);
		m_velY /= (v / 10);
	}
	// �ʒu���X�V.
	m_posX += m_velX * time;
	m_posY += m_velY * time;

	if (m_timer > 10.0f)
	{
		// ��莞�Ԍ�ɉ�ʊO�ɂ���G�����ł�����.
		if (m_posY < -32.0f || m_posY > HEIGHT + 32.0f)
		{
			return false;
		}
		if (m_posX < -32.0f || m_posX > WIDTH + 32.0f)
		{
			return false;
		}
	}
	return true;
}

// �܂��������ɔ�Ԓe.
void EnemyBBB::Fire()
{
	if (rand() % 1000 < 20 && 30 < m_timer && m_timer < 100)
	{
		BulletOne* pBlt = GetAppInst()->GetNextEnemyBlt();
		if (pBlt != NULL)
		{
			pBlt->Init(m_posX, m_posY, 0.0f, 10.0f);
		}
	}
}
