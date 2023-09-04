/*
	myBitmapFont.h
*/
#pragma once

#include <windows.h>
#include <d3d9.h>			// Direct3D9 (DirectX9 Graphics)�ɕK�v.
#include <d3dx9core.h>		// DirectX �X�v���C�g�ɕK�v.
#include <tchar.h>
#include <stdio.h>

// ������摜��\�����邽�߂̃N���X.
// �r�b�g�}�b�v�t�H���g�摜���ƂɃI�u�W�F�N�g�𐶐����邱��.
class MyBitmapFont
{
private:
	static ID3DXSprite* sm_pSpr;

	IDirect3DTexture9* m_pTex;

	// �R���X�g���N�^��private�ɂ��ăC���X�^���X�����ꏊ�𐧌�.
	MyBitmapFont() : m_pTex(NULL)
	{
	}

public:
	// �I�u�W�F�N�g������static�����o����B�R���X�g���N�^�͌Ăяo���Ȃ�.
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
