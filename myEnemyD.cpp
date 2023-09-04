/*
	myEnemyD.cpp
*/
#include "stdafx.h"
#include "myEnemyD.h"
#include "myBulletOne.h"
#include "myBulletBuffer.h"
#include "myApp.h"

// �ړ�.
bool EnemyDDD::Update(float time)
{
	EnemyBase::Update(time);		// �e��Update���Ăяo��.

	// �ʒu���X�V.
	m_posX += m_velX * time;
	m_posY += m_velY * time;

	// ���łɃ��[�v������.
	if (!isWarp) {
		// y���W���X�e�[�W�̏㔼����ʉ߂�����.
		if (m_posY >= HEIGHT / 2) {
			m_posX = rand() % WIDTH; // 1�`�X�e�[�W�̕�.
			isWarp = true; // ���[�v����.
		}
	}

	if (m_timer > 10.0f)
	{
		// ��莞�Ԍ�ɁA��ʊO�i���j�Ɉړ�����Ə���.
		if (m_posY > HEIGHT + 32.0f)
		{
			return false;
		}
		// ��ʏ���Ɉړ�����\��͂Ȃ����ی��ŉ�ʏ���Ɉړ������ꍇ�����ł�����.
		if (m_posY < 0.0f - 32.0f) {
			return false;
		}
	}
	return true;
}

// �������ɔ�Ԓe.
void EnemyDDD::Fire()
{
	if ((rand() % 1000 < 20) && (m_timer < 60))
	{
		for (int i = 0; i < 8; i++) {
			// BulletOne* pBlt = GetAppInst()->pEnemyBullet->Next();
			BulletOne* pBlt = GetAppInst()->GetNextEnemyBlt();
			if (pBlt != NULL)
			{
				// �p�x�����W�A���ɕϊ�.
				float angle = i * 3.14 / 4.0f;

				// �ړ����x.
				float speed = 6.0f;

				// �ړ��p�x.
				float velX = speed * cos(angle);
				float velY = speed * sin(angle);

				// float velX = (i - 1) * 2.0f;// -2.0f, 0.0f, +2.0f
				pBlt->Init(m_posX, m_posY, velX, velY);
			}
		}
	}
}
