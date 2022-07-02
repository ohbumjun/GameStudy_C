// CreateProcess : ���� ���μ����� ����
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define DIR_LEN MAX_PATH + 1

int _tmain(int argc, TCHAR* argv[])
{
	STARTUPINFO si = { 0, };

	// �ش� ����ü ������, ���� �����Ǵ� ���μ��� ���� ������ ��� ���� ���ȴ�.
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);
	// STARTF_USEPOSITION -> ����ü ��� �߿��� dwX, dwY �ݿ�
	// STARTF_USESIZE -> ����ü ��� �߿��� dwXSize, dwYSize �ݿ�
	si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
	si.dwX = 100; // ���μ��� ������ x ��ǥ
	si.dwY = 200; // ���μ��� ������ y ��ǥ
	si.dwXSize = 300;  // ���μ��� ������ x ũ��
	si.dwYSize = 200;  // ���μ��� ������ y ũ��
	// si.lpTitle = _T("I am a boy"); -> �ܼ� �������� Ÿ��Ʋ �� ����

	// ���� ���� ���μ��� main �Լ��� ������ �����̴�.
	// - AdderProcess.exe �� ���� ���� �̸��̰�
	// - 10, 20 �̶�� 2���� ���ڰ� �ٷ�, main �Լ��� �������ڰ� �ȴ�.
	TCHAR command[] = _T("AdderProcess.exe 10 20");
	TCHAR cDir[DIR_LEN];
	BOOL state;

	// ���� ���丮 Ȯ��
	GetCurrentDirectory(DIR_LEN, cDir);
	_fputts(cDir, stdout);
	_fputts(_T("\n"), stdout);

	// CreateProcess.exe �� ��� �����ϵ� �������.
	// ���α׷��� ����Ǹ鼭, ���� ���͸��� �Ʒ��� ��η� ��ȯ�ϱ� �����̴�.
	SetCurrentDirectory(_T("C:\\WinSystem"));

	// ���� ���丮 Ȯ��
	// - �Ϲ������� ���μ����� �����Ǹ�, ���μ����� ���� ���丮��
	// ���μ����� ���������� �����ϴ� ���͸��� �����ȴ�.
	// �Ʒ� �Լ��� ���ؼ�, ���� �߿� �ִ� ���μ����� ���� ���丮 ��ġ Ȯ���� �� �ִ�.
	GetCurrentDirectory(DIR_LEN, cDir);
	_fputts(cDir, stdout);
	_fputts(_T("\n"), stdout);

	// ���μ��� ����
	state = CreateProcess(
		NULL, // ���� ���� �̸�
		command, // main �Լ��� ���޵� ���ڿ� -> ���⿡�� ���� ���� �̸��� ���޵� 
						// ���, ǥ�� �˻� ��� (ã�ƺ���) �� ���� ���� ������ ã�� �ȴ�.
		NULL, NULL, TRUE,
		CREATE_NEW_CONSOLE,
		NULL, NULL, &si, &pi
	);

	if (state != 0)
		_fputts(_T("Creation OK \n"), stdout);
	else
		_fputts(_T("Creation Error !\n"), stdout);

	return 0;
}