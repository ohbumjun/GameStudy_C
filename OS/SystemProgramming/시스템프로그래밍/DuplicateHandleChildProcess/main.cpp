#define _CRT_SECURE_NO_WARNINGS

// ���� ������ �̸���
// ChildProcess.exe �� ������ �Ѵ�.
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	// idx 1��°�� ������ ����
	// �θ����μ����� �ڵ鰪�̴�.
	HANDLE hParent = (HANDLE)_ttoi(argv[1]);

	// �θ� ���μ����� ����� ������ ��ٸ���.
	DWORD isSuccess = WaitForSingleObject(hParent, INFINITE);

	if (isSuccess == WAIT_FAILED)
	{
		_tprintf(_T("Wait Failed returned"));
		Sleep(1000);
		return -1;
	}
	else
	{
		_tprintf(
			_T("General Lee Said, \ Don't Inform")
		_T("The Enemy My Death\n"));
		Sleep(1000);
		return 0;
	}
}