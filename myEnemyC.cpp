/*
	myEnemyC.cpp
*/
#include "stdafx.h"
#include "myEnemyC.h"
#include "myBulletOne.h"
#include "myBulletBuffer.h"
#include "myApp.h"

void EnemyCCC::Init(float x, float y, float vx, float vy)
{
	EnemyBase::Init(x, y, vx, vy);

	// EnemyCCCは移動パターンで動くため、最初の位置データで初期化しておく.
	m_posX = m_pPat[0].posX + m_offsetX;
	m_posY = m_pPat[0].posY;
}

// クラスCCCのshowは、表示パターン数が違うので別に定義.
void EnemyCCC::Show()
{
	assert(m_timer >= 0.0f);// タイマーが負の状態で呼ばれるべきではないのでアサートを仕掛けておく.
	if (m_timer < 0.0f)
	{
		return;
	}

	D3DXVECTOR3 cnt(32.0f, 32.0f, 0.0f);	// 表示時の中心位置.
	D3DXVECTOR3 pos(m_posX, m_posY, 0.0f);	// 表示位置.
	int time = (int)(m_timer / 10.0f);
	int anim = time % 8;	// テクスチャアニメーションのための値.
//	GetAppInst()->pSprite->Draw(m_pTex->GetTexture(), &(g_EnemyImageArea[anim]), &cnt, &pos, 0xFFFFFFFF);
	GetAppInst()->DrawSprite(m_pTex, &(g_EnemyImageArea[anim]), &cnt, &pos);
}

// パターンムーブのための処理.
bool EnemyCCC::Update(float time)
{
	EnemyBase::Update(time);	// 親のupdateを呼び出す.

	// 現在時刻はパターンのどこに相当するかを調べる.
	// パターン配列は、最後が{-1,-1,-1}としているのでそこまでループする.
	int id = 0;// あとでidを参照するので、forの外で宣言.

	for (id = m_patIndex; m_pPat[id].frame >= 0.0f; id++)
	{
		// 現在のパターンの時刻は、現在時刻(m_timer)よりも大きいか?
		if (m_pPat[id].frame >= m_timer)
		{
			break;// これ以上探索しない.
		}
	}
	// 現在、変数idはパターン配列での位置を示す.
	// 現在位置を次回以降で使えるように覚えておく.
	m_patIndex = id;

	// 位置 id での時刻設定が負であれば、パターンは終了.
	if (m_pPat[id].frame >= 0.0f)
	{
		// i-1とiのパターンから、現在時刻における位置を計算しなおす.
		// しかし、iが0の場合は(i-1)は -1 になるため配列外の要素にアクセスするため危険.
		// iが0の場合はi=1として保険を入れる.
		assert(id != 0);// EnemyBase::update()でtimerを先に更新するので、i が0になることはないはずなのでアサートを仕掛けておく.
		if (id == 0)
		{
			id = 1;// 保険.
		}

		// 区間時刻.
		float dt = m_pPat[id].frame - m_pPat[id - 1].frame;

		// 区間移動速度.
		m_velX = (m_pPat[id].posX - m_pPat[id - 1].posX) / dt;
		m_velY = (m_pPat[id].posY - m_pPat[id - 1].posY) / dt;

		// 区間中の時刻。区間開始位置からの相対時刻を求め、timeを置き換える.
		time = m_timer - m_pPat[id - 1].frame;
		m_posX = m_pPat[id - 1].posX + m_offsetX;
		m_posY = m_pPat[id - 1].posY;
	}

	// 初期位置と速度、経過時刻から現在位置を決定する.
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

// 三方向に飛ぶ弾.
void EnemyCCC::Fire()
{
	if ((rand() % 1000 < 20) && (m_timer < 60))
	{
		for (int i = 0; i < 3; i++) {
//			BulletOne* pBlt = GetAppInst()->pEnemyBullet->Next();
			BulletOne* pBlt = GetAppInst()->GetNextEnemyBlt();
			if (pBlt != NULL)
			{
				float velX = (i - 1) * 2.0f;// -2.0f, 0.0f, +2.0f
				pBlt->Init(m_posX, m_posY, velX, 7.0f);
			}
		}
	}
}
