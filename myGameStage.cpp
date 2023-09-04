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

// ���������.
// �X�e�[�W�f�[�^�����x���ǂނ��Ƃ�z�肵�A�J���ł���悤�ɂ��Ă���.
// ����́A�ŏ��ɔz��̌X�̒��g��������Ă���A�z����������.
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

// �t�@�C������̃X�e�[�W�f�[�^�̓ǂݍ���
void GameStage::ReadStage(const TCHAR* pFname)
{
	Release();// ���݂̃����������.

	FILE* fp = OpenFile(pFname, _T("r"));
	if (fp == NULL)
	{
		MessageBox(NULL, _T("�X�e�[�W�t�@�C��������܂���"), pFname, MB_OK);
		throw _T("�t�@�C���I�[�v���G���[");
	}

	// �܂��t�@�C�������ׂēǂ�ŁA�s���𐔂���B�P�s��1024���������łȂ���΂Ȃ�Ȃ�.
	int lines = 0;
	char buf[1024];
	while (fgets(buf, sizeof(buf), fp))
	{
		lines++;
	}
	fseek(fp, 0, SEEK_SET);		// �擪�ɖ߂�.

	// ������lines�͑S�̂̍s�����Ӗ����Ă���.
	m_ppStageData = new char* [lines];// char�|�C���^�̔z����m��.
	m_stageLines = lines;				// �s�����o���Ă���.

	int current = 0;// �ǂݍ��񂾍s�̑���.

	// �X�e�[�W�f�[�^���P�s���ǂ�ŋL�^����.
	while (fgets(buf, sizeof(buf), fp))
	{
		// �s�̕������𒲂ׁA���̕��̃��������m�ۂ���.
		// ���̌�ǂݍ��񂾕�������R�s�[����.
		size_t len = strlen(buf);// ���ǂ񂾍s�̒���.
		m_ppStageData[current] = new char[len + 1];// ���������m��.
		sprintf_s(m_ppStageData[current], len + 1, "%s", buf);// ��������R�s�[.
		current++;
	}
	fclose(fp);

	m_currentLine = 0;// ���݂̍s�ʒu���[���Ƀ��Z�b�g.

	// debug�p�A�ǂݍ��񂾃f�[�^��\�����Ă݂�.
	for (int i = 0; i < m_stageLines; i++)
	{
		printf("%s", m_ppStageData[i]);
	}
}

// �X�e�[�W��i�s����B�Ăяo����邽�тɌ��ݍs�̕��������͂���.
// ��͌�ɃX�e�[�W�f�[�^���P�s�i�s������̌Ăяo���ɔ�����.
bool GameStage::Update()
{
	App* pApp = GetAppInst();

	// ���݈ʒu���X�e�[�W�S�̂̍s���𒴂��Ă�����FALSE��Ԃ�.
	// �X�e�[�W���I��������Ƃ�m�点��.
	if (m_currentLine >= m_stageLines)
	{
		return false;
	}

	// �X�e�[�W�f�[�^����́i���ݍs�̎��o���j.
	char* pLine = m_ppStageData[m_currentLine];
	// �s�́A"A100 C300 A500"�̂悤�ȕ�����ɂȂ��Ă���̂�.
	// �ꕶ������͂��Ȃ���G�𐶐�����.
	for (char* pch = pLine; *pch != '\0'; pch++)
	{
		char tokenType = 0;	// �uA100�v�� A �̕����B�[�����������Ă���.
		float digit = 0;	// �uA100�v�� 100 �̕����i�o������G��x���W�A�I�t�Z�b�g�j.

		// �܂��A���t�@�x�b�g���������.
		if (isalpha(*pch) || *pch == '*' || *pch == '#' )
		{
			tokenType = *pch;
			pch++;// �ꕶ���i�߂�.
		}

		// �����������������.
		if (isdigit(*pch))
		{
			digit = (float)atof(pch);
			while (isdigit(*pch) || *pch == '.')
			{
				pch++;
			}
		}

		// �g�[�N���̎�ނɂ��A������ς���.
		switch (tokenType)
		{
			// �s�W�����v.
			case '#':
			{
				// #10 �̂悤�ȏꍇ�͎��̏����s��10�s�ڂɂȂ�.
				// switch�𔲂�����m_currentLine++������̂� digit-1 �����Ă���.
				m_currentLine = (int)(digit - 1);
				break;
			}

			// �G�`�̐���.
			case 'A':
			{
				pApp->CreateEnemyA(digit);
				break;
			}

			// �G�a�̐���.
			case 'B':
			{
				pApp->CreateEnemyB(digit);
				break;
			}

			// �G�b�̐���.
			case 'C':
			{
				pApp->CreateEnemyC(digit);
				break;
			}

			// �GD�̐���.
			case 'D':
			{
				pApp->CreateEnemyD(digit);
				break;
			}

			// �GE�̐���.
			case 'E':
			{
				pApp->CreateEnemyE(digit);
				break;
			}

			// �GF�̐���.
			case 'F':
			{
				pApp->CreateEnemyF(digit);
				break;
			}

			// BGM��炷.
			case '*':
			{
				SoundBGM bgm = (SoundBGM)digit;
				if (BGM_BEGIN <= bgm && bgm < BGM_COUNT)
				{
					// �SBGM��~.
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
