/*
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

static const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hRecvSock;
	SOCKADDR_IN adr;
	struct ip_mreq joinAdr;
	char buf[BUF_SIZE];
	int strLen;

	if (argc != 3)
	{
		cout << "argc : " << argc << endl;
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// SOCK_DGRAM : UDP ���� ����
	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);

	// ���� ����
	if (hRecvSock == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&adr, 0, sizeof(adr));
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = htonl(INADDR_ANY);
	adr.sin_port = htons(atoi(argv[2]));

	// ������ ������ IP �ּ�, PORT ��ȣ �Ҵ� 
	if (bind(hRecvSock, (SOCKADDR*)&adr, sizeof(adr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	// �����͸� �����ϴ� �������� �����̶�� ������ �߰������� ���ľ� �Ѵ�.
	joinAdr.imr_multiaddr.s_addr = inet_addr(argv[1]);
	joinAdr.imr_interface.s_addr = htonl(INADDR_ANY);

	// �׷� ����
	if (setsockopt(hRecvSock, IPPROTO_IP, 
	IP_ADD_MEMBERSHIP, (const char*)&joinAdr, sizeof(joinAdr)) == SOCKET_ERROR)
		ErrorHandling("setsockopt Error");

	// UDP �����̹Ƿ� listen �Լ��� ������ �ʿ����� �ʴ�.
	// 1:1 �� �̸� ������ �س��� ���� �ƴϱ� �����̴�.
	while (1)
	{
		// recvFrom, sendTo �Լ� -> unconnected ������ �̿��� ����, ���� �Լ�
		strLen = recvfrom(hRecvSock, buf, BUF_SIZE - 1, 0, NULL, 0);

		if (strLen < 0)
			break;

		buf[strLen] = 0;

		fputs(buf, stdout);
	}

	// ����� Ŭ�󸮾�Ʈ�� ������ ����
	closesocket(hRecvSock);

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