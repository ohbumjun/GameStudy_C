#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <process.h>

#define NUM_OF_GATE 6

LONG gTotalCount = 0;

// CRITICAL_SECTION gCriticalSection;
HANDLE hMutex;

void IncreaseCount()
{
	// EnterCriticalSection();
	// 해당 hMutex 커널 오브젝트의 상태가 
	// Signaled 상태가 될 때까지 기다린다.
	// Signaled 상태가 되면, 아래의 임계 영역을 들어가고
	// hMutex 커널 오브젝트는, 다시 Non-Signaled 상태가 된다.
	WaitForSingleObject(hMutex, INFINITE);

	gTotalCount++;

	// Leave Critical Section
	// 임계 영역을 나오면서, hMutex 커널 오브젝트의 상태를
	// Signaled 상태로 바꿔준다.
	ReleaseMutex(hMutex);
};

unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
	for (DWORD i = 0; i < 1000; ++i)
	{
		IncreaseCount();
	}

	return 0;
}

int _tmain(int argc, TCHAR** argv)
{
	DWORD dwThreadID[NUM_OF_GATE];
	HANDLE hThreads[NUM_OF_GATE];

	// Initialize Critical Section
	hMutex = CreateMutex(
		// LPSECURITI_ATTRIBUTES
		// 프로세스 생성시 보안 속성을 지정했던 것처럼
		// 보안 속성을 지정해줄 수 있다.
		// 프로세스도 커널 오브젝트이고, 뮤텍스도 커널 오브젝트이기 때문이다.
		NULL, 
		// 열쇠에 해당하는 크리티컬 섹션 오브젝트는 생성 후 초기화 되고 나면
		// 누구든 이 열쇠를 먼저 소유하는 쓰레드
		// 즉, EnterCriticalSection 함수를 먼저 호출하는 쓰레드가
		// 임계 영역에 들어갔다.
		// 뮤텍스는 뮤텍스 오브젝트를 생성하는 쓰레드에게 기회를 먼저 줄 수 있다.
		// FALSE 를 전달하면 크리티컬 섹션 처럼 먼저 차지하는 사림이
		// 임자가 되게 할 수도 있고
		// TRUE 를 전달하면, 뮤텍스를 생성하는 쓰레드가 먼저 
		// 기회를 얻을 수 있다.
		FALSE,

		// 뮤텍스에 이름을 붙여주기 위함
		// 해당 항목을 통해서, 이름있는 뮤텍스 기반 동기화 기법을 수행
		NULL
	);
	// 여러개 쓰레드 생성
	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		hThreads[i] = (HANDLE)_beginthreadex(
			NULL, 0,
			ThreadProc, NULL,
			CREATE_SUSPENDED, // 처음에는 Blocked 상태로
			(unsigned*)&hThreads[i]
		);

		if (hThreads[i] == NULL)
		{
			return -1;
		}
	}

	// 모두 Ready 상태로 바꿔준다.
	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		ResumeThread(hThreads[i]);
	}

	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		CloseHandle(hThreads[i]);
	}
	
	// DeleteCriticalSection(&hCritic);
	CloseHandle(hMutex);

	return 0;
}