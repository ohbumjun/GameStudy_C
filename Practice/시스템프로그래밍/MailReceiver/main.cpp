#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// �ش� �ּҰ�, ���� ������ �ּ��̴�.
#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[])
{
	// mailSlot �ڵ�
	HANDLE hMailSlot;

	TCHAR messageBox[50];

	DWORD bytesRead;

	// mail slot ����
	hMailSlot = CreateMailslot(
		SLOT_NAME,
		0,
		MAILSLOT_WAIT_FOREVER,
		NULL
	);

	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to create mailslot\n"), stdout);
		return 1;
	}

	// message ����
	_fputts(_T("**** Message ****"), stdout);

	while (1)
	{
		if (!ReadFile(
			hMailSlot,  // ���� ���� �ڵ�
			messageBox,  // ���� ������ ������ ����
			sizeof(TCHAR) * 50, // �о� ���� ���� �ִ� ũ��
			&bytesRead, // �о���� ������ ũ�⸦ ����Ʈ ������ ���
			NULL))
		{
			_fputts(_T("Unable to read!"), stdout);
			CloseHandle(hMailSlot);
			return 1;
		}

		if (!_tcsncmp(messageBox, _T("exit"), 4))
		{
			_fputts(_T("Good Bye !"), stdout);
			break;
		}

		// null ���� ����
		messageBox[bytesRead / sizeof(TCHAR)] = 0;
		_fputts(messageBox, stdout);
	}

	CloseHandle(hMailSlot);

	return 0;
}