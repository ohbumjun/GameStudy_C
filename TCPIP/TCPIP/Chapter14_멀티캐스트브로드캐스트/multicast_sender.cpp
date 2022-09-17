#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> // IP_MULTICAST_TLL 옵션을 위한 헤더 

using namespace std;

static const int BUF_SIZE = 1024;
static const int TTL = 64;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSendSock;
	SOCKADDR_IN mulAdr;
	int clntAdrSize, timeLive = TTL;
	FILE* fp;
	char buf[BUF_SIZE];

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
	hSendSock = socket(PF_INET, SOCK_DGRAM, 0);

	// 소켓 생성
	if (hSendSock == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&mulAdr, 0, sizeof(mulAdr));
	mulAdr.sin_family = AF_INET;
	// mulAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	// 중요한 점은, 반드시 IP 주소를 멀티캐스트 주소로 설정해야 한다는 것이다.
	mulAdr.sin_addr.s_addr = inet_addr(argv[1]);
	mulAdr.sin_port = htons(atoi(argv[2]));

	// 멀티캐스트 옵션 세팅 (TTP 정보 세팅)
	setsockopt(hSendSock, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)(void*)&timeLive, sizeof(timeLive));

	if ((fp = fopen("news.txt", "r")) == NULL)
		ErrorHandling("fopen() Error");

	// UDP 소켓이므로 listen 함수가 별도로 필요하지 않다.
	// 1:1 로 미리 연결을 해놓는 것이 아니기 때문이다.
	while (!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);

		fputs(buf, stdout);
		
		// UDP 소켓 기반 데이터 전송이므로 sendto 함수 사용
		sendto(hSendSock, buf, strlen(buf), 0, (SOCKADDR*)&mulAdr, sizeof(mulAdr));

		// 데이터 전송에 약간의 시간간격을 두기 위한 것
		Sleep(2000);
	}

	// 연결된 클라리언트에 데이터 전송
	closesocket(hSendSock);

	WSACleanup();

	return 0;
};

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
