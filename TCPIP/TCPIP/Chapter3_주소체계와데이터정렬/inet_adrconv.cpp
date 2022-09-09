#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <WinSock2.h>

using namespace std;

void ErrorHandling(const char* message);


int main(int argc, char* argv[])
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() Error");

	// inet_addr 함수 호출 예시
	{
		const char* addr = "127.212.124.78";
		unsigned long conv_addr = inet_addr(addr);

		if (conv_addr == INADDR_NONE)
			printf("Error occured\n");
		else
			printf("Network ordered int addr : %#lx \n", conv_addr);
	}

	// inet_ntoa 함수 호출 예시
	{
		struct sockaddr_in addr;
		char* strPtr;
		char strArr[20];

		addr.sin_addr.s_addr = htonl(0x1020304); // IP 주소 정보 , 네트워크 바이트 정렬 순서로 
		strPtr = inet_ntoa(addr.sin_addr); // 문자열로 변환
		strcpy(strArr, strPtr);
		printf("Dotted-Decimal notation : %s \n", strArr);
	}

	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
