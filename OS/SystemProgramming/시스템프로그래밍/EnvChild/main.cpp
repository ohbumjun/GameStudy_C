#define _CRT_SECURE_NO_WARNINGS

// 환경변수 참조하는 자식 프로세스
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define BUFSIZE 1024

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR value[BUFSIZE];

	// 등록된 환경 변수를 참조한다.
	if (GetEnvironmentVariable(_T("Good"), value, BUFSIZE) > 0)
		_tprintf(_T("[%s = %s] \n"), _T("Good"), value);

	if (GetEnvironmentVariable(_T("Hey"), value, BUFSIZE) > 0)
		_tprintf(_T("[%s = %s] \n"), _T("Hey"), value);

	if (GetEnvironmentVariable(_T("Big"), value, BUFSIZE) > 0)
		_tprintf(_T("[%s = %s] \n"), _T("Big"), value);

	Sleep(1000);

	return 0;
}