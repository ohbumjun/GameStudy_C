#define _CRT_SECURE_NO_WARNINGS

// 환경 변수 설정하는 부모 프로세스
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	// 프로세스 환경 변수를 등록할 때 사용하는 함수
	// key ~ value
	SetEnvironmentVariable(_T("Good"), _T("morning"));
	SetEnvironmentVariable(_T("Hey"), _T("Ho"));
	SetEnvironmentVariable(_T("Big"), _T("Boy"));

	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi = { 0, };
	si.cb = sizeof(si);

	CreateProcess(
		NULL, (LPWSTR)("EnvChild"),
		NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE |
		CREATE_UNICODE_ENVIRONMENT,
		
		// 아래 인자를 통해서, 자식 프로세스에게
		// 등록하고픈, 환경변수를 설정할 수 있다.
		NULL,  // 부모 프로세스 환경변수 등록
		NULL, &si, &pi
	);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}