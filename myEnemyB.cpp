/*
	myEnemyB.cpp
*/
#include "stdafx.h"
#include "myEnemyB.h"
#include "myBulletOne.h"
#include "myBulletBuffer.h"
#include "myApp.h"

// プレイヤーを追跡.
bool EnemyBBB::Update(float time)
{
	App* pApp = GetAppInst();
	EnemyBase::Update(time);		// 親のUpdateを呼び出す.

	if (m_timer < 50.0f)
	{
		float playerX, playerY;
		pApp->GetPlayerPos(playerX, playerY);

		// プレイヤー機に向けて速度ベクトルを調整.
		m_velX = playerX - m_posX;
		m_velY = playerY - m_posY;
		// 速度ベクトル調整.
		float v = (float)sqrt(m_velX * m_velX + m_velY * m_velY);
		m_velX /= (v / 10);
		m_velY /= (v / 10);
	}
	// 位置を更新.
	m_posX += m_velX * time;
	m_posY += m_velY * time;

	if (m_timer > 10.0f)
	{
		// 一定時間後に画面外にいる敵を消滅させる.
		if (m_posY < -32.0f || m_posY > HEIGHT + 32.0f)
		{
			return false;
		}
		if (m_posX < -32.0f || m_posX > WIDTH + 32.0f)
		{
			return false;
		}
	}
	return true;
}

// まっすぐ下に飛ぶ弾.
void EnemyBBB::Fire()
{
	if (rand() % 1000 < 20 && 30 < m_timer && m_timer < 100)
	{
		BulletOne* pBlt = GetAppInst()->GetNextEnemyBlt();
		if (pBlt != NULL)
		{
			pBlt->Init(m_posX, m_posY, 0.0f, 10.0f);
		}
	}
}
