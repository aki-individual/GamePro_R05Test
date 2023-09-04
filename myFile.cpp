/*
	myFile.cpp
*/
#include "stdafx.h"
#include "myFile.h"

////////////////////////// OpenFile //////////////////////////
// �J�����g�Ɛe�f�B���N�g������t�@�C����T��.
FILE* OpenFile(LPCTSTR pFname, LPCTSTR pMode)
{
	FILE* fp;
	int err = _tfopen_s(&fp, pFname, pMode);
	if (err == 0)
	{
		return fp;
	}

	// �e���炳����.
	_tprintf(_T("%s: �e�f�B���N�g������T��...\n"), pFname);
	int flen = (int)_tcslen(pFname) + 4;
	TCHAR* fn = new TCHAR[flen];
	_stprintf_s(fn, flen, _T("..\\%s"), pFname);
	err = _tfopen_s(&fp, fn, pMode);
	delete[] fn;
	if (err == 0)
	{
		return fp;
	}
	return NULL;
}

