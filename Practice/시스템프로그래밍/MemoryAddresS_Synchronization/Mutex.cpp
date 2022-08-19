#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <process.h>

#define NUM_OF_GATE 6

LONG gTotalCount = 0;

// CRITICAL_SECTION gCriticalSection;
HANDLE hMutex;

void IncreaseCount()
{
	// EnterCriticalSection();
	// �ش� hMutex Ŀ�� ������Ʈ�� ���°� 
	// Signaled ���°� �� ������ ��ٸ���.
	// Signaled ���°� �Ǹ�, �Ʒ��� �Ӱ� ������ ����
	// hMutex Ŀ�� ������Ʈ��, �ٽ� Non-Signaled ���°� �ȴ�.
	WaitForSingleObject(hMutex, INFINITE);

	gTotalCount++;

	// Leave Critical Section
	// �Ӱ� ������ �����鼭, hMutex Ŀ�� ������Ʈ�� ���¸�
	// Signaled ���·� �ٲ��ش�.
	ReleaseMutex(hMutex);
};

unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
	for (DWORD i = 0; i < 1000; ++i)
	{
		IncreaseCount();
	}

	return 0;
}

int _tmain(int argc, TCHAR** argv)
{
	DWORD dwThreadID[NUM_OF_GATE];
	HANDLE hThreads[NUM_OF_GATE];

	// Initialize Critical Section
	hMutex = CreateMutex(
		// LPSECURITI_ATTRIBUTES
		// ���μ��� ������ ���� �Ӽ��� �����ߴ� ��ó��
		// ���� �Ӽ��� �������� �� �ִ�.
		// ���μ����� Ŀ�� ������Ʈ�̰�, ���ؽ��� Ŀ�� ������Ʈ�̱� �����̴�.
		NULL, 
		// ���迡 �ش��ϴ� ũ��Ƽ�� ���� ������Ʈ�� ���� �� �ʱ�ȭ �ǰ� ����
		// ������ �� ���踦 ���� �����ϴ� ������
		// ��, EnterCriticalSection �Լ��� ���� ȣ���ϴ� �����尡
		// �Ӱ� ������ ����.
		// ���ؽ��� ���ؽ� ������Ʈ�� �����ϴ� �����忡�� ��ȸ�� ���� �� �� �ִ�.
		// FALSE �� �����ϸ� ũ��Ƽ�� ���� ó�� ���� �����ϴ� �縲��
		// ���ڰ� �ǰ� �� ���� �ְ�
		// TRUE �� �����ϸ�, ���ؽ��� �����ϴ� �����尡 ���� 
		// ��ȸ�� ���� �� �ִ�.
		FALSE,

		// ���ؽ��� �̸��� �ٿ��ֱ� ����
		// �ش� �׸��� ���ؼ�, �̸��ִ� ���ؽ� ��� ����ȭ ����� ����
		NULL
	);
	// ������ ������ ����
	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		hThreads[i] = (HANDLE)_beginthreadex(
			NULL, 0,
			ThreadProc, NULL,
			CREATE_SUSPENDED, // ó������ Blocked ���·�
			(unsigned*)&hThreads[i]
		);

		if (hThreads[i] == NULL)
		{
			return -1;
		}
	}

	// ��� Ready ���·� �ٲ��ش�.
	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		ResumeThread(hThreads[i]);
	}

	for (DWORD i = 0; i < NUM_OF_GATE; ++i)
	{
		CloseHandle(hThreads[i]);
	}
	
	// DeleteCriticalSection(&hCritic);
	CloseHandle(hMutex);

	return 0;
}