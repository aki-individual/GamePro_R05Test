#include "stdafx.h"
#include "myTexture.h"


MyTexture* MyTexture::sm_pStack[MAX_TEXTURE];
int MyTexture::sm_maxTexture = 0;


// �摜��ǂݍ��ށB�J�����g�Ŏ��s������..����ǂݍ���.
// pFname �t�@�C�����Bconst�C���q�����Ċ֐����Œl��ύX���Ȃ����Ƃ�錾����.
// ppTex �쐬����e�N�X�`���ւ̃|�C���^�����邽�߂̃|�C���^.
HRESULT LoadTexture(IDirect3DDevice9* pDev, LPCTSTR pFname, IDirect3DTexture9** ppTex)
{
	HRESULT hr = D3DXCreateTextureFromFile(pDev, pFname, ppTex);
	if (FAILED(hr))
	{
		// ���s�����̂ň��̃t�H���_����e�N�X�`����T���Ă݂�.
		int flen = (int)_tcslen(pFname) + 4;
		_tprintf(_T("%s: �e�f�B���N�g������T��...\n"), pFname);
		TCHAR* fn = new TCHAR[flen];
		_stprintf_s(fn, flen, _T("..\\%s"), pFname);
		hr = D3DXCreateTextureFromFile(pDev, fn, ppTex);
		delete[] fn;
		if (FAILED(hr))
		{
			// ���s�����̂ŗ�O�𔭐������ďI���.
			MessageBox(NULL, _T("�e�N�X�`���ǂݍ��ݎ��s"), pFname, MB_OK);
			throw hr;		// �G���[�����������̂ŗ�O�𑗂�.
		}
	}
	return hr;
}

// �摜��ǂݍ��ނ��߂̊֐�.
// pDev D3D�f�o�C�X.
// pFname �e�N�X�`���t�@�C����.
MyTexture* MyTexture::LoadTexture(IDirect3DDevice9* pDev, const TCHAR* pFname)
{
	if (sm_maxTexture >= MAX_TEXTURE)
	{
		return NULL;
	}

	// �I�u�W�F�N�g�𐶐�.
	MyTexture* pMyTex = new MyTexture;
	// ::�������t�����֐��̓O���[�o���֐����Ăяo�����Ƃ��Ӗ�����.
	::LoadTexture(pDev, pFname, &(pMyTex->m_pTex));
	sm_pStack[sm_maxTexture++] = pMyTex;// �X�^�b�N�ɐς�.
	return pMyTex;
}

void MyTexture::ReleaseAll()
{
	for (int i = sm_maxTexture - 1; i >= 0; i--)
	{
		if (sm_pStack[i])
		{
			delete sm_pStack[i];
		}
		sm_pStack[i] = NULL;
	}
	sm_maxTexture = 0;
}

// �\���Ɏg���e�N�X�`���̏���ݒ肷��.
void NumberDisp::Init(MyTexture* pTex, int w, int h)
{
	m_pTex = pTex;
	m_width = w;
	m_height = h;
}

// �����e�N�X�`�����摜�ŕ\������.
// max�ɂ�100000��1000�̂悤��10��N��̐��l�i�\�������������̐��l�j�����邱��.
void NumberDisp::Draw(ID3DXSprite* pSprite, int num, int max, float x, float y)
{
	num %= (max * 10);// num���ő包�𒴂��Ȃ��悤����.

	// num��max�Ŋ���ƁA�ŏ�ʂ̌��̐��l��0�`9�œ�����B������������\������.
	// num��max�Ŋ������]������̌v�Z�Ŏg���Bmax��10�Ŋ���A���̌��̌v�Z�ɔ�����.
	const D3DXVECTOR3 cnt(0, 0, 0);// �X�̉摜�̕\����ʒu�͍����.
	while (max > 0)
	{
		// �ŏ�ʂ̐��l�����o��.
		int val = num / max;
		// �\���ʒu�̌���.
		D3DXVECTOR3 pos(x, y, 0);
		// �\���������摜�̈�i�t�u���W�j��ݒ肷��.
		RECT rc = { val * m_width, 0, (val + 1) * m_width, m_height };
		//        {        left, top,               right,   bottom }
		pSprite->Draw(m_pTex->GetTexture(), &rc, &cnt, &pos, 0xFFFFFFFF);
		num %= max;	// �ŏ�ʂ���菜��.
		max /= 10;		// ���̌v�Z�ɔ�����.
		x += m_width;	// �\���ʒu�̍X�V.
	}
}

