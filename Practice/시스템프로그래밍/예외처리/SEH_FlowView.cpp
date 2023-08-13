/*
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	_tprintf(_T("start point !\n"));

	int* p = NULL;

	__try
	{
		// 예외 발생 지점
		*p = 100;

		_tprintf(_T("value : %d \n"), *p);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		_tprintf(_T("exception occured !\n"));
	}

	_tprintf(_T("end point ! \n"));
		
	return 0;
}
*/