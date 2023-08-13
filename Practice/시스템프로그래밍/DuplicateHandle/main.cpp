#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv)
{
	HANDLE hProcess;
	TCHAR cmdString[1024];

	// GetCurrentProcess �Լ��� ���ؼ�
	// ���� ��¥ �ڵ���
	// ��¥ �ڵ�� �����ؼ�
	// �ڵ� ���̺� ����ϰ� �ִ�.
	DuplicateHandle(
		GetCurrentProcess(),
		GetCurrentProcess(),
		GetCurrentProcess(),
		&hProcess, 0,
		TRUE, // ��� ������ �ڵ�� ������
		DUPLICATE_SAME_ACCESS
	);

	// �� �ڵ� ������
	// ���ڿ��� �����Ѵ�.
	_stprintf(
		cmdString,
		_T("%s %u"),
		_T("ChildProcess.exe"),
		(unsigned)hProcess
	);

	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi = { 0, };
	si.cb = sizeof(si);

	// ���ο� �ܼ�â�� ����
	// �ڽ� ���μ��� ����
	// ������ ���ڿ��� ������ �ڵ鰪��
	// �ڽ� ���μ����� main �Լ� ���ڷ� �����Ѵ�.
	BOOL isSuccess =
		CreateProcess(
			NULL, 
			cmdString, 
			NULL, 
			NULL, 
			TRUE,
			CREATE_NEW_CONSOLE,
			NULL, 
			NULL,
			&si, &pi
		);

	if (isSuccess == FALSE)
	{
		_tprintf(_T("CreateProcess failed\n"));
		return -1;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	_tprintf(_T("Parent Process\n"));
	_tprintf(_T("ooooops!"));

	return 0;
}