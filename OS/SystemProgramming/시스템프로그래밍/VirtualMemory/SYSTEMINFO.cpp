/*
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	SYSTEM_INFO si;

	// Allocation Granularity Boundary: �޸� �Ҵ��� ���� �ּҰ� �� �� �ִ� �⺻ ����
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