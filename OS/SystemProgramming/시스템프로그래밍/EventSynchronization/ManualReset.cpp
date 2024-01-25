#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <process.h>

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam);
unsigned int WINAPI CountThreadFunction(LPVOID lpParam);

TCHAR string[100];
HANDLE hEvent;

typedef struct _SynchString
{
	TCHAR string[100];
	HANDLE hEvent;
	HANDLE hMutex;
} SynchString;

SynchString gSynString;

int _tmain(int argc, TCHAR* Argv[])
{
	HANDLE hThread[2];
	DWORD dwThreadID[2];

	// Event Obj
	gSynString.hEvent = CreateEvent(
		NULL, // 상속 불가
		TRUE, // Manual Reset
		FALSE, // Non-Signaled 로 시작
		NULL // 이름 X
	);

	// Mutex (생산자가 먼저, 그 다음 소비자)
	gSynString.hMutex = CreateMutex(
		NULL, FALSE, NULL);

	if (gSynString.hMutex == NULL || gSynString.hEvent == NULL)
	{
		_fputts(_T("Event Creation Failed\n"), stdout);
		return -1;
	}

	hThread[0] = (HANDLE)_beginthreadex(
		NULL, 0,
		OutputThreadFunction,
		NULL, 0,
		(unsigned*)&dwThreadID[0]
	);

	hThread[1] = (HANDLE)_beginthreadex(
		NULL, 0,
		CountThreadFunction,
		NULL, 0,
		(unsigned*)&dwThreadID[1]
	);

	if (hThread[0] == 0 || hThread[1] == 0)
	{
		_fputts(_T("Thread Creation Failed\n"), stdout);
		return -1;
	}

	_fputts(_T("Insert String : "), stdout);
	_fgetts(gSynString.string, 30, stdin);

	// Signaled 상태로 바꿔준다.
	SetEvent(gSynString.hEvent);

	WaitForMultipleObjects(
		2,
		hThread, // 핸들 배열 
		TRUE,     // 모든 핸들이 신호 받은 상태로 될 때 리턴
		INFINITE // 무한 대기
	);

	CloseHandle(gSynString.hEvent);
	CloseHandle(gSynString.hMutex);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	_tprintf(_T("--- Total End ---"));

	return 0;
}

unsigned int WINAPI OutputThreadFunction(
	LPVOID lpParam
)
{
	WaitForSingleObject(gSynString.hEvent, INFINITE);

	// hMutex 를 통해 메모리 접근 동기화를 해주는 이유는
	// stdout 이라는 출력 스트림에 하나의 쓰레드만이 접근하여
	// 중복 출력을 막기 위해서이다.
	// ex) output string : output string length : 10 : Hello (이런식으로 될 수 있다)
	WaitForSingleObject(gSynString.hMutex, INFINITE);

	_fputts(_T("output string : "), stdout);
	_fputts(gSynString.string, stdout);

	ReleaseMutex(gSynString.hMutex);
	return 0;
}

unsigned int __stdcall CountThreadFunction(LPVOID lpParam)
{
	// 해당 이벤트가 Signaled 상태가 되기를 기다린다.
	// 이러한 쓰레드가 2개 이상이되는 것
	// 그런데 Event 가 자동 리셋 모드이다 보니
	// 하나의 쓰레드가 들어간 이후, 해당 Event 가
	// Signaled 상태가 되어 쓰레드가 블로킹 상태에서 빠져나올때
	// 해당 Event 를 다시 Non-Signaled 상태가 되어버리고
	// 이로 인해서, 하나의 쓰레드는 추가로 SetEvent 를 해주지 않으면
	// 영원히 실행되지 않는 상태에 놓이게 된다.
	WaitForSingleObject(gSynString.hEvent, INFINITE);
	WaitForSingleObject(gSynString.hMutex, INFINITE);

	// 입력되는 문자열의 길이 출력
	_tprintf(_T("output string length : %d\n"), _tcslen(gSynString.string) - 1);
	
	ReleaseMutex(gSynString.hMutex);

	return 0;
}
