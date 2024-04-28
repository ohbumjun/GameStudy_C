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
		NULL, // ��� �Ұ�
		TRUE, // Manual Reset
		FALSE, // Non-Signaled �� ����
		NULL // �̸� X
	);

	// Mutex (�����ڰ� ����, �� ���� �Һ���)
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

	// Signaled ���·� �ٲ��ش�.
	SetEvent(gSynString.hEvent);

	WaitForMultipleObjects(
		2,
		hThread, // �ڵ� �迭 
		TRUE,     // ��� �ڵ��� ��ȣ ���� ���·� �� �� ����
		INFINITE // ���� ���
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

	// hMutex �� ���� �޸� ���� ����ȭ�� ���ִ� ������
	// stdout �̶�� ��� ��Ʈ���� �ϳ��� �����常�� �����Ͽ�
	// �ߺ� ����� ���� ���ؼ��̴�.
	// ex) output string : output string length : 10 : Hello (�̷������� �� �� �ִ�)
	WaitForSingleObject(gSynString.hMutex, INFINITE);

	_fputts(_T("output string : "), stdout);
	_fputts(gSynString.string, stdout);

	ReleaseMutex(gSynString.hMutex);
	return 0;
}

unsigned int __stdcall CountThreadFunction(LPVOID lpParam)
{
	// �ش� �̺�Ʈ�� Signaled ���°� �Ǳ⸦ ��ٸ���.
	// �̷��� �����尡 2�� �̻��̵Ǵ� ��
	// �׷��� Event �� �ڵ� ���� ����̴� ����
	// �ϳ��� �����尡 �� ����, �ش� Event ��
	// Signaled ���°� �Ǿ� �����尡 ���ŷ ���¿��� �������ö�
	// �ش� Event �� �ٽ� Non-Signaled ���°� �Ǿ������
	// �̷� ���ؼ�, �ϳ��� ������� �߰��� SetEvent �� ������ ������
	// ������ ������� �ʴ� ���¿� ���̰� �ȴ�.
	WaitForSingleObject(gSynString.hEvent, INFINITE);
	WaitForSingleObject(gSynString.hMutex, INFINITE);

	// �ԷµǴ� ���ڿ��� ���� ���
	_tprintf(_T("output string length : %d\n"), _tcslen(gSynString.string) - 1);
	
	ReleaseMutex(gSynString.hMutex);

	return 0;
}
