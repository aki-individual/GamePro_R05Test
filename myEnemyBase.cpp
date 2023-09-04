/*
	myEnemyBase.cpp
*/
#include "stdafx.h"
#include "myEnemyBase.h"
#include "myApp.h"

// 敵テクスチャの表示領域のための配列.
const RECT g_EnemyImageArea[] = {
	{ 0,   0, 64,  64 },
	{ 0,  64, 64, 128 },
	{ 0, 128, 64, 192 },
	{ 0, 192, 64, 256 },
	{ 0, 256, 64, 320 },
	{ 0, 320, 64, 384 },
	{ 0, 384, 64, 448 },
	{ 0, 448, 64, 512 },
};

//////////////////
// EnemyBase
void EnemyBase::Show()
{
	assert(m_timer >= 0.0f);// タイマーが負の状態で呼ばれるべきではないのでアサートを仕掛けておく.
	if (m_timer < 0.0f)
	{
		return;
	}

	D3DXVECTOR3 cnt(32.0f, 32.0f, 0.0f);	// 表示時の中心位置.
	D3DXVECTOR3 pos(m_posX, m_posY, 0.0f);	// 表示位置.
	int time = (int)(m_timer / 10.0f);		// 経過時間を10フレーム単位に換算.
	int anim = time % 4;					// テクスチャアニメーションを4パターンで.
//	GetAppInst()->pSprite->Draw(m_pTex->GetTexture(), &(g_EnemyImageArea[anim]), &cnt, &pos, 0xFFFFFFFF);
	GetAppInst()->DrawSprite(m_pTex, &(g_EnemyImageArea[anim]), &cnt, &pos);
}

// 敵の初期化：初期位置、速度.
void EnemyBase::Init(float x, float y, float vx, float vy)
{
	m_posX = x; m_posY = y;
	m_velX = vx; m_velY = vy;
	m_timer = 0.0f;	// タイマーを0.0fにして、有効化する.
}

// 敵の移動処理：基底クラスでは時刻を進めるだけ.
bool EnemyBase::Update(float time)
{
	if (m_timer < 0.0f)
	{
		// タイマーがマイナス値なら存在しない敵.
	}
	else
	{
		m_timer += time;		// 時刻を進める.
	}
	return true;
}

// 位置(cx, cy)と自分の位置(posX, posY)は距離limit2より近いかどうか.
// limit2は距離の二乗.
bool EnemyBase::Collision(float cx, float cy, float limit2)
{
	return (GetDistance2(cx, cy, m_posX, m_posY) < limit2);
}


////////////////////////////////////////////////////////////////////////////////
// パターンムーブデータ.

// パターン１.
const PatternMove g_ptn1[] = {
	{   0, 100,   0 },
	{  30, 100,  50 },
	{  90, 300, 200 },
	{ 120, 100, 200 },
	{ 150, 100, 300 },
	{ 180, 200, 400 },
	{ 210, 400, 300 },
	{ 240, 500, 100 },
	{ 270, 700, 300 },
	{ 300, 500, 700 },
	{  -1,  -1,  -1 }// END
};

// パターン２.
const PatternMove g_ptn2[] = {
	{   0, 350,   0 },
	{  30, 350, 500 },
	{  40, 330, 300 },
	{  50, 275, 125 },
	{  60, 200, 100 },
	{  70, 125, 125 },
	{  80,  70, 200 },
	{  90,  50, 300 },
	{ 120,  50,   0 },
	{  -1,  -1,  -1 }// END
};

// パターン3.
const PatternMove g_ptn3[] = {
	{ 0, 0, 0},
	{ 30,WIDTH * 0.1,HEIGHT * 0.1 },
	{ 60,WIDTH * 0.1,HEIGHT * 0.9 },
	{ 90,WIDTH * 0.1,HEIGHT * 0.1 },
	{ 120,WIDTH * 0.5,HEIGHT * 0.1 },
	{ 150,WIDTH * 0.5,HEIGHT * 0.9 },
	{ 180,WIDTH * 0.5,HEIGHT * 0.1 },
	{ 210,WIDTH * 0.9,HEIGHT * 0.1 },
	{ 240,WIDTH * 0.9,HEIGHT * 0.9 },
	{ 270,WIDTH * 0.9,HEIGHT * 0.1 },
	{ 300,WIDTH * 0.9,HEIGHT * 0.9 },
	{ -1,  -1,  -1 },	// END
};

// パターン4.
const PatternMove g_ptn4[] = {
	{ 0, 0, 0},
	{40,WIDTH * 0.9,HEIGHT * 0.1},
	{80,WIDTH * 0.9,HEIGHT * 0.1},
	{120,WIDTH * 0.1,HEIGHT * 0.9},
	{160,WIDTH * 0.1,HEIGHT * 0.1},
	{200,WIDTH * 0.9,HEIGHT * 0.1},
	{240,WIDTH * 0.9,HEIGHT * 0.9},
	{300,WIDTH * 0.1,HEIGHT * 0.9},
	{  -1,  -1,  -1 },	// END
};