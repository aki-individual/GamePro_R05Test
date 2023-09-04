/*
	myEnemyManager.cpp
*/
#include "stdafx.h"
#include "myEnemyManager.h"
#include "myApp.h"

// デストラクタでは配列を解放する.
// 配列の中身をまず解放してから全体を解放.
EnemyManager::~EnemyManager()
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		if (m_ppBuffer[i] != NULL)
		{
			delete m_ppBuffer[i];
		}
	}
	delete[] m_ppBuffer;
	delete[] m_pTextBuff;
}

// メモリの確保等の初期化.
bool EnemyManager::Init()
{
	if (m_ppBuffer == NULL)
	{
		m_ppBuffer = new EnemyBase * [m_enemyMax];	// 配列の中身はEnemyBase*型.
		// ↑sizeof(EnemyBase*) を max 個分のメモリを確保しているだけで、コンストラクタは呼び出されない.
		// sizeof(EnemyBase*)はx64では１つあたり8バイト、x86では１つあたり4バイトのメモリサイズ.
		if (m_ppBuffer == NULL)
		{
			return false;
		}

		for (int i = 0; i < m_enemyMax; i++)
		{
			m_ppBuffer[i] = NULL;		// ポインタをNULLで初期化し、未使用状態とする.
		}
	}

	if (m_pTextBuff == NULL)
	{
		m_pTextBuff = new char[m_enemyMax + 1];	// デバッグ用文字列も確保する.
		// +1 は終端記号'\0'の分.
		if (m_pTextBuff == NULL)
		{
			return false;
		}
	}

	return true;
}

// 配列の全ての要素をリセットする.
void EnemyManager::ResetAll()
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		Purge(i);
	}
}

// 次の空き位置を探す.
int EnemyManager::Next()
{
	// リングバッファを、現在位置(m_index)から一周探索.
	for (int i = 0; i < m_enemyMax; i++)
	{
		// 現在位置(m_index)から空きを検索.
		int index = (m_index + i) % m_enemyMax;
		// index がm_enemyMax以上にならないよう、剰余にすることで配列の.
		// 現在位置から m_index - 1 の位置までを検索.
		if (m_ppBuffer[index] == NULL)
		{
			m_index = (index + 1) % m_enemyMax;
			return index;
		}
	}
	return -1;
}

// デバッグ用にm_pBuffer配列の中身をテキスト化する.
// 弾丸が有効：*.
// 弾丸が無効：-.
char* EnemyManager::GetText()
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		if (m_ppBuffer[i] != NULL)
		{
			m_pTextBuff[i] = m_ppBuffer[i]->Getch();
		}
		else
		{
			m_pTextBuff[i] = '-';
		}
	}
	// 文字列の最後は必ずNULL文字でターミネート.
	m_pTextBuff[m_enemyMax] = '\0';
	return m_pTextBuff;
}

// pEnemyを配列に追加.
// EnemyBase*型を引数にしているが、実際にはEnemyAAA*型やEnemyBBB*型が送られてくる.
// これらのクラスは、EnemyBaseの派生クラスなので、EnemyBase*型で受けることができる.
// 間違って引数を(EnemyBase pEnemy) と＊を付け忘れ、ポインタではなく実体で記述すると.
// データが一部欠落する現象が発生し深刻なバグの原因となる.
bool EnemyManager::Add(EnemyBase* pEnemy)
{
	int index = Next();		// 配列から空きを探査.
	if (0 <= index && index < m_enemyMax)
	{
		m_ppBuffer[index] = pEnemy;// 敵を管理用の配列に登録.
		return true;
	}
	return false;
}

// 敵配列のindex番目を無効にする.
// m_ppBuffer[index]は基本クラスのポインタだが、デストラクタをvirtual指定しているため.
// 適切な派生クラス(EnemyAAAやEnemyBBB)のデストラクタが呼び出される.
void EnemyManager::Purge(int index)
{
	if (0 <= index && index < m_enemyMax)
	{
		delete m_ppBuffer[index];
		m_ppBuffer[index] = NULL;// 配列の中身をNULLクリアして次の使用に備える.
	}
}

// 配列の各要素すべてに、処理を進めるよう指示する.
void EnemyManager::Update(float time)
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		if (m_ppBuffer[i] != NULL)
		{
			bool flag = m_ppBuffer[i]->Update(time);
			// FLASEが返るとこの要素は不要になる.
			if (!flag)
			{
				Purge(i);
			}
		}
	}
}

// 配列の全ての敵に対して、表示を促す.
void EnemyManager::Show()
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		// m_ppBuffer[i]の型はEnemyBase*だが、Show は virtual関数なので.
		// 派生クラスのshowが適切に呼び出される.
		// つまりifやswitchを使って分岐させる必要がない。ここがオブジェクト指向の利点の１つ.
		if (m_ppBuffer[i] != NULL)
		{
			m_ppBuffer[i]->Show();
		}
	}
}

void EnemyManager::Fire()
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		if (m_ppBuffer[i] != NULL)
		{
			m_ppBuffer[i]->Fire();
		}
	}
}

// (cx, cy)の位置は敵と衝突距離だったか.
// 衝突した場合は配列上の位置を引数pIndexが指場所に入れ、TRUEを返す.
// 衝突が無かった場合はFALSE返す.
bool EnemyManager::Collision(float cx, float cy, float limit2, int* pIndex)
{
	for (int i = 0; i < m_enemyMax; i++)
	{
		if (m_ppBuffer[i] != NULL)
		{
			if (m_ppBuffer[i]->Collision(cx, cy, limit2))
			{
				*pIndex = i;
				return true;
			}
		}
	}
	return false;
}

