#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	TIMEVAL timeout;

	// fd_set 설정 
	// 소켓을 그룹짓기 위해서 사용한다.
	// 구조체 내부에 있는 fd_array 에 등록해서 소켓의 변화를
	// select 함수를 통해 감지한다.
	fd_set reads, cpyReads;

	int adrSz;
	int strLen, fdNum, i;
	char buf[BUF_SIZE];

	if (argc != 2)
	{
		printf("Usage %s <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		printf("WSA StartUp Error\n");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		printf("bind Error\n");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		printf("listen Error\n");

	// 파일 디스크립터 모든 비트값 0으로 초기화
	FD_ZERO(&reads);

	// 서버 소켓을 fd_set 에 등록해준다.
	// 해당 파일 디스크립터를 1로 설정해준다.
	// 넣어줄 특성 fd 의 비트를 켤때 사용
	FD_SET(hServSock, &reads); 

	while (1)
	{
		// 원본을 보존하기 위해 copySet 을 사용한다
		// select 함수를 호출하면 변화가 생긴 fd를 제외한 나머지 위치의
		// 비트들은 전부 0으로 초기화 된다.
		// 따라서 원본을 보존해야 한다.
		cpyReads = reads;

		// select 함수는 기본적으로 fd(혹은 핸들) 에 변화가 생길 때까지
		// 대기하는 블로킹방식으로 한곳에 모아놓은 여러 개의
		// 파일 디스크립터(핸들)를 동시에 관찰한다.

		// 읽기, 쓰기, 예외 소켓을 구분한다.
		// int nfds : 윈도우에서는 사용하지 않는다.
		// fd_set* readfds   : 수신된 데이터가 있는지 확인할 fd
		// fd_set* writefds  : 보낼 준비가 된 데이터가 있는지 확인할 fd
		// fd_set* exceptfds : 예외발생이 있는지 확인할 fd
		// const timeval* timeout : 타임 아웃 나타낸다.
		// - NULL을 리턴하면 하나라도 만족할 때까지 대기한다.
		// - 양수값이라면 그 시간까지 대기

		int iRet = select(0, &cpyReads, nullptr, nullptr, nullptr);

		if (iRet == SOCKET_ERROR)
			break;

		// 세팅된 소켓 개수(fd_count)만큼 검사한다
		for (int i = 0; i < reads.fd_count; ++i)
		{
			// FD_ISSET : 변화 여부 확인
			// 원본과 커피셋을 비교하여 변화 여부를 확인할 수 있다.
			// 즉, reads.fd_array[i] 이 녀석의 fd 에 변화가 있는가
			if (FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				// 변화된 소켓은 클라가 접속을 요청한 hServSock 소켓의 
				// 변화가 같다
				if (reads.fd_array[i] == hServSock)
				{
					adrSz = sizeof(clntAddr);

					// 연결을 수락한다. 해당 연결 요청에 대한 소켓을 생성한다.
					hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &adrSz);

					// 관찰 대상으로 추가한다(수신)
					FD_SET(hClntSock, &reads);

					printf("connected client : %d\n", hClntSock);
				}
				else // read message
				{
					// 해당 괄호 안으로 들어오면
					// 클라이언트 소켓이라는 의미이다. 즉, 서버 소멧이 아닌 경우
					// 수신할 데이터가 있는지 확인해야 한다.
					// 수신한 데이터가 문자열 데이터인지, 
					// 아니면, 연결 종료를 의미하는 EOF 인지 확인해야 한다.
					strLen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);

					// EOF --> 연결 종료를 의미한다.
					if (strLen == 0)
					{
						// 관찰 대상에서 제외한다.
						FD_CLR(reads.fd_array[i], &reads);

						closesocket(cpyReads.fd_array[i]);

						printf("closed client: %d\n", cpyReads.fd_array[i]);
					}
					else
					{
						// echo
						send(reads.fd_array[i], buf, strLen, 0);
					}
				}
			}
		}
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}