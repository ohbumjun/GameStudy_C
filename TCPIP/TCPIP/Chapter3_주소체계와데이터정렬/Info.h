#pragma once


/*
IPv4 의 주소정보를 담기 위해 정의된 구조체

struct sockaddr_in
{
	sa_family_t    sin_family;  // 주소 체계
	uint16_t         sin_port;    // 16비트 TCP/UDP PORT 번호
	struct in_addr sin_addr;   // 32 비트 IP 주소
	char				 sin_zero[8]
}

struct in_addr
{
	in_addr_t       s_addr;     // 32비트 IPv4 인터넷 주소
}

*/