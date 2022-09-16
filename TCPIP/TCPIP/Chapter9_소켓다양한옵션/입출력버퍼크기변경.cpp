
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>

using namespace std;

static const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

void ShowSocketBufSize(SOCKET size);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	int sndBuf, rcvBuf, state;

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	ShowSocketBufSize(hSocket);

	sndBuf = 1024 * 3, rcvBuf = 1024 * 3;

	// ��� ���� ũ�� ����
	state = setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF,
		(char*)&sndBuf, sizeof(sndBuf));

	if (state == SOCKET_ERROR)
		ErrorHandling("setsockopt() Error");

	// �Է� ���� ũ�� ����
	state = setsockopt(hSocket, SOL_SOCKET, SO_SNDBUF,
		(char*)&rcvBuf, sizeof(rcvBuf));

	if (state == SOCKET_ERROR)
		ErrorHandling("setsockopt() Error");

	ShowSocketBufSize(hSocket);

	// ����� Ŭ�󸮾�Ʈ�� ������ ����
	closesocket(hSocket);

	WSACleanup();

	return 0;
};

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
};

void ShowSocketBufSize(SOCKET sock)
{
	int sndBuf, rcvBuf, state, len;

	len = sizeof(sndBuf);

	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf,
		&len);

	if (state == SOCKET_ERROR)
		ErrorHandling("getsockopt() error");

	len = sizeof(rcvBuf);

	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf,
		&len);

	if (state == SOCKET_ERROR)
		ErrorHandling("getsockopt() error");

	printf("Input Buffer size  : %d\n", rcvBuf);
	printf("Output Buffer size : %d\n", sndBuf);
}
