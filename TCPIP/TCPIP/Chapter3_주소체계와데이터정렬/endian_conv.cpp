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
	
	// ��Ʈ��ũ ����Ʈ ������ ���ĵǰԲ� ��ȯ ������ ��ģ��.
	// ��, �� �ص�� ������� ��ȯ�ϱ�
	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	
}