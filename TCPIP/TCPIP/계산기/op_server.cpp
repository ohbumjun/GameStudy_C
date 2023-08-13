/*
계산기 서버
1. 클라이언트는 서버에 접속하자마자, 피연산자의 개수정보를 1 바이트 정수 형태로 전달
2. 서버에 전달하는 정수 하나는 4바이트
3. 정수를 전달한 다음 연산의 종류 선택. 연산 정보는 1 바이트로 전달
4. 문자 +, -, * 중 하나를 선택
5. 서버는 연산 결과를 4바이트 정수 형태로 클라이언트에게 전달
6. 연산 결과 얻은 클라이언트는 서버와의 연결 종로
*/

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

	char opInfo[BUF_SIZE];
	int  opCnt = 0, result, recvCnt, recvLen;

	if (argc != 2)
	{
		cout << "argc : " << argc << endl;
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

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

	if (hClntSock == -1)
		ErrorHandling("accept() error");
	else
		printf("Connected client %d\n", 1);

	// TCP 는 데이터의 경계가 없다.
	// 클라이언트가 여러 번의 write 함수를 호출했더라고 하더라도
	// 서버는 한번의 receive 함수를 호출하면 된다.
	opCnt = 0;
	int readOpCnt = recv(hClntSock, (char*)&opCnt, 1, 0);

	cout << "opCnt        : " << opCnt << endl;

	recvLen = 0;

	// (int)opCnt[0] * 4 : 피연산자들 크기
	// + 1 : 연산자 종류
	while ((opCnt * 4 + 1) > recvLen)
	{
		int prevLen = recvLen;
		recvCnt = recv(hClntSock, &opInfo[recvLen], BUF_SIZE - 1, 0);
		recvLen += recvCnt;
	}
	
	result = calculate(opCnt, (int*)opInfo, opInfo[recvLen - 1]);

	send(hClntSock, (char*)&result, sizeof(result), 0);

	closesocket(hClntSock);

	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() Error");

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

int calculate(int opNum, int opnds[], char oprator)
{
	int result = opnds[0], i;

	switch (oprator)
	{
	case '+' :
		for (int i = 1; i < opNum; ++i)
			result += opnds[i];
		break;
	case '-':
		for (int i = 1; i < opNum; ++i)
			result -= opnds[i];
		break;
	case '*':
		for (int i = 1; i < opNum; ++i)
			result *= opnds[i];
		break;
	}

	return result;
}
*/