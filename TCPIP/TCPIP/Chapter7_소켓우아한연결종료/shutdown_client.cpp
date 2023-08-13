
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

static const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	FILE* fp;
	char buf[BUF_SIZE];
	int readCnt;

	if (argc != 3)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	fp = fopen("receive.dat", "wb");

	// TCP ����
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	// ���� ����
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// ������ ������ �������� ������ ���� ��û
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() Error");
	else
		printf("Connected......\n");

	while ((readCnt = recv(hSocket, buf, BUF_SIZE, 0)) != 0)
		fwrite((void*)buf, 1, readCnt, fp);

	puts("Received file Data");

	// �������� ������ ��� ���� �޾Ҵٰ� �˷��ֱ�
	// ������ �Է� ��Ʈ���� �������� �ʴٸ�, ������ �ش� ������ ���� �� �ְ� �ȴ�.
	send(hSocket, "Thank you", 10, 0);

	fclose(fp);

	// ������ ���� ���̺귯�� ���� 
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

