#define _CRT_SECURE_NO_WARNINGS

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

	// ���μ��� ��� ����
	// - �ڵ��� ��� �����ϵ��� ����
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	// �޽��� ������
	hMailSlot = CreateFile(
		SLOT_NAME,
		GENERIC_WRITE,  // ���Ǵ� �뵵 
									// fopen �� w, r, wb, rb ���� ��
		FILE_SHARE_READ,
		&sa, // ��� ����
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

	_tprintf(_T("Inheritable Handle : %d\n"), hMailSlot);

	// Handle ������ File ���·� ����
	FILE* file = _tfopen(_T("InheritableHandle.txt"), _T("wt"));
	_ftprintf(file, _T("%d"), hMailSlot);
	fclose(file);

	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);

	TCHAR command[] = _T("MailSender2_2.exe");

	// create_new_console
	// �ڽ� ���μ������� ���ο� �ܼ� �Ҵ�
	CreateProcess(
	NULL, command,
		NULL, NULL,
		TRUE, // �ڵ� ���̺� ��� ����
		CREATE_NEW_CONSOLE,
		NULL, NULL,
		&si, &pi
	);
 
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