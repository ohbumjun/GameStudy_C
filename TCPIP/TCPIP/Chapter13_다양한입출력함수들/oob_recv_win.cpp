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

	// 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	hAcptSock = socket(PF_INET, SOCK_STREAM, 0); // 서버 소켓

	memset(&recvAddr, 0, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	recvAddr.sin_port = htons(atoi(argv[1]));

	// IP 주소, PORT 번호 할당
	if (bind(hAcptSock, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	// 연결 요청 수락 상태 -> 연결 요청 대기 큐 : 5개 요청 대기 가능
	if (listen(hAcptSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen Error");

	// 클라이언트 연결 요청 수락하기
	sendAdrSize = sizeof(sendAddr);

	// 클라이언트 연결 요청에 대한 소켓
	hRecvSock = accept(hAcptSock, (SOCKADDR*)&sendAddr, &sendAdrSize);

	FD_ZERO(&read);
	FD_ZERO(&except);
	FD_SET (hRecvSock, &read);   // 수신 데이터가 있는 소켓 관찰 대상
	FD_SET (hRecvSock, &except); // 예외 데이터 소켓 관찰 대상

	while (1)
	{
		readCopy = read;
		exceptCopy = except;

		timeout.tv_sec  = 5;
		timeout.tv_usec = 0;

		result = select(0, &readCopy, 0, &exceptCopy, &timeout);

		// 관찰 대상에 변화가 발생했다면
		if (result > 0)
		{
			// 예외 상황이 발생했다면 (이 경우, 보내는 클라이언트 측이 MSG_OOB로 보냈다면)
			if (FD_ISSET(hRecvSock, &exceptCopy))
			{
				strLen = recv(hRecvSock, buf, BUF_SIZE - 1, MSG_OOB);
				buf[strLen] = 0;
				printf("Urgent message : %s\n", buf);
			}

			// 수신할 데이터가 존재한다면 
			if (FD_ISSET(hRecvSock, &readCopy))
			{
				strLen = recv(hRecvSock, buf, BUF_SIZE - 1, 0);

				// 연결 종료를 의미하는 EOF 메시지를 수신했다면
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

	// 연결된 클라이언트에 데이터 전송
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