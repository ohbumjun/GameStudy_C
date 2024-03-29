#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen = 0, idx = 0, readLen = 0;

	if (argc != 3)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// TCP 소켓
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	// 소켓 생성
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[1]));

	// 생성한 소켓을 바탕으로 서버에 연결 요청
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() Error");

	// 여러번의 read 함수 호출 (데이터의 경계 없음 확인하기)
	// 수신된 데이터를 1 바이트씩 읽기 
	while (readLen = recv(hSocket, &message[idx++], 1, 0))
	{
		if (readLen == -1)
			ErrorHandling("read() Error");

		strLen += readLen;
	}

	printf("Message from server : %s \n", message);

	// 생성된 소켓 라이브러리 해제 
	closesocket(hSocket);

	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
