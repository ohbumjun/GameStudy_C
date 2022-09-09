#include <stdio.h>
#include <WinSock2.h>

void ErrorHandling(const char*);

int main(int argc, char* argv)
{
	WSADATA wsaData;

	unsigned short host_port = 0x1234;
	unsigned short net_port;

	unsigned long host_addr = 0x12345678;
	unsigned long net_addr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp Error!");
	
	// 네트워크 바이트 순서로 정렬되게끔 변환 과정을 거친다.
	// 즉, 빅 앤디안 방식으로 변환하기
	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	
}