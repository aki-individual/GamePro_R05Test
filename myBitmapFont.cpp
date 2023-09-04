#include "stdafx.h"
#include "myBitmapFont.h"
#include "myTexture.h"

ID3DXSprite* MyBitmapFont::sm_pSpr = NULL;

// オブジェクトを生成し、フォントを読み込む.
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
	return this->DrawBitmapText(pStr, x, y, stride, 0xFF000000);	// 黒で.
}

// 文字列を描画する。英文字しか表示できないので、UNICODE非対応.
// 位置(x, y)に、文字列strを表示する。文字間はstrideで指定.
// ビットマップは1文字あたり16x16ドットを仮定している.
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

		// 表示範囲を設定する.
		// { left, top, right, bottom }.
		RECT rc = { u, v, u + size, v + size };
		sm_pSpr->Draw(m_pTex, &rc, &cnt, &pos, color);
		x += stride;
	}
	sm_pSpr->End();
	return x;
}
