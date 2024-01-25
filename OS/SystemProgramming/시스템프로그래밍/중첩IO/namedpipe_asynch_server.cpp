#define _CRT_SECURE_NO_WARNINGS

// 이름 있는 파이프 서버 중첩 I/O 방식
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
		hPipe = CreateNamedPipe(
			pipeName,
			//FILE_FLAG_OVERLAPPED 를 통해, 비동기 특성 부여
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, 
			PIPE_TYPE_MESSAGE |
			PIPE_READMODE_MESSAGE 
			| PIPE_WAIT, // 텍스트 모드로 통신
			PIPE_UNLIMITED_INSTANCES, // 최대 파이프 개수
			BUF_SIZE / 2, // 출력 버퍼 사이즈
			BUF_SIZE / 2, // 입력 버퍼 사이즈
			20000, // 클라이언트 타임-아웃
			NULL);

		// 그런데 왜 파이프의 출력 버퍼, 입력 버퍼의 크기를
		// BUF_SIZE / 2 로 세팅해준 것일까 ?
		// 왜냐하면
		// WriteFile 함수를 호출하면서 전송할 데이터 크기가 100바이트이고
		// 파이프의 출력 버퍼 역시 크기가 100 바이트라고 하면
		// 동기방식(블로킹 함수 호출 방식) I/O 연산이라 할지라고
		// 결코 블로킹 되지 않고, 바로 반환한다.
		// 동기 방식 I/O 연산에서 최종 목적지인 클라이언트에게
		// 데이터 전송이 완료되어야만 반환하는 구조가 아니라
		// 전송을 위해 할당된 내부 메모리 버퍼에 복사가
		// 이뤄지면 반환하는 구조이기 때문이다.
		
		// 그래서 출력 버퍼의 크기를 반으로 줄인 것이다.
		// 이를 통해 WriteFile 함수가 블로킹 되는 조건을 갖춘 것이다.
		// 해당 함수를 통해 전송하고자 하는 데이터 크기가
		// 버퍼 크기의 2배가 되기 때문이다.

		if (hPipe == INVALID_HANDLE_VALUE)
		{
			_tprintf(_T("CreateNamedPipe Failed"));
			return  -1;
		};

		_tprintf(_T("Connecting Pipe ...\n"));

		BOOL isSuccess;

		// 파이프를 연결 상태로 만들기 
		// 즉, 클라이언트의 연결 요청을 수락하는 역할
		// 클라이언트가 연결될 때까지 Blocked 상태에 놓인다.
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
		hPipe, 
		fileName, // read 내용 저장할 버퍼
		MAX_PATH * sizeof(TCHAR), // read 버퍼 사이즈
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

	// hEvent : 해당 입출력연산이 완료되었음을 확인하기 위한 것
	// 해당 입출력 연산이 완료되면, Signaled 상태가 된다.
	overlappedInst.hEvent = CreateEvent(
		NULL,
		// 수동 리셋 모드라니... 해당 이벤트 오브젝트는 입,출력 완료를 확인하기 위한
		// 용도로 반복적으로 사용하니까,  자동 리셋 모드로 변경해줘야 하지 않는가.
		TRUE, // manual-reset event
		// 초기 생성시 Non-Signaled 상태가 되어야 하지 않는가 ?
		TRUE, // 초기상태 True --> Signaled
		NULL  // unnamed event object
	);

	// Overlapped 구조체 변수의 멤버로 등록된 이벤트 오브젝트는
	// ReadFile, WriteFile 함수 호출시, 자동으로 Non-Signaled 상태가 된다.
	// 따라서, 이벤트 오브젝트를 Signaled 상태로 생성해도 문제되지 않는다.

	DWORD bytesWritten = 0, bytesRead = 0;
	DWORD bytesWrite = 0, bytesTransfer = 0;

	while (!feof(filePtr))
	{
		bytesRead = fread(dataBuf, 1, BUF_SIZE, filePtr);
		
		bytesWrite = bytesRead;

		// 1) overlapped 구조체 초기화가 끝나면, 해당 함수를 호출한다.
		// FILE_FLAG_OVERLAPPED 가 설정된 파이프와,
		// OVERLAOOED 구조체 변수의 포인터를 인자로 전달한다.
		isSuccess = WriteFile(
			hPipe, 
			dataBuf,  // 전송할 데이터 버퍼
			bytesWrite, // 전송할 데이터 크기
			&bytesWritten, // 전송된 데이터 크기
			&overlappedInst
		);

		// 중첩 I/O 에서는 WriteFile, ReadFile 함수가 NULL을 반환했다고 하여
		// 무조건 오류로 취급하고 프로세스를 종료해서는 안된다.
		// 블로킹 모드로 동작하는 I/O 연산에서는 I/O 연산 자체에 병목 현상이 발생하지 않는다.
		// (병목 현상 = I/O 연산이 밀려들어 작업이 지체되는 현상)
		// 왜냐하면, 하나의 I/O 연산이 끝날 때까지 블로킹 상태에 놓이기 때문이다.

		// 하지만, 중첩 I/O 는 상황이 다르다.
		// 하나의 I/O 연산이 끝나기 이전에 새로운 I/O 연산이 들어올 수 있다.
		// (그래서 중첩 I/O )
		// 때문에 과도한 I/O 연산 요청으로 어느 순간 이후부터는 병목 현상이 발생할 수 있다.
		// 이때에는 기존에 진행 중이던 I/O 연산이 완료된 것을 확인한 후에 다시 요청하면 된다.
		// 결코 프로세스를 종료하거나, 병목 현상 처리를 위해 동분서주 할 일이 아니다.
		// 따라서  WriteFile 함수 호출이후 결과가 NULL 이라면
		// 과도한 I/O 요청에 의한 문제인지 (ERROR_IO_PENDING) 확인하는 것이다.

		if (!isSuccess && GetLastError() != ERROR_IO_PENDING)
		{
			_tprintf(_T("Pipe Write Message Error!\n"));
			break;
		}

		/* 다른 작업을 할 수 있는 기회
		- WriteFile 함수가 중첩 I/O 방식으로 호출되었으니, 전송할 데이터 용량이 제법 크다면
		여전히 I/O 연산 진행 중에 있을 것이다.

		단, WriteFile 함수는, 호출과 동시에 바로 반환하였기 때문에
		다른 연산을 동시에 진행할 수 있다.
		
		즉, 데이터 전송과 또 다른 연산이 동시에 진행되는 것이다.
		*/
		_tprintf(_T("Other Work... \n"));

		WaitForSingleObject(overlappedInst.hEvent, INFINITE);

		// 요청한 I/O 연산이 잘 마무리 되었는지 확인할 차례이다.
		GetOverlappedResult(hPipe, &overlappedInst, &bytesTransfer, FALSE);

		_tprintf(_T("Transferred data      : %s \n"), dataBuf);
		_tprintf(_T("Transferred data size : %u \n"), bytesTransfer);
	}

	// 파이프 연결을 종료하기 위한 코드들
	// FlushFileBuffer : 출력 버퍼를 비우는 역할
	// 비운다는 것은, 목적지로의 전송을 의미한다.
	// 이미 전송되었다고 생각하는 데이터들이
	// 목적지의 상황에 따라, 아직까지 시스템 메모리에
	// 남아있을 수 있다.
	FlushFileBuffers(hPipe);

	// 파이프 연결 끊기
	// 클라이언트가 에러 메시지를 받을 수 있게 한다.
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return 1;
}