/*
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

DWORD FilterFunction(DWORD exptType);

int _tmain(int argc, TCHAR* argv[])
{
	int* p = NULL;
	int sel = 0;

	while (1)
	{
		_tprintf(_T("1 for memory access exception\n"));
		_tprintf(_T("2 for divide by 0 exception\n"));

		_tscanf(_T("%d"), &sel);
			
		if (sel == 3)
			break;

		__try
		{
			if (sel == 1)
			{
				*p = 100; // 예외 발생 지점
			}
			else // sel == 2
			{
				int n = 0;
				n = 7 / n; // 예외 발생 지점
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			DWORD exptType = GetExceptionCode();

			switch (exptType)
			{
			case EXCEPTION_ACCESS_VIOLATION :
			{
				_tprintf(_T("Access Violation\n"));
				break;
			}
			case EXCEPTION_INT_DIVIDE_BY_ZERO :
			{
				_tprintf(_T("Divide By Zero\n"));
				break;
			}
			}
		}
	}
}
*/