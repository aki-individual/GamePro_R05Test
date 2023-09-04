/*
	myBulletOne.h
*/
#pragma once

//////////////////////////////////
// BulletOne
// �e�ۂ̂��߂̃N���X�F���ꂼ��̒e�ۂ��������A�ړ��Ȃǒe�ۂ̊Ǘ�.
class BulletOne
{
private:
	float m_posX, m_posY;	// ���݈ʒu.
	float m_velX, m_velY;	// �e�ۑ��x.
	float m_timer;			// �e�ێˏo����̎���.

public:
	BulletOne();			// �R���X�g���N�^.
	virtual ~BulletOne();
	void Init(float x, float y, float vvx, float vvy);	// �ˏo������.
	void GetXY(float& x, float& y);	// ���݂�XY���W�l�𓾂�.
	void Update(float time);		// �e�ۂ�i�߂�.
	float GetTime()					// �����𓾂�.
	{
		return m_timer;
	}
	void Deactive()			// �����ɂ���.
	{
		m_timer = -1.0f;
	}
	bool IsActive()			// �^�C�}�[�����łȂ��Ȃ�L���Ȓe��.
	{
		return !(m_timer < 0.0f);
	}
};
