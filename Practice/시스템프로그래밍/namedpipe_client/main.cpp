#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>

#define BUF_SIZE 1024

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hPipe;
	TCHAR readDataBuf[BUF_SIZE + 1];
	LPCTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe");

	while (1)
	{
		// ������ ������ �������� 
		// Ŭ���̾�Ʈ�� �������ִ� �Լ� 
		// - ������ �����ϰų�, �����ϴ� Windows �ý��� �Լ�
		// - �׷���, Ŭ���̾�Ʈ ��������, ���� �� ����������
		// - ������ ��û�ϴ� ��ɵ� ���Ѵ�.
		hPipe = CreateFile(
			pipeName, // ������ ������ �̸�
			GENERIC_READ |
			GENERIC_WRITE, // �б�, ���� ��� ���� ����
			0, 
			NULL, 
			OPEN_EXISTING,
			0,
			NULL
		);

		// ����Ǿ��ٸ� ����������.
		// ���� �۾��� �����ϱ� �����̴�.
		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			_tprintf(_T("Could Not Open Pipe\n"));
			return 0;
		}

		// Pipe ������� ��ٸ��� �ִ� �ð���
		// �Ѿ��ٸ� ���� ����
		if (!WaitNamedPipe(pipeName, 
			20000))
		{
			_tprintf(_T("Could not open pipe\n"));
			return 0;
		}
	}

	DWORD pipeMode = PIPE_READMODE_MESSAGE |
		PIPE_WAIT; // �޽��� ������� ��� ����

	//�������� �Ӽ��� ��������ִ� �Լ� �̴�.
	BOOL isSuccess = SetNamedPipeHandleState(
		hPipe, // ���� �������� ����� �ڵ�
		&pipeMode, // ������ ��� ����
		NULL,
		NULL
	);

	if (!isSuccess)
	{
		_tprintf(_T("SetNamedPipeHandleState Failed"));
		return 0;
	}

	LPCTSTR fileName = _T("news.txt");
	DWORD bytesWritten = 0;

	// �������� ���ؼ�, ������ �����͸� ������ ���̴�.
	isSuccess = WriteFile(
		hPipe,    // ���� �������� ����� �ڵ�
		fileName, // ������ �޼���
		(_tcslen(fileName) + 1) * sizeof(TCHAR), // �޽��� ����
		&bytesWritten,
		NULL
	);

	if (!isSuccess)
	{
		_tprintf(_T("WriteFile Failed"));
		return 0;
	}

	DWORD bytesRead = 0;

	while (1)
	{
		// server ������ �������� ����
		// ������ ������ �о ���̴�.
		isSuccess = ReadFile(
			hPipe, // ���� �������� ����� �ڵ�
			readDataBuf,  // ������ ������ ����
			BUF_SIZE * sizeof(TCHAR), // ���� �������
			&bytesRead,
			NULL
			);


		// ���� ������
		// DisconnectNamedPipe �� ȣ���������ν�
		// Error �޼����� �ް� �ǰ�
		// ���� while �� break
		if (!isSuccess && GetLastError() != ERROR_MORE_DATA)
		{
			_tprintf(_T("EXIT!\n"));
			break;
		}
	
		// ���ڿ� ������ �˷��ֱ�
		readDataBuf[bytesRead] = 0;

		_tprintf(_T("Bytes Read %d \n"), bytesRead);
		_tprintf(_T("Client Read Data : %s\n"), readDataBuf);
	}

	CloseHandle(hPipe);

	return 0;
}