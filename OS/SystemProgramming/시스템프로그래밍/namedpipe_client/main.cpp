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
		// 서버가 생성한 파이프와 
		// 클라이언트를 연결해주는 함수 
		// - 파일을 생성하거나, 개방하는 Windows 시스템 함수
		// - 그러나, 클라이언트 영역에서, 서브 측 파이프로의
		// - 연결을 요청하는 기능도 지닌다.
		hPipe = CreateFile(
			pipeName, // 연결할 파이프 이름
			GENERIC_READ |
			GENERIC_WRITE, // 읽기, 쓰기 모드 동시 지정
			0, 
			NULL, 
			OPEN_EXISTING,
			0,
			NULL
		);

		// 연결되었다면 빠져나간다.
		// 다음 작업을 진행하기 위함이다.
		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			_tprintf(_T("Could Not Open Pipe\n"));
			return 0;
		}

		// Pipe 연결까지 기다리는 최대 시간을
		// 넘었다면 연결 실패
		if (!WaitNamedPipe(pipeName, 
			20000))
		{
			_tprintf(_T("Could not open pipe\n"));
			return 0;
		}
	}

	DWORD pipeMode = PIPE_READMODE_MESSAGE |
		PIPE_WAIT; // 메시지 기반으로 모드 변경

	//파이프의 속성을 변경시켜주는 함수 이다.
	BOOL isSuccess = SetNamedPipeHandleState(
		hPipe, // 서버 파이프와 연결된 핸들
		&pipeMode, // 변경할 모드 정보
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

	// 파이프를 통해서, 서버에 데이터를 전송할 것이다.
	isSuccess = WriteFile(
		hPipe,    // 서버 파이프와 연결된 핸들
		fileName, // 전송할 메세지
		(_tcslen(fileName) + 1) * sizeof(TCHAR), // 메시지 길이
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
		// server 측에서 파이프를 통해
		// 보내준 정보를 읽어낼 것이다.
		isSuccess = ReadFile(
			hPipe, // 서버 파이프와 연결된 핸들
			readDataBuf,  // 데이터 수신할 버퍼
			BUF_SIZE * sizeof(TCHAR), // 버퍼 사이즈수
			&bytesRead,
			NULL
			);


		// 서버 측에서
		// DisconnectNamedPipe 를 호출해줌으로써
		// Error 메세지를 받게 되고
		// 이제 while 문 break
		if (!isSuccess && GetLastError() != ERROR_MORE_DATA)
		{
			_tprintf(_T("EXIT!\n"));
			break;
		}
	
		// 문자열 끝임을 알려주기
		readDataBuf[bytesRead] = 0;

		_tprintf(_T("Bytes Read %d \n"), bytesRead);
		_tprintf(_T("Client Read Data : %s\n"), readDataBuf);
	}

	CloseHandle(hPipe);

	return 0;
}