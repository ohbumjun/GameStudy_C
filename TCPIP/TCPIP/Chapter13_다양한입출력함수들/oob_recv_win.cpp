/*
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
	SOCKET hAcptSock, hRecvSock;
	SOCKADDR_IN recvAddr, sendAddr;
	int sendAdrSize, strLen;

	char buf[BUF_SIZE];
	int result;

	fd_set read, except, readCopy, exceptCopy;
	struct timeval timeout;

	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	hAcptSock = socket(PF_INET, SOCK_STREAM, 0); // ���� ����

	memset(&recvAddr, 0, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	recvAddr.sin_port = htons(atoi(argv[1]));

	// IP �ּ�, PORT ��ȣ �Ҵ�
	if (bind(hAcptSock, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	// ���� ��û ���� ���� -> ���� ��û ��� ť : 5�� ��û ��� ����
	if (listen(hAcptSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen Error");

	// Ŭ���̾�Ʈ ���� ��û �����ϱ�
	sendAdrSize = sizeof(sendAddr);

	// Ŭ���̾�Ʈ ���� ��û�� ���� ����
	hRecvSock = accept(hAcptSock, (SOCKADDR*)&sendAddr, &sendAdrSize);

	FD_ZERO(&read);
	FD_ZERO(&except);
	FD_SET (hRecvSock, &read);   // ���� �����Ͱ� �ִ� ���� ���� ���
	FD_SET (hRecvSock, &except); // ���� ������ ���� ���� ���

	while (1)
	{
		readCopy = read;
		exceptCopy = except;

		timeout.tv_sec  = 5;
		timeout.tv_usec = 0;

		result = select(0, &readCopy, 0, &exceptCopy, &timeout);

		// ���� ��� ��ȭ�� �߻��ߴٸ�
		if (result > 0)
		{
			// ���� ��Ȳ�� �߻��ߴٸ� (�� ���, ������ Ŭ���̾�Ʈ ���� MSG_OOB�� ���´ٸ�)
			if (FD_ISSET(hRecvSock, &exceptCopy))
			{
				strLen = recv(hRecvSock, buf, BUF_SIZE - 1, MSG_OOB);
				buf[strLen] = 0;
				printf("Urgent message : %s\n", buf);
			}

			// ������ �����Ͱ� �����Ѵٸ� 
			if (FD_ISSET(hRecvSock, &readCopy))
			{
				strLen = recv(hRecvSock, buf, BUF_SIZE - 1, 0);

				// ���� ���Ḧ �ǹ��ϴ� EOF �޽����� �����ߴٸ�
				if (strLen == 0)
				{
					break;
					closesocket(hRecvSock);
				}
				else
				{
					buf[strLen] = 0;
					puts(buf);
				}
			}
		}
	};

	// ����� Ŭ���̾�Ʈ�� ������ ����
	closesocket(hAcptSock);

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