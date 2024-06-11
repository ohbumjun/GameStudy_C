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

	// WSA_FLAG_OVERLAPPED : overlapped io 가 가능한 소켓의 생성
	hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	memset(&sendAddr, 0, sizeof(sendAddr));
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_addr.s_addr = inet_addr(argv[1]);
	sendAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&sendAddr, sizeof(sendAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() Error");

	// manual-reset, non-signaled 형태의 이벤트 생성
	evObj = WSACreateEvent();

	// 모든 비트 0으로 초기화
	memset(&overlapped, 0, sizeof(overlapped));

	// 해당 overlapped 구조체 변수의 event 오브젝트를 사용하여 데이터의 전송 등이
	// 완료되었는지 등을 확인한다.
	overlapped.hEvent = evObj;
	dataBuf.len = strlen(msg) - 1;
	dataBuf.buf = msg;

	// WSASend 함수를 호출하고나서도 여전히 IO가 진행중이라면, 즉, 데이터를 계속해서 전송중이라면
	// 만약 리턴값이 SOCKET_ERROR 가 아니라면, 
	// 해당 함수 호출과 동시에 데이터 전송이 완료된 것, 그리고 sendBytes 에는 전송한 데이터의 크기가 들어있다.
	// IO 가 완료되면 WSAOverlapped 구조체 변수가 참조하는 Event 오브젝트는 signaled 상태가 된다.
	if (WSASend(hSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
	{
		// WSASend 함수를 호출하고나서도 여전히 IO가 진행중이라면, 즉, 데이터를 계속해서 전송중이라면
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			puts("Background data send");
			
			// 해당 IO가 끝나면 overlapped.hEvent, 즉 evObj 이벤트 커널 오브젝트가 signaled 상태가 된다
			// signaled 상태가 될 때까지 기다린다. (WSA_INFINITE)
			WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);

			// 실제 전송된 데이터의 크기를 확인
			// 4번째 : fwait -> 여전히 io 가 진행중인 상황인 경우, true 전달시 io 가 완료될 때까지
			//							대기하게 되고 false 를 전달시 FALSE 반환하면서 함수 빠져나온다.
			//	5번째 : oob 메시지 등 부가 정보를 확인하기 위한 정보
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

