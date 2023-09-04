/*
	myGameStage.h
*/
#pragma once

//////////////////////////////////
// GameStage
class GameStage
{
private:
	char** m_ppStageData;	// ステージデータ、テキストデータとして管理.
	int m_stageLines;		// ステージデータの行数.
	int m_currentLine;		// 現在までの進行位置.

public:
	GameStage() : m_ppStageData(NULL), m_stageLines(0), m_currentLine(0) {}
	~GameStage() { Release(); }
	void Rewind() { m_currentLine = 0; }	// ステージを最初に戻す.
	int GetCurrent() { return m_currentLine; }

	void Release();
	void ReadStage(const TCHAR* fname);	// ファイルからステージデータを読む.
	bool Update();						// ステージを進める(次の行に進む).
};
