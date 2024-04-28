#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <process.h>
#include <ctime>

#define NUM_OF_CUSTOMER 50
#define RANGE_MIN 10
#define RANGE_MAX (30 - RANGE_MIN)
#define TABLE_CNT 10

HANDLE hSemaphore;
DWORD randTimeArr[50];

void TakeMeal(DWORD time)
{
	// 해당 함수가 호출될 때마다, Semaphore 에서의 카운트가 하나씩 감소한다.
	// 세마 포어 카운듵가 0이 되면, 그 다음 임계영역을 들어가려는 쓰레드들은
	// 블로킹 상태가 된다.
	WaitForSingleObject(hSemaphore, INFINITE);

	_tprintf(_T("Enter Customer : %d\n"), GetCurrentThreadId());

	// 식사 중인 상태를 시뮬레이션 하는 함수
	Sleep(1000 * time);

	ReleaseSemaphore(
		// 반환하고자 하는 세마포어의 핸들
		hSemaphore,
		// 반환한다는 것은, 세마 포어 카운트의 증가를 의미한다.
		// 해당 인자를 통해 증가시킬 값의 크기를 결정할 수 있다.
		// 만약 세마포어 생성 시 결정한 최대 카운트 값(CreateSemaphore 3번째 인자)
		// 를 전달하게 되면, 카운트는 변경되지 않고 FALSE 만 반환
		1,
		// 변경되기 전 세마포어 카운트 값을 저장할 변수를 지정
		NULL
	);
}

unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
	TakeMeal((DWORD)lpParam);
	return 0;
};

int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadIDs[NUM_OF_CUSTOMER];
	HANDLE hThreads[NUM_OF_CUSTOMER];

	// random function seed 설정
	srand((unsigned)time(NULL));

	// 쓰레드에게 전달할 random 값 총 50개 생성
	// (손님마다 식사하는 시간이 다르다)
	for (int i = 0; i < NUM_OF_CUSTOMER; ++i)
	{
		randTimeArr[i] = (DWORD)(
			((double)rand() / (double)RAND_MAX)
			* RANGE_MAX + RANGE_MIN
			);
	}

	// 세마 포어 생성
	hSemaphore = CreateSemaphore(
		NULL, // 디폴트 보안 괸리자

		// 해당 값을 기반으로 임계 영역에 접근 가능한 쓰레드의 개수를 제한한다. 
		TABLE_CNT,  // 세마 포어 초기값

		// 세마포어가 지닐 수 있는 값의 최대 크기
		// 해당 값이 1 이 될 경우, 뮤텍스와 동일한 기능을 하는
		// 바이너리 세마포어가 구성된다.
		// 기본적으로 2번째 인자로 전달되는 값보다 커야 한다.
		TABLE_CNT, // 세마 포어 최대값

		// 세마포어의 이름
		NULL // unamed 세마 포어 구성
	);

	if (hSemaphore == NULL)
	{
		_tprintf(_T("Create Semaphore Error"));
	}

	// Customer 을 의미하는 쓰레드 생성
	for (int i = 0; i < NUM_OF_CUSTOMER; ++i)
	{
		hThreads[i] = (HANDLE)_beginthreadex(
			NULL, 
			0,
			ThreadProc,
			(void*)randTimeArr[i],
			CREATE_SUSPENDED,
			(unsigned*)&dwThreadIDs[i]
		);

		if (hThreads[i] == NULL)
		{
			_tprintf(_T("Create Thread Error"));
			return -1;
		}

	}

	for (int i = 0; i < NUM_OF_CUSTOMER; ++i)
	{
		ResumeThread(hThreads[i]);
	}

	WaitForMultipleObjects(
		NUM_OF_CUSTOMER, hThreads, TRUE,
		INFINITE
	);

	for (int i = 0; i < NUM_OF_CUSTOMER; ++i)
	{
		CloseHandle(hThreads[i]);
	}

	_tprintf(_T("--- End ---"));

	CloseHandle(hSemaphore);

	return 0;
}