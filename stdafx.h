/*
	stdafx.h
*/
#pragma once

// �W���w�b�_�t�@�C���̃C���N���[�h.
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <d3d9.h>		// Direct3D9 (DirectX9 Graphics)�ɕK�v.
#include <d3dx9core.h>	// DirectX �X�v���C�g�ɕK�v.
#include <tchar.h>
#include <locale.h>
#include <assert.h>

#define MEMORY_LEAK
#ifdef MEMORY_LEAK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// �ėp�I�Ȓ�`�Ȃ�.
#define WIDTH (1280)	// ��.
#define HEIGHT (720)	// ����.

// �N���X��\���̂̑O���Q�ƂȂ�.
// �N���X�O���錾:���̂̒�`����ɃN���X���������ɐ錾.
// MyTexture *pTex; �̂悤�ȃ|�C���^�錾�ɑ΂��ėL���ɂȂ�.
//class NumberDisp;
//class MyTexture;
//class BulletBuffer;
//class EnemyManager;
//class GameStage;
//class DataSet;
//class App;
//���K�v�ȃw�b�_���ʂɃC���N���[�h����Ηǂ��̂ŁA�O���錾���s�v�ɂȂ���.

// �ėp�I�ȃC�����C���֐��Ȃ�.
// �����̓������߂�B�u�����v�����߂Ȃ��̂́A�������̏���������邽��.
inline float GetDistance2(float x1, float y1, float x2, float y2)
{
	return ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2));
}
