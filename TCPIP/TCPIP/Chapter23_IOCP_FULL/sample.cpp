#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <process.h>
#include <windows.h>

#define BUF_SIZE 100
#define READ 3
#define WRITE 5

typedef struct
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

// buffer Info
typedef struct
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int rwMode; // Read Or Write
} PER_IO_DATA, * LPPER_IO_DATA;

unsigned __stdcall EchoThreadMain(LPVOID CompleitionPortID);
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	HANDLE hCompPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSok;
	SOCKADDR_IN servAdr;

	int recvBytes, i, flags = 0;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartUp() error!");

	// CP 오브젝트 생성
	hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	GetSystemInfo(&sysInfo);

	// 프로세서 개수만큼의 쓰레드 생성
	// 인자로 CP 오브젝트의 핸들 전달
	// 쓰레드는 해당 인자를 통해 CP 오브젝트에 접근
	for (int i = 0; i < sysInfo.dwNumberOfProcessors; ++i)
		_beginthreadex(NULL, 0, EchoThreadMain, (LPVOID)hCompPort, 0, NULL);

	// Overlapped IO 로 동작하는 서버 Socket 생성
	// 소켓은 블로킹 모드
	hServSok = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	// IP 주소, PORT 번호 할당
	if (bind(hServSok, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");

	// 클라이언트 연결 요청 대기 상태로 만들기
	// 연결 요쳥 대기 큐 사이즈는 6으로 설정
	if (listen(hServSok, 6) == SOCKET_ERROR)
		ErrorHandling("listen() error!");

	while (1)
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);

		// 클라이언트 연결 요청에 대한 새로운 소켓 생성
		// 연결 요청 대기 큐에서, 클라이언트 요청을 꺼내서 그에 맞는 소켓 새롭게 생성
		hClntSock = accept(hServSok, (SOCKADDR*)&clntAdr, &addrLen);

		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		handleInfo->hClntSock = hClntSock;

		// 새롭게 생성한 소켓의 주소 정보 복사
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		// CP 오브젝트와 클라이언트 연결 요청 소켓 연결
		CreateIoCompletionPort((HANDLE)hClntSock, hCompPort, (DWORD)handleInfo, 0);

		ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;

		// 입출력 IO 구분
		ioInfo->rwMode = READ;

		// 비동기 데이터 수신
		// Overlapped IO 형태로 진행 
		WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &(ioInfo->overlapped), NULL);
	}

}

// 쓰레드가 호출하는 메인 함수
unsigned __stdcall EchoThreadMain(LPVOID CompleitionPortIO)
{
	// CP 오브젝트 핸들을 인자로 넘겨준다.
	HANDLE hCompPort = (HANDLE)CompleitionPortIO;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;

	while (1)
	{
		/*
		완료된 IO의 확인, 이를 통해 클라이언트 연결 요청 소켓에 대한 정보 + overlapped 구조체에 대한 정보 획득

		IO가 완료되고 이에 대한 정보가 CP 오브젝트에 등록되었을 때 반환.

		반환시 3번째, 4번째 인자로 얻는 정보가 무엇인지 정확하게 알아야 한다.
		CreateIOCompleitionPort 함수와 WSARecv 함수에 전달한 인자 정보를
		이때 반환 받아서 사용할 수 있다.

		즉, 

		결과적으로 해당 함수의 호출은,
		Completion Port 오브젝트에 쓰레드를 할당하는 것이다.
		*/
		GetQueuedCompletionStatus(hCompPort, &bytesTrans,
			(PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);

		sock = handleInfo->hClntSock; // IO가 완료된 소켓의 핸들

		// 완료된 IO가 데이터 수신이라면 
		if (ioInfo->rwMode == READ)
		{
			puts("message received");

			// 클라이언트 연결 종료 요청 (EOF)
			if (bytesTrans == 0)
			{
				closesocket(sock);

				free(handleInfo);
				free(ioInfo);
				continue;
			}

			// 서버가 수신한 메세지를 클라이언트에게 재전송
			// OVERLAPPED 구조체 변수 정보 초기화

			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = bytesTrans; // 수신한 데이터 크기만큼
			ioInfo->rwMode = WRITE; // 데이터 전송 모드

			WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

			// 클라이언트로 메세지 재전송 이후
			// 클라이언트로부터 다시 메세지 수신
			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));

			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ; // 데이터 전송 모드

			// 다시 해당 클라이언트 연결 요청 소켓이 전송하는 데이터를
			// 비동기로 수신 (반복)
			WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL,
				&flags, &(ioInfo->overlapped), NULL);
		}
		// 완료된 IO가 데이터 송신이라면 
		else
		{
			puts("message sent");

			free(ioInfo);
		}
	}

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}