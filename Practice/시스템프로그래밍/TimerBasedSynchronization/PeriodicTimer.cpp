// �ֱ��� Ÿ�̸�

// �ش� �ڵ带 �־�� �Ѵ�. �ֳ��ϸ�
// CreateWaitableTimer, SetWaitableTimer �Լ���
// ����ǵ��� ��� ���� ���ο� ���Ǻ� ������ ���¸� ���ϰ� �ֱ� ����
// �翬�� ���������, ��� ���� ���� ������ �;� �Ѵ�.
#define _WIN32_WINNT 0x400

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;

	liDueTime.QuadPart = -100000000;

	// Ÿ�̸� ������Ʈ ����
	hTimer = CreateWaitableTimer(
		NULL, // ���� �Ӽ� -> �ڵ��� �ڽ� ���μ������� ����ϰ��� �ϴ� ���, NULL �� �ƴ� �ٸ� �� ����
		FALSE, // Ÿ�̸� ������Ʈ�� ���� ���� ���� ���, �ڵ� ���� ��� 
					// ������ ����, ���� ���� Ÿ�̸�, �ֱ��� Ÿ�̸Ӹ� �����ϴ� �ڵ尡 �ƴ϶�� ���̴�.
		_T("Waitable Timer") // Ÿ�̸� ������Ʈ �̸�
	);

	if (!hTimer)
	{
		_tprintf(_T("CreateWaitable Timer Failed (%d) \n"),
			GetLastError());

		return 1;
	}

	_tprintf(_T("Waiting For 10 sec...\n"));

	// Ÿ�̸��� �˶��� 10�ʷ� ����
	SetWaitableTimer(
		hTimer, // �˶��� ������ Ÿ�̸� ������Ʈ�� �ڵ�

		// �˶��︮�� �ð� ���� 
		// + ���� �����ϸ�, ���� �ð��� �ǹ��ϰ� �ǰ�
		// - ���� ���޵Ǹ� ���ð��� �ǹ��ϰ� �ȴ�.
		// ��, A�� B�п� �˶��� ����ּ��� ����, ���� �ð��� ������ ���, +
		// �������κ��� A�� �Ŀ� ����ּ���. ���� ��û�� �� ��� - �� ����
		// 1000�� ���� 1�� ������ �ð��� �����Ѵ�.
		&liDueTime,

		// Ÿ�̸Ӱ� �ֱ������� �˶��� �︮���� �� ����ϴ� ��������
		// 0�� ������ ���, �ֱ����� �˶��� ������� �ʰڴٴ� �ǹ̰� �ȴ�.
		// �ֱ��� Ÿ�̸Ӹ� ������ ���, Ÿ�̸� ������Ʈ�� �ݵ�� "�ڵ� ���� ���"�� �����ؾ� �Ѵ�.
		// �׷��� WaitForSingleObject �Լ��� ������ ������, �ش� Ÿ�̸� ������Ʈ�� �ٽ�
		// Signaled ���°� �ǰ�, 
		// �̷� ���� Timer �� �ֱ������� ������ �� �ֱ� �����̴�.
		5000,
		NULL,
		NULL,
		FALSE
	);

	// Ÿ�̸��� �˶��� ���
	while (1)
	{
		WaitForSingleObject(hTimer, INFINITE);
		_tprintf(_T("Timer was signaled.\n"));
		MessageBeep(MB_ICONEXCLAMATION);
	}

	return 0;
}