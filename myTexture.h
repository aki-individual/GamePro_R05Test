/*
	myTexture.h
*/
#pragma once

const int MAX_TEXTURE = 256;

////////////////////////// MyTexture //////////////////////////
// �e�N�X�`�����Ǘ����邽�߂̃N���X���쐬����.
// �e�N�X�`�����ݒ肳��Ă���΁A�f�X�g���N�^�ɂĉ������.
// ���̌��ʁA����Y���h�����Ƃ��ł���.
// �������A�I�u�W�F�N�g���̂�delete���Ȃ���΂Ȃ�Ȃ��̂�.
// ���ς�炸����Y��ɂ͒��ӂ��Ȃ���΂Ȃ�Ȃ�.
class MyTexture
{
private:
	static MyTexture* sm_pStack[MAX_TEXTURE];
	static int sm_maxTexture;

protected:
	IDirect3DTexture9* m_pTex;			// �Ǘ�����e�N�X�`��.

private:
	// �R���X�g���N�^���N���X��`���ŋL�q�B�����o�ϐ�pTex�� NULL �ŏ���������.
	MyTexture() : m_pTex(NULL)
	{
	}
	// �f�X�g���N�^�ł́ApTex���������B�����I�ɍs���邽�߁A����Y���h����.
	// private�̈�Ɉړ����Ă��邱�Ƃɒ��ڂ��Ă�������.
	virtual ~MyTexture()
	{
		if (m_pTex)
		{
			m_pTex->Release();
		}
	}

public:
	// �A�N�Z�b�T�BpTex���擾���邽�߂̊֐�.
	IDirect3DTexture9* GetTexture()
	{
		return m_pTex;
	}
	// MyTexture�I�u�W�F�N�g���쐬���e�N�X�`����ǂݍ���.
	// �N���X���\�b�h�ɂȂ��Ă���_�ɒ���.
	static MyTexture* LoadTexture(IDirect3DDevice9* pDev, const TCHAR* pFname);
	static void ReleaseAll();
};

// NumberDisp //////////////////////////////////////
// ���l��\������N���X�F�X�R�A�\����^�C�}�[�\��.
class NumberDisp
{
private:
	MyTexture* m_pTex;		// �X�R�A�\���Ɏg�p����e�N�X�`���i�O�`�X�̐����������сj.
	int m_width;			// �����P�̉���.
	int m_height;			// �����P�̏c��.

public:
	NumberDisp() : m_pTex(NULL), m_width(0), m_height(0) {}

	// �\���Ɏg���e�N�X�`���̏���ݒ�.
	void Init(MyTexture*, int w, int h);

	// �����e�N�X�`����\������.
	void Draw(ID3DXSprite*, int num, int max, float x, float y);
};

HRESULT LoadTexture(IDirect3DDevice9* pDev, LPCTSTR pFname, IDirect3DTexture9** ppTex);
