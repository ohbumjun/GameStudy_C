#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>
#include <bitset>

using namespace std;

static const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	
	char opMsg[BUF_SIZE];
	int opCnt = 0, result = 0;

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


	// 사용자로부터 입력을 받기
	fputs("Operand Count : ", stdout);

	// 1. 클라이언트는 서버에 접속하자마자, 피연산자의 개수정보를 1 바이트 정수 형태로 전달
	scanf("%d", &opCnt);

	opMsg[0] = (char)opCnt;

	cout << "opCnt : " <<  bitset<8>(opCnt) << endl;
	cout << "opMsg[0] : " <<  bitset<8>(opMsg[0]) << endl;

	// 2. 서버에 전달하는 정수 하나는 4바이트

	for (int i = 0; i < opCnt; ++i)
	{
		printf("Operand %d : ", i + 1);
		scanf("%d", (int*)&opMsg[i * 4 + 1]);
	}


	// 3. 정수를 전달한 다음 연산의 종류 선택.연산 정보는 1 바이트로 전달
	// 4. 문자 + , -, * 중 하나를 선택
	// - 개행 문자 지워주기
	fgetc(stdin);
	fputs("Operator : ", stdout);
	scanf("%c", &opMsg[1 + opCnt * 4]);

	// 5. 서버는 연산 결과를 4바이트 정수 형태로 클라이언트에게 전달
	// 한번의 write 함수 호출을 통해 묶어서 보내도 되고
	// 여러 번의 write 함수 호출을 통해 나눠서 보내도 된다.
	// 피연산자 개수, 피연산자, 연산자
	send(hSocket, opMsg, 1 + 1 + opCnt * 4, 0);

	recv(hSocket, (char*)&result, 4, 0);

	printf("Answer from server : %d \n", result);

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

