/*
	myBulletBuffer.cpp
*/
#include "stdafx.h"
#include "myBulletBuffer.h"
#include "myEnemyManager.h"
#include "myTexture.h"
#include "myApp.h"

// �f�X�g���N�^:�z���������Ă���.
BulletBuffer::~BulletBuffer()
{
	if (m_pBuffer)
	{
		delete[] m_pBuffer;			// delete m_pBuffer�łȂ����Ƃɒ��ӁB�z��ł�[]��t����.
	}
	if (m_pTextBuff)
	{
		delete[] m_pTextBuff;
	}
}

// �������̊m�ۓ��̏�����.
bool BulletBuffer::Init()
{
	if (m_pBuffer == NULL)
	{
		m_pBuffer = new BulletOne[m_bulletMax];
		if (m_pBuffer == NULL)
		{
			return false;
		}
	}
	if (m_pTextBuff == NULL)
	{
		m_pTextBuff = new char[m_bulletMax + 1];
		if (m_pTextBuff == NULL)
		{
			return false;
		}
	}
	return true;
}

void BulletBuffer::ResetAll()
{
	for (int i = 0; i < m_bulletMax; i++)
	{
		m_pBuffer[i].Deactive();
	}
}

// �����O�o�b�t�@��T�����āA�󂫂�������.
// return NULL		�󂫂Ȃ�.
// return NULL�ȊO	�󂫈ʒu.
BulletOne* BulletBuffer::Next()
{
	// �����O�o�b�t�@�����݈ʒu(m_index)����T��.
	for (int i = 0; i < m_bulletMax; i++)
	{
		// ���݈ʒu(m_index)����󂫂�����.
		int index = (m_index + i) % m_bulletMax;
		// index ��m_bulletMax�ȏ�ɂȂ�Ȃ��悤��]�ɂ�.
		// �z��̍Ō�܂Ń`�F�b�N������A���x�͐擪����(index - 1)�̈ʒu�܂ł��`�F�b�N����.
		if (!m_pBuffer[index].IsActive())
		{
			// �󂫂�����������...
			// �Ԃ��O�Ɍ����J�n�ʒu���P�i�߂�i�Ō�܂ōs������擪�ɖ߂�j.
			m_index = (m_index + 1) % m_bulletMax;

			// �󂢂Ă�ꏊ�̃|�C���^��Ԃ�.
			return &m_pBuffer[index];
		}
	}
	// �󂫂Ȃ�.
	return NULL;
}

// �����O�o�b�t�@���̃f�[�^�������X�V����.
void BulletBuffer::Update(float time)
{
	for (int i = 0; i < m_bulletMax; i++)
	{
		m_pBuffer[i].Update(time);
	}
}

// �f�o�b�O�p��m_pBuffer�z��̒��g���e�L�X�g������.
// �e�ۂ��L���Ȃ�*�A�����Ȃ�-�ƈꕶ���ɂ���.
char* BulletBuffer::GetText()
{
	for (int i = 0; i < m_bulletMax; i++)
	{
		if (m_pBuffer[i].IsActive())
		{
			m_pTextBuff[i] = '*';
		}
		else
		{
			m_pTextBuff[i] = '-';
		}
	}
	// ������̍Ō�͕K�� NULL���� �Ń^�[�~�l�[�g.
	m_pTextBuff[m_bulletMax] = '\0';
	return m_pTextBuff;
}

// �v���[���[�̒e�ۂƓG�L�����Ƃ̏Փ˔���.
void BulletBuffer::Collision()
{
	App* pApp = GetAppInst();
	EnemyManager* pEnemyMgr = pApp->GetEnemyMgr();

	// �e�e�ۂ̈ʒu(posX, posY)�ƁA�G�̋��������ȉ����ǂ�������.
	for (int i = 0; i < m_bulletMax; i++)
	{
		BulletOne& bullet = m_pBuffer[i];		// bullet�Œu������.
		if (!bullet.IsActive())
		{
			continue;
		}

		// �e�e�ۂ̌��݈ʒu���擾�A�G�̂��Âꂩ�ƏՓ˂��������ׂ�.
		float posX, posY;
		bullet.GetXY(posX, posY);
		int index;		// �Փ˂����G�B�z���̈ʒu.
		if (pEnemyMgr->Collision(posX, posY, 24 * 24, &index))
		{
			// �Փ˂����̂ŁA�e�ۂ𖳌���.
			bullet.Deactive();

			// ���_�����Z.
			pApp->AddScore( pEnemyMgr->GetScore(index) );

			// �G��������.
			pEnemyMgr->Purge(index);

			// ���ĉ��Đ�.
			pApp->PlaySE(SE_TEKI_DEAD);
		}
	}
}

// �e�N�X�`���w��t���ŁA�e�ۂ�`�悷��.
void BulletBuffer::Draw(MyTexture* pTex)
{
	D3DXVECTOR3 cnt(8, 8, 0);
	D3DXVECTOR3 pos(0, 0, 0);

	for (int i = 0; i < m_bulletMax; i++)
	{
		// �擾�����e�ۂ��L���Ȃ�\��.
		if (m_pBuffer[i].IsActive())
		{
			// ���݈ʒu��pos�\���̌^�̃f�[�^�Ɋi�[���ĕ\��.
			m_pBuffer[i].GetXY(pos.x, pos.y);
//			GetAppInst()->pSprite->Draw(pTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
			GetAppInst()->DrawSprite(pTex, &cnt, &pos);
		}
	}
}

// �v���C���[�ɓG�̒e�ۂ��Փ˂��Ă��邩�̔���.
// �߂�l�́Atrue=�Փ�  false=�������ĂȂ�.
bool BulletBuffer::CollisionPlayer(float x, float y, float limit2)
{
	App* pApp = GetAppInst();
	// �e�e�ۂ̈ʒu(xx, yy)�ƁA�G�̋��������ȉ����ǂ�������.
	for (int i = 0; i < m_bulletMax; i++)
	{
		BulletOne& bullet = m_pBuffer[i];		// bullet�Œu������.
		if (!bullet.IsActive()) {
			continue;
		}

		// �e�e�ۂ̌��݈ʒu���擾�A�v���C���[�ƏՓ˂��������ׂ�.
		float bx, by;
		bullet.GetXY(bx, by);
		if (GetDistance2(bx, by, x, y) < limit2)
		{
			// �Փ˂����̂ŁA�e�ۂ𖳌���.
			bullet.Deactive();
			return true;
		}
	}
	return false;
}
