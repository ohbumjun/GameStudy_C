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
	// �ش� �Լ��� ȣ��� ������, Semaphore ������ ī��Ʈ�� �ϳ��� �����Ѵ�.
	// ���� ���� ī��ܰ� 0�� �Ǹ�, �� ���� �Ӱ迵���� ������ ���������
	// ���ŷ ���°� �ȴ�.
	WaitForSingleObject(hSemaphore, INFINITE);

	_tprintf(_T("Enter Customer : %d\n"), GetCurrentThreadId());

	// �Ļ� ���� ���¸� �ùķ��̼� �ϴ� �Լ�
	Sleep(1000 * time);

	ReleaseSemaphore(
		// ��ȯ�ϰ��� �ϴ� ���������� �ڵ�
		hSemaphore,
		// ��ȯ�Ѵٴ� ����, ���� ���� ī��Ʈ�� ������ �ǹ��Ѵ�.
		// �ش� ���ڸ� ���� ������ų ���� ũ�⸦ ������ �� �ִ�.
		// ���� �������� ���� �� ������ �ִ� ī��Ʈ ��(CreateSemaphore 3��° ����)
		// �� �����ϰ� �Ǹ�, ī��Ʈ�� ������� �ʰ� FALSE �� ��ȯ
		1,
		// ����Ǳ� �� �������� ī��Ʈ ���� ������ ������ ����
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

	// random function seed ����
	srand((unsigned)time(NULL));

	// �����忡�� ������ random �� �� 50�� ����
	// (�մԸ��� �Ļ��ϴ� �ð��� �ٸ���)
	for (int i = 0; i < NUM_OF_CUSTOMER; ++i)
	{
		randTimeArr[i] = (DWORD)(
			((double)rand() / (double)RAND_MAX)
			* RANGE_MAX + RANGE_MIN
			);
	}

	// ���� ���� ����
	hSemaphore = CreateSemaphore(
		NULL, // ����Ʈ ���� ������

		// �ش� ���� ������� �Ӱ� ������ ���� ������ �������� ������ �����Ѵ�. 
		TABLE_CNT,  // ���� ���� �ʱⰪ

		// ������� ���� �� �ִ� ���� �ִ� ũ��
		// �ش� ���� 1 �� �� ���, ���ؽ��� ������ ����� �ϴ�
		// ���̳ʸ� ������� �����ȴ�.
		// �⺻������ 2��° ���ڷ� ���޵Ǵ� ������ Ŀ�� �Ѵ�.
		TABLE_CNT, // ���� ���� �ִ밪

		// ���������� �̸�
		NULL // unamed ���� ���� ����
	);

	if (hSemaphore == NULL)
	{
		_tprintf(_T("Create Semaphore Error"));
	}

	// Customer �� �ǹ��ϴ� ������ ����
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