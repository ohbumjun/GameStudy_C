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

*/