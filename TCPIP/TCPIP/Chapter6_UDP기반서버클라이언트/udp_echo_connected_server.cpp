/*
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>

using namespace std;

static const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);
int calculate(int opNum, int opnds[], char oprator);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int clntAdrSize;

	char message[BUF_SIZE];
	int strLen;

	if (argc != 2)
	{
		cout << "argc : " << argc << endl;
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// SOCK_DGRAM : UDP ���� ����
	hServSock = socket(PF_INET, SOCK_DGRAM, 0);

	// ���� ����
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// ������ ������ IP �ּ�, PORT ��ȣ �Ҵ� 
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	// UDP �����̹Ƿ� listen �Լ��� ������ �ʿ����� �ʴ�.
	// 1:1 �� �̸� ������ �س��� ���� �ƴϱ� �����̴�.
	while (1)
	{
		// recvFrom, sendTo �Լ� -> unconnected ������ �̿��� ����, ���� �Լ�
		clntAdrSize = sizeof(clntAddr);
		strLen = recvfrom(hServSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &clntAdrSize);
		sendto(hServSock, message, strLen, 0, (SOCKADDR*)&clntAddr, sizeof(clntAddr));
	}

	// ����� Ŭ�󸮾�Ʈ�� ������ ����
	closesocket(hServSock);

	WSACleanup();

	return 0;
};

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
*/