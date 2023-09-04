/*
	myTexture.h
*/
#pragma once

const int MAX_TEXTURE = 256;

////////////////////////// MyTexture //////////////////////////
// テクスチャを管理するためのクラスを作成する.
// テクスチャが設定されていれば、デストラクタにて解放する.
// この結果、解放忘れを防ぐことができる.
// ただし、オブジェクト自体をdeleteしなければならないので.
// 相変わらず解放忘れには注意しなければならない.
class MyTexture
{
private:
	static MyTexture* sm_pStack[MAX_TEXTURE];
	static int sm_maxTexture;

protected:
	IDirect3DTexture9* m_pTex;			// 管理するテクスチャ.

private:
	// コンストラクタをクラス定義内で記述。メンバ変数pTexを NULL で初期化する.
	MyTexture() : m_pTex(NULL)
	{
	}
	// デストラクタでは、pTexを解放する。自動的に行われるため、解放忘れを防げる.
	// private領域に移動していることに注目してください.
	virtual ~MyTexture()
	{
		if (m_pTex)
		{
			m_pTex->Release();
		}
	}

public:
	// アクセッサ。pTexを取得するための関数.
	IDirect3DTexture9* GetTexture()
	{
		return m_pTex;
	}
	// MyTextureオブジェクトを作成しテクスチャを読み込む.
	// クラスメソッドになっている点に注意.
	static MyTexture* LoadTexture(IDirect3DDevice9* pDev, const TCHAR* pFname);
	static void ReleaseAll();
};

// NumberDisp //////////////////////////////////////
// 数値を表示するクラス：スコア表示やタイマー表示.
class NumberDisp
{
private:
	MyTexture* m_pTex;		// スコア表示に使用するテクスチャ（０～９の数字が横並び）.
	int m_width;			// 数字１つの横幅.
	int m_height;			// 数字１つの縦幅.

public:
	NumberDisp() : m_pTex(NULL), m_width(0), m_height(0) {}

	// 表示に使うテクスチャの情報を設定.
	void Init(MyTexture*, int w, int h);

	// 数字テクスチャを表示する.
	void Draw(ID3DXSprite*, int num, int max, float x, float y);
};

HRESULT LoadTexture(IDirect3DDevice9* pDev, LPCTSTR pFname, IDirect3DTexture9** ppTex);
