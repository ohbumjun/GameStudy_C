/*
Iterative Echo ����
1. ������ �� ������ �ϳ��� Ŭ���̾�Ʈ�� ����Ǿ� ���� ���� ����
2. ������ �� 5���� Ŭ���̾�Ʈ���� ���������� ���� �����ϰ� ����
3. Ŭ���̾�Ʈ�� ���α׷� ����ڷκ��� ���ڿ� �����͸� �Է� �޾Ƽ� ������ ����
4. ������ ���� ���� ���ڿ� �����͸� Ŭ���̾�Ʈ���� ������. ��, ���� ��Ų��.
5. ������ Ŭ���̾�Ʈ���� ���ڿ� ���ڴ� Ŭ���̾�Ʈ�� Q�� �Է��� ������ ���

*/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>

using namespace std;

static const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int clntAdrSize;

	FILE* fp;
	char buf[BUF_SIZE];
	int readCnt;

	if (argc != 2)
	{
		cout << "argc : " << argc << endl;
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	fp = fopen("file_server_win.c", "rb");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	// ���� ����
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// IP �ּ�, PORT ��ȣ �Ҵ�
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	// ���� ��û ���� ���� -> ���� ��û ��� ť : 5�� ��û ��� ����
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen Error");

	// Ŭ���̾�Ʈ ���� ��û �����ϱ�
	clntAdrSize = sizeof(clntAddr);

	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAdrSize);

	while (1)
	{
		readCnt = fread((void*)buf, 1, BUF_SIZE, fp);
	
		if (readCnt < BUF_SIZE)
		{
			send(hClntSock, (char*)&buf, readCnt, 0);
			break;
		}

		send(hClntSock, (char*)&buf, BUF_SIZE, 0);
	};

	// ��� ��Ʈ���� �����Ѵ�. -> ��, �̹� ��� ���ۿ� �����ִ� �����ʹ� ��� �����Ѵ�.
	// SD_RECEIVE : �Է� ��Ʈ�� ����
	shutdown(hClntSock, SD_SEND);

	// �Է� ��Ʈ���� ���� �ֱ� ������, Ŭ���̾�Ʈ�� ������ �����ʹ� �о���� �� �ִ�.
	recv(hClntSock, (char*)&buf, BUF_SIZE, 0);

	printf("Message from Client : %s\n", buf);

	fclose(fp);

	// ����� Ŭ�󸮾�Ʈ�� ������ ����
	closesocket(hClntSock);
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
