#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hReadPipe, hWritePipe;

	TCHAR sendString[] = _T("anonymous pipe");
	TCHAR recvString[100];

	DWORD bytesWritten;
	DWORD bytesRead;

	/*pipe ����*/
	CreatePipe(
		&hReadPipe, 
		&hWritePipe, 
		NULL,  // �ڵ� ��� ����
		0);

	/*pipe �� ���� ���� �̿��� ������ �۽�*/
	WriteFile(
		hWritePipe, // hWritePipe �� ������ ������
		sendString,
		lstrlen(sendString) * sizeof(TCHAR),
		&bytesWritten, // number of bytes written 
		NULL
	);

	_tprintf(_T("string send : %s\n"), sendString);

	/*pipe �� �ٸ� ���� ���� �̿��� ������ ����*/
	ReadFile(
		hReadPipe,
		recvString, // ���� ������ ��Ƴ��� ����
		bytesWritten, // maximum number of bytes to be read
		&bytesRead, // number of bytes read
		NULL);

	// ������ Null ���� �ֱ� ?
	recvString[bytesRead / sizeof(TCHAR)] = 0;

	_tprintf(_T("string recv : %s\n"), recvString);

	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);

	return 0;
}