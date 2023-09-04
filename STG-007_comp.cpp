// STG-007_comp.cpp
// ソースファイルを整理して全体の見通しをよくする.

#include "stdafx.h"		// すべてのソースファイルで必ず先頭でインクルードするルールとする.
#include "myApp.h"		// 必要最小限の独自のヘッダファイルをインクルードする.

// argc コマンドラインから渡された引数の数.
// argv 引数の実体へのポインタ配列.
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Japanese");		// 日本語にロケールを設定.

#ifdef MEMORY_LEAK
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	App* pApp = GetAppInst();
	if (pApp->InitApp()) {
		pApp->MainLoop();
	}
	pApp->ReleaseDataSet();

#ifdef MEMORY_LEAK
	_CrtDumpMemoryLeaks();	// メモリリークを検出する.
#endif

	return 0;// 正常終了.
}
