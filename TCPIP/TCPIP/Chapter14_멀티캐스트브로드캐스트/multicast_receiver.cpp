/*
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

static const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hRecvSock;
	SOCKADDR_IN adr;
	struct ip_mreq joinAdr;
	char buf[BUF_SIZE];
	int strLen;

	if (argc != 3)
	{
		cout << "argc : " << argc << endl;
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// SOCK_DGRAM : UDP 소켓 생성
	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);

	// 소켓 생성
	if (hRecvSock == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&adr, 0, sizeof(adr));
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = htonl(INADDR_ANY);
	adr.sin_port = htons(atoi(argv[2]));

	// 목적지 소켓의 IP 주소, PORT 번호 할당 
	if (bind(hRecvSock, (SOCKADDR*)&adr, sizeof(adr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	// 데이터를 수신하는 측에서는 가입이라는 절차를 추가적으로 거쳐야 한다.
	joinAdr.imr_multiaddr.s_addr = inet_addr(argv[1]);
	joinAdr.imr_interface.s_addr = htonl(INADDR_ANY);

	// 그룹 가입
	if (setsockopt(hRecvSock, IPPROTO_IP, 
	IP_ADD_MEMBERSHIP, (const char*)&joinAdr, sizeof(joinAdr)) == SOCKET_ERROR)
		ErrorHandling("setsockopt Error");

	// UDP 소켓이므로 listen 함수가 별도로 필요하지 않다.
	// 1:1 로 미리 연결을 해놓는 것이 아니기 때문이다.
	while (1)
	{
		// recvFrom, sendTo 함수 -> unconnected 소켓을 이용한 전송, 수신 함수
		strLen = recvfrom(hRecvSock, buf, BUF_SIZE - 1, 0, NULL, 0);

		if (strLen < 0)
			break;

		buf[strLen] = 0;

		fputs(buf, stdout);
	}

	// 연결된 클라리언트에 데이터 전송
	closesocket(hRecvSock);

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