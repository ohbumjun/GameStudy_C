#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// �ڵ��� ��� Ȯ�ο� ����
int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hMailSlot;
	TCHAR message[50];
	DWORD bytesWritten;

	// �ڵ��� ��� �ڵ�
	// ������ ���, �θ� ���μ����� ���� �߿� ������ ����
	// �ڵ� ������ �����ϰ� �ִ�.
	// ���� �� �ڵ��� �̿��ؼ� �θ� ���μ����� ������ ���·�
	// ���� ���Կ� �޽����� ������ �� �ִ�.
	FILE* file = _tfopen(_T("InheritableHandle.txt"), _T("rt"));
	_ftscanf(file, _T("%d"), &hMailSlot);
	fclose(file);
	_tprintf(_T("Inheritable Handle : %d\n"), hMailSlot);

	while (1)
	{
		_fputts(_T("My CMD >"), stdout);
		_fgetts(message, sizeof(message) / sizeof(TCHAR), stdin);

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

		CloseHandle(hMailSlot);
		return 0;
	}
}