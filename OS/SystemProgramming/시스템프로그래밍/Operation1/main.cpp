#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// Operation2.exe 를 실행시키고 나서
// Operation1.exe 라는 문자열을 계속 출력
int _tmain(int argc, TCHAR* argv[])
{
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);

	TCHAR command[] = _T("Operation2.exe");

	CreateProcess(NULL, command, NULL, NULL,
		TRUE, 0, NULL, NULL, &si, &pi);

	while(1)
	{
		for (DWORD i = 0; i < 10000; ++i)
			for (DWORD j = 0; j < 10000; ++j) // BusyWaiting
			{}

		_fputts(_T("Operation1.exe\n"), stdout);
	}

	return 0;
}