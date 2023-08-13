/*
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

#define NUM_OF_GATE 6

LONG gTotalCount = 0;

CRITICAL_SECTION hCritic;

void IncreaseCount()
{
	// 전역으로 선언된 
	// 즉, 해당 함수들을 통해서, 여러개의 쓰레드가 값을 하나, 증가 및 감소 시킬 경우
	// 동시에 둘 이상의 쓰레드 접근에 의한 문제를 막을 수 있다.
	// gTotalCount += 1;
	InterlockedIncrement(&gTotalCount);
}

unsigned int WINAPI ThreadProc(LPVOID Param)
{
	for (DWORD i = 0; i < 1000; ++i)
	{
		IncreaseCount();
	}

	// 메모리 해제 + 쓰레드 정상 종료
	return 0;
};

int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadId[NUM_OF_GATE];
	HANDLE hThread[NUM_OF_GATE];

	InitializeCriticalSection(&hCritic);

	// 여러개 쓰레드 생성
	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		hThread[i] = (HANDLE)_beginthreadex(
			NULL, 0,
			ThreadProc, NULL,
			CREATE_SUSPENDED, // 처음에는 Blocked 상태로
			(unsigned*)&dwThreadId[i]
		);

		if (hThread[i] == NULL)
		{
			return -1;
		}
	}

	// 모두 Ready 상태로 바꿔준다.
	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		ResumeThread(hThread[i]);
	}

	// 모든 쓰레드들이 종료될 때까지 기다린다.
	WaitForMultipleObjects(
		NUM_OF_GATE, hThread, TRUE, INFINITE
	);

	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		CloseHandle(hThread[i]);
	}

	DeleteCriticalSection(&hCritic);

	return 0;
}

*/