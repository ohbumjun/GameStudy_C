#define _CRT_SECURE_NO_WARNINGS

// 이름있는 파이프 서버
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
	// 여러 클라이언트와 통신 가능하다
	// 하지만, 한순간에 한 클라이언트만 가능하다.
	while (1)
	{
		// 서버 입장에서 파이프 생성
		hPipe = CreateNamedPipe(
			pipeName, // 파이프 이름
			PIPE_ACCESS_DUPLEX, // 읽기, 쓰기 모드 (읽기,쓰기 모두 가능)
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, // 텍스트 모드로 통신
			PIPE_UNLIMITED_INSTANCES, // 최대 파이프 개수
			BUF_SIZE, // 출력 버퍼 사이즈
			BUF_SIZE, // 입력 버퍼 사이즈
			20000, // 클라이언트 타임-아웃
			NULL
		);

		if (hPipe == INVALID_HANDLE_VALUE)
		{
			_tprintf(_T("CreatePipe Failed"));
			return -1;
		}

		// 파이프를 연결 상태로 만들기 
		// 즉, 클라이언트의 연결 요청을 수락하는 역할
		// 클라이언트가 연결될 때까지 Blocked 상태에 놓인다.
		BOOL isSuccess = 0;
		isSuccess = ConnectNamedPipe(
			hPipe, NULL
		);

		if (isSuccess)
			CommToClient(hPipe);
		else
			CloseHandle(hPipe);
	}

	return 1;
}

// 클라이언트와 연결이 성립되면
// 이때 사용한 파이프 핸들을 전달하면서
// 해당 함수가 호출된다.
int CommToClient(HANDLE hPipe)
{
	TCHAR fileName[MAX_PATH];
	TCHAR dataBuf[BUF_SIZE];

	BOOL isSuccess;
	DWORD fileNameSize;

	// 해당 파이프 리소스를 가리키는
	// 핸들을 통해서, 데이터를 읽어들인다.
	isSuccess = ReadFile(
		hPipe, // 파이프 핸들
		fileName, // read 내용 저장할 버퍼
		MAX_PATH * sizeof(TCHAR), // read 버퍼 사이즈
		&fileNameSize,
		NULL
		);

	if (!isSuccess || fileNameSize == 0)
	{
		_tprintf(_T("Pipe read message error!\n"));
		return -1;
	}

	// read 모드로 읽는다.
	FILE* filePtr = _tfopen(fileName, _T("r"));

	if (filePtr == NULL)
	{
		_tprintf(_T("file open fault\n"));
		return -1;
	}

	DWORD bytesWritten = 0;
	DWORD bytesRead = 0;

	// file의 끝까지 계속 읽는다.
	while (!feof(filePtr))
	{
		// 해당 파일 포인터로부터, 데이터를 읽어들인다..
		bytesRead = fread(dataBuf, 1, BUF_SIZE, filePtr);

		WriteFile(
			hPipe, // 파이프 핸들
			dataBuf, // 전송할 데이터 버퍼
			bytesRead, // 전송할 데이터 크기
			&bytesWritten, // 전송된 데이터 크기
			NULL
		);

		// 전송할 크기만큼, 전송된 데이터 크기 정보가 세팅되지 않으면
		// 잘못 전송된 것
		if (bytesRead != bytesWritten)
		{
			_tprintf(_T("Pipe Written message error!\n"));
			break;
		}
	}

	// 파이프 연결을 종료하기 위한 코드들
	// FlushFileBuffer : 출력 버퍼를 비우는 역할
	// 비운다는 것은, 목적지로의 전송을 의미한다.
	// 이미 전송되었다고 생각하는 데이터들이
	// 목적지의 상황에 따라, 아직까지 시스템 메모리에
	// 남아있을 수 있다.
	FlushFileBuffers(hPipe);

	// 클라이언트가 에러 메시지를 받을 수 있게 한다.
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);
	return 1;
}
