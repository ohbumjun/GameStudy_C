#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	// ���޵� ���ڰ� 3���� �ƴϸ� X
	if (argc != 3)
		return -1;

	DWORD start = _ttoi(argv[1]);
	DWORD end  = _ttoi(argv[2]);

	DWORD total = 0;

	for (DWORD i = start; i <= end; ++i)
		total += i;

	_tprintf(_T("child process total : %d\n"), total);

	return total;
}