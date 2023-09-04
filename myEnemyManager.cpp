/*
	myEnemyManager.cpp
*/
#include "stdafx.h"
#include "myEnemyManager.h"
#include "myApp.h"

// �f�X�g���N�^�ł͔z����������.
// �z��̒��g���܂�������Ă���S�̂����.
EnemyManager::~EnemyManager()
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		if (m_ppBuffer[i] != NULL)
		{
			delete m_ppBuffer[i];
		}
	}
	delete[] m_ppBuffer;
	delete[] m_pTextBuff;
}

// �������̊m�ۓ��̏�����.
bool EnemyManager::Init()
{
	if (m_ppBuffer == NULL)
	{
		m_ppBuffer = new EnemyBase * [m_enemyMax];	// �z��̒��g��EnemyBase*�^.
		// ��sizeof(EnemyBase*) �� max ���̃��������m�ۂ��Ă��邾���ŁA�R���X�g���N�^�͌Ăяo����Ȃ�.
		// sizeof(EnemyBase*)��x64�ł͂P������8�o�C�g�Ax86�ł͂P������4�o�C�g�̃������T�C�Y.
		if (m_ppBuffer == NULL)
		{
			return false;
		}

		for (int i = 0; i < m_enemyMax; i++)
		{
			m_ppBuffer[i] = NULL;		// �|�C���^��NULL�ŏ��������A���g�p��ԂƂ���.
		}
	}

	if (m_pTextBuff == NULL)
	{
		m_pTextBuff = new char[m_enemyMax + 1];	// �f�o�b�O�p��������m�ۂ���.
		// +1 �͏I�[�L��'\0'�̕�.
		if (m_pTextBuff == NULL)
		{
			return false;
		}
	}

	return true;
}

// �z��̑S�Ă̗v�f�����Z�b�g����.
void EnemyManager::ResetAll()
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		Purge(i);
	}
}

// ���̋󂫈ʒu��T��.
int EnemyManager::Next()
{
	// �����O�o�b�t�@���A���݈ʒu(m_index)�������T��.
	for (int i = 0; i < m_enemyMax; i++)
	{
		// ���݈ʒu(m_index)����󂫂�����.
		int index = (m_index + i) % m_enemyMax;
		// index ��m_enemyMax�ȏ�ɂȂ�Ȃ��悤�A��]�ɂ��邱�ƂŔz���.
		// ���݈ʒu���� m_index - 1 �̈ʒu�܂ł�����.
		if (m_ppBuffer[index] == NULL)
		{
			m_index = (index + 1) % m_enemyMax;
			return index;
		}
	}
	return -1;
}

// �f�o�b�O�p��m_pBuffer�z��̒��g���e�L�X�g������.
// �e�ۂ��L���F*.
// �e�ۂ������F-.
char* EnemyManager::GetText()
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		if (m_ppBuffer[i] != NULL)
		{
			m_pTextBuff[i] = m_ppBuffer[i]->Getch();
		}
		else
		{
			m_pTextBuff[i] = '-';
		}
	}
	// ������̍Ō�͕K��NULL�����Ń^�[�~�l�[�g.
	m_pTextBuff[m_enemyMax] = '\0';
	return m_pTextBuff;
}

// pEnemy��z��ɒǉ�.
// EnemyBase*�^�������ɂ��Ă��邪�A���ۂɂ�EnemyAAA*�^��EnemyBBB*�^�������Ă���.
// �����̃N���X�́AEnemyBase�̔h���N���X�Ȃ̂ŁAEnemyBase*�^�Ŏ󂯂邱�Ƃ��ł���.
// �Ԉ���Ĉ�����(EnemyBase pEnemy) �Ɓ���t���Y��A�|�C���^�ł͂Ȃ����̂ŋL�q�����.
// �f�[�^���ꕔ�������錻�ۂ��������[���ȃo�O�̌����ƂȂ�.
bool EnemyManager::Add(EnemyBase* pEnemy)
{
	int index = Next();		// �z�񂩂�󂫂�T��.
	if (0 <= index && index < m_enemyMax)
	{
		m_ppBuffer[index] = pEnemy;// �G���Ǘ��p�̔z��ɓo�^.
		return true;
	}
	return false;
}

// �G�z���index�Ԗڂ𖳌��ɂ���.
// m_ppBuffer[index]�͊�{�N���X�̃|�C���^�����A�f�X�g���N�^��virtual�w�肵�Ă��邽��.
// �K�؂Ȕh���N���X(EnemyAAA��EnemyBBB)�̃f�X�g���N�^���Ăяo�����.
void EnemyManager::Purge(int index)
{
	if (0 <= index && index < m_enemyMax)
	{
		delete m_ppBuffer[index];
		m_ppBuffer[index] = NULL;// �z��̒��g��NULL�N���A���Ď��̎g�p�ɔ�����.
	}
}

// �z��̊e�v�f���ׂĂɁA������i�߂�悤�w������.
void EnemyManager::Update(float time)
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		if (m_ppBuffer[i] != NULL)
		{
			bool flag = m_ppBuffer[i]->Update(time);
			// FLASE���Ԃ�Ƃ��̗v�f�͕s�v�ɂȂ�.
			if (!flag)
			{
				Purge(i);
			}
		}
	}
}

// �z��̑S�Ă̓G�ɑ΂��āA�\���𑣂�.
void EnemyManager::Show()
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		// m_ppBuffer[i]�̌^��EnemyBase*�����AShow �� virtual�֐��Ȃ̂�.
		// �h���N���X��show���K�؂ɌĂяo�����.
		// �܂�if��switch���g���ĕ��򂳂���K�v���Ȃ��B�������I�u�W�F�N�g�w���̗��_�̂P��.
		if (m_ppBuffer[i] != NULL)
		{
			m_ppBuffer[i]->Show();
		}
	}
}

void EnemyManager::Fire()
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		if (m_ppBuffer[i] != NULL)
		{
			m_ppBuffer[i]->Fire();
		}
	}
}

// (cx, cy)�̈ʒu�͓G�ƏՓˋ�����������.
// �Փ˂����ꍇ�͔z���̈ʒu������pIndex���w�ꏊ�ɓ���ATRUE��Ԃ�.
// �Փ˂����������ꍇ��FALSE�Ԃ�.
bool EnemyManager::Collision(float cx, float cy, float limit2, int* pIndex)
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		if (m_ppBuffer[i] != NULL)
		{
			if (m_ppBuffer[i]->Collision(cx, cy, limit2))
			{
				*pIndex = i;
				return true;
			}
		}
	}
	return false;
}

