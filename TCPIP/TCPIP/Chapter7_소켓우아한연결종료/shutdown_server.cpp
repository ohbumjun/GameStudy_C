
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
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int clntAdrSize;

	FILE* fp;
	char buf[BUF_SIZE];
	int readCnt;

	if (argc != 2)
	{
		cout << "argc : " << argc << endl;
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	fp = fopen("file_server_win.c", "rb");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	// 소켓 생성
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() Error");

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

	// 클라이언트 연결 요청 수락하기
	clntAdrSize = sizeof(clntAddr);

	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAdrSize);

	while (1)
	{
		readCnt = fread((void*)buf, 1, BUF_SIZE, fp);
	
		if (readCnt < BUF_SIZE)
		{
			send(hClntSock, (char*)&buf, readCnt, 0);
			break;
		}

		send(hClntSock, (char*)&buf, BUF_SIZE, 0);
	};

	// 출력 스트림을 종료한다. -> 단, 이미 출력 버퍼에 남아있는 데이터는 계속 전송한다.
	// SD_RECEIVE : 입력 스트림 종료
	shutdown(hClntSock, SD_SEND);

	// 입력 스트림은 열려 있기 때문에, 클라이언트가 전송한 데이터는 읽어들일 수 있다.
	recv(hClntSock, (char*)&buf, BUF_SIZE, 0);

	printf("Message from Client : %s\n", buf);

	fclose(fp);

	// 연결된 클라리언트에 데이터 전송
	closesocket(hClntSock);
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
