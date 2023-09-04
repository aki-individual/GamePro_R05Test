/*
	myEnemyManager.h
*/
#pragma once

#include "myEnemyBase.h"

//////////////////////////////////
// EnemyManager
// �G�S�̂��Ǘ�����.
class EnemyManager
{
private:
	EnemyBase** m_ppBuffer;	// �G�̔z��A�����O�o�b�t�@�Ƃ��Ďg��.
	int m_enemyMax;			// �z��̍ő�l.
	int m_index;			// �z��̎g�p���݈ʒu.
	char* m_pTextBuff;		// �f�o�b�O�p�ɔz��̏󋵂𕶎���ɂ���.

	int Next();				// ���̈ʒu���m�ۂ���i�������炵���R�[�����Ȃ��̂�private�֐��Ƃ���j.

public:
	EnemyManager(int max)
		: m_ppBuffer(NULL)
		, m_enemyMax(max)
		, m_index(0)
		, m_pTextBuff(NULL)
	{}

	~EnemyManager();

	bool Init();					// �������̊m�ۓ��̏�����.
	void ResetAll();		// �z��̑S�Ă̗v�f�����Z�b�g����.
	bool Add(EnemyBase*);			// �z��ւ̓G�L�����̒ǉ�.
	void Update(float t);	// �G�z��S�̂��X�V.
	void Purge(int index);			// �z�񂩂�ʒuindex�̓G����菜��.
	void Show();			// �G�L�����N�^��\������.
	void Fire();					// �G�̍U��.
	bool Collision(float cx, float cy, float limit2, int* pIndex);
	// ���ʒu(cx, cy)����݂ċ���limit2�ȓ��̓G��pIndex�ɕԋp.
	char* GetText();				// �f�o�b�O�p�ɔz��̏󋵂𕶎��񉻂���.
	// �X�R�A���擾����
	int GetScore(int index)
	{
		return m_ppBuffer[index]->Score();
	}
};
