#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <process.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN sendAddr;

	WSABUF dataBuf;
	char msg[] = "Network is complete";
	int sendBytes = 0;

	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	memset(&sendAddr, 0, sizeof(sendAddr));
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_addr.s_addr = inet_addr(argv[1]);
	sendAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&sendAddr, sizeof(sendAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() Error");

	// manual-reset, non-signaled ������ �̺�Ʈ ����
	evObj = WSACreateEvent();

	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = evObj;
	dataBuf.len = strlen(msg) - 1;
	dataBuf.buf = msg;

	// WSASend �Լ��� ȣ���ϰ����� ������ IO�� �������̶��, ��, �����͸� ����ؼ� �������̶��
	// ���� ���ϰ��� SOCKET_ERROR �� �ƴ϶��, �ش� �Լ� ȣ��� ���ÿ� ������ ������ �Ϸ�� ��, �׸��� sendBytes ���� ������ �������� ũ�Ⱑ ����ִ�.
	if (WSASend(hSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
	{
		// WSASend �Լ��� ȣ���ϰ����� ������ IO�� �������̶��, ��, �����͸� ����ؼ� �������̶��
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			puts("Background data send");
			
			// �ش� IO�� ������ overlapped.hEvent, �� evObj �̺�Ʈ Ŀ�� ������Ʈ�� signaled ���°� �ȴ�
			// signaled ���°� �� ������ ��ٸ���.
			WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);

			// ���� ���۵� �������� ũ�⸦ Ȯ��
			WSAGetOverlappedResult(hSocket, &overlapped, (LPDWORD)&sendBytes, FALSE, NULL);
		}
		else
		{
			ErrorHandling("WSASend() Error");
		}
	}

	printf("Send data size : %d \n", sendBytes);
	WSACloseEvent(evObj);
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

