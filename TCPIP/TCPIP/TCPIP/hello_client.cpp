#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen;

	if (argc != 3)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	// ���� ����
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[1]));

	// ������ ������ �������� ������ ���� ��û
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() Error");

	// recv �Լ� ȣ���� ���ؼ�, �����κ��� ���۵Ǵ� ������ ����
	strLen = recv(hSocket, message, sizeof(message) - 1, 0);

	if (strLen == -1)
		ErrorHandling("read() Error");

	printf("Message from server : %s \n", message);

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