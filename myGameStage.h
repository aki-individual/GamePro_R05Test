/*
	myGameStage.h
*/
#pragma once

//////////////////////////////////
// GameStage
class GameStage
{
private:
	char** m_ppStageData;	// �X�e�[�W�f�[�^�A�e�L�X�g�f�[�^�Ƃ��ĊǗ�.
	int m_stageLines;		// �X�e�[�W�f�[�^�̍s��.
	int m_currentLine;		// ���݂܂ł̐i�s�ʒu.

public:
	GameStage() : m_ppStageData(NULL), m_stageLines(0), m_currentLine(0) {}
	~GameStage() { Release(); }
	void Rewind() { m_currentLine = 0; }	// �X�e�[�W���ŏ��ɖ߂�.
	int GetCurrent() { return m_currentLine; }

	void Release();
	void ReadStage(const TCHAR* fname);	// �t�@�C������X�e�[�W�f�[�^��ǂ�.
	bool Update();						// �X�e�[�W��i�߂�(���̍s�ɐi��).
};
