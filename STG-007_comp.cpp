// STG-007_comp.cpp
// �\�[�X�t�@�C���𐮗����đS�̂̌��ʂ����悭����.

#include "stdafx.h"		// ���ׂẴ\�[�X�t�@�C���ŕK���擪�ŃC���N���[�h���郋�[���Ƃ���.
#include "myApp.h"		// �K�v�ŏ����̓Ǝ��̃w�b�_�t�@�C�����C���N���[�h����.

// argc �R�}���h���C������n���ꂽ�����̐�.
// argv �����̎��̂ւ̃|�C���^�z��.
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Japanese");		// ���{��Ƀ��P�[����ݒ�.

#ifdef MEMORY_LEAK
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	App* pApp = GetAppInst();
	if (pApp->InitApp()) {
		pApp->MainLoop();
	}
	pApp->ReleaseDataSet();

#ifdef MEMORY_LEAK
	_CrtDumpMemoryLeaks();	// ���������[�N�����o����.
#endif

	return 0;// ����I��.
}
