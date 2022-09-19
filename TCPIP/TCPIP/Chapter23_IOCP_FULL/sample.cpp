#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <process.h>
#include <windows.h>

#define BUF_SIZE 100
#define READ 3
#define WRITE 5

typedef struct
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

// buffer Info
typedef struct
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int rwMode; // Read Or Write
} PER_IO_DATA, * LPPER_IO_DATA;

unsigned __stdcall EchoThreadMain(LPVOID CompleitionPortID);
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	HANDLE hCompPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSok;
	SOCKADDR_IN servAdr;

	int recvBytes, i, flags = 0;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() error!");

	// CP ������Ʈ ����
	hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	GetSystemInfo(&sysInfo);

	// ���μ��� ������ŭ�� ������ ����
	// ���ڷ� CP ������Ʈ�� �ڵ� ����
	// ������� �ش� ���ڸ� ���� CP ������Ʈ�� ����
	for (int i = 0; i < sysInfo.dwNumberOfProcessors; ++i)
		_beginthreadex(NULL, 0, EchoThreadMain, (LPVOID)hCompPort, 0, NULL);

	// Overlapped IO �� �����ϴ� ���� Socket ����
	// ������ ���ŷ ���
	hServSok = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	// IP �ּ�, PORT ��ȣ �Ҵ�
	if (bind(hServSok, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");

	// Ŭ���̾�Ʈ ���� ��û ��� ���·� �����
	// ���� �䫊 ��� ť ������� 6���� ����
	if (listen(hServSok, 6) == SOCKET_ERROR)
		ErrorHandling("listen() error!");

	while (1)
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);

		// Ŭ���̾�Ʈ ���� ��û�� ���� ���ο� ���� ����
		// ���� ��û ��� ť����, Ŭ���̾�Ʈ ��û�� ������ �׿� �´� ���� ���Ӱ� ����
		hClntSock = accept(hServSok, (SOCKADDR*)&clntAdr, &addrLen);

		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		handleInfo->hClntSock = hClntSock;

		// ���Ӱ� ������ ������ �ּ� ���� ����
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		// CP ������Ʈ�� Ŭ���̾�Ʈ ���� ��û ���� ����
		CreateIoCompletionPort((HANDLE)hClntSock, hCompPort, (DWORD)handleInfo, 0);

		ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;

		// ����� IO ����
		ioInfo->rwMode = READ;

		// �񵿱� ������ ����
		// Overlapped IO ���·� ���� 
		WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &(ioInfo->overlapped), NULL);
	}

}

// �����尡 ȣ���ϴ� ���� �Լ�
unsigned __stdcall EchoThreadMain(LPVOID CompleitionPortIO)
{
	// CP ������Ʈ �ڵ��� ���ڷ� �Ѱ��ش�.
	HANDLE hCompPort = (HANDLE)CompleitionPortIO;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;

	while (1)
	{
		/*
		�Ϸ�� IO�� Ȯ��, �̸� ���� Ŭ���̾�Ʈ ���� ��û ���Ͽ� ���� ���� + overlapped ����ü�� ���� ���� ȹ��

		IO�� �Ϸ�ǰ� �̿� ���� ������ CP ������Ʈ�� ��ϵǾ��� �� ��ȯ.

		��ȯ�� 3��°, 4��° ���ڷ� ��� ������ �������� ��Ȯ�ϰ� �˾ƾ� �Ѵ�.
		CreateIOCompleitionPort �Լ��� WSARecv �Լ��� ������ ���� ������
		�̶� ��ȯ �޾Ƽ� ����� �� �ִ�.

		��, 

		��������� �ش� �Լ��� ȣ����,
		Completion Port ������Ʈ�� �����带 �Ҵ��ϴ� ���̴�.
		*/
		GetQueuedCompletionStatus(hCompPort, &bytesTrans,
			(PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);

		sock = handleInfo->hClntSock; // IO�� �Ϸ�� ������ �ڵ�

		// �Ϸ�� IO�� ������ �����̶�� 
		if (ioInfo->rwMode == READ)
		{
			puts("message received");

			// Ŭ���̾�Ʈ ���� ���� ��û (EOF)
			if (bytesTrans == 0)
			{
				closesocket(sock);

				free(handleInfo);
				free(ioInfo);
				continue;
			}

			// ������ ������ �޼����� Ŭ���̾�Ʈ���� ������
			// OVERLAPPED ����ü ���� ���� �ʱ�ȭ

			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = bytesTrans; // ������ ������ ũ�⸸ŭ
			ioInfo->rwMode = WRITE; // ������ ���� ���

			WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

			// Ŭ���̾�Ʈ�� �޼��� ������ ����
			// Ŭ���̾�Ʈ�κ��� �ٽ� �޼��� ����
			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));

			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ; // ������ ���� ���

			// �ٽ� �ش� Ŭ���̾�Ʈ ���� ��û ������ �����ϴ� �����͸�
			// �񵿱�� ���� (�ݺ�)
			WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL,
				&flags, &(ioInfo->overlapped), NULL);
		}
		// �Ϸ�� IO�� ������ �۽��̶�� 
		else
		{
			puts("message sent");

			free(ioInfo);
		}
	}

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}