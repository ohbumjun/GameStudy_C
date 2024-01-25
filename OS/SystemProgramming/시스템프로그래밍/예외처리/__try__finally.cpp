#define _CRT_SECURE_NO_WARNINGS
/*
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	int a, b;

	__try
	{
		_tprintf(_T("a, b : "));
		_tscanf(_T("%d, %d"), &a, &b);

		if (b == 0)
			return -1;
	}
	__finally
	{
		_tprintf(_T("__finally block\n"));
	}

	_tprintf(_T("result : %d\n"), a / b);

	return 0;
}
*/