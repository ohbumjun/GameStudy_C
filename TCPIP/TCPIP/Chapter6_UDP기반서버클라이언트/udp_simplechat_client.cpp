#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <bitset>

using namespace std;

static const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[BUF_SIZE];
	int strLen;

	if (argc != 3)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// UDP ����
	hSocket = socket(PF_INET, SOCK_DGRAM, 0);

	// ���� ����
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// UDP ���Ͽ� ���� connect �Լ� ȣ�� --> connected �������� �����
	connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr));

	while (1)
	{
		fputs("Insert Message (q to quit) : ", stdout);

		fgets(message, sizeof(message), stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		// send, recv --> connected ������ �̿��� ������ �ۼ��ſ� ����
		send(hSocket, message, strlen(message), 0);

		strLen = recv(hSocket, message, sizeof(message) - 1, 0);

		message[strLen] = 0;

		printf("Message From Server : %s", message);
	}

	closesocket(hSocket);

	// ������ ���� ���̺귯�� ����
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
