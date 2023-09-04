/*
	myGameStage.cpp
*/
#include "stdafx.h"
#include "myGameStage.h"
#include "myFile.h"
#include "myEnemyManager.h"
#include "myEnemyA.h"
#include "myEnemyB.h"
#include "myEnemyC.h"
#include "myApp.h"

// メモリ解放.
// ステージデータを何度も読むことを想定し、開放できるようにしておく.
// 解放は、最初に配列の個々の中身を解放してから、配列を解放する.
void GameStage::Release()
{
	if (m_ppStageData != NULL)
	{
		for (int i = 0; i < m_stageLines; i++)
		{
			delete[] m_ppStageData[i];
		}
		delete[] m_ppStageData;
		m_ppStageData = NULL;
	}
}

// ファイルからのステージデータの読み込み
void GameStage::ReadStage(const TCHAR* pFname)
{
	Release();// 現在のメモリを解放.

	FILE* fp = OpenFile(pFname, _T("r"));
	if (fp == NULL)
	{
		MessageBox(NULL, _T("ステージファイルがありません"), pFname, MB_OK);
		throw _T("ファイルオープンエラー");
	}

	// まずファイルをすべて読んで、行数を数える。１行は1024文字未満でなければならない.
	int lines = 0;
	char buf[1024];
	while (fgets(buf, sizeof(buf), fp))
	{
		lines++;
	}
	fseek(fp, 0, SEEK_SET);		// 先頭に戻す.

	// ここでlinesは全体の行数を意味している.
	m_ppStageData = new char* [lines];// charポインタの配列を確保.
	m_stageLines = lines;				// 行数を覚えておく.

	int current = 0;// 読み込んだ行の総数.

	// ステージデータを１行ずつ読んで記録する.
	while (fgets(buf, sizeof(buf), fp))
	{
		// 行の文字数を調べ、その分のメモリを確保する.
		// その後読み込んだ文字列をコピーする.
		size_t len = strlen(buf);// 今読んだ行の長さ.
		m_ppStageData[current] = new char[len + 1];// メモリを確保.
		sprintf_s(m_ppStageData[current], len + 1, "%s", buf);// 文字列をコピー.
		current++;
	}
	fclose(fp);

	m_currentLine = 0;// 現在の行位置をゼロにリセット.

	// debug用、読み込んだデータを表示してみる.
	for (int i = 0; i < m_stageLines; i++)
	{
		printf("%s", m_ppStageData[i]);
	}
}

// ステージを進行する。呼び出されるたびに現在行の文字列を解析する.
// 解析後にステージデータを１行進行し次回の呼び出しに備える.
bool GameStage::Update()
{
	App* pApp = GetAppInst();

	// 現在位置がステージ全体の行数を超えていたらFALSEを返し.
	// ステージが終わったことを知らせる.
	if (m_currentLine >= m_stageLines)
	{
		return false;
	}

	// ステージデータを解析（現在行の取り出し）.
	char* pLine = m_ppStageData[m_currentLine];
	// 行は、"A100 C300 A500"のような文字列になっているので.
	// 一文字ずつ解析しながら敵を生成する.
	for (char* pch = pLine; *pch != '\0'; pch++)
	{
		char tokenType = 0;	// 「A100」の A の部分。ゼロ初期化しておく.
		float digit = 0;	// 「A100」の 100 の部分（出現する敵のx座標、オフセット）.

		// まずアルファベット部分を解析.
		if (isalpha(*pch) || *pch == '*' || *pch == '#' )
		{
			tokenType = *pch;
			pch++;// 一文字進める.
		}

		// 続く数字部分を解析.
		if (isdigit(*pch))
		{
			digit = (float)atof(pch);
			while (isdigit(*pch) || *pch == '.')
			{
				pch++;
			}
		}

		// トークンの種類により、処理を変える.
		switch (tokenType)
		{
			// 行ジャンプ.
			case '#':
			{
				// #10 のような場合は次の処理行が10行目になる.
				// switchを抜けた後m_currentLine++があるので digit-1 を入れておく.
				m_currentLine = (int)(digit - 1);
				break;
			}

			// 敵Ａの生成.
			case 'A':
			{
				pApp->CreateEnemyA(digit);
				break;
			}

			// 敵Ｂの生成.
			case 'B':
			{
				pApp->CreateEnemyB(digit);
				break;
			}

			// 敵Ｃの生成.
			case 'C':
			{
				pApp->CreateEnemyC(digit);
				break;
			}

			// 敵Dの生成.
			case 'D':
			{
				pApp->CreateEnemyD(digit);
				break;
			}

			// 敵Eの生成.
			case 'E':
			{
				pApp->CreateEnemyE(digit);
				break;
			}

			// 敵Fの生成.
			case 'F':
			{
				pApp->CreateEnemyF(digit);
				break;
			}

			// BGMを鳴らす.
			case '*':
			{
				SoundBGM bgm = (SoundBGM)digit;
				if (BGM_BEGIN <= bgm && bgm < BGM_COUNT)
				{
					// 全BGM停止.
					GetAppInst()->StopAllBGM();
					if (pApp->GetGameState() == GAME_STATE_PLAYING)
					{
						pApp->PlayBGM(bgm, true);
					}
				}
				break;
			}
		}
	}
	m_currentLine++;
	return true;
}
