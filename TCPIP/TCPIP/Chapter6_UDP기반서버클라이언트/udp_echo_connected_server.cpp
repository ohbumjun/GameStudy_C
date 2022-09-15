/*
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>

using namespace std;

static const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);
int calculate(int opNum, int opnds[], char oprator);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int clntAdrSize;

	char message[BUF_SIZE];
	int strLen;

	if (argc != 2)
	{
		cout << "argc : " << argc << endl;
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// SOCK_DGRAM : UDP 소켓 생성
	hServSock = socket(PF_INET, SOCK_DGRAM, 0);

	// 소켓 생성
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// 목적지 소켓의 IP 주소, PORT 번호 할당 
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	// UDP 소켓이므로 listen 함수가 별도로 필요하지 않다.
	// 1:1 로 미리 연결을 해놓는 것이 아니기 때문이다.
	while (1)
	{
		// recvFrom, sendTo 함수 -> unconnected 소켓을 이용한 전송, 수신 함수
		clntAdrSize = sizeof(clntAddr);
		strLen = recvfrom(hServSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &clntAdrSize);
		sendto(hServSock, message, strLen, 0, (SOCKADDR*)&clntAddr, sizeof(clntAddr));
	}

	// 연결된 클라리언트에 데이터 전송
	closesocket(hServSock);

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