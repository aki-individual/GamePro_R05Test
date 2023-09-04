/*
	myEnemyBase.h
*/
#pragma once

#include "myTexture.h"

//////////////////////////////////
// EnemyBase
// 敵１体を管理する基本クラス.
class EnemyBase
{
//private:
protected:
	// 以下の変数は「派生クラス」から頻繁にアクセスされるため.
	// privateではなくprotectedに指定しておく.
	MyTexture* m_pTex;		// 表示するテクスチャ.
	float m_posX, m_posY;	// 現在位置.
	float m_velX, m_velY;	// XY方向への速度成分.
	float m_timer;			// 生成してからの時刻.

	// このクラスは抽象クラスなので、派生クラス以外からはnewできないようにする.
	// そのため、コンストラクタをprotectedエリアに記述する.
	EnemyBase(MyTexture* pTex)
		: m_pTex(pTex)
		, m_posX(0.0f)
		, m_posY(0.0f)
		, m_velX(0.0f)
		, m_velY(0.0f)
		, m_timer(0.0f)
	{
		//_tprintf(_T("calll EnemyBase constractor\n"));
	}

public:
	// デストラクタは、子以外からもアクセスされる可能性があるため「public」.
	// 派生元として設計しているのでデストラクタをvirtual指定しておく.
	virtual ~EnemyBase()
	{}

	// 敵キャラの動作.
	virtual bool Update(float time);

	// デバッグ用に文字を返す.
	virtual char Getch()
	{
		return '.';
	}

	// 敵を位置(posX, posY)、移動速度(vx, vy)で初期化し出現させる.
	virtual void Init(float posX, float posY, float vx, float vy);
	// キャラクタを表示する.
	virtual void Show();
	// 位置(cx,cy)との距離がlimit2以下かどうかを判定.
	virtual bool Collision(float cx, float cy, float limit2);

	// スコアを返す。メンバ関数の実体を定義せず純粋仮想関数とする.
	virtual int Score() = 0;// 派生クラスでは必ず定義が必要となる.
	// 攻撃する。基本クラスでの攻撃方法は定義せず純粋仮想関数とする.
	virtual void Fire() = 0;
};

// パターンムーブのための構造体。時刻(frame)における位置(posX, posY)を示す.
// キーフレームで指定するので、キー間は補間しなければならない.
struct PatternMove
{
	float frame;		// 時刻（フレーム）.
	float posX, posY;	// 位置.
};

extern const RECT g_EnemyImageArea[];
extern const PatternMove g_ptn1[];
extern const PatternMove g_ptn2[];
extern const PatternMove g_ptn3[];
extern const PatternMove g_ptn4[];