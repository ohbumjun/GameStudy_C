/*
// 생성 가능한 쓰레드 개수 측정
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define MAX_THREADS (1024 * 10)

// 쓰레드의 main 함수에 해당하는 쓰레드 함수 
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	DWORD threadNum = (DWORD)lpParam;

	while (1)
	{
		_tprintf(_T("thread num : %d \n"), threadNum);
		Sleep(5000);
	}

	return 0;
}

DWORD cntOfThread = 0;

int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadID[MAX_THREADS];
	DWORD hThread[MAX_THREADS];

	while (1)
	{
		hThread[cntOfThread] = (DWORD)CreateThread(
			NULL, // 디폴트 보안 속성 (상속 여부)
			0, // 디폴트 스택 사이즈
			ThreadProc, // 쓰레드 함수
			(LPVOID)cntOfThread, // 쓰레드 함수 전달 인자
			0, // 디폴트 생성 flag
			&dwThreadID[cntOfThread] // 쓰레드 아이디 반환
		);

		// 쓰레드 생성 확인
		if (hThread[cntOfThread] == NULL)
		{
			_tprintf(
				_T("Maximum Thread Number : %d \n"),
				cntOfThread
				);
			break;
		}

		cntOfThread++;
	};

	for (DWORD i = 0; i < cntOfThread; ++i)
	{
		CloseHandle((HANDLE)hThread[i]);
	}

	return 0;
}

*/