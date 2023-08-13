/*
���� ����
1. Ŭ���̾�Ʈ�� ������ �������ڸ���, �ǿ������� ���������� 1 ����Ʈ ���� ���·� ����
2. ������ �����ϴ� ���� �ϳ��� 4����Ʈ
3. ������ ������ ���� ������ ���� ����. ���� ������ 1 ����Ʈ�� ����
4. ���� +, -, * �� �ϳ��� ����
5. ������ ���� ����� 4����Ʈ ���� ���·� Ŭ���̾�Ʈ���� ����
6. ���� ��� ���� Ŭ���̾�Ʈ�� �������� ���� ����
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

	// ���� ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	// ���� ����
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// IP �ּ�, PORT ��ȣ �Ҵ�
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	// ���� ��û ���� ���� -> ���� ��û ��� ť : 5�� ��û ��� ����
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen Error");

	// Ŭ���̾�Ʈ ���� ��û �����ϱ�
	clntAdrSize = sizeof(clntAddr);

	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAdrSize);

	if (hClntSock == -1)
		ErrorHandling("accept() error");
	else
		printf("Connected client %d\n", 1);

	// TCP �� �������� ��谡 ����.
	// Ŭ���̾�Ʈ�� ���� ���� write �Լ��� ȣ���ߴ���� �ϴ���
	// ������ �ѹ��� receive �Լ��� ȣ���ϸ� �ȴ�.
	opCnt = 0;
	int readOpCnt = recv(hClntSock, (char*)&opCnt, 1, 0);

	cout << "opCnt        : " << opCnt << endl;

	recvLen = 0;

	// (int)opCnt[0] * 4 : �ǿ����ڵ� ũ��
	// + 1 : ������ ����
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

	// ����� Ŭ�󸮾�Ʈ�� ������ ����
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