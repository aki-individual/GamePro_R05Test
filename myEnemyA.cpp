/*
	myEnemyA.cpp
*/
#include "stdafx.h"
#include "myEnemyA.h"
#include "myBulletOne.h"
#include "myBulletBuffer.h"
#include "myApp.h"

// 移動.
bool EnemyAAA::Update(float time)
{
	EnemyBase::Update(time);		// 親のUpdateを呼び出す.

	// 位置を更新.
	m_posX += m_velX * time;
	m_posY += m_velY * time;
	if (m_timer > 10.0f)
	{
		// 一定時間後に、画面外（下）に移動すると消滅.
		if (m_posY > HEIGHT + 32.0f)
		{
			return false;
		}
		// 画面上方に移動する予定はないが保険で画面上方に移動した場合も消滅させる.
		if ( m_posY < 0.0f - 32.0f) {
			return false;			
		}
	}
	return true;
}

// プレイヤー方向に発射.
void EnemyAAA::Fire()
{
	if ((rand() % 1000 < 10) && (m_timer < 60))
	{
		App* pApp = GetAppInst();

		float playerX, playerY;
		pApp->GetPlayerPos(playerX, playerY);

		// プレイヤーへのベクトルを算出
		float dx = playerX - m_posX;
		float dy = playerY - m_posY;
		float dd = (float)sqrt(dx * dx + dy * dy);
		// 弾の速度を出す
		dx /= dd; dy /= dd;
		dx *= 5;  dy *= 5;

		BulletOne* pBlt = pApp->GetNextEnemyBlt();
		if (pBlt != NULL) {
			pBlt->Init(m_posX, m_posY, dx, dy);
		}
	}
}
