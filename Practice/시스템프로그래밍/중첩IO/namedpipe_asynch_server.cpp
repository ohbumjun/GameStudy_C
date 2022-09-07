#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define BUF_SIZE 1024

int CommToClient(HANDLE);


int _tmain(int argc, TCHAR* argv[])
{
	LPCTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe");
	HANDLE hPipe;

	while (1)
	{
		hPipe = CreateNamedPipe(
			pipeName,
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
			PIPE_TYPE_MESSAGE |
			PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			BUF_SIZE / 2, // 출력 버퍼 사이즈
			BUF_SIZE / 2, // 입력 버퍼 사이즈
			20000, NULL);

		if (hPipe == INVALID_HANDLE_VALUE)
		{
			_tprintf(_T("CreateNamedPipe Failed"));
			return  -1;
		};

		BOOL isSuccess;
		isSuccess = ConnectNamedPipe(hPipe, NULL) ? TRUE :
			(GetLastError() == ERROR_PIPE_CONNECTED);

		if (isSuccess)
			CommToClient(hPipe);
		else
			CloseHandle(hPipe);
	}

	return 1;

}

int CommToClient(HANDLE hPipe)
{
	TCHAR fileName[MAX_PATH];
	TCHAR dataBuf[BUF_SIZE];
	BOOL isSuccess;
	DWORD fileNameSize;

	isSuccess = ReadFile(
		hPipe, fileName, MAX_PATH * sizeof(TCHAR),
		&fileNameSize, NULL
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
	overlappedInst.hEvent = CreateEvent(
		NULL,
		TRUE,
		TRUE,
		NULL
	);

	DWORD bytesWritten = 0, bytesRead = 0;
	DWORD bytesWrite = 0, bytesTransfer = 0;

	while (!feof(filePtr))
	{
		bytesRead = fread(dataBuf, 1, BUF_SIZE, filePtr);
		
		bytesWrite = bytesRead;
		
		isSuccess = WriteFile(hPipe, dataBuf, bytesWrite,
			&bytesWritten, &overlappedInst);
	}
}