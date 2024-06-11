
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

	// 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// 서버 소켓
	// Overlapped IO가 가능한 넌-블로킹 모드의 소켓이 생성된다.
	// hListenSock = socket(PF_INET, SOCK_STREAM, 0);
	hListenSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	memset(&listenAdr, 0, sizeof(listenAdr));
	listenAdr.sin_family = AF_INET;
	listenAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	listenAdr.sin_port = htons(atoi(argv[1]));

	// IP 주소, PORT 번호 할당
	if (bind(hListenSock, (SOCKADDR*)&listenAdr, sizeof(listenAdr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	// 연결 요청 수락 상태 -> 연결 요청 대기 큐 : 5개 요청 대기 가능
	if (listen(hListenSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen Error");

	// 연결 요청에 대응되는 소켓 생성
	recvAdrSize = sizeof(recvAdr);
	
	hRecvSock = accept(hListenSock, (SOCKADDR*)&recvAdr, &recvAdrSize);

	evObj = WSACreateEvent();

	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = evObj;

	// 수신된 데이터 정보가 저장될 버퍼의 정보를 지닌 WSABUF 구조체
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;

	// 해당 함수 호출 이후에도, 데이터의 수신이 계속된다면
	if (WSARecv(hRecvSock, &dataBuf, 1, 
		(LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL) == SOCKET_ERROR)
	{
		// 해당 함수 호출 이후에도, 데이터의 수신이 계속된다면
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			puts("Background data receive");

			// 해당 IO가 끝나면 overlapped.hEvent, 즉 evObj 이벤트 커널 오브젝트가 signaled 상태가 된다
			// signaled 상태가 될 때까지 기다린다. (WSA_INFINITE)
			WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);

			// 실제 전송된 데이터의 크기를 확인
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

