#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> // IP_MULTICAST_TLL �ɼ��� ���� ��� 

using namespace std;

static const int BUF_SIZE = 1024;
static const int TTL = 64;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSendSock;
	SOCKADDR_IN mulAdr;
	int clntAdrSize, timeLive = TTL;
	FILE* fp;
	char buf[BUF_SIZE];

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
	hSendSock = socket(PF_INET, SOCK_DGRAM, 0);

	// ���� ����
	if (hSendSock == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&mulAdr, 0, sizeof(mulAdr));
	mulAdr.sin_family = AF_INET;
	// mulAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	// �߿��� ����, �ݵ�� IP �ּҸ� ��Ƽĳ��Ʈ �ּҷ� �����ؾ� �Ѵٴ� ���̴�.
	mulAdr.sin_addr.s_addr = inet_addr(argv[1]);
	mulAdr.sin_port = htons(atoi(argv[2]));

	// ��Ƽĳ��Ʈ �ɼ� ���� (TTP ���� ����)
	setsockopt(hSendSock, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)(void*)&timeLive, sizeof(timeLive));

	if ((fp = fopen("news.txt", "r")) == NULL)
		ErrorHandling("fopen() Error");

	// UDP �����̹Ƿ� listen �Լ��� ������ �ʿ����� �ʴ�.
	// 1:1 �� �̸� ������ �س��� ���� �ƴϱ� �����̴�.
	while (!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);

		fputs(buf, stdout);
		
		// UDP ���� ��� ������ �����̹Ƿ� sendto �Լ� ���
		sendto(hSendSock, buf, strlen(buf), 0, (SOCKADDR*)&mulAdr, sizeof(mulAdr));

		// ������ ���ۿ� �ణ�� �ð������� �α� ���� ��
		Sleep(2000);
	}

	// ����� Ŭ�󸮾�Ʈ�� ������ ����
	closesocket(hSendSock);

	WSACleanup();

	return 0;
};

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
