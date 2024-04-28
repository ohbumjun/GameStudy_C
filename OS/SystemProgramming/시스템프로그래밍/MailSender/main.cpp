#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// �ش� �ּҰ�, ���� ������ �ּ��̴�.
#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hMailSlot;
	TCHAR message[50];

	// number of bytes write
	DWORD bytesWritten;

	// �޽��� ������
	hMailSlot = CreateFile(
		SLOT_NAME,
		GENERIC_WRITE,  // ���Ǵ� �뵵 
									// fopen �� w, r, wb, rb ���� ��
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING, // ���� ���
								// ���ο� ���� ���� or ���� ���� ����
								// �̹� �������� ���� ���Կ� ����
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to create mailSlot!\n"), stdout);
		return 1;
	}

	while (1)
	{
		_fputts(_T("MY CMD >"), stdout);
		_fgetts(message,
			sizeof(message) / sizeof(TCHAR),
			stdin);

		if (!WriteFile(
			hMailSlot, 
			message, // ������ �����Ͱ� ����Ǿ� �ִ� ����
			_tcslen(message) * sizeof(TCHAR),
			&bytesWritten, // �Լ� ȣ�� �� ���۵� ������ ũ��
								  // ����Ʈ ������ ������ 
			NULL))
		{
			_fputts(_T("Unable to write!"), stdout);
			CloseHandle(hMailSlot);
			return 1;
		}

		if (!_tcscmp(message, _T("exit")))
		{
			_fputts(_T("Good Bye !"), stdout);
			break;
		}
	}

	CloseHandle(hMailSlot);

	return 0;
}