#include "stdafx.h"
#include "myTexture.h"


MyTexture* MyTexture::sm_pStack[MAX_TEXTURE];
int MyTexture::sm_maxTexture = 0;


// 画像を読み込む。カレントで失敗したら..から読み込む.
// pFname ファイル名。const修飾子をつけて関数内で値を変更しないことを宣言する.
// ppTex 作成するテクスチャへのポインタを入れるためのポインタ.
HRESULT LoadTexture(IDirect3DDevice9* pDev, LPCTSTR pFname, IDirect3DTexture9** ppTex)
{
	HRESULT hr = D3DXCreateTextureFromFile(pDev, pFname, ppTex);
	if (FAILED(hr))
	{
		// 失敗したので一つ上のフォルダからテクスチャを探してみる.
		int flen = (int)_tcslen(pFname) + 4;
		_tprintf(_T("%s: 親ディレクトリから探索...\n"), pFname);
		TCHAR* fn = new TCHAR[flen];
		_stprintf_s(fn, flen, _T("..\\%s"), pFname);
		hr = D3DXCreateTextureFromFile(pDev, fn, ppTex);
		delete[] fn;
		if (FAILED(hr))
		{
			// 失敗したので例外を発生させて終わる.
			MessageBox(NULL, _T("テクスチャ読み込み失敗"), pFname, MB_OK);
			throw hr;		// エラーが発生したので例外を送る.
		}
	}
	return hr;
}

// 画像を読み込むための関数.
// pDev D3Dデバイス.
// pFname テクスチャファイル名.
MyTexture* MyTexture::LoadTexture(IDirect3DDevice9* pDev, const TCHAR* pFname)
{
	if (sm_maxTexture >= MAX_TEXTURE)
	{
		return NULL;
	}

	// オブジェクトを生成.
	MyTexture* pMyTex = new MyTexture;
	// ::だけが付いた関数はグローバル関数を呼び出すことを意味する.
	::LoadTexture(pDev, pFname, &(pMyTex->m_pTex));
	sm_pStack[sm_maxTexture++] = pMyTex;// スタックに積む.
	return pMyTex;
}

void MyTexture::ReleaseAll()
{
	for (int i = sm_maxTexture - 1; i >= 0; i--)
	{
		if (sm_pStack[i])
		{
			delete sm_pStack[i];
		}
		sm_pStack[i] = NULL;
	}
	sm_maxTexture = 0;
}

// 表示に使うテクスチャの情報を設定する.
void NumberDisp::Init(MyTexture* pTex, int w, int h)
{
	m_pTex = pTex;
	m_width = w;
	m_height = h;
}

// 数字テクスチャを画像で表示する.
// maxには100000や1000のような10のN乗の数値（表示したい桁数の数値）を入れること.
void NumberDisp::Draw(ID3DXSprite* pSprite, int num, int max, float x, float y)
{
	num %= (max * 10);// numが最大桁を超えないよう調整.

	// numをmaxで割ると、最上位の桁の数値が0～9で得られる。これを桁数分表示する.
	// numをmaxで割った余りを次の計算で使う。maxも10で割り、次の桁の計算に備える.
	const D3DXVECTOR3 cnt(0, 0, 0);// 個々の画像の表示基準位置は左上に.
	while (max > 0)
	{
		// 最上位の数値を取り出す.
		int val = num / max;
		// 表示位置の決定.
		D3DXVECTOR3 pos(x, y, 0);
		// 表示したい画像領域（ＵＶ座標）を設定する.
		RECT rc = { val * m_width, 0, (val + 1) * m_width, m_height };
		//        {        left, top,               right,   bottom }
		pSprite->Draw(m_pTex->GetTexture(), &rc, &cnt, &pos, 0xFFFFFFFF);
		num %= max;	// 最上位を取り除く.
		max /= 10;		// 次の計算に備える.
		x += m_width;	// 表示位置の更新.
	}
}

