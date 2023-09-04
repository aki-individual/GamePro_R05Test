/*
	myBitmapFont.h
*/
#pragma once

#include <windows.h>
#include <d3d9.h>			// Direct3D9 (DirectX9 Graphics)に必要.
#include <d3dx9core.h>		// DirectX スプライトに必要.
#include <tchar.h>
#include <stdio.h>

// 文字列画像を表示するためのクラス.
// ビットマップフォント画像ごとにオブジェクトを生成すること.
class MyBitmapFont
{
private:
	static ID3DXSprite* sm_pSpr;

	IDirect3DTexture9* m_pTex;

	// コンストラクタをprivateにしてインスタンス生成場所を制限.
	MyBitmapFont() : m_pTex(NULL)
	{
	}

public:
	// オブジェクト生成はstaticメンバから。コンストラクタは呼び出せない.
	static MyBitmapFont* LoadFont(IDirect3DDevice9* pDev, const TCHAR* pFont);
	virtual ~MyBitmapFont()
	{
		if (m_pTex) {
			m_pTex->Release();
		}
		if (sm_pSpr) {
			sm_pSpr->Release();
		}
	}
	float DrawBitmapText(char* pStr, float x, float y, float stride);
	float DrawBitmapText(char* pStr, float x, float y, float stride, DWORD color);
};
