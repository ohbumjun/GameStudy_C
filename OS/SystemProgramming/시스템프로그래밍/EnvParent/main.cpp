#define _CRT_SECURE_NO_WARNINGS

// ȯ�� ���� �����ϴ� �θ� ���μ���
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	// ���μ��� ȯ�� ������ ����� �� ����ϴ� �Լ�
	// key ~ value
	SetEnvironmentVariable(_T("Good"), _T("morning"));
	SetEnvironmentVariable(_T("Hey"), _T("Ho"));
	SetEnvironmentVariable(_T("Big"), _T("Boy"));

	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi = { 0, };
	si.cb = sizeof(si);

	CreateProcess(
		NULL, (LPWSTR)("EnvChild"),
		NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE |
		CREATE_UNICODE_ENVIRONMENT,
		
		// �Ʒ� ���ڸ� ���ؼ�, �ڽ� ���μ�������
		// ����ϰ���, ȯ�溯���� ������ �� �ִ�.
		NULL,  // �θ� ���μ��� ȯ�溯�� ���
		NULL, &si, &pi
	);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}