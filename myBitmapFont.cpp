#include "stdafx.h"
#include "myBitmapFont.h"
#include "myTexture.h"

ID3DXSprite* MyBitmapFont::sm_pSpr = NULL;

// �I�u�W�F�N�g�𐶐����A�t�H���g��ǂݍ���.
MyBitmapFont* MyBitmapFont::LoadFont(IDirect3DDevice9* pDev, const TCHAR* pFname)
{
	MyBitmapFont* pFont = new MyBitmapFont;
	if (pFont->sm_pSpr == NULL)
	{
		D3DXCreateSprite(pDev, &(pFont->sm_pSpr));
	}

	::LoadTexture(pDev, pFname, &(pFont->m_pTex));
	return pFont;
}

float MyBitmapFont::DrawBitmapText(char* pStr, float x, float y, float stride)
{
	return this->DrawBitmapText(pStr, x, y, stride, 0xFF000000);	// ����.
}

// �������`�悷��B�p���������\���ł��Ȃ��̂ŁAUNICODE��Ή�.
// �ʒu(x, y)�ɁA������str��\������B�����Ԃ�stride�Ŏw��.
// �r�b�g�}�b�v��1����������16x16�h�b�g�����肵�Ă���.
float MyBitmapFont::DrawBitmapText(char* pStr, float x, float y, float stride, DWORD color)
{
	sm_pSpr->Begin(D3DXSPRITE_ALPHABLEND);
	D3DXVECTOR3 cnt(0, 0, 0);
	for (unsigned int i = 0; pStr[i]; i++)
	{
		D3DXVECTOR3 pos(x, y, 0);
		int c = (int)pStr[i];
		int u = (c % 16) * 16;
		int v = (c / 16) * 16;
		const int size = 16;

		// �\���͈͂�ݒ肷��.
		// { left, top, right, bottom }.
		RECT rc = { u, v, u + size, v + size };
		sm_pSpr->Draw(m_pTex, &rc, &cnt, &pos, color);
		x += stride;
	}
	sm_pSpr->End();
	return x;
}
