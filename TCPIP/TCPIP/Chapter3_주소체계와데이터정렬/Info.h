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

// 소켓에 인터넷 주소 할당하기
int bind(int sockfd, struct sockaddr *myAddr, socklen_t addrlen);
	- sockfd : 주소정보 (IP, PORT)를 할당할 소켓의 파일 디스크립터
	- myAddr : 할당하고자 하는 주소정보를 지니는, 구조체 변수의 주소값
	- addrlen : 2번째 인자 길이 정보

// 리눅스에 X, 윈도우만이 가지고 있는 함수 -> 운영체제 간 이식성 떨어진다
// 1) inet_ntoa 의 함수 역할과 동일
INT WSAAPI WSAStringToAddressA(
  [in]           LPSTR               AddressString, // IP, PORT 번호 문자열 주소
  [in]           INT                   AddressFamily, // 주소체계
  [in, optional] LPWSAPROTOCOL_INFOA lpProtocolInfo, 
  [out]          LPSOCKADDR          lpAddress, // 주소정보 담을 구조체 변수 주소 전달
  [in, out]      LPINT               lpAddressLength // 4번째 인자 변수크기를 담고 있는, 변수의 주소값
);

// 2) inet_addr 역할
INT WSAAPI WSAAddressToStringA(
  [in]           LPSOCKADDR          lpsaAddress,
  [in]           DWORD               dwAddressLength,
  [in, optional] LPWSAPROTOCOL_INFOA lpProtocolInfo,
  [in, out]      LPSTR               lpszAddressString,
  [in, out]      LPDWORD             lpdwAddressStringLength
);
*/