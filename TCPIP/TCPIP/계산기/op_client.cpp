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

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// TCP ����
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	// ���� ����
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	// ������ ������ �������� ������ ���� ��û
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() Error");
	else
		printf("Connected......\n");


	// ����ڷκ��� �Է��� �ޱ�
	fputs("Operand Count : ", stdout);

	// 1. Ŭ���̾�Ʈ�� ������ �������ڸ���, �ǿ������� ���������� 1 ����Ʈ ���� ���·� ����
	scanf("%d", &opCnt);

	opMsg[0] = (char)opCnt;

	cout << "opCnt : " <<  bitset<8>(opCnt) << endl;
	cout << "opMsg[0] : " <<  bitset<8>(opMsg[0]) << endl;

	// 2. ������ �����ϴ� ���� �ϳ��� 4����Ʈ

	for (int i = 0; i < opCnt; ++i)
	{
		printf("Operand %d : ", i + 1);
		scanf("%d", (int*)&opMsg[i * 4 + 1]);
	}


	// 3. ������ ������ ���� ������ ���� ����.���� ������ 1 ����Ʈ�� ����
	// 4. ���� + , -, * �� �ϳ��� ����
	// - ���� ���� �����ֱ�
	fgetc(stdin);
	fputs("Operator : ", stdout);
	scanf("%c", &opMsg[1 + opCnt * 4]);

	// 5. ������ ���� ����� 4����Ʈ ���� ���·� Ŭ���̾�Ʈ���� ����
	// �ѹ��� write �Լ� ȣ���� ���� ��� ������ �ǰ�
	// ���� ���� write �Լ� ȣ���� ���� ������ ������ �ȴ�.
	// �ǿ����� ����, �ǿ�����, ������
	send(hSocket, opMsg, 1 + 1 + opCnt * 4, 0);

	recv(hSocket, (char*)&result, 4, 0);

	printf("Answer from server : %d \n", result);

	// ������ ���� ���̺귯�� ���� 
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

