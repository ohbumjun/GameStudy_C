
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
	WSAEVENT hEventArr[WSA_MAXIMUM_WAIT_EVENTS]; // Event 오브젝트 정보들의 배열
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

	// 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

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

	// manual reset + non-signaled 형태의 Event 오브젝트 형성
	newEvent = WSACreateEvent();

	// 소켓과 이벤트 오브젝트 연결
	// 즉, 소켓에 대해서 연결요청이 있을 경우, newEvent 가 Signaled 상태로 바뀌게 된다.
	// hServSock 소켓에서 3번째 인자로 전달된 이벤트 중 하나라도 발생하면, newEvent 의 커널오브젝트를 signaled 상태로 바꾼다.
	// 3번째 인자로 전달가능한 이벤트 종류
	// FD_ACCEPT, FD_READ, FD_WRITE, FD_OOB, FD_CONNECT, FD_CLOSE, FD_QOS 등등
	// 소켓 하나당, 한번의 함수 호출이 진행된다. selec 함수와 달리 매 함수 호출때마다 재등록을 할 필요가 없다.
	if (WSAEventSelect(hServSock, newEvent, FD_ACCEPT) == SOCKET_ERROR)
		ErrorHandling("WSAEventSelect() Error");

	hSorckArr[numOfClntSock] = hServSock;
	hEventArr[numOfClntSock] = newEvent; // hSorckArr[i] 와 hEventArr[i] 는 서로 연결된 녀석들이다.

	numOfClntSock++;

	while (1)
	{
		// 여러 이벤트 중에서 하나라도 Signaled 상태가 되면 반환
		// 그리고 하나라도 Signaled 상태가 되지 않는다면, 반환하지 않는다.
		// event 들이 manual reset 이어야만, 해당 함수를 호출한다고 하더라도 다시 non-signaled 로 만들어지지 않는다.
		posInfo = WSAWaitForMultipleEvents(
			numOfClntSock, hEventArr, FALSE, WSA_INFINITE, FALSE
		);

		// WSAWaitForMultipleEvents 에서 fWaitAll 가 TRUE 면, 리턴값 - WSA_WAIT_EVENT_0 는 해당 이벤트 오브젝트 배열에서
		// signaled 상태가 된 오브젝트 idx 가 된다.
		startIdx = posInfo - WSA_WAIT_EVENT_0;

		for (int i = startIdx; i < numOfClntSock; ++i)
		{
			// 각각의 이벤트를 돌면서 어떤 이벤트가 Signaled 상태인지 확인한다.
			// 단, 0을 넘겨주었으므로 호출하자마자 반환하게 된다.
			// 해당 event 와 연결된 소켓에 발생한 이벤트 정보를 확인하기
			int sigEventIdx = WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);

			// 여기에 걸린다는 것은, 해당 이벤트 오브젝트는 sinaled상태가된 오브젝트가 아니라는 의미이다.
			if ((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT))
				continue;
			else
			{
				sigEventIdx = i;

				// 어떤 이벤트로 인해서 signaled 상태가 되었는지 확인한다.
				// 해당 함수를 호출하게 되면 hEventArr[sigEventIdx] 는 non-signaled 상태로 바뀌게 된다.
				WSAEnumNetworkEvents(hSorckArr[sigEventIdx], hEventArr[sigEventIdx], &netEvents);

				// 연결 요청에 대한 Event가 발생했다면
				if (netEvents.lNetworkEvents & FD_ACCEPT)
				{
					// Error 여부 확인
					if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
					{
						puts("Accept Error");
						break;
					}

					clntAdrLen = sizeof(clntAddr);
					
					// 클라이언트 연결 요청에 대한 소켓을 생성한다
					hClntSock = accept(hSorckArr[sigEventIdx], (SOCKADDR*)&clntAddr, &clntAdrLen);

					// 비동기 Event 를 만들어낸다.
					newEvent = WSACreateEvent();

					// hClntSock 소켓에 대해서 수신할 데이터가 존재하는지. 연결 종료가 요청되었는지 를 확인한다.
					WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);

					hEventArr[numOfClntSock] = newEvent;

					hSorckArr[numOfClntSock] = hClntSock;

					numOfClntSock++;

					puts("connected new client ...");
				}

				// 데이터 수신할 때 (서버 소켓이 수신할 데이터가 존재할 때)
				if (netEvents.lNetworkEvents & FD_READ)
				{
					// Error 여부 확인
					if (netEvents.iErrorCode[FD_READ_BIT] != 0)
					{
						puts("Read Error");
						break;
					}

					// 데이터를 수신한다.
					strLen = recv(hSorckArr[sigEventIdx], msg, sizeof(msg), 0);

					fputs("Server Received Msg : ", stdout);
					fputs(msg, stdout);

					// 그리고 다시 보낸다.
					send(hSorckArr[sigEventIdx], msg, strLen, 0);
				}

				// 클라이언트로부터 연결 종료가 요청되었다면
				if (netEvents.lNetworkEvents & FD_CLOSE)
				{
					// Error 여부 확인
					if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
					{
						puts("Close Error");
						break;
					}

					// 이벤트 오브젝트를 삭제한다.
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

