/*
	stdafx.h
*/
#pragma once

// 標準ヘッダファイルのインクルード.
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <d3d9.h>		// Direct3D9 (DirectX9 Graphics)に必要.
#include <d3dx9core.h>	// DirectX スプライトに必要.
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

// 汎用的な定義など.
#define WIDTH (1280)	// 幅.
#define HEIGHT (720)	// 高さ.

// クラスや構造体の前方参照など.
// クラス前方宣言:実体の定義より先にクラス名だけを先に宣言.
// MyTexture *pTex; のようなポインタ宣言に対して有効になる.
//class NumberDisp;
//class MyTexture;
//class BulletBuffer;
//class EnemyManager;
//class GameStage;
//class DataSet;
//class App;
//↑必要なヘッダを個別にインクルードすれば良いので、前方宣言が不要になった.

// 汎用的なインライン関数など.
// 距離の二乗を求める。「距離」を求めないのは、平方根の処理を避けるため.
inline float GetDistance2(float x1, float y1, float x2, float y2)
{
	return ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2));
}
