#pragma once


/*
IPv4 �� �ּ������� ��� ���� ���ǵ� ����ü

struct sockaddr_in
{
	sa_family_t    sin_family;  // �ּ� ü��
	uint16_t         sin_port;    // 16��Ʈ TCP/UDP PORT ��ȣ
	struct in_addr sin_addr;   // 32 ��Ʈ IP �ּ�
	char				 sin_zero[8]
}

struct in_addr
{
	in_addr_t       s_addr;     // 32��Ʈ IPv4 ���ͳ� �ּ�
}

// ��Ʈ��ũ ����Ʈ ������ ��ȯ���ִ� �Լ�
unsigned short htons(unsigned short);
unsigned short ntohs(unsigned short);
unsigned long  htonl(unsigned long);
unsigned long  ntohl(unsigned long);

// ���ͳ� �ּ� �ʱ�ȭ
struct sockaddr_in addr;
char* serv_ip    = "211.217.168.13";          // IP �ּ� ���ڿ� ����
char* serv_port = "9190";                         // PORT ��ȣ ���ڿ� ����
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;                        // �ּ� ü�� ����
addr.sin_addr.s_addr = inet_addr(serv_ip); // ���ڿ� ����� IP �ּ� �ʱ�ȭ
addr.sin_port = htons(atoi(serv_port));      //  ���ڿ� ����� PORT ��ȣ �ʱ�ȭ

*/