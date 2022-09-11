#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment (lib, "ws2_32")
#include <WinSock2.h>

/*
// WSAStartUp 역할 : 1) 프로그램에서 요구하는 윈도우 소켓의 버전을 알리고
//								2) 해당 버전을 지원하는 라이브러리 초기화 작업 진행
// wVersionRequested : 프로그래머가 사용할 윈속의 버전 정보 전달
// 상위 8비트   : 부 버전 정보, 하위 8비트 : 주 버전 정보
// lpWSAData : WSADATA 구조체 변수의 주소값 전달
int WSAStartUp(WORD wVersionRequested, LPWSADATA lpWSAData);

// 윈속 라이브러리 해제 
int WSACleanup(void);

// 성공시 소켓 핸들 반환
SOCKET socket(int af, int type, int protocol);

// IP 주소, PORT 번호 할당 목적 (즉, 소켓에 주소 할당)
int bind(SOCKET s, const struct sockaddr* name, int namelen);

// 연결 요청 수락 상태로 만들기
int listen(SOCKET s, int backlog);

// 클라이언트 프로그램에서의 연결 요청 수락
SOCKET accept(SOCKET s, struct sockaddr* addr, int* addrlen);

// 클라이언트에서 소켓 기반 연결 요청시 호출하는 함수
int connect(SOCKET s, const struct sockaddr* name, int namelen);

// 소켓 닫을 때 
int closesocket(SOCKET s);
*/

/* 윈도우 기반 입출력 함수
int send(SOCKET s, const char* buf, int len, int flags);
	s : 데이터 전송 대상 소켓 핸들
	buf : 전송 데이터
	len : 전송할 바이트 수
	flags : 옵션

int recv(SOCKET s, const char* buf, int len, int flags);
	s : 데이터 수신 대상 소켓 핸들
	buf : 전송 데이터
	len : 전송할 바이트 수
	flags : 옵션
*/