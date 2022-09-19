#pragma once

/*

> Overlapped IO 소켓 생성                 ---------------------------------------------------------------------

SOCKET WSASocket(
	int af, int type, int protocol, LPWAPROTOCOL_INFO lpProtocolInfo, GROUP g, DWORD dwFlags);

	- af       : 프로토콜 체계
	- type     : 소켓 데이터 전송방식
	- protocol : 두 소켓 사이에 사용되는 프로토콜 정보 전달
	- lpProtocolInfo : 생성되는 소켓의 특성 정보를 담는 WSAPROTOCOL_INFO 구조체 변수의 주소값 전달
	- g : 
	- dwFlags : 소켓의 속성 정보 전달 --> WSA_FLAG_OVERLAPPED 를 사용하면 Overlapped IO가 가능한 넌-블로킹 모드의 소켓이 생성된다.

> Overlapped IO 를 진행하는 WSASend 함수 ---------------------------------------------------------------------

int WSASend(
	SOCKET s,                                                     // 소켓 핸들
	LPWSABUF lpBuffers,                                           // 전송할 데이터 정보를 지니닌 WSABUF 구조체 변수 배열 주소값
	DWORD dwBufferCount,                                          // 두번째 인자 배열 정보 길이
	LPDWORD lpNumberOfBytesSEnt,                                  // 전송된 바이트수가 저장될 변수의 주소값
	DWORD dwFlags,                                                // 함수의 데이터 전송특성을 변경하는 경우에 사용된다.
	LPWSAOVERLAPPED lpOverlapped,                                 // WSAOVERLAPPED 구조체 변수의 주소값 전달
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine        // Completion Routine 이라는 함수의 주소값 전달 --> 이를 통해서 데이터 전송의 완료를 확인할 수 있다.
);

typedef struct __WSABUF
{
	u_long len;    // 전송할 데이터 크기
	char FAR* buf; // 버퍼의 주소값
} WSABYF, *LPWSABUF;


typedef struct _WSAOVERLAPPED
{
	DWORD Internal;
	DWORD InternalHigh;
	DWORD Offset;
	DWORD OffsetHigh;  // 여기까지 4개 변수는 운영체제 내부적으로 사용
	WSAEVENT hEvent;   // 실제 사용하는 것은 이 구제치이다.
} WSAOVERLAPPED, *LPWSAOVERLAPPED;

> Overlapped IO 의 실제 전송된 데이터의 크기를 확인하는 함수 ---------------------------------------------------------------------

BOOL WSAGetOverlappedResult
{ SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags};

- s : Overlapped IO가 진행된 소켓 핸들
- lpOverlapped : Overlapped IO 진행 시 전달한 WSAOVERLAPPED 구조체 변수의 주소값 전달
- lpcbTransfer :실제 송수신된 바이트 크기를 저장할 변수의 주소값 전달
- fWait : 여전히 IO가 진행중인 상황의 경우 TRUE 전달시 IO가 완료될 때까지 대기, FALSE 전달 시 반환하면서 함수를 빠져나간다.
- lpdwFlags : WSARecv 함수가 호출된 경우, 부수적인 정ㅈ보(수신된 메시지가 OOB 메시지인지)를 얻기 위해 사용한다.불필요하면 NULL을 전달한다.


> Overlapped IO 를 진행하는 WSARecv 함수 ---------------------------------------------------------------------

int WSAAPI WSARecv(
  [in]      SOCKET                             s,                    // Overlapped IO 속성이 부여된 소켓 핸들
  [in, out] LPWSABUF                           lpBuffers,            // 수신된 데이터 정보가 저장될 버퍼의 정보를 지닌 WSABUF 구조체 배열의 주소값 전달.
  [in]      DWORD                              dwBufferCount,        // 두번째 인자로 전달된 배열의 길이정보 전달 
  [out]     LPDWORD                            lpNumberOfBytesRecvd, // 수신된 데이터의 크기정보가 저장될 변수의 주소값 전달
  [in, out] LPDWORD                            lpFlags,              // 전송특성과 곤련된 정보를 지정하거나 수신하는 경우
  [in]      LPWSAOVERLAPPED                    lpOverlapped,         // WSAOVERLAPPED 구조체 변수의 주소 값 전달  
  [in]      LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine   // Completion Routine 이라는 함수의 주소값 전달
);


void CALLBACK CompRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);

- 첫번째 매개변수 : 오류정보 (정상 종료시 0 전달)
- 두번째   : 완료된 입출력 데이터의 크기정보 전달
- 세번째   : WSASend, WSARecv 함수의 매개변수 lpOverlapped로 전달된 값 
- 네번째   : 입출력 함수 호출시 전달된 특성 정보 및 0이 전달
- CALLBACK : 쓰레드 main 함수에 선언되는 키워드는 WINAPI와 마찬가지로 함수의 호출 규약을 둔 것.
*/