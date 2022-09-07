#define _CRT_SECURE_NO_WARNINGS

// �̸� �ִ� ������ ���� ��ø I/O ���
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define BUF_SIZE 1024

int CommToClient(HANDLE);


int _tmain(int argc, TCHAR* argv[])
{
	LPCTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe");
	HANDLE hPipe;

	// while ?
	// ���� Ŭ���̾�Ʈ�� ��� �����ϴ�
	// ������, �Ѽ����� �� Ŭ���̾�Ʈ�� �����ϴ�.
	while (1)
	{
		hPipe = CreateNamedPipe(
			pipeName,
			//FILE_FLAG_OVERLAPPED �� ����, �񵿱� Ư�� �ο�
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, 
			PIPE_TYPE_MESSAGE |
			PIPE_READMODE_MESSAGE 
			| PIPE_WAIT, // �ؽ�Ʈ ���� ���
			PIPE_UNLIMITED_INSTANCES, // �ִ� ������ ����
			BUF_SIZE / 2, // ��� ���� ������
			BUF_SIZE / 2, // �Է� ���� ������
			20000, // Ŭ���̾�Ʈ Ÿ��-�ƿ�
			NULL);

		// �׷��� �� �������� ��� ����, �Է� ������ ũ�⸦
		// BUF_SIZE / 2 �� �������� ���ϱ� ?
		// �ֳ��ϸ�
		// WriteFile �Լ��� ȣ���ϸ鼭 ������ ������ ũ�Ⱑ 100����Ʈ�̰�
		// �������� ��� ���� ���� ũ�Ⱑ 100 ����Ʈ��� �ϸ�
		// ������(���ŷ �Լ� ȣ�� ���) I/O �����̶� �������
		// ���� ���ŷ ���� �ʰ�, �ٷ� ��ȯ�Ѵ�.
		// ���� ��� I/O ���꿡�� ���� �������� Ŭ���̾�Ʈ����
		// ������ ������ �Ϸ�Ǿ�߸� ��ȯ�ϴ� ������ �ƴ϶�
		// ������ ���� �Ҵ�� ���� �޸� ���ۿ� ���簡
		// �̷����� ��ȯ�ϴ� �����̱� �����̴�.
		
		// �׷��� ��� ������ ũ�⸦ ������ ���� ���̴�.
		// �̸� ���� WriteFile �Լ��� ���ŷ �Ǵ� ������ ���� ���̴�.
		// �ش� �Լ��� ���� �����ϰ��� �ϴ� ������ ũ�Ⱑ
		// ���� ũ���� 2�谡 �Ǳ� �����̴�.

		if (hPipe == INVALID_HANDLE_VALUE)
		{
			_tprintf(_T("CreateNamedPipe Failed"));
			return  -1;
		};

		_tprintf(_T("Connecting Pipe ...\n"));

		BOOL isSuccess;

		// �������� ���� ���·� ����� 
		// ��, Ŭ���̾�Ʈ�� ���� ��û�� �����ϴ� ����
		// Ŭ���̾�Ʈ�� ����� ������ Blocked ���¿� ���δ�.
		isSuccess = ConnectNamedPipe(hPipe, NULL) ? TRUE :
			(GetLastError() == ERROR_PIPE_CONNECTED);

		if (isSuccess)
		{
			_tprintf(_T("Pipe Connected\n"));
			CommToClient(hPipe);
		}
		else
		{
			_tprintf(_T("CloseHandle\n"));
			CloseHandle(hPipe);
		}
	}

	return 1;

}

// Ŭ���̾�Ʈ�� ������ �����Ǹ�
// �̶� ����� ������ �ڵ��� �����ϸ鼭
// �ش� �Լ��� ȣ��ȴ�.
int CommToClient(HANDLE hPipe)
{
	TCHAR fileName[MAX_PATH];
	TCHAR dataBuf[BUF_SIZE];
	BOOL isSuccess;
	DWORD fileNameSize;

	// �ش� ������ ���ҽ��� ����Ű��
	// �ڵ��� ���ؼ�, �����͸� �о���δ�.
	isSuccess = ReadFile(
		hPipe, 
		fileName, // read ���� ������ ����
		MAX_PATH * sizeof(TCHAR), // read ���� ������
		&fileNameSize, 
		NULL
	);

	if (!isSuccess || fileNameSize == 0)
	{
		_tprintf(_T("Pipe Read Message Error\n"));
		return  -1;
	}

	FILE* filePtr = _tfopen(fileName, _T("r"));

	if (filePtr == NULL)
	{
		_tprintf(_T("File open fault!\n"));
		return  -1;
	}

	OVERLAPPED overlappedInst;
	memset(&overlappedInst, 0, sizeof(overlappedInst));

	// hEvent : �ش� ����¿����� �Ϸ�Ǿ����� Ȯ���ϱ� ���� ��
	// �ش� ����� ������ �Ϸ�Ǹ�, Signaled ���°� �ȴ�.
	overlappedInst.hEvent = CreateEvent(
		NULL,
		// ���� ���� �����... �ش� �̺�Ʈ ������Ʈ�� ��,��� �ϷḦ Ȯ���ϱ� ����
		// �뵵�� �ݺ������� ����ϴϱ�,  �ڵ� ���� ���� ��������� ���� �ʴ°�.
		TRUE, // manual-reset event
		// �ʱ� ������ Non-Signaled ���°� �Ǿ�� ���� �ʴ°� ?
		TRUE, // �ʱ���� True --> Signaled
		NULL  // unnamed event object
	);

	// Overlapped ����ü ������ ����� ��ϵ� �̺�Ʈ ������Ʈ��
	// ReadFile, WriteFile �Լ� ȣ���, �ڵ����� Non-Signaled ���°� �ȴ�.
	// ����, �̺�Ʈ ������Ʈ�� Signaled ���·� �����ص� �������� �ʴ´�.

	DWORD bytesWritten = 0, bytesRead = 0;
	DWORD bytesWrite = 0, bytesTransfer = 0;

	while (!feof(filePtr))
	{
		bytesRead = fread(dataBuf, 1, BUF_SIZE, filePtr);
		
		bytesWrite = bytesRead;

		// 1) overlapped ����ü �ʱ�ȭ�� ������, �ش� �Լ��� ȣ���Ѵ�.
		// FILE_FLAG_OVERLAPPED �� ������ ��������,
		// OVERLAOOED ����ü ������ �����͸� ���ڷ� �����Ѵ�.
		isSuccess = WriteFile(
			hPipe, 
			dataBuf,  // ������ ������ ����
			bytesWrite, // ������ ������ ũ��
			&bytesWritten, // ���۵� ������ ũ��
			&overlappedInst
		);

		// ��ø I/O ������ WriteFile, ReadFile �Լ��� NULL�� ��ȯ�ߴٰ� �Ͽ�
		// ������ ������ ����ϰ� ���μ����� �����ؼ��� �ȵȴ�.
		// ���ŷ ���� �����ϴ� I/O ���꿡���� I/O ���� ��ü�� ���� ������ �߻����� �ʴ´�.
		// (���� ���� = I/O ������ �з���� �۾��� ��ü�Ǵ� ����)
		// �ֳ��ϸ�, �ϳ��� I/O ������ ���� ������ ���ŷ ���¿� ���̱� �����̴�.

		// ������, ��ø I/O �� ��Ȳ�� �ٸ���.
		// �ϳ��� I/O ������ ������ ������ ���ο� I/O ������ ���� �� �ִ�.
		// (�׷��� ��ø I/O )
		// ������ ������ I/O ���� ��û���� ��� ���� ���ĺ��ʹ� ���� ������ �߻��� �� �ִ�.
		// �̶����� ������ ���� ���̴� I/O ������ �Ϸ�� ���� Ȯ���� �Ŀ� �ٽ� ��û�ϸ� �ȴ�.
		// ���� ���μ����� �����ϰų�, ���� ���� ó���� ���� ���м��� �� ���� �ƴϴ�.
		// ����  WriteFile �Լ� ȣ������ ����� NULL �̶��
		// ������ I/O ��û�� ���� �������� (ERROR_IO_PENDING) Ȯ���ϴ� ���̴�.

		if (!isSuccess && GetLastError() != ERROR_IO_PENDING)
		{
			_tprintf(_T("Pipe Write Message Error!\n"));
			break;
		}

		/* �ٸ� �۾��� �� �� �ִ� ��ȸ
		- WriteFile �Լ��� ��ø I/O ������� ȣ��Ǿ�����, ������ ������ �뷮�� ���� ũ�ٸ�
		������ I/O ���� ���� �߿� ���� ���̴�.

		��, WriteFile �Լ���, ȣ��� ���ÿ� �ٷ� ��ȯ�Ͽ��� ������
		�ٸ� ������ ���ÿ� ������ �� �ִ�.
		
		��, ������ ���۰� �� �ٸ� ������ ���ÿ� ����Ǵ� ���̴�.
		*/
		_tprintf(_T("Other Work... \n"));

		WaitForSingleObject(overlappedInst.hEvent, INFINITE);

		// ��û�� I/O ������ �� ������ �Ǿ����� Ȯ���� �����̴�.
		GetOverlappedResult(hPipe, &overlappedInst, &bytesTransfer, FALSE);

		_tprintf(_T("Transferred data      : %s \n"), dataBuf);
		_tprintf(_T("Transferred data size : %u \n"), bytesTransfer);
	}

	// ������ ������ �����ϱ� ���� �ڵ��
	// FlushFileBuffer : ��� ���۸� ���� ����
	// ���ٴ� ����, ���������� ������ �ǹ��Ѵ�.
	// �̹� ���۵Ǿ��ٰ� �����ϴ� �����͵���
	// �������� ��Ȳ�� ����, �������� �ý��� �޸𸮿�
	// �������� �� �ִ�.
	FlushFileBuffers(hPipe);

	// ������ ���� ����
	// Ŭ���̾�Ʈ�� ���� �޽����� ���� �� �ְ� �Ѵ�.
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return 1;
}