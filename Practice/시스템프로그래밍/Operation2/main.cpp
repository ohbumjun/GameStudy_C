#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	while (1)
	{
		for (DWORD i = 0; i < 10000; ++i)
			for (DWORD j = 0; j < 10000; ++j) // BusyWaiting
			{
			}

		_fputts(_T("Operation2.exe\n"), stdout);
	}

	return 0;
}