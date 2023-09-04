/*
	myBulletBuffer.cpp
*/
#include "stdafx.h"
#include "myBulletBuffer.h"
#include "myEnemyManager.h"
#include "myTexture.h"
#include "myApp.h"

// デストラクタ:配列を解放している.
BulletBuffer::~BulletBuffer()
{
	if (m_pBuffer)
	{
		delete[] m_pBuffer;			// delete m_pBufferでないことに注意。配列では[]を付ける.
	}
	if (m_pTextBuff)
	{
		delete[] m_pTextBuff;
	}
}

// メモリの確保等の初期化.
bool BulletBuffer::Init()
{
	if (m_pBuffer == NULL)
	{
		m_pBuffer = new BulletOne[m_bulletMax];
		if (m_pBuffer == NULL)
		{
			return false;
		}
	}
	if (m_pTextBuff == NULL)
	{
		m_pTextBuff = new char[m_bulletMax + 1];
		if (m_pTextBuff == NULL)
		{
			return false;
		}
	}
	return true;
}

void BulletBuffer::ResetAll()
{
	for (int i = 0; i < m_bulletMax; i++)
	{
		m_pBuffer[i].Deactive();
	}
}

// リングバッファを探索して、空きを見つける.
// return NULL		空きなし.
// return NULL以外	空き位置.
BulletOne* BulletBuffer::Next()
{
	// リングバッファを現在位置(m_index)から探索.
	for (int i = 0; i < m_bulletMax; i++)
	{
		// 現在位置(m_index)から空きを検索.
		int index = (m_index + i) % m_bulletMax;
		// index がm_bulletMax以上にならないよう剰余にし.
		// 配列の最後までチェックしたら、今度は先頭から(index - 1)の位置までをチェックする.
		if (!m_pBuffer[index].IsActive())
		{
			// 空きが見つかったら...
			// 返す前に検索開始位置を１つ進める（最後まで行ったら先頭に戻る）.
			m_index = (m_index + 1) % m_bulletMax;

			// 空いてる場所のポインタを返す.
			return &m_pBuffer[index];
		}
	}
	// 空きなし.
	return NULL;
}

// リングバッファ内のデータを順次更新する.
void BulletBuffer::Update(float time)
{
	for (int i = 0; i < m_bulletMax; i++)
	{
		m_pBuffer[i].Update(time);
	}
}

// デバッグ用にm_pBuffer配列の中身をテキスト化する.
// 弾丸が有効なら*、無効なら-と一文字にする.
char* BulletBuffer::GetText()
{
	for (int i = 0; i < m_bulletMax; i++)
	{
		if (m_pBuffer[i].IsActive())
		{
			m_pTextBuff[i] = '*';
		}
		else
		{
			m_pTextBuff[i] = '-';
		}
	}
	// 文字列の最後は必ず NULL文字 でターミネート.
	m_pTextBuff[m_bulletMax] = '\0';
	return m_pTextBuff;
}

// プレーヤーの弾丸と敵キャラとの衝突判定.
void BulletBuffer::Collision()
{
	App* pApp = GetAppInst();
	EnemyManager* pEnemyMgr = pApp->GetEnemyMgr();

	// 各弾丸の位置(posX, posY)と、敵の距離が一定以下かどうか判定.
	for (int i = 0; i < m_bulletMax; i++)
	{
		BulletOne& bullet = m_pBuffer[i];		// bulletで置き換え.
		if (!bullet.IsActive())
		{
			continue;
		}

		// 各弾丸の現在位置を取得、敵のいづれかと衝突したか調べる.
		float posX, posY;
		bullet.GetXY(posX, posY);
		int index;		// 衝突した敵。配列上の位置.
		if (pEnemyMgr->Collision(posX, posY, 24 * 24, &index))
		{
			// 衝突したので、弾丸を無効に.
			bullet.Deactive();

			// 得点を加算.
			pApp->AddScore( pEnemyMgr->GetScore(index) );

			// 敵も無効に.
			pEnemyMgr->Purge(index);

			// 撃墜音再生.
			pApp->PlaySE(SE_TEKI_DEAD);
		}
	}
}

// テクスチャ指定付きで、弾丸を描画する.
void BulletBuffer::Draw(MyTexture* pTex)
{
	D3DXVECTOR3 cnt(8, 8, 0);
	D3DXVECTOR3 pos(0, 0, 0);

	for (int i = 0; i < m_bulletMax; i++)
	{
		// 取得した弾丸が有効なら表示.
		if (m_pBuffer[i].IsActive())
		{
			// 現在位置をpos構造体型のデータに格納して表示.
			m_pBuffer[i].GetXY(pos.x, pos.y);
//			GetAppInst()->pSprite->Draw(pTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
			GetAppInst()->DrawSprite(pTex, &cnt, &pos);
		}
	}
}

// プレイヤーに敵の弾丸が衝突しているかの判定.
// 戻り値は、true=衝突  false=当たってない.
bool BulletBuffer::CollisionPlayer(float x, float y, float limit2)
{
	App* pApp = GetAppInst();
	// 各弾丸の位置(xx, yy)と、敵の距離が一定以下かどうか判定.
	for (int i = 0; i < m_bulletMax; i++)
	{
		BulletOne& bullet = m_pBuffer[i];		// bulletで置き換え.
		if (!bullet.IsActive()) {
			continue;
		}

		// 各弾丸の現在位置を取得、プレイヤーと衝突したか調べる.
		float bx, by;
		bullet.GetXY(bx, by);
		if (GetDistance2(bx, by, x, y) < limit2)
		{
			// 衝突したので、弾丸を無効に.
			bullet.Deactive();
			return true;
		}
	}
	return false;
}
