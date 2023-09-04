/*
	myEnemyC.cpp
*/
#include "stdafx.h"
#include "myEnemyC.h"
#include "myBulletOne.h"
#include "myBulletBuffer.h"
#include "myApp.h"

void EnemyCCC::Init(float x, float y, float vx, float vy)
{
	EnemyBase::Init(x, y, vx, vy);

	// EnemyCCC�͈ړ��p�^�[���œ������߁A�ŏ��̈ʒu�f�[�^�ŏ��������Ă���.
	m_posX = m_pPat[0].posX + m_offsetX;
	m_posY = m_pPat[0].posY;
}

// �N���XCCC��show�́A�\���p�^�[�������Ⴄ�̂ŕʂɒ�`.
void EnemyCCC::Show()
{
	assert(m_timer >= 0.0f);// �^�C�}�[�����̏�ԂŌĂ΂��ׂ��ł͂Ȃ��̂ŃA�T�[�g���d�|���Ă���.
	if (m_timer < 0.0f)
	{
		return;
	}

	D3DXVECTOR3 cnt(32.0f, 32.0f, 0.0f);	// �\�����̒��S�ʒu.
	D3DXVECTOR3 pos(m_posX, m_posY, 0.0f);	// �\���ʒu.
	int time = (int)(m_timer / 10.0f);
	int anim = time % 8;	// �e�N�X�`���A�j���[�V�����̂��߂̒l.
//	GetAppInst()->pSprite->Draw(m_pTex->GetTexture(), &(g_EnemyImageArea[anim]), &cnt, &pos, 0xFFFFFFFF);
	GetAppInst()->DrawSprite(m_pTex, &(g_EnemyImageArea[anim]), &cnt, &pos);
}

// �p�^�[�����[�u�̂��߂̏���.
bool EnemyCCC::Update(float time)
{
	EnemyBase::Update(time);	// �e��update���Ăяo��.

	// ���ݎ����̓p�^�[���̂ǂ��ɑ������邩�𒲂ׂ�.
	// �p�^�[���z��́A�Ōオ{-1,-1,-1}�Ƃ��Ă���̂ł����܂Ń��[�v����.
	int id = 0;// ���Ƃ�id���Q�Ƃ���̂ŁAfor�̊O�Ő錾.

	for (id = m_patIndex; m_pPat[id].frame >= 0.0f; id++)
	{
		// ���݂̃p�^�[���̎����́A���ݎ���(m_timer)�����傫����?
		if (m_pPat[id].frame >= m_timer)
		{
			break;// ����ȏ�T�����Ȃ�.
		}
	}
	// ���݁A�ϐ�id�̓p�^�[���z��ł̈ʒu������.
	// ���݈ʒu������ȍ~�Ŏg����悤�Ɋo���Ă���.
	m_patIndex = id;

	// �ʒu id �ł̎����ݒ肪���ł���΁A�p�^�[���͏I��.
	if (m_pPat[id].frame >= 0.0f)
	{
		// i-1��i�̃p�^�[������A���ݎ����ɂ�����ʒu���v�Z���Ȃ���.
		// �������Ai��0�̏ꍇ��(i-1)�� -1 �ɂȂ邽�ߔz��O�̗v�f�ɃA�N�Z�X���邽�ߊ댯.
		// i��0�̏ꍇ��i=1�Ƃ��ĕی�������.
		assert(id != 0);// EnemyBase::update()��timer���ɍX�V����̂ŁAi ��0�ɂȂ邱�Ƃ͂Ȃ��͂��Ȃ̂ŃA�T�[�g���d�|���Ă���.
		if (id == 0)
		{
			id = 1;// �ی�.
		}

		// ��Ԏ���.
		float dt = m_pPat[id].frame - m_pPat[id - 1].frame;

		// ��Ԉړ����x.
		m_velX = (m_pPat[id].posX - m_pPat[id - 1].posX) / dt;
		m_velY = (m_pPat[id].posY - m_pPat[id - 1].posY) / dt;

		// ��Ԓ��̎����B��ԊJ�n�ʒu����̑��Ύ��������߁Atime��u��������.
		time = m_timer - m_pPat[id - 1].frame;
		m_posX = m_pPat[id - 1].posX + m_offsetX;
		m_posY = m_pPat[id - 1].posY;
	}

	// �����ʒu�Ƒ��x�A�o�ߎ������猻�݈ʒu�����肷��.
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

// �O�����ɔ�Ԓe.
void EnemyCCC::Fire()
{
	if ((rand() % 1000 < 20) && (m_timer < 60))
	{
		for (int i = 0; i < 3; i++) {
//			BulletOne* pBlt = GetAppInst()->pEnemyBullet->Next();
			BulletOne* pBlt = GetAppInst()->GetNextEnemyBlt();
			if (pBlt != NULL)
			{
				float velX = (i - 1) * 2.0f;// -2.0f, 0.0f, +2.0f
				pBlt->Init(m_posX, m_posY, velX, 7.0f);
			}
		}
	}
}
