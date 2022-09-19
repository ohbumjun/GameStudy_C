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

	// Overlapped IO�� �����ϴ� ���� ����
	// ���ŷ ���� ����
	hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	
	// ���� ���� �ּҰ� ����
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// ���� ���������� ���� ��û
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() Error");

	while (1)
	{
		// Ŭ���̾�Ʈ�� ������ ������ ���ڿ� �Է�
		fputs("Input message(Q to Quit) : ", stdout);

		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		strLen = strlen(message);

		// �Է¹��� ���� ������ ����
		send(hSocket, message, strLen, 0);

		readLen = 0;

		// TCP�� ������ ��谡 �����Ƿ�
		// ������ �����͸� �����κ��� �ٽ� ���� ������ ������ �ݺ�
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

