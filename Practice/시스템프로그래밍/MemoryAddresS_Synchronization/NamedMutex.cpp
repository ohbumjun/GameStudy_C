#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <process.h>

#define NUM_OF_GATE 6

HANDLE hMutex;
DWORD dwWaitResult;

void ProcessBaseCriticalSection()
{
	// 뮤텍스 소유 시도를 하고
	// 이후 Thread Running 을 일정 시간 간격 출력한다.
	dwWaitResult = WaitForSingleObject(hMutex, INFINITE);

	switch (dwWaitResult)
	{
		// 쓰레드가 뮤텍스를 소유
	case WAIT_OBJECT_0 :
	{
		_tprintf(_T("thread got mutex\n"));
		break;
	}
		// time-out 발생
	case WAIT_TIMEOUT:
	{
		_tprintf(_T("timer expired\n"));
		return;
	}
		// 뮤텍스 반환이 적절히 이뤄지지 않았다.
	// 뮤텍스 소유자가 반환을 적절히 하지 않은 상황에서 
	// 반환되는 값이다.
	case WAIT_ABANDONED:
	{
		return;
	}
	}

	for (DWORD i = 0; i < 5; ++i)
	{
		_tprintf(_T("Thread Running !\n"));
		Sleep(1000);
	}

	ReleaseMutex(hMutex);
}

int _tmain(int argc, TCHAR* argv[])
{
#if 0
	hMutex = CreateMutex(
		NULL,
		FALSE,
		_T("NamedMutex")
	);
#else
	hMutex = OpenMutex(
		MUTEX_ALL_ACCESS, // 접근 권한 요청
		FALSE, // 핸들 상속 X
		_T("NamedMutex") // 뮤텍스 오브젝트 이름
	);
#endif

	if (hMutex == NULL)
	{
		_tprintf(_T("Create Mutex Error\n"));
		return -1;
	}

	ProcessBaseCriticalSection();

	CloseHandle(hMutex);

	return 0;
}