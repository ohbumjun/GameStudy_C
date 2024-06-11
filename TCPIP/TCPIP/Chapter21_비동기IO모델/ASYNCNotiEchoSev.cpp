
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <process.h>

using namespace std;

static const int BUF_SIZE = 100;

void CompressSockets(SOCKET hSorckArr[], int idx, int total);
void CompressEvents(WSAEVENT hEventArr[], int idx, int total);
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	SOCKET hSorckArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT hEventArr[WSA_MAXIMUM_WAIT_EVENTS]; // Event ������Ʈ �������� �迭
	WSAEVENT newEvent;
	WSANETWORKEVENTS netEvents;

	int numOfClntSock = 0;
	int strLen;
	int posInfo, startIdx;
	int clntAdrLen;
	char msg[BUF_SIZE];

	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

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

	// manual reset + non-signaled ������ Event ������Ʈ ����
	newEvent = WSACreateEvent();

	// ���ϰ� �̺�Ʈ ������Ʈ ����
	// ��, ���Ͽ� ���ؼ� �����û�� ���� ���, newEvent �� Signaled ���·� �ٲ�� �ȴ�.
	// hServSock ���Ͽ��� 3��° ���ڷ� ���޵� �̺�Ʈ �� �ϳ��� �߻��ϸ�, newEvent �� Ŀ�ο�����Ʈ�� signaled ���·� �ٲ۴�.
	// 3��° ���ڷ� ���ް����� �̺�Ʈ ����
	// FD_ACCEPT, FD_READ, FD_WRITE, FD_OOB, FD_CONNECT, FD_CLOSE, FD_QOS ���
	// ���� �ϳ���, �ѹ��� �Լ� ȣ���� ����ȴ�. selec �Լ��� �޸� �� �Լ� ȣ�⶧���� ������ �� �ʿ䰡 ����.
	if (WSAEventSelect(hServSock, newEvent, FD_ACCEPT) == SOCKET_ERROR)
		ErrorHandling("WSAEventSelect() Error");

	hSorckArr[numOfClntSock] = hServSock;
	hEventArr[numOfClntSock] = newEvent; // hSorckArr[i] �� hEventArr[i] �� ���� ����� �༮���̴�.

	numOfClntSock++;

	while (1)
	{
		// ���� �̺�Ʈ �߿��� �ϳ��� Signaled ���°� �Ǹ� ��ȯ
		// �׸��� �ϳ��� Signaled ���°� ���� �ʴ´ٸ�, ��ȯ���� �ʴ´�.
		// event ���� manual reset �̾�߸�, �ش� �Լ��� ȣ���Ѵٰ� �ϴ��� �ٽ� non-signaled �� ��������� �ʴ´�.
		posInfo = WSAWaitForMultipleEvents(
			numOfClntSock, hEventArr, FALSE, WSA_INFINITE, FALSE
		);

		// WSAWaitForMultipleEvents ���� fWaitAll �� TRUE ��, ���ϰ� - WSA_WAIT_EVENT_0 �� �ش� �̺�Ʈ ������Ʈ �迭����
		// signaled ���°� �� ������Ʈ idx �� �ȴ�.
		startIdx = posInfo - WSA_WAIT_EVENT_0;

		for (int i = startIdx; i < numOfClntSock; ++i)
		{
			// ������ �̺�Ʈ�� ���鼭 � �̺�Ʈ�� Signaled �������� Ȯ���Ѵ�.
			// ��, 0�� �Ѱ��־����Ƿ� ȣ�����ڸ��� ��ȯ�ϰ� �ȴ�.
			// �ش� event �� ����� ���Ͽ� �߻��� �̺�Ʈ ������ Ȯ���ϱ�
			int sigEventIdx = WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);

			// ���⿡ �ɸ��ٴ� ����, �ش� �̺�Ʈ ������Ʈ�� sinaled���°��� ������Ʈ�� �ƴ϶�� �ǹ��̴�.
			if ((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT))
				continue;
			else
			{
				sigEventIdx = i;

				// � �̺�Ʈ�� ���ؼ� signaled ���°� �Ǿ����� Ȯ���Ѵ�.
				// �ش� �Լ��� ȣ���ϰ� �Ǹ� hEventArr[sigEventIdx] �� non-signaled ���·� �ٲ�� �ȴ�.
				WSAEnumNetworkEvents(hSorckArr[sigEventIdx], hEventArr[sigEventIdx], &netEvents);

				// ���� ��û�� ���� Event�� �߻��ߴٸ�
				if (netEvents.lNetworkEvents & FD_ACCEPT)
				{
					// Error ���� Ȯ��
					if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
					{
						puts("Accept Error");
						break;
					}

					clntAdrLen = sizeof(clntAddr);
					
					// Ŭ���̾�Ʈ ���� ��û�� ���� ������ �����Ѵ�
					hClntSock = accept(hSorckArr[sigEventIdx], (SOCKADDR*)&clntAddr, &clntAdrLen);

					// �񵿱� Event �� ������.
					newEvent = WSACreateEvent();

					// hClntSock ���Ͽ� ���ؼ� ������ �����Ͱ� �����ϴ���. ���� ���ᰡ ��û�Ǿ����� �� Ȯ���Ѵ�.
					WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);

					hEventArr[numOfClntSock] = newEvent;

					hSorckArr[numOfClntSock] = hClntSock;

					numOfClntSock++;

					puts("connected new client ...");
				}

				// ������ ������ �� (���� ������ ������ �����Ͱ� ������ ��)
				if (netEvents.lNetworkEvents & FD_READ)
				{
					// Error ���� Ȯ��
					if (netEvents.iErrorCode[FD_READ_BIT] != 0)
					{
						puts("Read Error");
						break;
					}

					// �����͸� �����Ѵ�.
					strLen = recv(hSorckArr[sigEventIdx], msg, sizeof(msg), 0);

					fputs("Server Received Msg : ", stdout);
					fputs(msg, stdout);

					// �׸��� �ٽ� ������.
					send(hSorckArr[sigEventIdx], msg, strLen, 0);
				}

				// Ŭ���̾�Ʈ�κ��� ���� ���ᰡ ��û�Ǿ��ٸ�
				if (netEvents.lNetworkEvents & FD_CLOSE)
				{
					// Error ���� Ȯ��
					if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
					{
						puts("Close Error");
						break;
					}

					// �̺�Ʈ ������Ʈ�� �����Ѵ�.
					WSACloseEvent(hEventArr[sigEventIdx]);
					closesocket(hSorckArr[sigEventIdx]);

					numOfClntSock--;

					CompressSockets(hSorckArr, sigEventIdx, numOfClntSock);
					CompressEvents(hEventArr, sigEventIdx, numOfClntSock);
				}
			}
		}

	}

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

