
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

static const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen = 0, idx = 0, readLen = 0, readCnt = 0;

	if (argc != 3)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// TCP 소켓
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	// 소켓 생성
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// 생성한 소켓을 바탕으로 서버에 연결 요청
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() Error");
	else
		printf("Connected......\n");

	while (1)
	{
		// 사용자로부터 입력을 받기
		fputs("Input message(Q to Quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		/*
		해당 코드는, read, write 함수가 호출될 때마다
		문자열 단위로, 실제 입출력이 이루어진다.
		라는 것을 가정하고 있다.

		하지만, TCP 는 데이터의 경계가 존재하지 않는다.
		해당 클라이언트는 TCP 클라이언트이다.

		둘 이상의 write 함수호출로 전달된 문자열 정보가 묶여서
		한번에 서버로 전송될 수 있다.

		한편, 이렇게 되면, 클라이언트는 한번에 둘 이상의
		문자열 정보를 서버로부터 되돌려 받아서, 원하는 결과를
		얻지 못할 수도 있다.

		즉, 서버 입장에서는
		"문자열의 길이가 제법 긴 편이니, 문자열을 두 개의 패킷에 나눠서
		보내야겠군" 이라고 생각하고

		서버는 한번의 write 함수를 호출로, 데이터 전송을 명령했지만
		전송할 데이터의 크기가 크다면,
		운영체제는 내부적으로 이를 여러 개의 조각으로 나눠서
		클라이언트에게 전송할 수도 있는 일이다.

		그리고, 클라이언트는 모든 조각이 서버로부터 전송되지 않았음에도 불구하고
		read 함수를 호출할지도 모른다.
		-----------------------------------------------------------------------------
		따라서 Client 입장에서는 자신이 전달한 메세지 크기만큼의
		문자열 데이터가 다시 돌아올 때까지 기다리는 로직을 구성할 것이다.
		*/
		strLen = send(hSocket, message, strlen(message), 0);

		readLen = 0;

		while (readLen < strLen)
		{
			readCnt = recv(hSocket, &message[readLen], BUF_SIZE - 1, 0);

			if (readCnt == -1)
				ErrorHandling("recv() Error");

			readLen += readCnt;
		}
		
		//message[strLen] = 0;
		message[readLen] = 0;
		
		
		printf("Message from server : %s \n", message);
	}

	// 생성된 소켓 라이브러리 해제 
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

