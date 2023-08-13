/*
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

#define NUM_OF_GATE 6

LONG gTotalCount = 0;

CRITICAL_SECTION hCritic;

void IncreaseCount()
{
	// �������� ����� 
	// ��, �ش� �Լ����� ���ؼ�, �������� �����尡 ���� �ϳ�, ���� �� ���� ��ų ���
	// ���ÿ� �� �̻��� ������ ���ٿ� ���� ������ ���� �� �ִ�.
	// gTotalCount += 1;
	InterlockedIncrement(&gTotalCount);
}

unsigned int WINAPI ThreadProc(LPVOID Param)
{
	for (DWORD i = 0; i < 1000; ++i)
	{
		IncreaseCount();
	}

	// �޸� ���� + ������ ���� ����
	return 0;
};

int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadId[NUM_OF_GATE];
	HANDLE hThread[NUM_OF_GATE];

	InitializeCriticalSection(&hCritic);

	// ������ ������ ����
	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		hThread[i] = (HANDLE)_beginthreadex(
			NULL, 0,
			ThreadProc, NULL,
			CREATE_SUSPENDED, // ó������ Blocked ���·�
			(unsigned*)&dwThreadId[i]
		);

		if (hThread[i] == NULL)
		{
			return -1;
		}
	}

	// ��� Ready ���·� �ٲ��ش�.
	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		ResumeThread(hThread[i]);
	}

	// ��� ��������� ����� ������ ��ٸ���.
	WaitForMultipleObjects(
		NUM_OF_GATE, hThread, TRUE, INFINITE
	);

	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		CloseHandle(hThread[i]);
	}

	DeleteCriticalSection(&hCritic);

	return 0;
}

*/