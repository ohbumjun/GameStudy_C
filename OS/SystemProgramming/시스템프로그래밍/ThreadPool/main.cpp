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

// Work�� Thread ������ ���� ����ü
struct __ThreadPool
{
	// Work �� ����ϱ� ���� �迭 (���, ���� ť ���·� �����ص־� �Ѵ�)
	WORK workList[WORK_MAX];

	// Thread �� Thread �� Event Object
	WorkerThread workerThreadList[THREAD_MAX];
	// �ش� Event �� �ʿ��� ������, �Ҵ�� ���� ���� �����尡 Blocked ���¿� ���̰� �ϱ� �����̴�.
	HANDLE workerEventList[THREAD_MAX];

	// Work �� ���� ����
	DWORD idxOfCurrentWork; // ó���Ǿ���� Work �� ��ġ 
	DWORD idxOfLastAddedWork; // �߰��Ǿ�� �� Work �� ������ �� Idx 

	// Number Of Thread
	DWORD threadIdx;
} gThreadPool;


// Mutex ���� �Լ���
// gThreadPool �� ���� ������ ��ȣ�ϱ� ���� ����
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

	// Work ���
	gThreadPool.workList[gThreadPool.idxOfLastAddedWork++] = work;

	// Work ��� ����, ��� ���� ��������� ��� ���� ���� �����ϵ��� �Ѵ�.
	// �׷��� ��� ��� ���� �ʿ�� ����. �������� �������� �κ��̴�.
	for (DWORD i = 0; i < gThreadPool.threadIdx; ++i)
		SetEvent(gThreadPool.workerEventList[i]);

	ReleaseMutex();

	return 1;
}

// ThreadPool ���� Work �� ������ �� ȣ��
// gThreadPool �� ���� ������ ��ȣ�ϱ� ���� ����
WORK GetWorkFromPool(void)
{
	WORK work = NULL;

	AcquireMutex();

	// ���� ó���ؾ��� Work �� �������� �ʴ´ٸ�
	if (!(gThreadPool.idxOfCurrentWork < gThreadPool.idxOfLastAddedWork))
	{
		ReleaseMutex();
		return NULL;
	}

	work = gThreadPool.workList[gThreadPool.idxOfCurrentWork++];

	ReleaseMutex();

	return work;
}

// �ش� ������ŭ Thread ����
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

	// ������ ������ �� ��ȯ
	return numOfThread;
}

// ���޵Ǿ���� ���
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
			// Work �� �Ҵ�� ������ Blocked ���¿� �ְ� �ȴ�.
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

	// �ټ��� Work ���
	for (int i = 0; i < 100; ++i)
	{
		AddWorkToPool(TestFunction);
	}

	// main �����尡 ���� �Ҹ�Ǵ� ���� ����
	Sleep(50000);

	return 0;
}