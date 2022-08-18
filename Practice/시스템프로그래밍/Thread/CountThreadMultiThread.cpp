#include <stdio.h>
#include <tchar.h>
#include <process.h>
#include <windows.h>

// _beginthreadex 버전
// - 내부적으로 CreateThread 를 호출한다.
// - 다만 쓰레드를 생성하기에 앞서서 쓰레드를 위해
// 독립적인 메모리 블록을 할당한다는 차이점이 있다.
// - 그 외 함수 인자 등은 동일하고, 인자 및 리턴 타입 형식만이 조금 다르다.

#define MAX_THREADS (1024 * 10)

unsigned int WINAPI ThreadProc(LPVOID lpParam)
{

	// _beginthreadex 함수를 호출하면, 각각의 쓰레드를 위해서
	// 메모리를 할당하게 된다.

	// 따라서 쓰레드 종료 시에는 할당한 메모리를 반환해야 한다.
	// 해당 역할을 하는 함수가 바로 _endthreadex 이다
	// 해당 함수는 내부적으로 쓰레드에 할당된 메모리를 해제하고
	// ExitThread 함수를 호출한다.
	// 단, return 문을 활용하여 쓰레드를 종료하게 되면
	// 알아서 _endthreadex 함수가 호출된다.

	DWORD threadNum = (DWORD)lpParam;

	while (1)
	{
		_tprintf(_T("thread num : %d \n"), threadNum);
		Sleep(1000);
	}

	return 0;
}

DWORD cntOfThread = 0;

int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadId[MAX_THREADS];
	HANDLE hThread[MAX_THREADS];

	// 생성 가능한 최대 개수의 쓰레드 생성

	while (1)
	{
		hThread[cntOfThread] = (HANDLE)_beginthreadex(
			NULL,
			0,
			ThreadProc,
			(LPVOID)cntOfThread,
			0,
			(unsigned*)
			&dwThreadId[cntOfThread]);

		if (hThread[cntOfThread] == NULL)
		{
			_tprintf(
				_T("MAXIMUM THREAD SIZE : %d \n"), cntOfThread
			);
			break;
		};

		cntOfThread++;
	}

	for (DWORD i = 0; i < cntOfThread; ++i)
	{
		CloseHandle(hThread[i]);
	}
}