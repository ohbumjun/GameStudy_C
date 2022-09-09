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

// 네트워크 바이트 순서로 변환해주는 함수
unsigned short htons(unsigned short);
unsigned short ntohs(unsigned short);
unsigned long  htonl(unsigned long);
unsigned long  ntohl(unsigned long);

// 인터넷 주소 초기화
struct sockaddr_in addr;
char* serv_ip    = "211.217.168.13";          // IP 주소 문자열 지정
char* serv_port = "9190";                         // PORT 번호 문자열 지정
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;                        // 주소 체계 지정
addr.sin_addr.s_addr = inet_addr(serv_ip); // 문자열 기반의 IP 주소 초기화
addr.sin_port = htons(atoi(serv_port));      //  문자열 기반의 PORT 번호 초기화

*/