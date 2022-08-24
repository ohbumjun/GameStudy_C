#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define WORK_MAX 10000
#define THREAD_MAX 50

typedef void (*WORK) (void);

DWORD AddWorkToPool(WORK work);
WORK GetWorkFromPool(void);
DWORD MakeThreadToPool(DWORD numOfThread);
void WorkerThreadFunction(LPVOID pParam);

typedef struct __WorkerThread
{
	HANDLE hThread;
	DWORD idThread;
} WorkerThread;

// Work와 Thread 관리를 위한 구조체
struct __ThreadPool
{
	// Work 를 등록하기 위한 배열 (사실, 원형 큐 형태로 구현해둬야 한다)
	WORK workList[WORK_MAX];

	// Thread 와 Thread 별 Event Object
	WorkerThread workerThreadList[THREAD_MAX];
	// 해당 Event 가 필요한 이유는, 할당된 일이 없는 쓰레드가 Blocked 상태에 놓이게 하기 위함이다.
	HANDLE workerEventList[THREAD_MAX];

	// Work 에 대한 정보
	DWORD idxOfCurrentWork; // 처리되어야할 Work 의 위치 
	DWORD idxOfLastAddedWork; // 추가되어야 할 Work 가 가지게 될 Idx 

	// Number Of Thread
	DWORD threadIdx;
} gThreadPool;


// Mutex 관련 함수들
// gThreadPool 에 대한 접근을 보호하기 위해 정의
static HANDLE mutex = NULL;

void InitMutex(void)
{
	mutex = CreateMutex(NULL, FALSE, NULL);
}

void DeInitMutex(void)
{
	BOOL ret = CloseHandle(mutex);
}

void AcquireMutex(void)
{
	DWORD ret = WaitForSingleObject(mutex, INFINITE);

	if (ret == WAIT_FAILED)
		_tprintf(_T("Error Occur ! \n"));
};

void ReleaseMutex(void)
{
	BOOL ret = ReleaseMutex(mutex);

	if (ret == 0)
		_tprintf(_T("Error Occur ! \n"));
}


DWORD AddWorkToPool(WORK work)
{
	AcquireMutex();

	if (gThreadPool.idxOfLastAddedWork >= WORK_MAX)
	{
		_tprintf(_T("Pool Full Of Work fail ! \n"));
		return NULL;
	}

	// Work 등록
	gThreadPool.workList[gThreadPool.idxOfLastAddedWork++] = work;

	// Work 등록 이후, 대기 중인 쓰레드들을 모두 깨워 일을 시작하도록 한다.
	// 그런데 사실 모두 깨울 필요는 없다. 정교함이 떨어지는 부분이다.
	for (DWORD i = 0; i < gThreadPool.threadIdx; ++i)
		SetEvent(gThreadPool.workerEventList[i]);

	ReleaseMutex();

	return 1;
}

// ThreadPool 에서 Work 를 가져올 때 호출
// gThreadPool 에 대한 접근을 보호하기 위해 정의
WORK GetWorkFromPool(void)
{
	WORK work = NULL;

	AcquireMutex();

	// 현재 처리해야할 Work 이 존재하지 않는다면
	if (!(gThreadPool.idxOfCurrentWork < gThreadPool.idxOfLastAddedWork))
	{
		ReleaseMutex();
		return NULL;
	}

	work = gThreadPool.workList[gThreadPool.idxOfCurrentWork++];

	ReleaseMutex();

	return work;
}

// 해당 개수만큼 Thread 생성
DWORD MakeThreadToPool(DWORD numOfThread)
{
	InitMutex();

	DWORD capacity = WORK_MAX - (gThreadPool.threadIdx);

	if (capacity < numOfThread)
		numOfThread = capacity;

	for (DWORD i = 0; i < numOfThread; ++i)
	{
		DWORD idThread;
		HANDLE hThread;

		gThreadPool.workerEventList[gThreadPool.threadIdx] = CreateEvent(NULL, FALSE, FALSE, NULL);

		hThread = CreateThread(
			NULL, 0,
			(LPTHREAD_START_ROUTINE)WorkerThreadFunction,
			(LPVOID)gThreadPool.threadIdx,
			0, &idThread
		);

		gThreadPool.workerThreadList[gThreadPool.threadIdx].hThread = hThread;
		gThreadPool.workerThreadList[gThreadPool.threadIdx].idThread = idThread;

		gThreadPool.threadIdx++;
	}

	// 생성된 쓰레드 수 반환
	return numOfThread;
}

// 전달되어야할 대상
void WorkerThreadFunction(LPVOID pParam)
{
	// pParam : idx

	WORK workFunction;
	HANDLE event = gThreadPool.workerEventList[(DWORD)pParam];

	while (1)
	{
		workFunction = GetWorkFromPool();

		if (workFunction == NULL)
		{
			// Work 가 할당될 때까지 Blocked 상태에 있게 된다.
			WaitForSingleObject(event, INFINITE);
			continue;
		}

		workFunction();
	}
}

LONG gTotalCount = 0;

void TestFunction()
{
	static int i = 0;

	InterlockedIncrement(&gTotalCount);

	_tprintf(_T("Good Test : %d , Processing Thread : %d -- \n"), gTotalCount, GetCurrentThreadId());
}

int _tmain(int argc, TCHAR* argv[])
{
	MakeThreadToPool(3);

	// 다수의 Work 등록
	for (int i = 0; i < 100; ++i)
	{
		AddWorkToPool(TestFunction);
	}

	// main 쓰레드가 먼저 소멸되는 현상 방지
	Sleep(50000);

	return 0;
}