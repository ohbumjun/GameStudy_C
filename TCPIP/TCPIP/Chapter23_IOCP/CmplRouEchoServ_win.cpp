// #include <stdio.h>
// #include <stdlib.h>
// #include <WinSock2.h>
// 
// #define BUF_SIZE 1024
// 
// void CALLBACK ReadCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
// void CALLBACK WriteCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
// void ErrorHandling(const char* message);
// 
// typedef struct
// {
// 	SOCKET hClntScok;
// 	char buf[BUF_SIZE];
// 	WSABUF wsaBuf;
// } PER_IO_DATA, *LPPER_IO_DATA;
// 
// int main(int argc, char* argv[])
// {
// 	WSADATA wsaData;
// 	SOCKET hLisnSock, hRecvSok;
// 	SOCKADDR_IN lisnAdr, recvAdr;
// 
// 	LPWSAOVERLAPPED lpOvlp;
// 	DWORD recvBytes;
// 
// 	LPPER_IO_DATA hbInfo;
// 	int mode = 1, recvAdrSize, flagInfo = 0;
// 
// 	if (argc != 2)
// 	{
// 		printf("Usage : %s <port>\n", argv[0]);
// 		exit(1);
// 	}
// 
// 	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
// 		ErrorHandling("WSAStartUp() error!");
// 
// 	// Overlapped IO 로 동작하는 Socket 생성
// 	hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
// 
// 	// hLisnSock 핸들이 참조하는 소켓의 입출력 모드를 넌블로킹 모드로 하겠다.
// 	ioctlsocket(hLisnSock, FIONBIO, (u_long*)&mode);
// 
// 	memset(&lisnAdr, 0, sizeof(lisnAdr));
// 	lisnAdr.sin_family = AF_INET;
// 	lisnAdr.sin_addr.s_addr = htonl(INADDR_ANY);
// 	lisnAdr.sin_port = htons(atoi(argv[1]));
// 
// 	if (bind(hLisnSock, (SOCKADDR*)&lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR)
// 		ErrorHandling("bind() error!");
// 
// 	if (listen(hLisnSock, 6) == SOCKET_ERROR)
// 		ErrorHandling("listen() error!");
// 
// 	recvAdrSize = sizeof(recvAdr);
// 	
// 	while (1)
// 	{
// 		// 쓰레드를 alertable wait 상태로 만든다.
// 		// 즉, 해당 시점에 완료된 IO가 있다면 CP를 실행한다.
// 		// 다시 말해서 CP 의 실행을 위해 해당 함수를 반복호출하는 것이다.
// 		SleepEx(100, TRUE); 
// 
// 		// 클라이언트 연결 요청에 대한 소켓 생성
// 		// ioctlsocket 함수 호출을 통해 세팅한 hLisnSock 소켓의 경우
// 		// 해당 accept 함수 호출 시점에 클라이언트 연결 요청이 없다면 INVALID_SOCKET 반환
// 		// WSAGetLastError 의 경우 WSAEWOULDBLOCK 반환
// 		hRecvSok = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSize);
// 
// 		if (hRecvSok == INVALID_SOCKET)
// 		{
// 			if (WSAGetLastError() == WSAEWOULDBLOCK)
// 				continue;
// 			else 
// 				ErrorHandling("accept() error!");
// 		}
// 
// 		puts("Client connected...");
// 
// 		/*
// 		매번 WSARecv 함수 호출 때마다 각각 다른 overlapped 구조체를 넘겨주기 위해
// 		동적 할당
// 		*/
// 		lpOvlp = (LPWSAOVERLAPPED)malloc(sizeof(WSAOVERLAPPED));
// 		memset(lpOvlp, 0, sizeof(WSAOVERLAPPED));
// 
// 		hbInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
// 		hbInfo->hClntScok = (DWORD)hRecvSok;
// 		(hbInfo->wsaBuf).buf = hbInfo->buf;
// 		(hbInfo->wsaBuf).len = BUF_SIZE;
// 
// 		// CP 에서는 Event가 필요없다
// 		// 여기서는 특정 구조체를 저장하는 용도로 사용
// 		lpOvlp->hEvent = (HANDLE)hbInfo;
// 
// 		/*
// 		CP 기반의 Overlapped IO 실행
// 		CR로 ReadCompRouting 함수 등록
// 		*/
// 		WSARecv(hRecvSok, &(hbInfo->wsaBuf), 1, &recvBytes, (LPDWORD)&flagInfo, 
// 			lpOvlp, ReadCompRoutine);
// 	}
// 
// 	closesocket(hRecvSok);
// 	closesocket(hLisnSock); // 서버 소켓 소멸
// 
// 	WSACleanup();
// 
// 	return 0;
// }
// 
// void CALLBACK ReadCompRoutine(DWORD dwError, DWORD szRecvBytes, 
// 	LPWSAOVERLAPPED lpOverlapped, DWORD flags)
// {
// 	// 해당 함수가 호출되었따는 것은, 클라이언트로부터 데이터 수신이 완료되었다는 것을 의미
// 	// 따라서 클라이언트에게 다시 이 데이터를 에코해야 한다.
// 
// 	/*
// 	WSARecv 함수 호출시 전달했던 LOWSAOVERLAPPED 가 가리키는 구조체 변수에
// 	저장한 변수를 여기서 활용한다.
// 	*/
// 
// 	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
// 	SOCKET hSock = hbInfo->hClntScok;
// 	LPWSABUF bufInfo = &(hbInfo->wsaBuf);
// 	DWORD sendBytes;
// 
// 	// 클라이언트의 EOF 전달 (연결 종료 요청)
// 	if (szRecvBytes == 0)
// 	{
// 		closesocket(hSock);
// 		free(lpOverlapped->hEvent);
// 		puts("Client disconnected ...");
// 	}
// 	else
// 	{
// 		bufInfo->len = szRecvBytes;
// 
// 		// CR 기반의 Overlapped IO 실행
// 		// CR로 WriteCompRoutine 함수를 등록한다.
// 		// 즉, ReadCompRoutine 함수가 호출되면 WSASend 함수를 호출하면서
// 		// 넌블로킹 모드로 데이터가 송신되게 하고, 송신이 완료되면
// 		// WriteCompRouting 함수가 호출되게 한다.
// 		WSASend(hSock, bufInfo, 1, &sendBytes, 0, lpOverlapped, WriteCompRoutine);
// 	}
// 
// }
// 
// void CALLBACK WriteCompRoutine(DWORD dwError, DWORD szRecvBytes,
// 	LPWSAOVERLAPPED lpOverlapped, DWORD flags)
// {
// 	// 에코 메세지가 전송된 이후에 해당 함수가 호출된다
// 	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
// 	SOCKET hSock = hbInfo->hClntScok;
// 	LPWSABUF bufInfo = &(hbInfo->wsaBuf);
// 	DWORD recvBytes;
// 
// 	int flagInfo = 0;
// 
// 	/*
// 	다시 데이터를 수신해야 하므로
// 	CR 기반의 Overlapped IO 실행
// 	CR 로 ReadCompRouting 함수 등록
// 	
// 	즉, WriteCompRoutine 함수가 호출되면 다시 WSARecv함수를 호출하면서
// 	넌블로킹 모드로 데이터의 수신을 기다린다 (해당 과정 반복)
// 	*/
// 	WSARecv(hSock, &(hbInfo->wsaBuf), 1, &recvBytes, (LPDWORD)&flagInfo,
// 		lpOverlapped, ReadCompRoutine);
// }
// 
// void ErrorHandling(const char* message)
// {
// 
// }
