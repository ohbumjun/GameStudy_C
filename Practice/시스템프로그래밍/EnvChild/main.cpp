#define _CRT_SECURE_NO_WARNINGS

// ȯ�溯�� �����ϴ� �ڽ� ���μ���
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define BUFSIZE 1024

int _tmain(int argc, TCHAR* argv[])
{
	TCHAR value[BUFSIZE];

	// ��ϵ� ȯ�� ������ �����Ѵ�.
	if (GetEnvironmentVariable(_T("Good"), value, BUFSIZE) > 0)
		_tprintf(_T("[%s = %s] \n"), _T("Good"), value);

	if (GetEnvironmentVariable(_T("Hey"), value, BUFSIZE) > 0)
		_tprintf(_T("[%s = %s] \n"), _T("Hey"), value);

	if (GetEnvironmentVariable(_T("Big"), value, BUFSIZE) > 0)
		_tprintf(_T("[%s = %s] \n"), _T("Big"), value);

	Sleep(1000);

	return 0;
}