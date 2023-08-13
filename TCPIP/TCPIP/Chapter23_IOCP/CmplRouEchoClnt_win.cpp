#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <process.h>

#define BUF_SIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	WSABUF dataBuf;
	char message[BUF_SIZE];
	int strLen, readLen;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// Overlapped IO로 진행하는 소켓 생성
	// 블로킹 모드로 진행
	hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	
	// 서버 소켓 주소값 세팅
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// 서버 소켓으로의 연결 요청
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() Error");

	while (1)
	{
		// 클라이언트가 서버로 전송할 문자열 입력
		fputs("Input message(Q to Quit) : ", stdout);

		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		strLen = strlen(message);

		// 입력받은 문자 서버로 전송
		send(hSocket, message, strLen, 0);

		readLen = 0;

		// TCP는 데이터 경계가 없으므로
		// 전송한 데이터를 서버로부터 다시 전부 수신할 때까지 반복
		while (1)
		{
			readLen = recv(hSocket, &message[readLen], BUF_SIZE - 1, 0);

			if (readLen >= strLen)
				break;
		}

		message[strLen] = '\0';
		printf("Message From Server : %s", message);
	}

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

