/*
	myEnemyD.cpp
*/
#include "stdafx.h"
#include "myEnemyD.h"
#include "myBulletOne.h"
#include "myBulletBuffer.h"
#include "myApp.h"

// 移動.
bool EnemyDDD::Update(float time)
{
	EnemyBase::Update(time);		// 親のUpdateを呼び出す.

	// 位置を更新.
	m_posX += m_velX * time;
	m_posY += m_velY * time;

	// すでにワープしたか.
	if (!isWarp) {
		// y座標がステージの上半分を通過したか.
		if (m_posY >= HEIGHT / 2) {
			m_posX = rand() % WIDTH; // 1～ステージの幅.
			isWarp = true; // ワープ完了.
		}
	}

	if (m_timer > 10.0f)
	{
		// 一定時間後に、画面外（下）に移動すると消滅.
		if (m_posY > HEIGHT + 32.0f)
		{
			return false;
		}
		// 画面上方に移動する予定はないが保険で画面上方に移動した場合も消滅させる.
		if (m_posY < 0.0f - 32.0f) {
			return false;
		}
	}
	return true;
}

// 八方向に飛ぶ弾.
void EnemyDDD::Fire()
{
	if ((rand() % 1000 < 20) && (m_timer < 60))
	{
		for (int i = 0; i < 8; i++) {
			// BulletOne* pBlt = GetAppInst()->pEnemyBullet->Next();
			BulletOne* pBlt = GetAppInst()->GetNextEnemyBlt();
			if (pBlt != NULL)
			{
				// 角度をラジアンに変換.
				float angle = i * 3.14 / 4.0f;

				// 移動速度.
				float speed = 6.0f;

				// 移動角度.
				float velX = speed * cos(angle);
				float velY = speed * sin(angle);

				// float velX = (i - 1) * 2.0f;// -2.0f, 0.0f, +2.0f
				pBlt->Init(m_posX, m_posY, velX, velY);
			}
		}
	}
}
