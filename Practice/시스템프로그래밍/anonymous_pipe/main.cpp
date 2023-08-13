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

	/*pipe 생성*/
	CreatePipe(
		&hReadPipe, 
		&hWritePipe, 
		NULL,  // 핸들 상속 여부
		0);

	/*pipe 의 한쪽 끝을 이용한 데이터 송신*/
	WriteFile(
		hWritePipe, // hWritePipe 에 데이터 보내기
		sendString,
		lstrlen(sendString) * sizeof(TCHAR),
		&bytesWritten, // number of bytes written 
		NULL
	);

	_tprintf(_T("string send : %s\n"), sendString);

	/*pipe 의 다른 한쪽 끝을 이용한 데이터 수신*/
	ReadFile(
		hReadPipe,
		recvString, // 읽은 정보를 담아내는 버퍼
		bytesWritten, // maximum number of bytes to be read
		&bytesRead, // number of bytes read
		NULL);

	// 마지막 Null 문자 넣기 ?
	recvString[bytesRead / sizeof(TCHAR)] = 0;

	_tprintf(_T("string recv : %s\n"), recvString);

	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);

	return 0;
}