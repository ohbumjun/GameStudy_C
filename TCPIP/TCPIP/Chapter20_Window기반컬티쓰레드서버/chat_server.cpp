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

	// 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// 뮤텍스 생성
	hMutex = CreateMutex(NULL, FALSE, NULL);

	// 서버 소켓
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// IP 주소, PORT 번호 할당
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	// 연결 요청 수락 상태 -> 연결 요청 대기 큐 : 5개 요청 대기 가능
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen Error");

	while (1)
	{
		// 클라이언트 연결 요청 수락하기
		clntAdrSize = sizeof(clntAddr);

		// 연결 요청에 대한 소켓 생성
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAdrSize);

		WaitForSingleObject(hMutex, INFINITE);

		clntSocks[clntCnt++] = hClntSock;

		ReleaseMutex(hMutex);

		// 매 클라이언트 요청에 대해 새로운 쓰레드를 생성한다.
		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);

		printf("Connected %dth Client IP : %s\n", clntCnt, inet_ntoa(clntAddr.sin_addr));
	}


	// 연결된 클라이언트에 데이터 전송
	closesocket(hServSock);

	WSACleanup();

	return 0;
};

unsigned WINAPI HandleClnt(void* arg)
{
	SOCKET hClntSock = *((SOCKET*)arg);
	int strLen = 0;
	char msg[BUF_SIZE];

	// 해당 쓰레드가 하는 역할은
	// 각각의 클라이언트로부터 데이터를 수신하는 것이다.
	// 그리도 다른 클라이언트들에게 데이터를 전송하는 것이다.
	while ((strLen = recv(hClntSock, msg, sizeof(msg), 0)) != 0)
	{
		SendMsg(msg, strLen);
	}

	fputs("Client Disconnected", stdout);

	WaitForSingleObject(hMutex, INFINITE); // 뮤텍스 획득

	// remove disconnected client (즉, 자기 자신을 제거하는 것이다)
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

	ReleaseMutex(hMutex); // 뮤텍스 반납

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

