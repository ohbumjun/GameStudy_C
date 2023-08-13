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

	int i;
	struct hostent* host;

	if (argc != 2)
	{
		cout << "argc : " << argc << endl;
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// 소켓 라이브러리 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	host = gethostbyname(argv[1]);

	if (!host)
		ErrorHandling("gethost ...error\n");

	printf("Official Name : %s\n", host->h_name);
	for (int i = 0; host->h_aliases[i]; ++i)
		printf("Aliases %d : %s\n", i + 1, host->h_aliases[i]);
	printf("Address Type : %s \n", host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");
	for (int i = 0; host->h_addr_list[i]; ++i)
		printf("IP addr %d : %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	WSACleanup();

	return 0;
};

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
