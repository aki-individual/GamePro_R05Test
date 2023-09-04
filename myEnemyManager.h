/*
	myEnemyManager.h
*/
#pragma once

#include "myEnemyBase.h"

//////////////////////////////////
// EnemyManager
// 敵全体を管理する.
class EnemyManager
{
private:
	EnemyBase** m_ppBuffer;	// 敵の配列、リングバッファとして使う.
	int m_enemyMax;			// 配列の最大値.
	int m_index;			// 配列の使用現在位置.
	char* m_pTextBuff;		// デバッグ用に配列の状況を文字列にする.

	int Next();				// 次の位置を確保する（内部からしかコールしないのでprivate関数とする）.

public:
	EnemyManager(int max)
		: m_ppBuffer(NULL)
		, m_enemyMax(max)
		, m_index(0)
		, m_pTextBuff(NULL)
	{}

	~EnemyManager();

	bool Init();					// メモリの確保等の初期化.
	void ResetAll();		// 配列の全ての要素をリセットする.
	bool Add(EnemyBase*);			// 配列への敵キャラの追加.
	void Update(float t);	// 敵配列全体を更新.
	void Purge(int index);			// 配列から位置indexの敵を取り除く.
	void Show();			// 敵キャラクタを表示する.
	void Fire();					// 敵の攻撃.
	bool Collision(float cx, float cy, float limit2, int* pIndex);
	// ↑位置(cx, cy)からみて距離limit2以内の敵をpIndexに返却.
	char* GetText();				// デバッグ用に配列の状況を文字列化する.
	// スコアを取得する
	int GetScore(int index)
	{
		return m_ppBuffer[index]->Score();
	}
};
