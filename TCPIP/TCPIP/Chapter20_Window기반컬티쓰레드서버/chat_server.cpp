#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <process.h>

using namespace std;

static const int BUF_SIZE = 1024;
static const int MAX_CLNT = 256;

unsigned WINAPI HandleClnt(void* arg);
void SendMsg(char* msg, int len);
void ErrorHandling(const char* message);

int clntCnt = 0;
SOCKET clntSocks[MAX_CLNT];
HANDLE hMutex;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int clntAdrSize;
	HANDLE hThread;

	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// ���ؽ� ����
	hMutex = CreateMutex(NULL, FALSE, NULL);

	// ���� ����
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

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

	while (1)
	{
		// Ŭ���̾�Ʈ ���� ��û �����ϱ�
		clntAdrSize = sizeof(clntAddr);

		// ���� ��û�� ���� ���� ����
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAdrSize);

		WaitForSingleObject(hMutex, INFINITE);

		clntSocks[clntCnt++] = hClntSock;

		ReleaseMutex(hMutex);

		// �� Ŭ���̾�Ʈ ��û�� ���� ���ο� �����带 �����Ѵ�.
		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);

		printf("Connected %dth Client IP : %s\n", clntCnt, inet_ntoa(clntAddr.sin_addr));
	}


	// ����� Ŭ���̾�Ʈ�� ������ ����
	closesocket(hServSock);

	WSACleanup();

	return 0;
};

unsigned WINAPI HandleClnt(void* arg)
{
	SOCKET hClntSock = *((SOCKET*)arg);
	int strLen = 0;
	char msg[BUF_SIZE];

	// �ش� �����尡 �ϴ� ������
	// ������ Ŭ���̾�Ʈ�κ��� �����͸� �����ϴ� ���̴�.
	// �׸��� �ٸ� Ŭ���̾�Ʈ�鿡�� �����͸� �����ϴ� ���̴�.
	while ((strLen = recv(hClntSock, msg, sizeof(msg), 0)) != 0)
	{
		SendMsg(msg, strLen);
	}

	fputs("Client Disconnected", stdout);

	WaitForSingleObject(hMutex, INFINITE); // ���ؽ� ȹ��

	// remove disconnected client (��, �ڱ� �ڽ��� �����ϴ� ���̴�)
	for (int i = 0; i < clntCnt; ++i)
	{
		if (hClntSock == clntSocks[i])
		{
			while (i++ < clntCnt - 1)
				clntSocks[i] = clntSocks[i+1];
			break;
		}
	}

	clntCnt--;

	ReleaseMutex(hMutex); // ���ؽ� �ݳ�

	closesocket(hClntSock);

	return 0;
}

void SendMsg(char* msg, int len) // send To All
{
	WaitForSingleObject(hMutex, INFINITE);

	for (int i = 0; i < clntCnt; ++i)
		send(clntSocks[i], msg, len, 0);

	ReleaseMutex(hMutex);
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

