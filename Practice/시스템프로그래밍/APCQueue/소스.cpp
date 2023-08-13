#define _WIN32_WINNT0x0400

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

VOID CALLBACK APCProc(ULONG_PTR);

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hThread = GetCurrentThread();

	QueueUserAPC(APCProc, hThread, (ULONG_PTR)1);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)2);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)3);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)4);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)5);

	Sleep(5000);

	// 알림 가능한 상태로 만들어주기
	SleepEx(INFINITE, TRUE);

	return 0;
}

VOID CALLBACK APCProc(ULONG_PTR dwParam)
{
	_tprintf(_T("Asynchronous procedure call num : %u \n"), (DWORD)dwParam);
}