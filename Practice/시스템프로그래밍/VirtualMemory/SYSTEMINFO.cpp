/*
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	SYSTEM_INFO si;

	// Allocation Granularity Boundary: 메모리 할당의 시작 주소가 될 수 있는 기본 단위
	DWORD allocGranularity;
	DWORD pageSize;

	GetSystemInfo(&si);	
	pageSize = si.dwPageSize;
	allocGranularity = si.dwAllocationGranularity;

	_tprintf(_T("Page Size : %u KByte \n"), pageSize / 1024);
	_tprintf(
		_T("Allocation Granularity Boundary : %u KByte \n"),
		allocGranularity / 1024);

	return 1;
}
*/