
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

	char message[30];
	int strLen = 0, idx = 0, readLen = 0, readCnt = 0;

	if (argc != 3)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

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

	while (1)
	{
		// ����ڷκ��� �Է��� �ޱ�
		fputs("Input message(Q to Quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		/*
		�ش� �ڵ��, read, write �Լ��� ȣ��� ������
		���ڿ� ������, ���� ������� �̷������.
		��� ���� �����ϰ� �ִ�.

		������, TCP �� �������� ��谡 �������� �ʴ´�.
		�ش� Ŭ���̾�Ʈ�� TCP Ŭ���̾�Ʈ�̴�.

		�� �̻��� write �Լ�ȣ��� ���޵� ���ڿ� ������ ������
		�ѹ��� ������ ���۵� �� �ִ�.

		����, �̷��� �Ǹ�, Ŭ���̾�Ʈ�� �ѹ��� �� �̻���
		���ڿ� ������ �����κ��� �ǵ��� �޾Ƽ�, ���ϴ� �����
		���� ���� ���� �ִ�.

		��, ���� ���忡����
		"���ڿ��� ���̰� ���� �� ���̴�, ���ڿ��� �� ���� ��Ŷ�� ������
		�����߰ڱ�" �̶�� �����ϰ�

		������ �ѹ��� write �Լ��� ȣ���, ������ ������ ���������
		������ �������� ũ�Ⱑ ũ�ٸ�,
		�ü���� ���������� �̸� ���� ���� �������� ������
		Ŭ���̾�Ʈ���� ������ ���� �ִ� ���̴�.

		�׸���, Ŭ���̾�Ʈ�� ��� ������ �����κ��� ���۵��� �ʾ������� �ұ��ϰ�
		read �Լ��� ȣ�������� �𸥴�.
		-----------------------------------------------------------------------------
		���� Client ���忡���� �ڽ��� ������ �޼��� ũ�⸸ŭ��
		���ڿ� �����Ͱ� �ٽ� ���ƿ� ������ ��ٸ��� ������ ������ ���̴�.
		*/
		strLen = send(hSocket, message, strlen(message), 0);

		readLen = 0;

		while (readLen < strLen)
		{
			readCnt = recv(hSocket, &message[readLen], BUF_SIZE - 1, 0);

			if (readCnt == -1)
				ErrorHandling("recv() Error");

			readLen += readCnt;
		}
		
		//message[strLen] = 0;
		message[readLen] = 0;
		
		
		printf("Message from server : %s \n", message);
	}

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

