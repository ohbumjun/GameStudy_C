#define _CRT_SECURE_NO_WARNINGS

// 실행 파일의 이름을
// ChildProcess.exe 로 만들어야 한다.
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	// idx 1번째로 들어오는 값은
	// 부모프로세스의 핸들값이다.
	HANDLE hParent = (HANDLE)_ttoi(argv[1]);

	// 부모 프로세스가 종료될 때까지 기다린다.
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