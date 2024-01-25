#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// 데이터 영역을 공유하는 쓰레드 특성 활용
static int total = 0;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	DWORD* nPtr = (DWORD*)lpParam;

	// 인자를 2개 전달한다고 가정
	DWORD numOne = *nPtr;
	DWORD numTwo = *(nPtr + 1);

	for (DWORD i = numOne; i <= numTwo; ++i)
	{
		total += i;
	}

	// 정상적 종료
	return 0;
};

int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadID[3];
	HANDLE hThread[3];

	DWORD paramThread[] = { 1,3,4,7,8,10 };

	hThread[0] = CreateThread(
		NULL,
		0,
		ThreadProc,
		(LPVOID)(&paramThread[0]), // 1, 3 을 사용
		0,
		&dwThreadID[0]
	);

	hThread[1] = CreateThread(
		NULL,
		0,
		ThreadProc,
		(LPVOID)(&paramThread[2]), // 1, 3 을 사용
		0,
		&dwThreadID[1]
	);

	hThread[2] = CreateThread(
		NULL,
		0,
		ThreadProc,
		(LPVOID)(&paramThread[4]), // 1, 3 을 사용
		0,
		&dwThreadID[2]
	);

	if (hThread[0] == NULL ||
		hThread[1] == NULL ||
		hThread[2] == NULL)
	{
		_tprintf(_T("Thread 생성 실패"));

		// 비 정상적 종료
		return -1;
	}

	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	_tprintf(_T("Total : %d\n"), total);

	// 프로세스와 쓰레드를 분리하기 위해서
	// 바로 CloseHandle 함수를 호출한다.
	// 쓰레드를 생성하게 되면
	// 1) 쓰레드 
	// 2) 쓰레드를 생성한 프로세스
	// 총 2개의 Usage Count 가 생기게 되고
	// 쓰레드가 종료될 때
	// 부모 프로세스가 자식 프로세스의 핸들 인자로 CloseHandle 함수를 호출할 때 감소
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(hThread[2]);

	// 정상적 종료
	return 0;
}