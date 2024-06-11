
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <process.h>

using namespace std;

static const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hListenSock, hRecvSock;
	SOCKADDR_IN listenAdr, recvAdr;
	int recvAdrSize;

	WSABUF dataBuf;
	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	char buf[BUF_SIZE];
	int recvBytes = 0, flags = 0;

	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// ���� ����
	// Overlapped IO�� ������ ��-���ŷ ����� ������ �����ȴ�.
	// hListenSock = socket(PF_INET, SOCK_STREAM, 0);
	hListenSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	memset(&listenAdr, 0, sizeof(listenAdr));
	listenAdr.sin_family = AF_INET;
	listenAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	listenAdr.sin_port = htons(atoi(argv[1]));

	// IP �ּ�, PORT ��ȣ �Ҵ�
	if (bind(hListenSock, (SOCKADDR*)&listenAdr, sizeof(listenAdr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	// ���� ��û ���� ���� -> ���� ��û ��� ť : 5�� ��û ��� ����
	if (listen(hListenSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen Error");

	// ���� ��û�� �����Ǵ� ���� ����
	recvAdrSize = sizeof(recvAdr);
	
	hRecvSock = accept(hListenSock, (SOCKADDR*)&recvAdr, &recvAdrSize);

	evObj = WSACreateEvent();

	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = evObj;

	// ���ŵ� ������ ������ ����� ������ ������ ���� WSABUF ����ü
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;

	// �ش� �Լ� ȣ�� ���Ŀ���, �������� ������ ��ӵȴٸ�
	if (WSARecv(hRecvSock, &dataBuf, 1, 
		(LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL) == SOCKET_ERROR)
	{
		// �ش� �Լ� ȣ�� ���Ŀ���, �������� ������ ��ӵȴٸ�
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			puts("Background data receive");

			// �ش� IO�� ������ overlapped.hEvent, �� evObj �̺�Ʈ Ŀ�� ������Ʈ�� signaled ���°� �ȴ�
			// signaled ���°� �� ������ ��ٸ���. (WSA_INFINITE)
			WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);

			// ���� ���۵� �������� ũ�⸦ Ȯ��
			WSAGetOverlappedResult(hRecvSock, 
				&overlapped, (LPDWORD)&recvBytes, FALSE, NULL);
		}
		else
		{
			ErrorHandling("WSARecv() Error");
		}
	}

	printf("Received message : %s\n", buf);
	WSACloseEvent(evObj);
	closesocket(hRecvSock);
	closesocket(hListenSock);


	WSACleanup();

	return 0;
};

void CompressSockets(SOCKET hSorckArr[], int idx, int total)
{
	for (int i = idx; i < total; ++i)
		hSorckArr[i] = hSorckArr[i + 1];
}

void CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
	for (int i = idx; i < total; ++i)
		hEventArr[i] = hEventArr[i + 1];
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

